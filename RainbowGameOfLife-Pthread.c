/*
INTEGRANTES:
    - Thomas Lincoln Victor da Silva - 156603
    - Luiz Fernando de Cristo Moloni - 159325
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <omp.h>

#define N 2048        // Tamanho do Tabuleiro - Deve ser: 2048
#define GEN 2000       // Número de Gerações - Deve ser: 2000
#define NUM_THREADS 2 // Número de threads para paralelização

// Estrutura para passar dados para as threads
struct ThreadData
{
    float **grid;    // domínio atual
    float **newGrid; // próximo domínio que foi gerado
    int start_row;   // primeira linha da threads
    int end_row;     // útlima linha da thread
    float local_sum; // quantidade de células vivas de cada threads
};

// Função para imprimir a Matriz
void PrintGrid(float **grid)
{
    for (int i = 0; i < 50; i++)
    {
        for (int j = 0; j < 50; j++)
        {
            printf("%.f|", grid[i][j]);
        }
        printf("\n");
    }
}

// Função que gera uma matriz de tamanho NxN com valor igual a 0 e 1 -> gera a matriz inicial com o glider e o R-pentomino
void GenerateGrid(float **grid)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            grid[i][j] = 0.0;
        }
    }

    // Posiciona um glider na posição 1,1 que é uma figura que vai se movimento de uma forma específica até sumir
    // Figura de um Glider a partir da posição (1,1)
    int line = 1, col = 1;

    grid[line][col + 1] = 1.0;
    grid[line + 2][col] = 1.0;
    grid[line + 1][col + 2] = 1.0;
    grid[line + 2][col + 1] = 1.0;
    grid[line + 2][col + 2] = 1.0;

    // Figura de um R-Pentomino a partir da posição (10,30)
    line = 10, col = 30;

    grid[line][col + 1] = 1.0;
    grid[line][col + 2] = 1.0;
    grid[line + 1][col] = 1.0;
    grid[line + 1][col + 1] = 1.0;
    grid[line + 2][col + 1] = 1.0;
}

// Função que retorna a quantidade de vizinhos vivos de cada célula
int getNeighbors(float **grid, int i, int j)
{
    int count = 0;

    // Verificando se a célula está na borda superior
    if (i == 0)
    {
        // Verificando se a célula está na borda superior esquerda
        if (j == 0)
        {
            if (grid[i][j + 1] == 1.0)
                count++;
            if (grid[i + 1][j + 1] == 1.0)
                count++;
            if (grid[i + 1][j] == 1.0)
                count++;
            if (grid[i + 1][N - 1] == 1.0)
                count++;
            if (grid[i][N - 1] == 1.0)
                count++;
            if (grid[N - 1][N - 1] == 1.0)
                count++;
            if (grid[N - 1][j] == 1.0)
                count++;
            if (grid[N - 1][j + 1] == 1.0)
                count++;
        }
        // Verificando se a célula está na borda superior direita
        else if (j == N - 1)
        {
            if (grid[i][j - 1] == 1.0)
                count++;
            if (grid[i + 1][j - 1] == 1.0)
                count++;
            if (grid[i + 1][j] == 1.0)
                count++;
            if (grid[i + 1][0] == 1.0)
                count++;
            if (grid[i][0] == 1.0)
                count++;
            if (grid[N - 1][0] == 1.0)
                count++;
            if (grid[N - 1][j] == 1.0)
                count++;
            if (grid[N - 1][j - 1] == 1.0)
                count++;
        }
        // Verificando se a célula está na borda superior central
        else
        {
            if (grid[i][j - 1] == 1.0)
                count++;
            if (grid[i + 1][j - 1] == 1.0)
                count++;
            if (grid[i + 1][j] == 1.0)
                count++;
            if (grid[i + 1][j + 1] == 1.0)
                count++;
            if (grid[i][j + 1] == 1.0)
                count++;
            if (grid[N - 1][j + 1] == 1.0)
                count++;
            if (grid[N - 1][j] == 1.0)
                count++;
            if (grid[N - 1][j - 1] == 1.0)
                count++;
        }
    }
    else if (i == N - 1)
    { // Verificando se a célula está na borda inferior
        // Verificando se a célula está na borda inferior esquerda
        if (j == 0)
        {
            if (grid[i - 1][j] == 1.0)
                count++;
            if (grid[i - 1][j + 1] == 1.0)
                count++;
            if (grid[i][j + 1] == 1.0)
                count++;
            if (grid[0][j + 1] == 1.0)
                count++;
            if (grid[0][j] == 1.0)
                count++;
            if (grid[0][N - 1] == 1.0)
                count++;
            if (grid[i][N - 1] == 1.0)
                count++;
            if (grid[i - 1][N - 1] == 1.0)
                count++;
        }
        // Verificando se a célula está na borda inferior direita
        else if (j == N - 1)
        {
            if (grid[i][j - 1] == 1.0)
                count++;
            if (grid[i - 1][j - 1] == 1.0)
                count++;
            if (grid[i - 1][j] == 1.0)
                count++;
            if (grid[i - 1][0] == 1.0)
                count++;
            if (grid[i][0] == 1.0)
                count++;
            if (grid[0][0] == 1.0)
                count++;
            if (grid[0][j] == 1.0)
                count++;
            if (grid[0][j - 1] == 1.0)
                count++;
        }
        // Verificando se a célula está na borda inferior central
        else
        {
            if (grid[i][j - 1] == 1.0)
                count++;
            if (grid[i - 1][j - 1] == 1.0)
                count++;
            if (grid[i - 1][j] == 1.0)
                count++;
            if (grid[i - 1][j + 1] == 1.0)
                count++;
            if (grid[i][j + 1] == 1.0)
                count++;
            if (grid[0][j + 1] == 1.0)
                count++;
            if (grid[0][j] == 1.0)
                count++;
            if (grid[0][j - 1] == 1.0)
                count++;
        }
    }
    else
    {
        // Verificando se a célula está na borda esquerda
        if (j == 0)
        {
            if (grid[i - 1][j] == 1.0)
                count++;
            if (grid[i - 1][j + 1] == 1.0)
                count++;
            if (grid[i][j + 1] == 1.0)
                count++;
            if (grid[i + 1][j + 1] == 1.0)
                count++;
            if (grid[i + 1][j] == 1.0)
                count++;
            if (grid[i + 1][N - 1] == 1.0)
                count++;
            if (grid[i][N - 1] == 1.0)
                count++;
            if (grid[i - 1][N - 1] == 1.0)
                count++;
        }
        // Verificando se a célula está na borda direita
        else if (j == N - 1)
        {
            if (grid[i - 1][j] == 1.0)
                count++;
            if (grid[i - 1][j - 1] == 1.0)
                count++;
            if (grid[i][j - 1] == 1.0)
                count++;
            if (grid[i + 1][j - 1] == 1.0)
                count++;
            if (grid[i + 1][j] == 1.0)
                count++;
            if (grid[i + 1][0] == 1.0)
                count++;
            if (grid[i][0] == 1.0)
                count++;
            if (grid[i - 1][0] == 1.0)
                count++;
        }
        // Verificando se a célula está no centro da matriz
        else
        {
            if (grid[i - 1][j - 1] == 1.0)
                count++;
            if (grid[i - 1][j] == 1.0)
                count++;
            if (grid[i - 1][j + 1] == 1.0)
                count++;
            if (grid[i][j + 1] == 1.0)
                count++;
            if (grid[i + 1][j] == 1.0)
                count++;
            if (grid[i + 1][j + 1] == 1.0)
                count++;
            if (grid[i + 1][j - 1] == 1.0)
                count++;
            if (grid[i][j - 1] == 1.0)
                count++;
        }
    }

    return count;
}

//NÃO UTILIZADA
// Função para calcular o número de células vivas em uma parte da matriz, isso é feito para paralelizar o calculo, assim, cada thread executa o calculo em x linhas
void *CalculateLivingCells(void *arg)
{
    struct ThreadData *data = (struct ThreadData *)arg;
    float **newGrid = data->newGrid;
    int start_row = data->start_row;
    int end_row = data->end_row;

    float local_sum = 0.0;

    for (int i = start_row; i < end_row; i++)
    {
        for (int j = 0; j < N; j++)
        {
            local_sum += newGrid[i][j];
        }
    }

    data->local_sum = local_sum;

    return NULL;
}

// Função que libera a memória alocada para a Matriz
void FreeGrid(float **grid)
{
    for (int i = 0; i < N; i++)
    {
        free(grid[i]);
    }
    free(grid);
}

// Função a ser executada por cada thread para calcular a próxima geração
void *CalculateNextGen(void *arg)
{
    struct ThreadData *data = (struct ThreadData *)arg;
    int count = 0;

    float local_sum = 0.0;

    int start_row = data->start_row;
    int end_row = data->end_row;

    for (int i = start_row; i < end_row; i++)
    {
        for (int j = 0; j < N; j++)
        {
            count = getNeighbors(data->grid, i, j);

            // Verificando se a célula está viva
            if (data->grid[i][j] == 1.0)
            {
                // Verificando se a célula tem menos de 2 vizinhos vivos --> Morrem por abandono
                if (count < 2)
                {
                    data->newGrid[i][j] = 0.0;
                }
                // Verificando se a célula tem 2 ou 3 vizinhos vivos --> Continua Viva
                else if (count == 2 || count == 3)
                {
                    data->newGrid[i][j] = 1.0;
                }
                // Verificando se a célula tem mais de 3 vizinhos vivos --> Morre por superpopulação
                else if (count >= 3)
                {
                    data->newGrid[i][j] = 0.0;
                }
            }
            else
            {
                // Verificando se a célula tem exatamente 3 vizinhos vivos --> Se torna Viva
                if (count == 3)
                {
                    data->newGrid[i][j] = 1.0;
                }
                else
                {
                    data->newGrid[i][j] = 0.0;
                }
            }
            // Somando a quantidade de células vivas de cada thread
            local_sum += data->newGrid[i][j];
        }
    }
    
    // Armazenando a quantidade de células vivas de cada thread
    data->local_sum = local_sum;

    return NULL;
}

int main()
{
    // Alocação dos Grids
    float **grid = (float **)malloc(N * sizeof(float *));
    float **newGrid = (float **)malloc(N * sizeof(float *));

    // Criação das Threads
    pthread_t threads[NUM_THREADS];
    struct ThreadData threadData[NUM_THREADS];

    // Alocação dos espaços da matriz.
    for (int i = 0; i < N; i++)
    {
        // Alocando a Matirz da Geração Atual - Grid
        grid[i] = (float *)malloc(N * sizeof(float));
        // Alocando a Matriz da Próxima Geração - NewGrid
        newGrid[i] = (float *)malloc(N * sizeof(float));
    }

    // Gerando a Matriz da Geração Atual
    GenerateGrid(grid);

    for (int t = 0; t < NUM_THREADS; t++)
    {
        // aqui separamos a carga de cada thread de forma que seja equilibrado
        threadData[t].start_row = t * (N / NUM_THREADS);
        threadData[t].end_row = (t + 1) * (N / NUM_THREADS);
    }

    clock_t start_time = clock();

    // loop para a criação das gerações
    for (int gen = 0; gen < GEN; gen++)
    {
        // -------------------------------------
        // REGIÃO PARALELA
        //--------------------------------------
        for (int t = 0; t < NUM_THREADS; t++)
        {
            threadData[t].grid = grid;
            threadData[t].newGrid = newGrid;
            pthread_create(&threads[t], NULL, CalculateNextGen, &threadData[t]);
        }

        // Esperando as threads terminarem
        for (int t = 0; t < NUM_THREADS; t++)
        {
            pthread_join(threads[t], NULL);
        }

        // -------------------------------------
        // FIM DA REGIÃO PARALELA
        //--------------------------------------

        // Trocando as matrizes de gerações
        float **temp = grid;
        grid = newGrid;
        newGrid = temp;

        // Somando as contagens locais para obter o total de células vivas
        float total_cells = 0.0;
        for (int t = 0; t < NUM_THREADS; t++)
        {
            total_cells += threadData[t].local_sum;
        }

        // Imprimindo o número de células vivas na geração atual
        printf("Geração %d: Número de células vivas = %.0f\n", gen + 1, total_cells);

        // Imprimindo a Matriz da Geração Atual   
        if(gen <= 5){
            printf("\n");
            PrintGrid(grid);
        }
    }

    // Parar o cronômetro
    clock_t end_time = clock();

    // Calcular o tempo decorrido em segundos
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Tempo total para as %d gerações: %.2f segundos\n", GEN, elapsed_time);

    // Liberando a memória alocada para as Matrizes
    FreeGrid(grid);
    FreeGrid(newGrid);

    return 0;
}