#include <stdio.h>
#include <stdlib.h>

typedef struct 
{
	int row;
	int colum;
	int key;
	int value;
	char name[40];
	
}matrix;




int numberDi;


int *matrixDi;




int **matrixMul;
int **matrixKey;



int **fillMatrixMul( int **pMatrixMul, int numberDi, int pMatrixDi[numberDi + 1])
{
	pMatrixMul = (int**) malloc(numberDi * sizeof(int *));

	for(int i = 0; i < numberDi ; i++)
	{
		pMatrixMul[i] = (int*) malloc(numberDi  * sizeof(int));
	}

	for (int i = 0; i < numberDi ; i++)
	{
		for (int j = 0; j < numberDi ; j++)
		{
			if(i + 1 == j)
			{
				pMatrixMul[i][j] = pMatrixDi[i] * pMatrixDi[i + 1] * pMatrixDi[j + 1]; 
			}
			else
			{
				pMatrixMul[i][j] = 0;
			}
			
		}
	}

	return pMatrixMul;
	
}


int **fillMatrixKey( int **pMatrixKey, int numberDi )
{
	pMatrixKey = (int**) malloc(numberDi  * sizeof(int *));

	for(int i = 0; i < numberDi ; i++)
	{
		pMatrixKey[i] = (int*) malloc(numberDi  * sizeof(int));
	}

	for (int i = 0; i < numberDi ; i++)
	{
		for (int j = 0; j < numberDi ; j++)
		{
			pMatrixKey[i][j] = (i  <= j) ?  0 : 0; 
			
		}
	}

	return pMatrixKey;
	
}

int calKeyDi(int pMatrixDi[numberDi + 1], int pI, int pJ, int pKey)
{
	int result;
	result = pMatrixDi[pI] * pMatrixDi[pJ + 1] * pMatrixDi[pKey];

	return result;
}

matrix minMatrixCalIJ(int **pMatrixMul, int pMatrixDi[numberDi + 1], int pI, int pJ)
{

	int min;
	int minAux;

	matrix pMatrix;
	min = pMatrixMul[pI][pI] + pMatrixMul[pI + 1][pJ] + calKeyDi(pMatrixDi, pI, pJ, pI + 1);





	pMatrix.key = pI;

	for (int k = pI + 1; k < pJ; k++)
	{
		minAux = pMatrixMul[pI][k] + pMatrixMul[k + 1][pJ] + calKeyDi(pMatrixDi, pI, pJ, k + 1);
		
		
		if(minAux < min )	
		{
			min = minAux;
			pMatrix.key = k;
		}
	}

	

	pMatrix.value = min;
	pMatrix.key += 1;

	
	return pMatrix;
}


int **finalMatrix(int **pMatrixMul, int **pMatrixKey, int pMatrixDi[numberDi + 1])
{
	matrix n;

	for (int j = 1; j < numberDi; j++)
	{
		for (int i = j - 1 ; i >= 0 ; i--)
		{
			n = minMatrixCalIJ(pMatrixMul, pMatrixDi, i, j);
			pMatrixMul[i][j] = n.value; 
			pMatrixKey[i][j] = n.key;

		}
	}

	return pMatrixMul;
} 






int main(int argc, char const *argv[])
{
	
	numberDi = 4 ;
	int matrixDi[4 + 1] = {20, 2, 30, 12, 8};


	matrixMul =  fillMatrixMul(matrixMul, numberDi, matrixDi);
	matrixKey   = fillMatrixKey(matrixKey, numberDi);

	matrixMul = finalMatrix(matrixMul, matrixKey, matrixDi);






	for (int i = 0; i < numberDi; i++)
	{
		for (int j = 0; j < numberDi; j++)
		{
			printf("%d\t",matrixMul[i][j]);
		}

		printf("\n\n");
	}


	printf("\n\n\n\n");

	for (int i = 0; i < numberDi; i++)
	{
		for (int j = 0; j < numberDi; j++)
		{
			printf("%d\t",matrixKey[i][j]);
		}

		printf("\n\n");
	}
	

	return 0;
}