#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 10                // Tamanho do Tabuleiro - Deve ser: 2048
#define GEN 5               // Número de Gerações - Deve ser: 2000

//Função para imprimir a Matriz
void PrintGrid(float **grid){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            printf("%.0f ", grid[i][j]);
        }
        printf("\n");
    }
}

// Função que gera uma matriz de tamanho NxN com valor igual a 1
void GenerateGrid(float **grid){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            grid[i][j] = 1;
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