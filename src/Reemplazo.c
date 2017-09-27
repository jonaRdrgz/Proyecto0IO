#include <stdio.h>
#include <stdlib.h>
#define colManteReventa 2

typedef struct  {
  int llaves[31];
  int valor;
  int ganancia;
  int numeroLlaves;
  int idGx;	
} GX ;

int vidaUtilEquipo = 3;

int plazoProyecto  = 5;






void llenar(int **pManteReventa, int pManteReventaAux[vidaUtilEquipo][2])
{
	for(int i = 0; i < vidaUtilEquipo; i++)
	{
		for(int j = 0; j < colManteReventa; j++)
		{
			pManteReventa[i][j] = pManteReventaAux[i][j];
		}
	}

}


int minValores(int *pValores, int pContador)
{
	int min = pValores[0];
	for(int i = 1; i < pContador; i++)
	{
		if(pValores[i] < min)
		{
			min = pValores[i];
		}
	}
	
	return min;
}




GX minCtx(int plazoProyecto, int *pCtx, GX pGx, GX tablaFinal[plazoProyecto], int vidaUtilEquipo)
{
	int contador = vidaUtilEquipo;
	int *valores = (int*)malloc((contador) * sizeof(int));
	int *llaves = (int*)malloc((contador) * sizeof(int));

	if(pGx.idGx == plazoProyecto)
	{
		pGx.valor = 0;
		pGx.llaves[0] = -1;
	}
	else if(pGx.idGx == plazoProyecto -1)
	{
		pGx.valor = pCtx[0] + tablaFinal[plazoProyecto - (pGx.idGx + 1)].valor;
		pGx.llaves[0] = pGx.idGx + 1;
		pGx.numeroLlaves += 1;
	}
	else
	{
		int min;
		for(int i = 1; i < vidaUtilEquipo + 1; i++)
		{
			if(i + pGx.idGx <= plazoProyecto)
			{
				valores[i - 1] = pCtx[i - 1] + tablaFinal[plazoProyecto - i - pGx.idGx].valor;
				llaves[i - 1] = pGx.idGx + i ;
				contador = i;
			}
		}
		
		min = minValores(valores, contador);
		
		for (int i = 0; i < vidaUtilEquipo ; i++)
		{
			if(min == valores[i])
			{
				pGx.llaves[pGx.numeroLlaves] = llaves[i];
				pGx.numeroLlaves += 1;
			}
		}

		pGx.valor = min;
		
		
	}

	return pGx;
}




void calcularCtx(int *pCtx, int **pManteReventa, int costoInicial)
{
	int acumuladoMante = 0;
	for(int i = 0; i < vidaUtilEquipo; i++)
	{
		acumuladoMante = 0;
		for(int k = 0; k < i + 1; k++)
		{
			acumuladoMante += pManteReventa[k][1];
		}

		pCtx[i] = (costoInicial + acumuladoMante) - pManteReventa[i][0];

	}

}



void reemplazo(int vidaUtilEquipo, int pManteReventaAux[vidaUtilEquipo][2], int costoInicial, GX tablaFinal[plazoProyecto + 1])
{
	int **manteReventa;//manteReventa -> col 0 -> reventa;
					  // 			  -> col 1 -> mantenimiento;
	int *ctx;
	ctx = (int*) malloc(vidaUtilEquipo * sizeof(int));
	manteReventa = (int**) malloc(vidaUtilEquipo * sizeof(int *));

	for(int i = 0; i < vidaUtilEquipo; i++)
	{
		manteReventa[i] = (int*) malloc(colManteReventa * sizeof(int));
	}

	llenar(manteReventa,pManteReventaAux);
	
	calcularCtx(ctx, manteReventa, costoInicial);
	


	

	for(int i = plazoProyecto; i >= 0; i--)
	{	
		GX gx;
		gx.idGx = i;
		gx.numeroLlaves = 0;
		gx = minCtx(plazoProyecto, ctx, gx, tablaFinal, vidaUtilEquipo);
		gx.ganancia = costoInicial - gx.valor;
		tablaFinal[plazoProyecto - i] = gx;
	}
	
	

	free(ctx);
	free(manteReventa);

	ctx = NULL;
	manteReventa = NULL;


}

