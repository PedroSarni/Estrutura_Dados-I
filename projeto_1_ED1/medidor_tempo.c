#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define EXECUCOES 100
#define QUANTIDADE_N 5

/*
    Trabalho realizado por:
    Pedro Artur Barberá Sarni (17918418)
    Breno de Sousa Cad (17881714)
    Thales Amaral Gontijo (17925010)
    Murilo Fransciscato Ataide (17916308)
*/
//definindo variaveis globais para contagem de atribuições e comparações
long long count_atribuicoes = 0;
long long count_comparacoes = 0;

// Função 1: Inversão da ordem do vetor
void inverter_arr(int arr[], int n)
{
    //definindo o inicio do array
    int l = 0;
    count_atribuicoes++;
    //definindo o final
    int r = n - 1;
    count_atribuicoes++;
    //criando uma variavel temporária
    int temp;

    //enquanto o começo do array for menor que o final (isso garante que todo o vetor estará invertido)
    // count comparações está dentro do while porque ele trata todos os casos em que ocorre a comparação,
    //até quando ela não é verdadeira.

    while (count_comparacoes++, l < r)  
    {
        //guarda o valor do começo do array na variável temporária
        temp = arr[l];
        count_atribuicoes++;
        //atribui o final do array no começo
        arr[l] = arr[r];
        count_atribuicoes++;
        // atribui o inicio ao final (via variavel temporaria).
        arr[r] = temp;
        count_atribuicoes++;

        l++;
        count_atribuicoes++;
        r--;
        count_atribuicoes++;
    }
}


// Função 2: Busca sequencial
void busca_sequencial(int arr[], int n, int target)
{
    for (int i = 0;(count_comparacoes++, i < n); i++)
    {
        //compara se o elemento atual do vetor é igual ao desejado.
        count_comparacoes++;
        if (arr[i] == target)
        {
            //se for, encerra o loop e retorna a função.
            return;
        }
    // se não, incrementa o i e verifica o elemento da próxima posição.
    }
}


// Função 3: Busca binária iterativa
void bin_int(int arr[], int l, int r, int target)
{
    //enquanto o começo do array for menor que o final
    while (count_comparacoes++,l <= r)
    {
        //define o meio do vetor, para separá-lo em 2.
        int meio = l + (r - l) / 2;
        count_atribuicoes++;
        //verifica se o valor desejado está no meio.
        count_comparacoes++;
        if (arr[meio] == target)
        {
            //se sim, retorna a função e encerra o loop.
            return;
        }
        //se o valor do meio for menor que o número desejado, o que nos interssa é a
        //metade da direita do vetor (considerando que ele está ordenado de forma crescente).
        count_comparacoes++;
        if (arr[meio] < target)
        {
            //por isso o começo do vetor agora está à direita do meio, pois nos interessa
            //só a metade da direita. 
            l = meio + 1;
            count_atribuicoes++;
        }
        //mesma coisa da parte de cima, porém, no caso onde o valor desejado é menor que
        // o valor que está no meio, então, nos interessa o vetor à esquerda do meio.
        count_comparacoes++;
        if (arr[meio] > target)
        {
            //definindo o final do vetor como o número à esquerda do meio.
            r = meio - 1;
            count_atribuicoes++;
        }
    }
}


// Função 4: Busca binária recursiva
void bin_rec(int arr[], int l, int r, int target)
{
    //enquanto o começo do array for menor que o final.
    count_comparacoes++;
    if (l > r)
    {
        return;
    }
    //definindo o meio assim como na busca binária interativa.
    int meio = l + (r - l) / 2;
    count_atribuicoes++;
    //verifica se o valor desejado está no meio.
    count_comparacoes++;
    if (arr[meio] == target)
    {
        return;
    }
    //se o valor do meio for menor que o número desejado, o que nos interssa é a
    //metade da direita do vetor (considerando que ele está ordenado de forma crescente).
    count_comparacoes++;
    if (arr[meio] < target)
    {
        // com isso, chamamos a própria função recursivamente definindo o começo do array como 
        // meio + 1 (logo a direita do meio)
        bin_rec(arr, meio + 1, r, target);
    }
    //mesma coisa da parte de cima, porém, no caso onde o valor desejado é menor que
    // o valor que está no meio, então, nos interessa o vetor à esquerda do meio.
    count_comparacoes++;
    if (arr[meio] > target)
    {
        //chamamos recursivamente a função, só que agora definindo o final da função
        //como meio-1 (logo a esquerda do meio). 
        bin_rec(arr, l, meio - 1, target);
    }
}


// Calcula o tempo decorrido em nanossegundos
long long calcular_tempo(struct timespec inicio, struct timespec fim)
{
    //definindo a diferença entre o tempo final e o tempo inicial em segundos e nanosegundos
    long segundos = fim.tv_sec - inicio.tv_sec;
    long nanosegundos = fim.tv_nsec - inicio.tv_nsec;

    if (nanosegundos < 0)
    {
        segundos -= 1;
        nanosegundos += 1000000000;
    }
    // retornando o tempo total em nanossegundos (segundos * 1 bilhão + nanosegundos);
    return (long long)segundos * 1000000000LL + nanosegundos;
}


int main()
{
    
    int tamanhos[QUANTIDADE_N] = {
        1000,
        50000,
        100000,
        500000,
        1000000
    };

    struct timespec inicio, fim;

    // Cria o arquivo CSV
    FILE *arquivo = fopen("resultados.csv", "w");

    if (arquivo == NULL)
    {
        printf("Erro ao criar o arquivo CSV.\n");
        return 1;
    }

    // Cabeçalho do CSV
    fprintf(arquivo, "Algoritmo,N,TempoMedio,Atribuicoes,Comparacoes\n");


    for (int t = 0; t < QUANTIDADE_N; t++)  
    {
        int N = tamanhos[t];

        int *vetor = malloc(N * sizeof(int));

        if (vetor == NULL)
        {
            printf("Erro ao alocar vetor.\n");
            fclose(arquivo);
            return 1;
        }

        // Prepara o vetor ordenado
        for (int i = 0; i < N; i++)
        {
            vetor[i] = i;
        }


        /*
         * =====================================================
         * 1 - INVERSÃO
         * =====================================================
         */

        long long soma = 0;

        for (int i = 0; i < EXECUCOES; i++)
        {
            // O vetor começa ordenado novamente
            for (int j = 0; j < N; j++)
            {
                vetor[j] = j;
            }

            clock_gettime(CLOCK_MONOTONIC, &inicio);

            inverter_arr(vetor, N);

            clock_gettime(CLOCK_MONOTONIC, &fim);

            soma += calcular_tempo(inicio, fim);
        }


        //ordena o vetor novamente para realizar os proximos algoritmos
        for (int j = 0; j < N; j++)
            {
                vetor[j] = j;
            }


        long long tempo_medio = soma / EXECUCOES;

        long segundos_medio = tempo_medio / 1000000000LL;
        long nanosegundos_medio = tempo_medio % 1000000000LL;

        printf("Inversao - N = %d\n", N);
        printf("Tempo decorrido: %ld.%09ld segundos\n\n",
               segundos_medio, nanosegundos_medio);

        fprintf(arquivo, "Inversao,%d,%lld,%lld,%lld\n", N, tempo_medio,count_atribuicoes,count_comparacoes);


        /*
         * =====================================================
         * 2 - BUSCA SEQUENCIAL
         * =====================================================
         */

        //resetando as atribuições e comparações para contar na busca sequencial.
        long long count_atribuicoes = 0;
        long long count_comparacoes = 0;

        soma = 0;

        // Pior caso: elemento não está no vetor
        int target = -1;

        for (int i = 0; i < EXECUCOES; i++)
        {
            clock_gettime(CLOCK_MONOTONIC, &inicio);

            busca_sequencial(vetor, N, target);

            clock_gettime(CLOCK_MONOTONIC, &fim);

            soma += calcular_tempo(inicio, fim);
        }

        tempo_medio = soma / EXECUCOES;

        segundos_medio = tempo_medio / 1000000000LL;
        nanosegundos_medio = tempo_medio % 1000000000LL;

        printf("Busca sequencial - N = %d\n", N);
        printf("Tempo decorrido: %ld.%09ld segundos\n\n",
               segundos_medio, nanosegundos_medio);

        fprintf(arquivo, "Busca Sequencial,%d,%lld\n", N, tempo_medio);


        /*
         * =====================================================
         * 3 - BUSCA BINÁRIA ITERATIVA
         * =====================================================
         */

        soma = 0;
        //resetando as atribuições e comparações para contar na busca binaria interativa.
        long long count_atribuicoes = 0;
        long long count_comparacoes = 0;
        // Pior caso: elemento não está no vetor
        target = -1;

        for (int i = 0; i < EXECUCOES; i++)
        {
            clock_gettime(CLOCK_MONOTONIC, &inicio);

            bin_int(vetor, 0, N - 1, target);

            clock_gettime(CLOCK_MONOTONIC, &fim);

            soma += calcular_tempo(inicio, fim);
        }

        tempo_medio = soma / EXECUCOES;

        segundos_medio = tempo_medio / 1000000000LL;
        nanosegundos_medio = tempo_medio % 1000000000LL;

        printf("Busca binaria iterativa - N = %d\n", N);
        printf("Tempo decorrido: %ld.%09ld segundos\n\n",
               segundos_medio, nanosegundos_medio);

        fprintf(arquivo, "Busca Binaria Iterativa,%d,%lld\n", N, tempo_medio);


        /*
         * =====================================================
         * 4 - BUSCA BINÁRIA RECURSIVA
         * =====================================================
         */

        soma = 0;

         //resetando as atribuições e comparações para contar na busca binária recursiva.
        long long count_atribuicoes = 0;
        long long count_comparacoes = 0;

        target = -1;

        for (int i = 0; i < EXECUCOES; i++)
        {
            clock_gettime(CLOCK_MONOTONIC, &inicio);

            bin_rec(vetor, 0, N - 1, target);

            clock_gettime(CLOCK_MONOTONIC, &fim);

            soma += calcular_tempo(inicio, fim);
        }

        tempo_medio = soma / EXECUCOES;

        segundos_medio = tempo_medio / 1000000000LL;
        nanosegundos_medio = tempo_medio % 1000000000LL;

        printf("Busca binaria recursiva - N = %d\n", N);
        printf("Tempo decorrido: %ld.%09ld segundos\n\n",
               segundos_medio, nanosegundos_medio);

        fprintf(arquivo, "Busca Binaria Recursiva,%d,%lld\n", N, tempo_medio);


        free(vetor);
        //resetando as atribuicoes e comparações para começarmos um novo
        //caso de teste nas funções implementadas.
        long long count_atribuicoes = 0;
        long long count_comparacoes = 0;
    }
    fclose(arquivo);

    printf("Resultados salvos em resultados.csv\n");

    return 0;
}