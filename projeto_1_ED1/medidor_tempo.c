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


// Função 1: Inversão da ordem do vetor
void inverter_arr(int arr[], int n)
{
    int l = 0;
    int r = n - 1;
    int temp;

    while (l < r)
    {
        temp = arr[l];
        arr[l] = arr[r];
        arr[r] = temp;

        l++;
        r--;
    }
}


// Função 2: Busca sequencial
void busca_sequencial(int arr[], int n, int target)
{
    for (int i = 0; i < n; i++)
    {
        if (arr[i] == target)
        {
            return;
        }
    }
}


// Função 3: Busca binária iterativa
void bin_int(int arr[], int l, int r, int target)
{
    while (l <= r)
    {
        int meio = l + (r - l) / 2;

        if (arr[meio] == target)
        {
            return;
        }

        if (arr[meio] < target)
        {
            l = meio + 1;
        }

        if (arr[meio] > target)
        {
            r = meio - 1;
        }
    }
}


// Função 4: Busca binária recursiva
void bin_rec(int arr[], int l, int r, int target)
{
    if (l > r)
    {
        return;
    }

    int meio = l + (r - l) / 2;

    if (arr[meio] == target)
    {
        return;
    }

    if (arr[meio] < target)
    {
        bin_rec(arr, meio + 1, r, target);
    }

    if (arr[meio] > target)
    {
        bin_rec(arr, l, meio - 1, target);
    }
}


// Calcula o tempo decorrido em nanossegundos
long long calcular_tempo(struct timespec inicio, struct timespec fim)
{
    long segundos = fim.tv_sec - inicio.tv_sec;
    long nanosegundos = fim.tv_nsec - inicio.tv_nsec;

    if (nanosegundos < 0)
    {
        segundos -= 1;
        nanosegundos += 1000000000;
    }

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
    fprintf(arquivo, "Algoritmo,N,TempoMedio\n");


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

        fprintf(arquivo, "Inversao,%d,%lld\n", N, tempo_medio);


        /*
         * =====================================================
         * 2 - BUSCA SEQUENCIAL
         * =====================================================
         */

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
    }

    fclose(arquivo);

    printf("Resultados salvos em resultados.csv\n");

    return 0;
}