#include <stdio.h>

#define numeroNodos  4


float probabilidades[numeroNodos];

/*void llenarArreglo(){

	for(int i =0; i<numeroNodos; i++){
		probabilidades[i] = ;
	}
}*/

/*int calcularJuegosAGanar(int totalJuegos){
	int mitadJuegos = totalJuegos/2;
	return mitadJuegos+1;
}*/


void casosBase(float matrix[numeroNodos+2] [numeroNodos+1]){
	for (int i = 0; i<numeroNodos+2;i++){
		for(int j = 0; j<numeroNodos+1; j++){
			matrix[i][j] = -1;
			
		}
	}
	for(int i=1; i < numeroNodos+2;i++ ){
		matrix[i][i-1] = 0;
		
	}
	for(int i=1; i < numeroNodos+1;i++ ){
		matrix[i][i] = probabilidades[i];
		
	}
}
float calcularProbabilidadesIJ(int i, int j){
	float suma =0;
	for(int k =i; k<=j; k++){
		suma += probabilidades[k];
	}
	return suma;
}


float llenarCasilla(float matrix[numeroNodos+2][numeroNodos+1], int i, int j){
	float menor =100000000000000;
	for(int k = i; k<=j; k++){
		float newMenor = matrix[i][k-1] + matrix[k+1][j] + calcularProbabilidadesIJ(i,j);
		if(newMenor<=menor){
			menor = newMenor;
			printf("K ganador: %d\n",k );
		}
	}
	printf("ganador: %f\n",menor );
	return menor;
}



void llenarMatriz(float matrix[numeroNodos+2][numeroNodos+1]){
	
	for (int i = 1; i<numeroNodos+2){

	}
			matrix[i][j] = llenarCasilla(matrix, i, j);
	
	
	
}



void imprimirMatriz(float matrix[numeroNodos+2][numeroNodos+1]){
	for(int i = 0; i< numeroNodos+2; i++){
	
		for (int j = 0; j < numeroNodos+1; j++)
		{
			printf("%f    ", matrix[i][j]);
		}
		printf("%s\n","" );
	}
}

void main(){
	probabilidades[0] = 0;
	probabilidades[1] = 0.18;
	probabilidades[2] = 0.32;
	probabilidades[3]= 0.39;
	probabilidades[4] = 0.11;
	float matrix[numeroNodos+2][numeroNodos+1];
	casosBase(matrix);
	llenarMatriz(matrix);
	imprimirMatriz(matrix);

}