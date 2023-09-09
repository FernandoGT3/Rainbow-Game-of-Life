#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 50                // Tamanho do Tabuleiro - Deve ser: 2048
#define GEN 5               // Número de Gerações - Deve ser: 2000

//Função para imprimir a Matriz
void PrintGrid(float **grid){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            printf("%.1f ", grid[i][j]);
        }
        printf("\n");
    }
}

// Função que gera uma matriz de tamanho NxN com valor igual a 1
void GenerateGrid(float **grid){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            grid[i][j] = 0.0;
        }
    }

    //Figura de um Glider a partir da posição (1,1)
    int line = 1, col = 1;

    grid[line][col + 1] = 1.0;
    grid[line + 2][col] = 1.0;
    grid[line + 1][col + 2] = 1.0;
    grid[line + 2][col + 1] = 1.0;
    grid[line + 2][col + 2] = 1.0;

    //Figura de um R-Pentomino a partir da posição (10,30)
    line = 10, col = 30;

    grid[line  ][col + 1] = 1.0;
    grid[line  ][col + 2] = 1.0;
    grid[line + 1][col  ] = 1.0;
    grid[line + 1][col + 1] = 1.0;
    grid[line + 2][col + 1] = 1.0;
}

//Função que retorna a quantidade de vizinhos vivos de cada célula
void getNeighbors(float **grid, int i, int j){
    int count = 0;

    //Verificando se a célula está na borda superior
    if(i == 0){
        //Verificando se a célula está na borda superior esquerda
        if(j == 0){
            if(grid[i][j + 1] == 1.0) count++;
            if(grid[i + 1][j + 1] == 1.0) count++;
            if(grid[i + 1][j] == 1.0) count++;
            if(grid[i + 1][N - 1] == 1.0) count++;
            if(grid[i][N - 1] == 1.0) count++;
            if(grid[N - 1][N - 1] == 1.0) count++;
            if(grid[N - 1][j] == 1.0) count++;
            if(grid[N - 1][j + 1] == 1.0) count++;
        }
        //Verificando se a célula está na borda superior direita
        else if(j == N - 1){
            if(grid[i][j - 1] == 1.0) count++;
            if(grid[i + 1][j - 1] == 1.0) count++;
            if(grid[i + 1][j] == 1.0) count++;
            if(grid[i + 1][0] == 1.0) count++;
            if(grid[i][0] == 1.0) count++;
            if(grid[N - 1][0] == 1.0) count++;
            if(grid[N - 1][j] == 1.0) count++;
            if(grid[N - 1][j - 1] == 1.0) count++;
        }
        //Verificando se a célula está na borda superior central
        else{
            if(grid[i][j - 1] == 1.0) count++;
            if(grid[i + 1][j - 1] == 1.0) count++;
            if(grid[i + 1][j] == 1.0) count++;
            if(grid[i + 1][j + 1] == 1.0) count++;
            if(grid[i][j + 1] == 1.0) count++;
            if(grid[N - 1][j + 1] == 1.0) count++;
            if(grid[N - 1][j] == 1.0) count++;
            if(grid[N - 1][j - 1] == 1.0) count++;
        }
    }else if (i == N - 1){ //Verificando se a célula está na borda inferior
        //Verificando se a célula está na borda inferior esquerda
        if(j == 0){
            if(grid[i - 1][j] == 1.0) count++;
            if(grid[i - 1][j + 1] == 1.0) count++;
            if(grid[i][j + 1] == 1.0) count++;
            if(grid[0][j + 1] == 1.0) count++;
            if(grid[0][j] == 1.0) count++;
            if(grid[0][N - 1] == 1.0) count++;
            if(grid[i][N - 1] == 1.0) count++;
            if(grid[i - 1][N - 1] == 1.0) count++;
        }
        //Verificando se a célula está na borda inferior direita
        else if(j == N - 1){
            if(grid[i][j - 1] == 1.0) count++;
            if(grid[i - 1][j - 1] == 1.0) count++;
            if(grid[i - 1][j] == 1.0) count++;
            if(grid[i - 1][0] == 1.0) count++;
            if(grid[i ][0] == 1.0) count++;
            if(grid[0][0] == 1.0) count++;
            if(grid[0][j] == 1.0) count++;
            if(grid[0][j - 1] == 1.0) count++;
        }
        //Verificando se a célula está na borda inferior central
        else{
            if(grid[i][j - 1] == 1.0) count++;
            if(grid[i - 1][j - 1] == 1.0) count++;
            if(grid[i - 1][j] == 1.0) count++;
            if(grid[i - 1][j + 1] == 1.0) count++;
            if(grid[i][j + 1] == 1.0) count++;
            if(grid[0][j + 1] == 1.0) count++;
            if(grid[0][j] == 1.0) count++;
            if(grid[0][j - 1] == 1.0) count++;
        }
    }else{
        //Verificando se a célula está na borda esquerda
        if(j == 0){
            if(grid[i - 1][j] == 1.0) count++;
            if(grid[i - 1][j + 1] == 1.0) count++;
            if(grid[i][j + 1] == 1.0) count++;
            if(grid[i + 1][j + 1] == 1.0) count++;
            if(grid[i + 1][j] == 1.0) count++;
            if(grid[i + 1][N - 1] == 1.0) count++;
            if(grid[i][N - 1] == 1.0) count++;
            if(grid[i - 1][N - 1] == 1.0) count++;
        }
        //Verificando se a célula está na borda direita
        else if(j == N - 1){
            if(grid[i - 1][j] == 1.0) count++;
            if(grid[i - 1][j - 1] == 1.0) count++;
            if(grid[i][j - 1] == 1.0) count++;
            if(grid[i + 1][j - 1] == 1.0) count++;
            if(grid[i + 1][j] == 1.0) count++;
            if(grid[i + 1][0] == 1.0) count++;
            if(grid[i][0] == 1.0) count++;
            if(grid[i - 1][0] == 1.0) count++;
        }
        //Verificando se a célula está no centro da matriz
        else{
            if(grid[i - 1][j - 1] == 1.0) count++;
            if(grid[i - 1][j] == 1.0) count++;
            if(grid[i - 1][j + 1] == 1.0) count++;
            if(grid[i][j + 1] == 1.0) count++;
            if(grid[i + 1][j] == 1.0) count++;
            if(grid[i + 1][j + 1] == 1.0) count++;
            if(grid[i + 1][j - 1] == 1.0) count++;
            if(grid[i][j - 1] == 1.0) count++;
        }
    }
}

//Função que libera a memória alocada para a Matriz
void FreeGrid(float **grid){
    for(int i = 0; i < N; i++){
        free(grid[i]);
    }
    free(grid);
}

int main(){
    //Alocando a Matirz da Geração Atual - Grid
    float **grid = (float **)malloc(N * sizeof(float *));
    for(int i = 0; i < N; i++){
        grid[i] = (float*)malloc(N * sizeof(float));
    }

    //Alocando a Matriz da Próxima Geração - NewGrid
    float **newGrid = (float **)malloc(N * sizeof(float *));
    for(int i = 0; i < N; i++){
        newGrid[i] = (float*)malloc(N * sizeof(float));
    }

    GenerateGrid(grid);
    PrintGrid(grid);

    FreeGrid(grid);
    FreeGrid(newGrid);

    return 0;
}