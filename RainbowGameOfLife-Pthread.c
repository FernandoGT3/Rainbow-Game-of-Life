#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define N 2048        // Tamanho do Tabuleiro - Deve ser: 2048
#define GEN 500       // Número de Gerações - Deve ser: 2000
#define NUM_THREADS 4 // Número de threads para paralelização

// Estrutura para passar dados para as threads
struct ThreadData
{
    float **grid;
    float **newGrid;
    int start_row;
    int end_row;
    float local_sum;
};

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
// Função para calcular o número de células vivas em uma parte da matriz
void *CalculateLivingCells(void *arg)
{
    struct ThreadData *data = (struct ThreadData *)arg;
    float **grid = data->grid;
    int start_row = data->start_row;
    int end_row = data->end_row;

    float local_sum = 0.0;

    for (int i = start_row; i < end_row; i++)
    {
        for (int j = 0; j < N; j++)
        {
            local_sum += grid[i][j];
        }
    }

    data->local_sum = local_sum;

    return NULL;
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
    float **grid = (float **)malloc(N * sizeof(float *));
    float **newGrid = (float **)malloc(N * sizeof(float *));
    pthread_t threads[NUM_THREADS];
    struct ThreadData threadData[NUM_THREADS];
    clock_t start_time, end_time;

    for (int i = 0; i < N; i++)
    {
        grid[i] = (float *)malloc(N * sizeof(float));
        newGrid[i] = (float *)malloc(N * sizeof(float));
    }

    GenerateGrid(grid);

    start_time = clock();

    for (int gen = 0; gen < GEN; gen++)
    {
        // Criando threads para calcular a próxima geração
        for (int t = 0; t < NUM_THREADS; t++)
        {
            int start_row = t * (N / NUM_THREADS);
            int end_row = (t + 1) * (N / NUM_THREADS);
            threadData[t].grid = grid;
            threadData[t].newGrid = newGrid;
            threadData[t].start_row = start_row;
            threadData[t].end_row = end_row;
            pthread_create(&threads[t], NULL, CalculateNextGen, &threadData[t]);
        }

        // Esperando as threads terminarem
        for (int t = 0; t < NUM_THREADS; t++)
        {
            pthread_join(threads[t], NULL);
        }

        // Trocando as matrizes de gerações
        float **temp = grid;
        grid = newGrid;
        newGrid = temp;

        // Inicializando dados para o cálculo de células vivas
        for (int t = 0; t < NUM_THREADS; t++)
        {
            threadData[t].local_sum = 0.0;
        }

        // Criando threads para calcular células vivas
        for (int t = 0; t < NUM_THREADS; t++)
        {
            int start_row = t * (N / NUM_THREADS);
            int end_row = (t + 1) * (N / NUM_THREADS);
            threadData[t].start_row = start_row;
            threadData[t].end_row = end_row;
            pthread_create(&threads[t], NULL, CalculateLivingCells, &threadData[t]);
        }

        // Esperando as threads terminarem o cálculo de células vivas
        for (int t = 0; t < NUM_THREADS; t++)
        {
            pthread_join(threads[t], NULL);
        }

        // Somando as contagens locais para obter o total de células vivas
        float total_cells = 0.0;
        for (int t = 0; t < NUM_THREADS; t++)
        {
            total_cells += threadData[t].local_sum;
        }

        // Imprimindo o número de células vivas na geração atual
        printf("Geração %d: Número de células vivas = %.0f\n", gen + 1, total_cells);
    }

    end_time = clock();

    printf("Tempo de execução: %lf segundos\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
    FreeGrid(grid);
    FreeGrid(newGrid);

    return 0;
}