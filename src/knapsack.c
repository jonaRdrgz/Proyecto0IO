#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


GtkBuilder    *myBuilder; 
GtkWidget     *windowInitial;
GtkWidget     *windowCreateData;
GtkWidget     *windowFinal;
GtkWidget     *chooseFileButton;
GtkWidget     *spinButtonObject;
GtkWidget     *spinButtonCapacity;

//CreateData 
GtkWidget     ***tableD0;
GtkWidget 		*scrolledTable;
GtkWidget 		*columnD0;

GtkWidget     ***tableP;
GtkWidget 		*columnP;
GtkWidget 		*scrolledTableP;

GtkWidget     *label_table_DNumber;
GtkWidget     *label_betterPath;

GtkWidget 		*scrolledwindow_optimalSolution;
GtkWidget 		*scrolledwindow_itemCost;
GtkWidget 		*scrolledwindow_itemValue;

char bufferForFile[9];
int loadFileFlag = 0;
char *filename;

int begin = -1;
int end = -1;


//final
GtkWidget       *filenameEntry;
GtkWidget       *windowSave;


//Guardar Archivo
FILE *infoFile;
FILE *infoFileAux;

const char *alphabetNodes[27]={"A","B","C","D","E","F","G","H","I","J",
"K","L","M","N","Ñ","O","P","Q","R","S","T","U","V","W","X","Y","Z"};

//
int totalObjects = 0;
int knapsackCapacity;
const char **header;

const char *alphabet[27]={"A","B","C","D","E","F","G","H","I","J",
"K","L","M","N","Ñ","O","P","Q","R","S","T","U","V","W","X","Y","Z"};

const char *headerMatriz[4] = {"","Valor","Costo","Cantidad"};

int totalObjectsCount = 0;
int Capacity = 0;

//

int inputNumberNodes;
int numberNodes = 0;
int totalNodes = 0;
int totalCycles = 0;




typedef struct  {
   char color[2];
   int value;	
   int cost;	
   int totalObjects;
} Objects;

Objects *objectProblem;

void on_window_main_destroy(){gtk_main_quit();}
void on_window_initial_knapsack_destroy() {
	gtk_main_quit();
}

void on_window_tableData_destroy() {
  free(tableD0);
	gtk_main_quit();
}

void on_window_finalTable_destroy() {
  free(tableP);
  gtk_main_quit();
}


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

void setTotalObjectsCount(int totalObjects,int knapsackCapacit) {
	totalObjectsCount = totalObjects;
	Capacity = knapsackCapacit;
}	


void createTableD0 ()
{

	tableD0 = calloc(totalObjects,sizeof(GtkWidget**));
	columnD0 = gtk_grid_new ();


	for(int j = 0; j < totalObjects; j++) {
		tableD0[j] = calloc(10,sizeof(GtkWidget*));
	}

	gtk_container_add(GTK_CONTAINER(scrolledTable), columnD0);

	for(int i = 0; i < totalObjects; i++)
	{
		for(int j = 0; j < 4; j++)
		{	
			tableD0[i][j] = gtk_entry_new();
			gtk_entry_set_width_chars(GTK_ENTRY(tableD0[i][j]),10);
			gtk_grid_attach (GTK_GRID(columnD0),tableD0[i][j], j, i, 1, 1);


			if (i == 0 && j != 0){
		        gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),headerMatriz[j]);
		        gtk_widget_set_name(tableD0[i][j],"rowHeader");
		        gtk_widget_set_sensitive(tableD0[i][j],FALSE);
	      	}
	      	if (j ==0 && i!=0){
	        	gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),alphabet[i-1]);
	      	}
	      	if (j ==3 && i!=0){
	        	gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),"INF");
	      	}
		}
	}
	gtk_widget_set_sensitive(tableD0[0][0],FALSE);
  	gtk_widget_set_name(tableD0[0][0],"rowHeader");
	gtk_widget_show_all(windowCreateData); 
}

void createFinalTable(Objects finalMatrix[knapsackCapacity+1][totalObjects-1],int knapsackCapacityInt) {
  tableP = calloc(knapsackCapacityInt,sizeof(GtkWidget**));

  columnP = gtk_grid_new ();
  gtk_container_add (GTK_CONTAINER (scrolledTableP), columnP);

  for(int j = 0; j < knapsackCapacityInt; j++) {
    tableP[j] = calloc(totalObjects,sizeof(GtkWidget*));
  }

  for(int i =0; i < knapsackCapacityInt; i++) 
  {
    for(int j=0; j < totalObjects; j++) 
    {
      char rowStr[10];
      char value[10];
      char total[10];

      tableP[i][j] = gtk_entry_new();
      gtk_entry_set_width_chars(GTK_ENTRY( tableP[i][j]),10);
      gtk_widget_set_sensitive( tableP[i][j],FALSE);
      gtk_grid_attach (GTK_GRID (columnP),tableP[i][j] , j, i, 1, 1);
      if (i == 0 && j != 0){
        gtk_entry_set_text (GTK_ENTRY(tableP[i][j]),header[j]);
        gtk_widget_set_name(tableP[i][j],"rowHeader");
      }
      if (j ==0 && i!=0){
        sprintf(rowStr, "%d", i-1);
        gtk_entry_set_text (GTK_ENTRY(tableP[i][j]),rowStr);
        gtk_widget_set_name(tableP[i][j],"rowHeader");
      }
      if (j != 0 && i != 0) {
        Objects object = finalMatrix[i-1][j-1];
        

        char setText[10000];
        char xValue[10];
        sprintf(value, "%d", object.value);
        sprintf(total, "%d", object.totalObjects);
        sprintf(xValue, "%d", j);

        strcpy(setText, value);
        strcat(setText, ", x");
        strcat(setText, xValue);
        strcat(setText, " = ");
        strcat(setText, total);
  
        gtk_entry_set_text (GTK_ENTRY(tableP[i][j]),setText);
        if (strcmp(object.color, "R") == 0) {
          gtk_widget_set_name(tableP[i][j],"notGetObject");
        }
        else {
          gtk_widget_set_name(tableP[i][j],"getObject");
        }
      }
    }
  }
  gtk_widget_set_sensitive(tableP[0][0],FALSE);
  gtk_widget_set_name(tableP[0][0],"rowHeader");
  gtk_widget_show_all(windowFinal);
}


void setTableFile(int Matriz[totalObjects-1][3]){
	tableD0 = calloc(totalObjects,sizeof(GtkWidget**));


	columnD0 = gtk_grid_new ();
	gtk_container_add (GTK_CONTAINER (scrolledTable), columnD0);

	for(int j = 0; j < totalObjects; j++) {
		tableD0[j] = calloc(totalObjects,sizeof(GtkWidget*));
	}

	for(int i =0; i < totalObjects; i++) 
	{
		for(int j=0; j < 4; j++) 
		{
			char str[100];
			tableD0[i][j] = gtk_entry_new();
			gtk_entry_set_width_chars(GTK_ENTRY(tableD0[i][j]),10);
			gtk_grid_attach (GTK_GRID (columnD0),tableD0[i][j] , j, i, 1, 1);

			if(i!=0 && j==0){
				gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),header[i]); 
			}

			if (i == 0 && j != 0){
				gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),headerMatriz[j]);
				gtk_widget_set_name(tableD0[i][j],"rowHeader");
				gtk_widget_set_sensitive(tableD0[i][j],FALSE);
			}
			if (j !=0 && i!=0) {
				if(Matriz[i-1][j-1]==100000){
					gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),"INF");
				}
				else{
					sprintf(str, "%d", Matriz[i-1][j-1]);
					gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),str);
				}
				
			}
		}
	}
	gtk_widget_set_sensitive(tableD0[0][0],FALSE);
  	gtk_widget_set_name(tableD0[0][0],"rowHeader");
	gtk_widget_show_all(windowCreateData); 
}

void appendChar(int _val) {
	if (strlen(bufferForFile) == 0) {
		char dato = (char)_val;
		char auxiliar[] = {dato,'\0'};
		strcat(bufferForFile,auxiliar);
	} else{
		char dato = (char)_val;
		char auxiliar[]={dato,'\0'};
		strcat(bufferForFile,auxiliar);
	}
}

void cleanBuffer(){
	memset(bufferForFile,'\0',strlen(bufferForFile));
}

int getNext() {
	cleanBuffer();

	int ch;

	while (feof(infoFileAux)==0) {   
		ch = fgetc(infoFileAux);
		if (ch=='\n') {
			return 0;
		}
		if (ch==';') {
			return 1;
		}
		appendChar(ch);
	}
	return 0;
}

int loadData(char *filename)
{
	infoFile = fopen(filename, "r");
	int ch;


	while(feof(infoFile) == 0) {  
		ch = fgetc(infoFile);
		if (ch == '\n'){
			totalObjectsCount ++;
		}
	}

	
	fclose(infoFile);
	return totalObjectsCount;
}


void createTablesFromFile(){
	int matrizAux[totalObjects-1][3];
	header = malloc(totalObjects * sizeof(char*));
	infoFileAux = fopen(filename, "r");

	int i = 0;
	int j = 0;
	int flag = 0;
	int accion = getNext();

	while (i < totalObjects) {
		while (j < 4) {
	 		if (accion == 1 && i!=0 && flag==1) {
			 	int valor = atoi(bufferForFile);
			 	printf("%d\n",valor );
		 		matrizAux[i-1][j-1] = valor;
		 	}
		 	if (flag == 0 && accion==1 && i !=0) {
		 		char * data = malloc(1000);
	 			strcpy(data,bufferForFile);
				header[i] = data;
		 		flag = 1;
		 	}
		 	j ++;
		 	accion = getNext();	
	 	}
		j = 0;
	 	i ++;
	 	flag = 0;
	 	accion = getNext();

	}
	setTableFile(matrizAux);
}

void createInfoFile(char *filename) {

    infoFile = fopen(filename,"w+");
    for(int i =0; i < totalObjects; i++) 
  {
        for(int j=0; j < 4; j++) 
        {
          if(strcmp(gtk_entry_get_text(GTK_ENTRY(tableD0[i][j])),"INF")==0){
          	fprintf(infoFile,"%d;", 100000);
          }
          else{
          	fprintf(infoFile,"%s;",(gtk_entry_get_text(GTK_ENTRY(tableD0[i][j]))));
          }
          
        }
         fprintf(infoFile,"\n");
  }
  fclose(infoFile);
}

void saveFile()
{


    char filename[1000] = "examples/Mochila/";

    int len = gtk_entry_get_text_length (GTK_ENTRY(filenameEntry));
    if(len != 0 )
    {
    	
    	strcat(filename,gtk_entry_get_text (GTK_ENTRY(filenameEntry)));
    	strcat(filename,".txt");
    	
    	createInfoFile(filename);

    	printf("%s\n","nose" );
    	gtk_entry_set_text(GTK_ENTRY(filenameEntry),"");

    	//gtk_widget_show_all(windowSave);
    }
}

int loadFile()
{

	filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(chooseFileButton));

	int flag = loadData(filename);

	return flag;

}

void createMatrixFile()
{


		gtk_widget_hide(windowInitial);
		loadFileFlag = 1;
		totalObjects = loadFile();
		printf("TOtal%d\n",totalObjects );

		createTablesFromFile();

		gtk_widget_show_all(windowCreateData);
	
}

void destroy()
{
	gtk_widget_hide(windowSave);
}

void createMatrix()
{
	totalObjects = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(spinButtonObject));
	createTableD0();
	gtk_widget_hide(windowInitial);
  	gtk_widget_show_now(windowCreateData);
}
void createOptimalSolutionLabel(int optimalSolution[totalObjects]) {
  char setText[1000000];
  char setCost[1000000];
  char setValue[1000000];
  char xvalue[5];
  char arrayValue[5];
  char arrayCost[5];
  char arrayText[5];
  char knapValue[5];

  strcpy(setText, "Z = ");
  sprintf(arrayText, "%d", optimalSolution[0]);
  strcat(setText,arrayText);

  strcpy(setValue, "Z = ");
  strcpy(setCost, " ");

  for (int i = 1; i < totalObjects; ++i)
  {
    Objects object = objectProblem[i-1];

    //Solucion Optima
    sprintf(arrayText, "%d", optimalSolution[i]);
    sprintf(xvalue, "%d", i);
    strcat(setText, "  X" );
    strcat(setText,xvalue);
    strcat(setText, " = " );
    strcat(setText,arrayText);

    //Maximizar
    sprintf(arrayValue, "%d", object.value);
    sprintf(xvalue, "%d", i);
    strcat(setValue,arrayValue);
    strcat(setValue, "  X" );
    strcat(setValue,xvalue);
    strcat(setValue, " + " );
    
    //Sujeto a
    sprintf(arrayCost, "%d", object.cost);
    sprintf(xvalue, "%d", i);
    strcat(setCost,arrayCost);
    strcat(setCost, "  X" );
    strcat(setCost,xvalue);
    strcat(setCost, " + " );
  }

  strcat(setCost, "  <= " );
  sprintf(knapValue, "%d", knapsackCapacity);
  strcat(setCost, knapValue);

  GtkWidget *labelOptimal = gtk_label_new(" ");
  gtk_label_set_text(GTK_LABEL(labelOptimal),setText);
  printf("%s\n", setText);
  gtk_widget_set_name(labelOptimal,"newLabel1");
  gtk_container_add (GTK_CONTAINER (scrolledwindow_optimalSolution), labelOptimal);

  GtkWidget *labelCost = gtk_label_new(" ");
  gtk_label_set_text(GTK_LABEL(labelCost),setCost);
  gtk_widget_set_name(labelCost,"newLabel2");
  printf("%s\n", setCost);
  gtk_container_add (GTK_CONTAINER (scrolledwindow_itemCost), labelCost);

  GtkWidget *labelValue = gtk_label_new(" ");
  printf("%s\n", setValue);
  gtk_label_set_text(GTK_LABEL(labelValue),setValue);
  gtk_widget_set_name(labelValue,"newLabel3");
  gtk_container_add (GTK_CONTAINER (scrolledwindow_itemValue), labelValue);
}

void createHeader() {
	header = malloc(totalObjects * sizeof(char*));
	for(int i =0; i < totalObjects; i++) 
	{
		for(int j=0; j < 4; j++) 
		{
			if (j == 0){
				header[i] = gtk_entry_get_text(GTK_ENTRY(tableD0[i][j]));
			}
		}
	}
	printf("header");
}
void createObjects(){

  objectProblem = malloc (totalObjects-1*sizeof(int));
  Objects data;
  for (int i = 1; i < totalObjects;i ++)
  {
    data.value = atoi(gtk_entry_get_text(GTK_ENTRY(tableD0[i][1])));
    data.cost = atoi(gtk_entry_get_text(GTK_ENTRY(tableD0[i][2])));
    if (strcmp(gtk_entry_get_text(GTK_ENTRY(tableD0[i][3])),"INF") == 0) {
      data.totalObjects = 100000;
    }
    else {
      data.totalObjects = atoi(gtk_entry_get_text(GTK_ENTRY(tableD0[i][3])));
    }
    objectProblem[i-1] = data;
  }
}

void suelo(Objects todos[totalObjectsCount]){
	for (int i=0;i<totalObjectsCount;i++){
		Objects objetos;
		objetos = todos[i];
		if (objetos.totalObjects>1){
			int suelo = objetos.cost / (Capacity-1);
			objetos.totalObjects = suelo;
		}
		

	}

}


void getOptimalSolution(int *optimalSolution, Objects matrizObjects[Capacity][totalObjectsCount]) {
	////printf("entro en getOptimalSolution\n");
	////printf("getOptimalSolution objetos: %d , capacidad: %d \n",totalObjectsCount, Capacity);
/*	for (int i = 0; i < Capacity; ++i)
	{
		for (int j = 0; j < totalObjectsCount; ++j)
		{
			Objects object = matrizObjects[i][j];
			////printf("Se creo el objeto en la posicion [%d][%d]: color: %s, valor: %d, costo: %d, cantidad: %d \n", i,j,  object.color, object.value, object.cost, object.totalObjects);
		}
	}*/

	int row = Capacity-1;
	int column = totalObjectsCount-1;

	Objects object = matrizObjects[row][column];
	optimalSolution[0] = object.value; //Solucion optima

	for (int i = totalObjectsCount; i > 0; --i)
	{	
		////printf("Se obtuvo el objeto en la posicion [%d][%d]: color: %s, valor: %d, costo: %d, cantidad: %d \n", row,column,  object.color, object.value, object.cost, object.totalObjects);
		
		optimalSolution[i] = object.totalObjects;

		if (strcmp(object.color, "R") == 0) {
			column--;
		}
		else {
			column--;
			row = row - object.cost;
		}
		object = matrizObjects[row][column];
	}

	for (int j = 0; j < totalObjectsCount+1; ++j)
		{
			////printf("Se creo el objeto en la posicion [%d]: valor: %d \n", j, optimalSolution[j] );
		}
}

void knapsackAlgorithm(Objects matrizObjects[Capacity][totalObjectsCount],Objects todos[totalObjectsCount]){
	int matriz[Capacity][totalObjectsCount];
	suelo (todos);

	int con = 0;

	Objects zero;
	zero.cost = 0;
	zero.value = 0;
	zero.totalObjects =0;

	for (int i=0;i<Capacity;i++){
		for (int j=0;j<totalObjectsCount;j++){
			matriz[i][j] =0;
		}
	}

	for (int i=0; i < Capacity; i++) {
		for (int j=0; j< totalObjectsCount; j++) {
			matrizObjects[i][j] = zero;
		}
	}
	
	for (int j=0; j<totalObjectsCount; j++){
		for (int i=0;i<Capacity;i++){
			Objects actual = todos[j];
			Objects guardar;
			guardar.cost = 0;
			guardar.value = 0;
			guardar.totalObjects =0;

			
			if (j==0) {
				if (actual.cost<=i) {
					strcpy (guardar.color,"V");
					int aux = con +1;
					if (actual.totalObjects==1){
						con =1;
					}
					else{
					if (aux * actual.cost == i && i!=0 && con <= actual.cost){
						con ++;

					}	}
				}
				else{ 
					strcpy (guardar.color,"R");
				}

				guardar.value = actual.value * con;
				guardar.totalObjects = con;
				guardar.cost = actual.cost * con;
					
				matriz[i][j] = con * actual.value;
				matrizObjects[i][j] = guardar;
			}

			else {
				if (actual.cost > i) {
					guardar = matrizObjects[i][j-1];
					guardar.totalObjects = 0;
					strcpy (guardar.color,"R");
					matrizObjects[i][j] =guardar;
					matriz[i][j] = matriz[i][j-1];

				}
				else {
					guardar.cost = matrizObjects[i][j-1].cost;
					strcpy (guardar.color,"R");		
					int optimo = matriz[i][j-1];
					int candidato = actual.value + matriz[i-actual.cost][j-1];
					int cuenta_candidato = 1;
					int cuenta_optimo = 0;  

					for ( int contador = 1; contador <= actual.totalObjects; contador++) {
						if (actual.cost * contador <= i) {
							if (candidato < (contador * actual.value + matriz[i-(contador*actual.cost)][j-1]) ) {
								candidato = contador * actual.value + matriz[i-(contador*actual.cost)][j-1];
								cuenta_candidato = contador;
							}
						}
						else {
							break;
						}
					}
					//printf(" OPtimo:%dCandidato:%d\n",optimo,candidato );

					if (optimo < candidato) {
						optimo = candidato;
						cuenta_optimo = cuenta_candidato;
						guardar.cost = actual.cost * cuenta_optimo;
						strcpy (guardar.color,"V");
					}					
						
					guardar.value = optimo;
					guardar.totalObjects = cuenta_optimo;
					matriz[i][j] = optimo;
					matrizObjects[i][j] = guardar;
				}
			}
		}
	}
}



void excecuteAlgorithm() {
	//int lenName = strlen(gtk_entry_get_text (GTK_ENTRY(g_entry_fileName))) + 21;
  
/*	char fileName[lenName]; */
/*	strcpy(fileName,"examples/Mochila/");*/
/*	strcat(fileName, gtk_entry_get_text (GTK_ENTRY(g_entry_fileName)));*/
/*	strcat(fileName, ".cvs");*/
/*	printf("%d\n",fileName);
	createFile(fileName);*/

	knapsackCapacity = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(spinButtonCapacity));
	printf("capacidad%d\n", knapsackCapacity);
	createHeader();

	createObjects();

	setTotalObjectsCount(totalObjects-1,knapsackCapacity+1);
	Objects finalMatrix[knapsackCapacity+1][totalObjects-1];
	knapsackAlgorithm(finalMatrix,objectProblem);

	int optimalSolution[totalObjects];
	getOptimalSolution(optimalSolution,finalMatrix);
	createOptimalSolutionLabel(optimalSolution);


	createFinalTable(finalMatrix,knapsackCapacity+2);
	gtk_widget_hide(windowCreateData);
	gtk_widget_show_now(windowFinal);

}




int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	myCSS();



	myBuilder = gtk_builder_new();
	gtk_builder_add_from_file (myBuilder, "glade/window_knapsack.glade", NULL);

	windowInitial = GTK_WIDGET(gtk_builder_get_object(myBuilder, "window_initial_knapsack"));
	spinButtonObject = GTK_WIDGET(gtk_builder_get_object(myBuilder, "spinButtonObject"));
	windowCreateData = GTK_WIDGET(gtk_builder_get_object(myBuilder, "window_createData"));

	gtk_spin_button_set_range (GTK_SPIN_BUTTON(spinButtonObject),1,100);
	gtk_spin_button_set_increments (GTK_SPIN_BUTTON(spinButtonObject),1,3);
	scrolledTable = GTK_WIDGET(gtk_builder_get_object(myBuilder, "scrolledwindow_table"));

	spinButtonCapacity = GTK_WIDGET(gtk_builder_get_object(myBuilder, "spinButtonCapacity"));
	gtk_spin_button_set_range (GTK_SPIN_BUTTON(spinButtonCapacity),1,100);
	gtk_spin_button_set_increments (GTK_SPIN_BUTTON(spinButtonCapacity),1,3);

	scrolledTableP = GTK_WIDGET(gtk_builder_get_object(myBuilder, "scrolledwindow_tableP"));


	windowFinal  = GTK_WIDGET(gtk_builder_get_object(myBuilder, "windowFinal"));
	filenameEntry = GTK_WIDGET(gtk_builder_get_object(myBuilder, "filenameEntry"));
	chooseFileButton = GTK_WIDGET(gtk_builder_get_object(myBuilder, "chooseFileButton"));
	scrolledwindow_itemValue = GTK_WIDGET(gtk_builder_get_object(myBuilder, "scrolledwindow_itemValue"));
	scrolledwindow_itemCost = GTK_WIDGET(gtk_builder_get_object(myBuilder, "scrolledwindow_itemCost"));
	scrolledwindow_optimalSolution = GTK_WIDGET(gtk_builder_get_object(myBuilder, "scrolledwindow_optimalSolution"));
/*
    
    windowSave = GTK_WIDGET(gtk_builder_get_object(myBuilder, "windowSave"));
*/
	gtk_builder_connect_signals(myBuilder, NULL);


	g_object_unref(myBuilder);

	gtk_widget_show_all(windowInitial);                
	gtk_main();
	return 0;
}


