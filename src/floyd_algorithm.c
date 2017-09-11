#include <stdio.h>
#include <stdlib.h>

#define MIN(a, b) ((a) < (b) ? a : b) // return min a , b
#define path(a, b, c, d) ((a) != (b) ? d : c) 
#define INF 1000
#define MAX  256
#define TOTALNODE 5



void floydAux(int matrix[MAX][MAX], int matrixAux[MAX][MAX], int matrixP[MAX][MAX], int totalNode, int node)
{

	for (int i = 0; i < totalNode; i++)
	{	int p;
		for (int j = 0; j < totalNode; j++)
		{	
			p = MIN(matrix[i][j], matrix[i][node - 1] + matrix[node - 1][j]);
			matrixAux[i][j] = p;

			matrixP[i][j] = path(p, matrix[i][j], matrixP[i][j], node);

			
		}
	}

	for (int i = 0; i < totalNode; i++)
	{	
		for (int j = 0; j < totalNode; j++)
		{	
			matrix[i][j] = matrixAux[i][j];			
		}
	}



}

void printMatrix(int matrix[MAX][MAX], int totalNode)
{
 
	for (int i = 0; i < totalNode; i++)
	{
		for (int j = 0; j < totalNode; j++)
		{
			printf("%d\t", matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n\n\n");

}

void floyd(int matrix[MAX][MAX], int matrixP[MAX][MAX], int totalNode)
{

	printf("%s%d\n","Tabla de distancia #", 0);
	printMatrix(matrix, totalNode);

	
	for (int k = 1; k <= totalNode; k++)
	{
		
		int matrixAux[totalNode][totalNode];
		floydAux(matrix, matrixAux,matrixP, totalNode, k);
		printf("%s%d\n","Tabla de distancia #",k  );
		printMatrix(matrixAux, totalNode);

	}
	printf("%s\n","Tabla P" );
	printMatrix(matrixP, totalNode);
	
	
	
}




int main(int argc, char const *argv[])
{

	int matrix0[TOTALNODE][TOTALNODE] =  {
											{0, 6, INF, 4, 7},
											{9, 0, 7, INF, INF},
											{INF, 5, 0, INF, 14},
											{8, 1, INF, 0, 15},
											{2, INF, 2, 19, 0}

										};

	int matrix[MAX][MAX];
	int matrixP[MAX][MAX];

	for(int i = 0; i < TOTALNODE; i++)
	{	
		for (int j = 0; j < TOTALNODE; j++)
		{	
			
			matrix[i][j] = matrix0[i][j];
			matrixP[i][j] = 0;
		}
	}
		

	floyd(matrix, matrixP, TOTALNODE);


	return 0;
}