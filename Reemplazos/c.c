#include <stdio.h>
#include <stdlib.h>
#include "Reemplazo.c"


int main(int argc, char const *argv[])
{
	/*reemplazo(vidaUtilEquipo, manteReventaAux);*/

	int costoInicial = 500;
	vidaUtilEquipo = 3;
	plazoProyecto = 3;

	
	int manteReventaAux[3][2] = { 
									{400, 30},
									{300, 40},
									{250, 60}
		
								 };
	


	
	GX tablaFinal[plazoProyecto + 1];
	reemplazo(vidaUtilEquipo, manteReventaAux, costoInicial, tablaFinal);
	for (int i = 0; i < plazoProyecto + 1; ++i)
	{
		printf("%d %d ||%d||", tablaFinal[i].idGx, tablaFinal[i].valor, tablaFinal[i].ganancia);
		for(int k = 0; k < tablaFinal[i].numeroLlaves; k++)
		{
			printf("%d| ", tablaFinal[i].llaves[k]);
		}
		printf("\n");
	}

	return 0;
}
