#include <stdio.h>
#include <stdlib.h>

typedef struct 
{
	float value;
	int key;
	
}node;




int numberNode;

float **matrixProb;
int **matrixKey;

void sortNode(node p[numberNode][numberNode])
{

}


float **fillMatrixProb( float **pMatrixProb, int numberNode, float pNodeProb[numberNode])
{
	pMatrixProb = (float**) malloc(numberNode * sizeof(float *));

	for(int i = 0; i < numberNode; i++)
	{
		pMatrixProb[i] = (float*) malloc(numberNode * sizeof(float));
	}

	for (int i = 0; i < numberNode; i++)
	{
		for (int j = 0; j < numberNode; j++)
		{
			pMatrixProb[i][j] = (i + 1 == j) ? pNodeProb[i] : 0; 
			
		}
	}

	return pMatrixProb;
	
}


int **fillMatrixKey( int **pMatrixKey, int numberNode)
{
	pMatrixKey = (int**) malloc(numberNode * sizeof(int *));

	for(int i = 0; i < numberNode; i++)
	{
		pMatrixKey[i] = (int*) malloc(numberNode * sizeof(int));
	}

	for (int i = 0; i < numberNode; i++)
	{
		for (int j = 0; j < numberNode; j++)
		{
			pMatrixKey[i][j] = (i  <= j) ?  0 : -1; 
			
		}
	}

	return pMatrixKey;
	
}

float calProb(float **pMatrixProb,int pI, int pJ)
{
	float result = 0;


	for(int i = pI; i < pJ; i++)
	{
		result += pMatrixProb[i][i + 1];
	}


	return result;
}



node min(float **pMatrixProb, int pI, int pJ)
{
	node pNode;
	float min = pMatrixProb[pI][pI] + pMatrixProb[pI + 1][pJ];
	float minAux;
	pNode.value = min;
	pNode.key = pI + 1;
	for (int k = pI + 1; k < pJ; k++)
	{
		minAux = pMatrixProb[pI][k] + pMatrixProb[k + 1][pJ];
		if(minAux < min )	
		{
			min = minAux;
			pNode.key = k + 1;
		}
	}

	pNode.value = min;
	printf("%d\n",pNode.key );


	return pNode;
}


float **calProbMatixFinal(float **pMatrixProb, int **pMatrixKey, int pNumberNode)
{
	node n;

	for (int j = 1; j < numberNode; j++)
	{
		for (int i = j - 1 ; i >= 0 ; i--)
		{
			n = min(pMatrixProb, i, j);
			pMatrixProb[i][j] = calProb(pMatrixProb, i, j) + n.value; 
			pMatrixKey[i][j] = n.key;
		}
	}

	return pMatrixProb;
}