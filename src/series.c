#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
//#include "C_code/series.c"

#define gtk_spin_button_get_value_as_float gtk_spin_button_get_value

GtkBuilder      *myBuilder; 
GtkWidget       *windowInitial;
GtkWidget       *windowCreateData;
GtkWidget       *windowFinal;
//initiial
GtkWidget 		*loadFileButton;
GtkWidget 		*chooseFileButton;
GtkWidget  		*spinButtonGame;
GtkWidget 		*spinButtonGamePH;
GtkWidget 		*spinButtonGamePV;
GtkWidget 		*saveFileButton;
GtkWidget 		*filenameEntry;
GtkWidget 		*windowSave;
GtkWidget 		*labelA;
GtkWidget 		*labelB;

//final 
GtkWidget       ***tableP0;
GtkWidget 		*scrolledTableSerie;
GtkWidget 		*tableP;

//create Data
GtkWidget 		*execGameButton;
GtkWidget 		*scrolleGameSerieH;
GtkWidget 		**tableHV0;
GtkWidget 		*tableHV;

//Guardar Archivo
FILE *infoFile;

char loadGame[1000];

int inputNumberGames;

float inputPH;
float inputPV;

int juegosCasa[1000];

int juegosAGanar;

void on_window_main_destroy(){gtk_main_quit();}

void myCSS(void){
    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen *screen;

    provider = gtk_css_provider_new ();
    display = gdk_display_get_default ();
    screen = gdk_display_get_default_screen (display);
    gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    const gchar *myCssFile = "myStyle.css";
    GError *error = 0;

    gtk_css_provider_load_from_file(provider, g_file_new_for_path(myCssFile), &error);
    g_object_unref (provider);
}

void llenarSeries(){

	for(int i =0; i<inputNumberGames; i++){
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
				matrix[i][j] = (inputPH * matrix[i-1][j]) + ((1-inputPH) * matrix[i][j-1]);
			}
			else{
				matrix[i][j] = (inputPV * matrix[i-1][j]) + ((1-inputPV) * matrix[i][j-1]);
			}
		}
	}
}
void imprimirMatriz(float matrix[juegosAGanar+1][juegosAGanar+1]){
	for(int i = 0; i< juegosAGanar+1; i++){
	
		for (int j = 0; j < juegosAGanar+1; j++)
		{
			printf("%f    ", matrix[i][j]);
		}
	}
}


void createInfoFile(char *filename) {
  infoFile = fopen(filename,"w+");
  fprintf(infoFile,"%d\n",inputNumberGames);
  fprintf(infoFile,"%f\n",inputPH);
  fprintf(infoFile,"%f\n",inputPV);

  for(int i = 0; i < inputNumberGames; i++) 
  {
    fprintf(infoFile,"%d",juegosCasa[i]);
  }
  
  fclose(infoFile);
}



 	
void createTable()
{
	tableP0 = calloc(10,sizeof(GtkWidget**));
    tableP = gtk_grid_new ();
    for(int j = 0; j < 10; j++) {
    	tableP0[j] = calloc(10,sizeof(GtkWidget*));
  	}

  	llenarSeries();

  	for(int i = 0; i < inputNumberGames; i++)
	{
		if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(tableHV0[i]))){
			juegosCasa[i] = 1;
		}
		else {
			juegosCasa[i] = 0;
		}
	}
	

	juegosAGanar = calcularJuegosAGanar(inputNumberGames);
	float matrix[juegosAGanar+1][juegosAGanar+1];
	casosBase(matrix, juegosAGanar+1);
	llenarMatriz(matrix, juegosAGanar+1);




   	gtk_container_add(GTK_CONTAINER(scrolledTableSerie), tableP);

   	for(int i = 0; i < juegosAGanar + 1; i++)
   	{
   		for(int j = 0; j < juegosAGanar + 1; j++)
   		{	char number[14];
   			sprintf(number, "%.4f",matrix[i][j] );

   			tableP0[i][j] = gtk_entry_new();
   			gtk_entry_set_text (GTK_ENTRY(tableP0[i][j]), number);
   			gtk_entry_set_width_chars(GTK_ENTRY(tableP0[i][j]),8);
   			gtk_grid_attach (GTK_GRID(tableP),tableP0[i][j], j, i, 1, 1);
   			gtk_widget_set_sensitive(tableP0[i][j],FALSE);

   		}
   	}


   	char numberA[14];
   	sprintf(numberA, "%.4f",matrix[juegosAGanar][juegosAGanar] );

   	char probA[1000] = "La probabilidad de A\nde ganar es: ";
   	strcat(probA, numberA);

   	gtk_label_set_text(GTK_LABEL(labelA), probA);

   	char numberB[14];
   	sprintf(numberB, "%.4f",1 - matrix[juegosAGanar][juegosAGanar] );

   	char probB[1000] = "La probabilidad de B\nde ganar es: ";
   	strcat(probB, numberB);

   	gtk_label_set_text(GTK_LABEL(labelB), probB);

}

void createTableHV()
{
	tableHV0 = calloc(inputNumberGames,sizeof(GtkWidget*));
    tableHV = gtk_grid_new ();

    gtk_container_add (GTK_CONTAINER (scrolleGameSerieH), tableHV);

    char text[14];
  	char number[4];


    for(int j = 0; j < inputNumberGames ; j++) {

    	strcpy(text,"Game ");
	  	sprintf(number, "%d", j + 1);
	  	strcat(text,number);
	    tableHV0[j] = gtk_check_button_new_with_label (text); 
	    gtk_grid_attach (GTK_GRID (tableHV),tableHV0[j] ,j % 5 ,  j / 5  , 1, 1);
	    memset(text,'\0',strlen(text));
  	}
	
}

int loadData(char *filename)
{
	infoFile = fopen(filename, "r");
	if(infoFile != NULL)
	{
		fscanf(infoFile, "%i", &inputNumberGames);
		fscanf(infoFile, "%f", &inputPH);
		fscanf(infoFile, "%f", &inputPV);
		fscanf(infoFile, "%s", loadGame);

		fclose(infoFile);
		return 1;
	}
	return 0;
}

void saveFile()
{
	

	char filename[1000] = "examples/series/";
	
	int len = gtk_entry_get_text_length (GTK_ENTRY(filenameEntry));
	if(len != 0 )
	{
		
		strcat(filename,gtk_entry_get_text (GTK_ENTRY(filenameEntry)));
		strcat(filename,".txt");
		
	
		inputPH = gtk_spin_button_get_value_as_float (GTK_SPIN_BUTTON(spinButtonGamePH));
		inputPV = gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(spinButtonGamePV));
		for(int i = 0; i < inputNumberGames; i++)
		{
			if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(tableHV0[i]))){
				juegosCasa[i] = 1;
			}
			else {
				juegosCasa[i] = 0;
			}
		}

		createInfoFile(filename);
		
		gtk_entry_set_text(GTK_ENTRY(filenameEntry),"");

		gtk_widget_show_all(windowSave);
	}
	




}
void destroy()
{
	gtk_widget_hide(windowSave);
}

void createTableHV1()
{
	tableHV0 = calloc(inputNumberGames,sizeof(GtkWidget*));
    tableHV = gtk_grid_new ();

    gtk_container_add (GTK_CONTAINER (scrolleGameSerieH), tableHV);

    char text[14];
  	char number[4];

  	gtk_spin_button_set_value (GTK_SPIN_BUTTON(spinButtonGamePH),inputPH);

	gtk_spin_button_set_value (GTK_SPIN_BUTTON(spinButtonGamePV),inputPV);



    for(int j = 0; j < inputNumberGames ; j++) {

    	strcpy(text,"Game ");
	  	sprintf(number, "%d", j + 1);
	  	strcat(text,number);
	    tableHV0[j] = gtk_check_button_new_with_label (text); 
	    gtk_grid_attach (GTK_GRID (tableHV),tableHV0[j] ,j % 5 ,  j / 5  , 1, 1);
	    memset(text,'\0',strlen(text));
  	}

  	for (int i = 0; i < inputNumberGames; ++i)
  	{
  		if(loadGame[i] == '1')
  		{
  			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tableHV0[i]), TRUE);
  		}
  		else
  		{
  			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(tableHV0[i]), FALSE);
  		}
  	}
	
}

int loadFile()
{
	char *filename;
	filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(chooseFileButton));

	int flag = loadData(filename);

	return flag;

}

void createGameFile()
{
	

	
	if(loadFile() == 1)
	{
		gtk_widget_hide(windowInitial);
		createTableHV1();

		gtk_widget_show_all(windowCreateData);
	}
	else
	{

	}
	 

}



void createGame()
{
	gtk_widget_hide(windowInitial); 
	inputNumberGames = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(spinButtonGame));
	createTableHV();

	gtk_widget_show_all(windowCreateData);
	 

}



void execGame()
{
	gtk_widget_hide(windowCreateData);  

	inputPH = gtk_spin_button_get_value_as_float (GTK_SPIN_BUTTON(spinButtonGamePH));
	inputPV = gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(spinButtonGamePV));

	createTable();
	gtk_widget_show_all(windowFinal);  
	
}



int main(int argc, char *argv[])
{
   

    gtk_init(&argc, &argv);
    myCSS();


 
    myBuilder = gtk_builder_new();
    gtk_builder_add_from_file (myBuilder, "glade/window_series.glade", NULL);
 
    windowInitial = GTK_WIDGET(gtk_builder_get_object(myBuilder, "window_series"));

    windowCreateData = GTK_WIDGET(gtk_builder_get_object(myBuilder, "window_create_data_series"));

    windowFinal  = GTK_WIDGET(gtk_builder_get_object(myBuilder, "window_final_series"));


  

    spinButtonGame = GTK_WIDGET(gtk_builder_get_object(myBuilder, "spinButtonGame"));
    gtk_spin_button_set_range (GTK_SPIN_BUTTON(spinButtonGame),1,99);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(spinButtonGame),2,3);

    spinButtonGamePH = GTK_WIDGET(gtk_builder_get_object(myBuilder, "spinButtonGamePH"));
    gtk_spin_button_set_range (GTK_SPIN_BUTTON(spinButtonGamePH),0,1);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(spinButtonGamePH),0.01,1);

    spinButtonGamePV = GTK_WIDGET(gtk_builder_get_object(myBuilder, "spinButtonGamePV"));
    gtk_spin_button_set_range (GTK_SPIN_BUTTON(spinButtonGamePV),0,1);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(spinButtonGamePV),0.01,1);

    execGameButton =  GTK_WIDGET(gtk_builder_get_object(myBuilder, "execute_serie"));









    scrolleGameSerieH = GTK_WIDGET(gtk_builder_get_object(myBuilder, "scrolleGameSerieH"));
    saveFileButton = GTK_WIDGET(gtk_builder_get_object(myBuilder, "saveFileButton"));
    filenameEntry = GTK_WIDGET(gtk_builder_get_object(myBuilder, "filenameEntry"));
    
	scrolledTableSerie = GTK_WIDGET(gtk_builder_get_object(myBuilder, "gtkScrolledWindowTableSerie"));
    
    chooseFileButton = GTK_WIDGET(gtk_builder_get_object(myBuilder, "chooseFileButton"));
    loadFileButton = GTK_WIDGET(gtk_builder_get_object(myBuilder, "loadFileButton"));

    windowSave = GTK_WIDGET(gtk_builder_get_object(myBuilder, "windowSave"));

    labelA = GTK_WIDGET(gtk_builder_get_object(myBuilder, "PH1"));
    labelB = GTK_WIDGET(gtk_builder_get_object(myBuilder, "PH2"));


    gtk_builder_connect_signals(myBuilder, NULL);

  
    
    g_object_unref(myBuilder);
 
    gtk_widget_show_all(windowInitial);                
    gtk_main();
 
    return 0;
}


