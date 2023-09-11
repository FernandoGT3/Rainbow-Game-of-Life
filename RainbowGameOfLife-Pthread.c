#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define N 50         // Tamanho do Tabuleiro - Deve ser: 2048
#define GEN 4        // Número de Gerações - Deve ser: 2000
#define NUM_THREADS 20 // Número de threads para paralelização

// Função para imprimir a Matriz
void PrintGrid(float **grid)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%.f|", grid[i][j]);
        }
        printf("\n");
    }
}

// Função que gera uma matriz de tamanho NxN com valor igual a 1

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

// ... (Outras funções do seu código)

// Função que copia a nova geração para a geração atual
void CopyGrid(float **grid, float **newGrid)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            grid[i][j] = newGrid[i][j];
        }
    }
}

// Função que cria a próxima geração
void CreateNextGen(float **grid, float **newGrid)
{
    int count = 0;

    // Pegando a quantidade de vizinhos vivos de cada célula na posição i,j
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            count = getNeighbors(grid, i, j);

            // Verificando se a célula está viva
            if (grid[i][j] == 1.0)
            {
                // Verificando se a célula tem menos de 2 vizinhos vivos --> Morrem por abandono
                if (count < 2)
                {
                    newGrid[i][j] = 0.0;
                }
                // Verificando se a célula tem 2 ou 3 vizinhos vivos --> Continua Viva
                else if (count == 2 || count == 3)
                {
                    newGrid[i][j] = 1.0;
                }
                // Verificando se a célula tem mais de 3 vizinhos vivos --> Morre por superpopulação
                else if (count >= 3)
                {
                    newGrid[i][j] = 0.0;
                }
            }
            else
            {
                // Verificando se a célula tem exatamente 3 vizinhos vivos --> Se torna Viva
                if (count == 3)
                {
                    newGrid[i][j] = 1.0;
                }
                else
                {
                    newGrid[i][j] = 0.0;
                }
            }
        }
    }

    CopyGrid(grid, newGrid);
}

float LivingCells(float **grid)
{
    float sum = 0.0;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            sum += grid[i][j];
        }
    }

    return sum;
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

// Estrutura para passar dados para as threads
struct ThreadData
{
    float **grid;
    float **newGrid;
    int start_row;
    int end_row;
};

// ...

// Função a ser executada por cada thread para calcular a próxima geração
void *CalculateNextGen(void *arg)
{
    struct ThreadData *data = (struct ThreadData *)arg;

    for (int i = data->start_row; i < data->end_row; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int count = getNeighbors(data->grid, i, j);

            if (data->grid[i][j] == 1.0)
            {
                if (count < 2)
                {
                    data->newGrid[i][j] = 0.0;
                }
                else if (count == 2 || count == 3)
                {
                    data->newGrid[i][j] = 1.0;
                }
                else if (count >= 3)
                {
                    data->newGrid[i][j] = 0.0;
                }
            }
            else
            {
                if (count == 3)
                {
                    data->newGrid[i][j] = 1.0;
                }
                else
                {
                    data->newGrid[i][j] = 0.0;
                }
            }
        }
    }

    return NULL;
}

int main()
{
    // Alocando a Matriz da Geração Atual - Grid
    float **grid = (float **)malloc(N * sizeof(float *));
    for (int i = 0; i < N; i++)
    {
        grid[i] = (float *)malloc(N * sizeof(float));
    }

    // Gerando a Matriz da Geração Atual
    GenerateGrid(grid);

    // Inicialização das threads
    pthread_t threads[NUM_THREADS];
    struct ThreadData thread_data[NUM_THREADS];

    // Matriz para armazenar a nova geração de cada thread
    float ***newGrids = (float ***)malloc(NUM_THREADS * sizeof(float **));
    for (int i = 0; i < NUM_THREADS; i++)
    {
        newGrids[i] = (float **)malloc(N * sizeof(float *));
        for (int j = 0; j < N; j++)
        {
            newGrids[i][j] = (float *)malloc(N * sizeof(float));
        }
    }

    // ...

    clock_t start_time = clock();
    // Execução das threads
    for (int generation = 0; generation < GEN; generation++)
    {
        for (int i = 0; i < NUM_THREADS; i++)
        {
            thread_data[i].grid = grid;
            thread_data[i].newGrid = newGrids[i];
            thread_data[i].start_row = i * (N / NUM_THREADS);
            thread_data[i].end_row = (i + 1) * (N / NUM_THREADS);
            pthread_create(&threads[i], NULL, CalculateNextGen, &thread_data[i]);
        }

        for (int i = 0; i < NUM_THREADS; i++)
        {
            pthread_join(threads[i], NULL);
        }

        // Combinação dos resultados das threads
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                // Inicializa newGrid com 0.0
                grid[i][j] = 0.0;
                for (int t = 0; t < NUM_THREADS; t++)
                {
                    // Soma os resultados das threads
                    grid[i][j] += newGrids[t][i][j];
                }
            }
        }

        // PrintGrid(grid);
        printf("Geração %d: %.0f células vivas\n", generation + 1, LivingCells(grid));
    }

    // ...
    // Parar o cronômetro
    clock_t end_time = clock();

    // Calcular o tempo decorrido em segundos
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Tempo total para as %d gerações: %.2f segundos\n", GEN, elapsed_time);
    
        // Liberação de memória
        FreeGrid(grid);

    for (int i = 0; i < NUM_THREADS; i++)
    {
        for (int j = 0; j < N; j++)
        {
            free(newGrids[i][j]);
        }
        free(newGrids[i]);
    }
    free(newGrids);

    return 0;
}
