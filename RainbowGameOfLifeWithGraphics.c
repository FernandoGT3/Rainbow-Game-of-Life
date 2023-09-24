/*
INTEGRANTES:
    - Thomas Lincoln Victor da Silva - 156603
    - Luiz Fernando de Cristo Moloni - 159325
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

// Definir as dimensões do tabuleiro
#define WIDTH 400
#define HEIGHT 400
#define SQUARE_SIZE 3

#define N 400  // Tamanho do Tabuleiro - Deve ser: 2048
#define GEN 1000 // Número de Gerações - Deve ser: 2000

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

// Função que gera uma matriz de tamanho NxN com valor igual a 0 e 1
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
            if (grid[i][j + 1] > 0.0)
                count++;
            if (grid[i + 1][j + 1] > 0.0)
                count++;
            if (grid[i + 1][j] > 0.0)
                count++;
            if (grid[i + 1][N - 1] > 0.0)
                count++;
            if (grid[i][N - 1] > 0.0)
                count++;
            if (grid[N - 1][N - 1] > 0.0)
                count++;
            if (grid[N - 1][j] > 0.0)
                count++;
            if (grid[N - 1][j + 1] > 0.0)
                count++;
        }
        // Verificando se a célula está na borda superior direita
        else if (j == N - 1)
        {
            if (grid[i][j - 1] > 0.0)
                count++;
            if (grid[i + 1][j - 1] > 0.0)
                count++;
            if (grid[i + 1][j] > 0.0)
                count++;
            if (grid[i + 1][0] > 0.0)
                count++;
            if (grid[i][0] > 0.0)
                count++;
            if (grid[N - 1][0] > 0.0)
                count++;
            if (grid[N - 1][j] > 0.0)
                count++;
            if (grid[N - 1][j - 1] > 0.0)
                count++;
        }
        // Verificando se a célula está na borda superior central
        else
        {
            if (grid[i][j - 1] > 0.0)
                count++;
            if (grid[i + 1][j - 1] > 0.0)
                count++;
            if (grid[i + 1][j] > 0.0)
                count++;
            if (grid[i + 1][j + 1] > 0.0)
                count++;
            if (grid[i][j + 1] > 0.0)
                count++;
            if (grid[N - 1][j + 1] > 0.0)
                count++;
            if (grid[N - 1][j] > 0.0)
                count++;
            if (grid[N - 1][j - 1] > 0.0)
                count++;
        }
    }
    else if (i == N - 1)
    { // Verificando se a célula está na borda inferior
        // Verificando se a célula está na borda inferior esquerda
        if (j == 0)
        {
            if (grid[i - 1][j] > 0.0)
                count++;
            if (grid[i - 1][j + 1] > 0.0)
                count++;
            if (grid[i][j + 1] > 0.0)
                count++;
            if (grid[0][j + 1] > 0.0)
                count++;
            if (grid[0][j] > 0.0)
                count++;
            if (grid[0][N - 1] > 0.0)
                count++;
            if (grid[i][N - 1] > 0.0)
                count++;
            if (grid[i - 1][N - 1] > 0.0)
                count++;
        }
        // Verificando se a célula está na borda inferior direita
        else if (j == N - 1)
        {
            if (grid[i][j - 1] > 0.0)
                count++;
            if (grid[i - 1][j - 1] > 0.0)
                count++;
            if (grid[i - 1][j] > 0.0)
                count++;
            if (grid[i - 1][0] > 0.0)
                count++;
            if (grid[i][0] > 0.0)
                count++;
            if (grid[0][0] > 0.0)
                count++;
            if (grid[0][j] > 0.0)
                count++;
            if (grid[0][j - 1] > 0.0)
                count++;
        }
        // Verificando se a célula está na borda inferior central
        else
        {
            if (grid[i][j - 1] > 0.0)
                count++;
            if (grid[i - 1][j - 1] > 0.0)
                count++;
            if (grid[i - 1][j] > 0.0)
                count++;
            if (grid[i - 1][j + 1] > 0.0)
                count++;
            if (grid[i][j + 1] > 0.0)
                count++;
            if (grid[0][j + 1] > 0.0)
                count++;
            if (grid[0][j] > 0.0)
                count++;
            if (grid[0][j - 1] > 0.0)
                count++;
        }
    }
    else
    {
        // Verificando se a célula está na borda esquerda
        if (j == 0)
        {
            if (grid[i - 1][j] > 0.0)
                count++;
            if (grid[i - 1][j + 1] > 0.0)
                count++;
            if (grid[i][j + 1] > 0.0)
                count++;
            if (grid[i + 1][j + 1] > 0.0)
                count++;
            if (grid[i + 1][j] > 0.0)
                count++;
            if (grid[i + 1][N - 1] > 0.0)
                count++;
            if (grid[i][N - 1] > 0.0)
                count++;
            if (grid[i - 1][N - 1] > 0.0)
                count++;
        }
        // Verificando se a célula está na borda direita
        else if (j == N - 1)
        {
            if (grid[i - 1][j] > 0.0)
                count++;
            if (grid[i - 1][j - 1] > 0.0)
                count++;
            if (grid[i][j - 1] > 0.0)
                count++;
            if (grid[i + 1][j - 1] > 0.0)
                count++;
            if (grid[i + 1][j] > 0.0)
                count++;
            if (grid[i + 1][0] > 0.0)
                count++;
            if (grid[i][0] > 0.0)
                count++;
            if (grid[i - 1][0] > 0.0)
                count++;
        }
        // Verificando se a célula está no centro da matriz
        else
        {
            if (grid[i - 1][j - 1] > 0.0)
                count++;
            if (grid[i - 1][j] > 0.0)
                count++;
            if (grid[i - 1][j + 1] > 0.0)
                count++;
            if (grid[i][j + 1] > 0.0)
                count++;
            if (grid[i + 1][j] > 0.0)
                count++;
            if (grid[i + 1][j + 1] > 0.0)
                count++;
            if (grid[i + 1][j - 1] > 0.0)
                count++;
            if (grid[i][j - 1] > 0.0)
                count++;
        }
    }

    return count;
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
            if (grid[i][j] > 0.0)
            {
                // Verificando se a célula tem menos de 2 vizinhos vivos --> Morrem por abandono
                if (count < 2)
                {
                    newGrid[i][j] = 0.0;
                }
                // Verificando se a célula tem 2 ou 3 vizinhos vivos --> Continua Viva
                else if (count == 2 || count == 3)
                {
                    newGrid[i][j] = grid[i][j];
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
                    float value = count / 8.0;
                    if(value > 1.0)
                        value = 1.0;
                    newGrid[i][j] = value;
                }
                else
                {
                    newGrid[i][j] = 0.0;
                }
            }
        }
    }
}

float LivingCells(float **grid)
{
    float sum = 0.0;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if(grid[i][j] > 0.0)
                sum++;
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

void DrawBoard(SDL_Renderer *renderer, float **grid)
{
    // Limpar a tela com a cor de fundo (branco)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Definir as cores para preto e branco
    SDL_Color BlackColor = {0, 0, 0, 255};
    SDL_Color WhiteColor = {255, 255, 255, 255};
    SDL_Color GrayColor = {105, 105, 105, 255};
    SDL_Color LightgrayColor = {211, 211, 211, 255};

    SDL_Color color;

    // Desenhar o tabuleiro de xadrez baseado no grid
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            // Calcular as coordenadas do quadrado
            int x = j * SQUARE_SIZE;
            int y = i * SQUARE_SIZE;

            // Escolher a cor com base no valor do grid
            if(grid[i][j] > 0.7 && grid[i][j] <= 1.0)
                color = WhiteColor;
            else if(grid[i][j] > 0.3 && grid[i][j] <= 0.7)
                color = LightgrayColor;
            else if(grid[i][j] > 0.0 && grid[i][j] <= 0.3)
                color = GrayColor;
            else
                color = BlackColor;

            // Configurar a cor de desenho
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

            // Desenhar o quadrado
            SDL_Rect square = {x, y, SQUARE_SIZE, SQUARE_SIZE};
            SDL_RenderFillRect(renderer, &square);
        }
    }

    // Atualizar o renderer
    SDL_RenderPresent(renderer);
}

int main()
{

    SDL_Window *window;
    SDL_Renderer *renderer;

    // Inicializar o SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erro ao inicializar o SDL: %s", SDL_GetError());
        return 1;
    }

    // Criar uma janela
    if (SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erro ao criar janela e renderer: %s", SDL_GetError());
        return 1;
    }

    // Alocando a Matirz da Geração Atual - Grid
    float **grid = (float **)malloc(N * sizeof(float *));
    for (int i = 0; i < N; i++)
    {
        grid[i] = (float *)malloc(N * sizeof(float));
    }

    // Alocando a Matriz da Próxima Geração - NewGrid
    float **newGrid = (float **)malloc(N * sizeof(float *));
    for (int i = 0; i < N; i++)
    {
        newGrid[i] = (float *)malloc(N * sizeof(float));
    }

    time_t start, end;

    // Gerando a Matriz da Geração Atual
    GenerateGrid(grid);

    // Pegando a quantidade de vizinhos vivos de cada célula na posição i,j
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            getNeighbors(grid, i, j);
        }
    }

    // clock_t start_time = clock();
    time(&start);

    // Criando a Nova Geração
    for (int generation = 0; generation < GEN; generation++)
    {
        CreateNextGen(grid, newGrid);

        // copia a nova geração para a geração atual
        float **temp = grid;
        grid = newGrid;
        newGrid = temp;

        // Chame a função para desenhar o tabuleiro
        DrawBoard(renderer, grid);

        // PrintGrid(grid);
        printf("Geração %d: %.0f células vivas\n", generation + 1, LivingCells(grid));

        // Aguarde um curto período de tempo para exibir a geração
    }

    // Parar o cronômetro
    // clock_t end_time = clock();
    time(&end);

    // double elapsed_time = (double)(end_time - start_time)/CLOCKS_PER_SEC;

    // Calcular o tempo decorrido em segundos
    //printf("Tempo total para as %d gerações: %.2f segundos\n", GEN, elapsed_time);
    printf("Tempo total para as %d gerações: %.2f segundos\n", GEN, difftime(end, start));

    // Liberando a memória alocada para as Matrizes
    FreeGrid(grid);
    FreeGrid(newGrid);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}