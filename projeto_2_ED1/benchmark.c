#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "ListaComCursor.h"

/*
 * TRABALHO 2 - COMPARACAO DE LISTAS COM CURSOR
 *
 * O mesmo benchmark deve ser compilado SEPARADAMENTE com:
 *   - lista_sequencial.c
 *   - lista_encadeada.c
 *
 * A pasta "resultados" deve existir antes de executar o programa.
 * O tempo medido inclui SOMENTE as operacoes de cada experimento.
 */

#define EXECUCOES 10
#define QUANTIDADE_N 5
#define QUANTIDADE_CASOS 5
#define OPERACOES 100

/* Tamanhos iniciais exigidos pelo enunciado. */
const int tamanhos[QUANTIDADE_N] = {1000, 50000, 100000, 500000, 1000000};

/* O numero do caso (0 a 4) identifica cada experimento. */
const char *nomes_casos[QUANTIDADE_CASOS] = {
    "insercoes_inicio",
    "acessos_aleatorios",
    "remocoes_inicio",
    "remocoes_meio",
    "percurso_completo"
};

/*
 * Guarda os resultados das consultas fora da regiao cronometrada.
 * volatile ajuda a impedir que o compilador descarte seu uso.
 */
static volatile long long resultado_consultas = 0;

/*
 * Calcula a diferenca entre dois instantes, em nanossegundos.
 * E a mesma ideia do medidor de tempo do trabalho anterior.
 */
long long calcular_tempo(struct timespec inicio, struct timespec fim) {
    long segundos = fim.tv_sec - inicio.tv_sec;
    long nanosegundos = fim.tv_nsec - inicio.tv_nsec;

    if (nanosegundos < 0) {
        segundos--;
        nanosegundos += 1000000000L;
    }

    return (long long)segundos * 1000000000LL + nanosegundos;
}

/*
 * Preenche uma lista vazia com 0, 1, 2, ..., n-1.
 *
 * inserir_depois() NAO move o cursor quando a lista ja possui elementos.
 * Por isso, avancamos o cursor apos cada insercao, a partir da segunda.
 * Assim, os numeros ficam na ordem correta nas duas implementacoes.
 *
 * Retorna 0 se alguma operacao falhar e 1 se tudo der certo.
 */
int preencher_lista(ListaCursor *lista, int n) {
    for (int i = 0; i < n; i++) {
        if (!inserir_depois(lista, i)) {
            return 0;
        }
        if (i > 0 && !avancar_cursor(lista)) {
            return 0;
        }
    }

    return tamanho_lista(lista) == n;
}

/*
 * Gera 100 indices para os acessos aleatorios.
 *
 * A semente depende apenas de n e da repeticao, entao os dois executaveis
 * recebem a MESMA sequencia de indices no mesmo ambiente de C.
 * Indices repetidos sao permitidos pelo enunciado.
 *
 * A rejeicao de alguns valores evita o pequeno vies de rand() % n,
 * garantindo indices uniformes quando RAND_MAX + 1 >= n.
 * Tudo isso acontece ANTES de iniciar o cronometro.
 */
void gerar_indices(int indices[OPERACOES], int n, int repeticao) {
    srand(12345u + (unsigned)n + (unsigned)repeticao);

    unsigned long long faixa = (unsigned long long)RAND_MAX + 1ULL;
    unsigned long long limite = faixa - (faixa % (unsigned)n);

    for (int i = 0; i < OPERACOES; i++) {
        unsigned int sorteado;
        do {
            sorteado = (unsigned int)rand();
        } while ((unsigned long long)sorteado >= limite);

        indices[i] = (int)(sorteado % (unsigned)n);
    }
}

/*
 * Executa um dos cinco casos do enunciado.
 * Esta funcao e chamada ENTRE as duas leituras do cronometro.
 * A lista ja foi preenchida e o cursor ja foi preparado, quando necessario.
 *
 * 'soma_valores' acumula os valores consultados; depois do cronometro,
 * essa soma permite conferir se os acessos produziram o resultado esperado.
 */
int executar_experimento(ListaCursor *lista, int caso,
                         const int indices[OPERACOES], long long *soma_valores) {
    int valor;

    switch (caso) {
        case 0:  /* 100 insercoes no inicio, reposicionando antes de CADA uma */
            for (int i = 0; i < OPERACOES; i++) {
                mover_inicio(lista);
                if (!inserir_antes(lista, -(i + 1))) {
                    return 0;
                }
            }
            return 1;

        case 1:  /* 100 acessos aleatorios: mover e consultar */
            for (int i = 0; i < OPERACOES; i++) {
                if (!mover_para_indice(lista, indices[i]) ||
                    !obter_atual(lista, &valor)) {
                    return 0;
                }
                *soma_valores += valor;
            }
            return 1;

        case 2:  /* 100 remocoes no inicio */
        case 3:  /* 100 remocoes no meio; so muda a posicao inicial */
            for (int i = 0; i < OPERACOES; i++) {
                if (!remover_atual(lista)) {
                    return 0;
                }
            }
            return 1;

        case 4:  /* Visita todos os elementos, incluindo o primeiro */
            if (!obter_atual(lista, &valor)) {
                return 0;
            }
            *soma_valores += valor;

            while (avancar_cursor(lista)) {
                if (!obter_atual(lista, &valor)) {
                    return 0;
                }
                *soma_valores += valor;
            }
            return 1;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    /* Argumento usado apenas no nome dos arquivos; nao escolhe o TAD. */
    if (argc != 2 ||
        (strcmp(argv[1], "sequencial") != 0 &&
         strcmp(argv[1], "encadeada") != 0)) {
        fprintf(stderr, "Uso: %s sequencial|encadeada\n", argv[0]);
        return 1;
    }

    const char *implementacao = argv[1];
    char caminho_tempos[100];
    char caminho_medias[100];

    /* A pasta resultados/ ja deve estar criada. */
    snprintf(caminho_tempos, sizeof(caminho_tempos),
             "resultados/tempos_%s.csv", implementacao);
    snprintf(caminho_medias, sizeof(caminho_medias),
             "resultados/medias_%s.csv", implementacao);

    FILE *tempos = fopen(caminho_tempos, "w");
    if (!tempos) {
        perror("Nao foi possivel abrir o CSV de tempos");
        return 1;
    }

    FILE *medias = fopen(caminho_medias, "w");
    if (!medias) {
        perror("Nao foi possivel abrir o CSV de medias");
        fclose(tempos);
        return 1;
    }

    fprintf(tempos, "Implementacao,Experimento,N,Repeticao,TempoNs\n");
    fprintf(medias, "Implementacao,Experimento,N,Execucoes,TempoMedioNs\n");

    /* 1o laco: os cinco tamanhos; 2o: os cinco casos. */
    for (int t = 0; t < QUANTIDADE_N; t++) {
        int n = tamanhos[t];

        for (int caso = 0; caso < QUANTIDADE_CASOS; caso++) {
            long long soma_tempos = 0;

            /* 3o laco: dez REPETICOES INDEPENDENTES de cada combinacao. */
            for (int repeticao = 1; repeticao <= EXECUCOES; repeticao++) {
                int indices[OPERACOES];
                long long soma_valores = 0;
                struct timespec inicio, fim;

                /* Cada repeticao comeca com uma lista NOVA de tamanho n. */
                ListaCursor *lista = criar_lista();
                if (!lista) {
                    fprintf(stderr, "Erro ao criar lista.\n");
                    goto erro;
                }

                if (!preencher_lista(lista, n)) {
                    fprintf(stderr, "Erro ao preencher lista de N=%d.\n", n);
                    destruir_lista(lista);
                    goto erro;
                }

                /* Sorteio de indices antes da medicao. */
                if (caso == 1) {
                    gerar_indices(indices, n, repeticao);
                }

                /*
                 * Posicionamento inicial tambem fica FORA do tempo.
                 * Excecao: caso 0, no qual cada mover_inicio e medido.
                 */
                if (caso == 2 || caso == 4) {
                    mover_inicio(lista);
                } else if (caso == 3) {
                    if (!mover_para_indice(lista, n / 2)) {
                        fprintf(stderr, "Erro ao posicionar cursor.\n");
                        destruir_lista(lista);
                        goto erro;
                    }
                }

                /* Cronometra apenas as operacoes do caso escolhido. */
                if (clock_gettime(CLOCK_MONOTONIC, &inicio) != 0) {
                    perror("clock_gettime");
                    destruir_lista(lista);
                    goto erro;
                }

                int sucesso = executar_experimento(lista, caso, indices,
                                                   &soma_valores);

                if (clock_gettime(CLOCK_MONOTONIC, &fim) != 0) {
                    perror("clock_gettime");
                    destruir_lista(lista);
                    goto erro;
                }

                /* Verificacoes realizadas somente DEPOIS do cronometro. */
                if (sucesso && caso == 0) {
                    sucesso = tamanho_lista(lista) == n + OPERACOES;
                } else if (sucesso && (caso == 2 || caso == 3)) {
                    sucesso = tamanho_lista(lista) == n - OPERACOES;
                } else if (sucesso && caso == 1) {
                    long long esperado = 0;
                    for (int i = 0; i < OPERACOES; i++) {
                        esperado += indices[i];
                    }
                    sucesso = soma_valores == esperado;
                } else if (sucesso && caso == 4) {
                    long long esperado = (long long)n * (n - 1) / 2;
                    sucesso = soma_valores == esperado;
                }

                /* Resultado utilizado fora do cronometro (evita descarte). */
                resultado_consultas += soma_valores;

                if (!sucesso) {
                    fprintf(stderr, "Falha: %s, N=%d, repeticao=%d.\n",
                            nomes_casos[caso], n, repeticao);
                    destruir_lista(lista);
                    goto erro;
                }

                long long tempo_ns = calcular_tempo(inicio, fim);
                soma_tempos += tempo_ns;

                /* Grava o tempo individual sem afetar o intervalo medido. */
                fprintf(tempos, "%s,%s,%d,%d,%lld\n", implementacao,
                        nomes_casos[caso], n, repeticao, tempo_ns);

                destruir_lista(lista);
            }

            /* Media dos 10 tempos do bloco completo de operacoes. */
            double media_ns = (double)soma_tempos / EXECUCOES;
            fprintf(medias, "%s,%s,%d,%d,%.2f\n", implementacao,
                    nomes_casos[caso], n, EXECUCOES, media_ns);

            printf("%-19s | N=%7d | media: %.2f ns\n",
                   nomes_casos[caso], n, media_ns);
        }
    }

    fclose(tempos);
    fclose(medias);
    printf("\nResultados salvos em %s e %s\n",
           caminho_tempos, caminho_medias);
    return 0;

erro:
    /* Nenhuma media incompleta e registrada; fecha arquivos antes de sair. */
    fclose(tempos);
    fclose(medias);
    return 1;
}
