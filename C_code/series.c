#include <stdio.h>


#define n 7

int juegosAGanar;
int juegosCasa[n];
float Ph = 0.57;
float Pr = 0.49;

void llenarSeries(){
	for(int i =0; i<n; i++){
		juegosCasa[i] = 0;
	}
}

int calcularJuegosAGanar(int totalJuegos){
	int mitadJuegos = totalJuegos/2;
	return mitadJuegos+1;
}
void casosBase(float matrix[juegosAGanar+1][juegosAGanar+1], int tamMatriz){
	for(int i=0; i < tamMatriz;i++ ){
		matrix[0][i] = 1.0;
		matrix[i][0] = 0.0;
	}
}

void llenarMatriz(float matrix[juegosAGanar+1][juegosAGanar+1], int tamMatriz){
	for(int i =1; i<tamMatriz; i++ ){
		for(int j = 1; j<tamMatriz; j++){
			int juegoActual = juegosAGanar-i + juegosAGanar -j;
			if(juegosCasa[juegoActual] == 1){
				matrix[i][j] = (Ph * matrix[i-1][j]) + ((1-Ph) * matrix[i][j-1]);
			}
			else{
				matrix[i][j] = (Pr * matrix[i-1][j]) + ((1-Pr) * matrix[i][j-1]);
			}
		}
	}
}
void imprimirMatriz(float matrix[juegosAGanar+1][juegosAGanar+1]){
	for(int i = 0; i< juegosAGanar+1; i++){
		printf("%s\n", "");
		for (int j = 0; j < juegosAGanar+1; j++)
		{
			printf("%f    ", matrix[i][j]);
		}
	}
}
int  main(void){
	llenarSeries();
	juegosCasa[0] = 1;
	juegosCasa[1] = 1;
	juegosCasa[5] = 1;
	juegosCasa[6] = 1;
	juegosAGanar = calcularJuegosAGanar(n);
	printf("Juegos a ganar %d\n",juegosAGanar );
	float matrix[juegosAGanar+1][juegosAGanar+1];
	casosBase(matrix, juegosAGanar+1);
	llenarMatriz(matrix, juegosAGanar+1);
	imprimirMatriz(matrix);

	return 0;
}

