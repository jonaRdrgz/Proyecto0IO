#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "arbolesAlgoritmo.c"

int inputNumberNode;

int *globalIJ;

const char *rowHeader[3] = {"Nombre","Filas","Columnas"};
const char *rowHeader1[3] = {"Posición","Texto","Peso"};
const char *alphabetNodes[27]={"A","B","C","D","E","F","G","H","I","J",
"K","L","M","N","Ñ","O","P","Q","R","S","T","U","V","W","X","Y","Z"};

GtkWidget       *windowInitial;
GtkWidget       *windowSetNode;
GtkWidget 		*windowFinal;

GtkWidget       ***initialTable;
GtkWidget       *tableData;
GtkWidget       *containerTable;


GtkWidget       ***initialTableA;
GtkWidget       *tableDataA;
GtkWidget       *containerTableA;

GtkWidget       ***initialTableB;
GtkWidget       *tableDataB;
GtkWidget       *containerTableB;

GtkWidget       ***initialTableFinal;
GtkWidget       *tableDataFinal;
GtkWidget       *containerTableFinal;

GtkWidget       *filenameEntry;
GtkWidget       *saveFile;

GtkWidget       *chooseFileButton;


FILE            *infoFile;








node 			*nodeList;


GtkWidget       *spinNumberNode;



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



void createInfoFile(char *filename) 
{
  infoFile = fopen(filename,"w+");

  fprintf(infoFile, "%d\n", inputNumberNode);
 
  for(int i = 1; i < inputNumberNode + 1; i++) 
  {
      fprintf(infoFile, "%s", gtk_entry_get_text(GTK_ENTRY(initialTable[i][0])));
      fprintf(infoFile,"\n");

      fprintf(infoFile,"%.3f",atof(gtk_entry_get_text(GTK_ENTRY(initialTable[i][1]))));
      fprintf(infoFile,"\n");
  }
  fclose(infoFile);
}







void saveFilebtn()
{


    char filename[1000] = "examples/arbol/";

    int len = gtk_entry_get_text_length (GTK_ENTRY(filenameEntry));
    if(len != 0 )
    {
      
      strcat(filename,gtk_entry_get_text (GTK_ENTRY(filenameEntry)));
      strcat(filename,".txt");
      createInfoFile(filename);
      
      gtk_entry_set_text(GTK_ENTRY(filenameEntry),"");

      //gtk_widget_show_all(windowSave);
    }
}


void saveFile_btn()
{
  saveFilebtn();
}

void calculateProbaNodeList()
{
	float result;
	result = 0;

	float temp;
	temp = 0;


	for (int i = 0; i < inputNumberNode; i++)
	{
		result += nodeList[i].value;
	}

	for (int i = 0; i < inputNumberNode; i++)
	{
		temp = nodeList[i].value / result;
		nodeList[i].value = temp;
	}

}




void sortNodeList() 
{
	node temp;
	
	
	for(int i=0; i < inputNumberNode; i++)
	{
		for(int j = i+1; j < inputNumberNode; j++) 
		{
			node nodeList1 = nodeList[i];
			node nodeList2 = nodeList[j];
			if(strcmp(nodeList1.name, nodeList2.name) > 0) {
				temp = nodeList1;
				nodeList[i] = nodeList2;
				nodeList[j] = temp;
			}

		}
  	}




}

void createNode() 
{
  nodeList = calloc(inputNumberNode,sizeof(node));
  
  for (int row = 1; row < inputNumberNode + 1; row ++)
  {
   
   node pNode;
    strcpy(pNode.name, gtk_entry_get_text(GTK_ENTRY(initialTable[row][0])));
    pNode.value = atof(gtk_entry_get_text(GTK_ENTRY(initialTable[row][1])));


    nodeList[row - 1] = pNode;
}


}

void createTableA() 
{
  int keys = inputNumberNode + 2;

  initialTableA= calloc(keys,sizeof(GtkWidget**));

  tableDataA = gtk_grid_new ();
  gtk_container_add (GTK_CONTAINER (containerTableA), tableDataA);

  for(int j = 0; j < keys; j++) {
    initialTableA[j] = calloc(keys,sizeof(GtkWidget*));
  }

  for(int row =0; row < keys; row++) 
  {
    for(int column=0; column < keys; column++) 
    {
      char str[10];
      initialTableA[row][column] = gtk_entry_new();
      gtk_entry_set_width_chars(GTK_ENTRY(initialTableA[row][column]),8);
      gtk_grid_attach (GTK_GRID (tableDataA),initialTableA[row][column] , column, row, 1, 1);
      gtk_widget_set_sensitive(initialTableA[row][column],FALSE);


      if(row == 0 && column == 0)
      {
      	sprintf(str, "%d", 0);
        gtk_entry_set_text (GTK_ENTRY(initialTableA[row][column]),str);
      }

      else if(row == 0)
      {
      	sprintf(str, "%d", column - 1);
        gtk_entry_set_text (GTK_ENTRY(initialTableA[row][column]),str);
      }

      else if(column == 0)
      {
      	sprintf(str, "%d", row);
        gtk_entry_set_text (GTK_ENTRY(initialTableA[row][column]),str);
      }

      else
      {
      	float number;
      	number = matrixProb[row - 1][column - 1];
      	sprintf(str, "%.3f", number);
      	gtk_entry_set_text (GTK_ENTRY(initialTableA[row][column]),str);
      }
        
      
    }
  }
  

}

void createTableB() 
{
  int keys = inputNumberNode + 2;

  initialTableB= calloc(keys,sizeof(GtkWidget**));

  tableDataB = gtk_grid_new ();
  gtk_container_add (GTK_CONTAINER (containerTableB), tableDataB);

  for(int j = 0; j < keys; j++) {
    initialTableB[j] = calloc(keys,sizeof(GtkWidget*));
  }

  for(int row =0; row < keys; row++) 
  {
    for(int column = 0 ; column < keys; column++) 
    {
      char str[10];
      initialTableB[row][column] = gtk_entry_new();
      gtk_entry_set_width_chars(GTK_ENTRY(initialTableB[row][column]),8);
      gtk_grid_attach (GTK_GRID (tableDataB),initialTableB[row][column] , column, row, 1, 1);
      gtk_widget_set_sensitive(initialTableB[row][column],FALSE);

      if(row == 0 && column == 0)
      {
      	sprintf(str, "%d", 0);
        gtk_entry_set_text (GTK_ENTRY(initialTableB[row][column]),str);
      }

      else if(row == 0)
      {
      	sprintf(str, "%d", column - 1);
        gtk_entry_set_text (GTK_ENTRY(initialTableB[row][column]),str);
      }

      else if(column == 0)
      {
      	sprintf(str, "%d", row);
        gtk_entry_set_text (GTK_ENTRY(initialTableB[row][column]),str);
      }

      else
      {
      	int number;
      	number = matrixKey[row - 1][column - 1];
      	sprintf(str, "%d", number);
      	gtk_entry_set_text (GTK_ENTRY(initialTableB[row][column]),str);
      }

      
    }
  }
  
}


void createTableFinal() 
{
  int keys = inputNumberNode + 1;
  initialTableFinal = calloc(keys,sizeof(GtkWidget**));

  tableDataFinal = gtk_grid_new ();
  gtk_container_add (GTK_CONTAINER (containerTableFinal), tableDataFinal);

  for(int j = 0; j < keys; j++) {
    initialTableFinal[j] = calloc(3,sizeof(GtkWidget*));
  }

  for(int row =0; row < keys; row++) 
  {
    for(int column=0; column < 3; column++) 
    {
    	char str[40];
      initialTableFinal[row][column] = gtk_entry_new();
      gtk_entry_set_width_chars(GTK_ENTRY(initialTableFinal[row][column]),8);
      gtk_grid_attach (GTK_GRID (tableDataFinal),initialTableFinal[row][column] , column, row, 1, 1);
      gtk_widget_set_sensitive(initialTableFinal[row][column],FALSE);

      if (row == 0){
        gtk_entry_set_text (GTK_ENTRY(initialTableFinal[row][column]),rowHeader1[column]);
        gtk_widget_set_name(initialTableFinal[row][column],"header");
        
      }
      else if(column == 0)
      {
      	
      	sprintf(str, "%d", row);
      	gtk_entry_set_text (GTK_ENTRY(initialTableFinal[row][column]),str);
      }
      else if(column == 1)
      {
      	sprintf(str, "%s",nodeList[row - 1].name);
      	gtk_entry_set_text (GTK_ENTRY(initialTableFinal[row][column]),str);
      }
      else 
      {
      	float number;
      	number = nodeList[row - 1].value;
      	sprintf(str, "%.2f", number);
      	gtk_entry_set_text (GTK_ENTRY(initialTableFinal[row][column]),str);
      }
    }
  }
  
}




void getTree()
{
	
	createNode();
	sortNodeList();
	calculateProbaNodeList();



	numberNode = inputNumberNode + 1;
	float nodeProb[inputNumberNode];

	for (int i = 0; i < inputNumberNode; i++)
	{
		nodeProb[i] = nodeList[i].value;
	}






	matrixProb  = fillMatrixProb(matrixProb, numberNode, nodeProb);
	matrixKey   = fillMatrixKey(matrixKey, numberNode);

	matrixProb = calProbMatixFinal(matrixProb, matrixKey,  numberNode);


	createTableA();
	createTableB();
	createTableFinal();
	gtk_widget_hide(windowSetNode);
	gtk_widget_show_all(windowFinal);

	
}



static void print_text (GtkWidget *widget, gpointer p)
{
    int * pointer = p;
    //printf("%d", pointer[0]);
    //gtk_widget_set_sensitive(initialTable[pointer[0]+1][pointer[1]-1],FALSE);
    gtk_entry_set_text (GTK_ENTRY(initialTable[pointer[0]+1][pointer[1]-1]),gtk_entry_get_text(GTK_ENTRY(widget)));
    //printf("%s\n","Hola" );
}




void createSetNodeData()
{
  int keys = inputNumberNode + 1;
  initialTable = calloc(keys,sizeof(GtkWidget**));

  tableData = gtk_grid_new ();
  gtk_container_add (GTK_CONTAINER (containerTable), tableData);

  for(int j = 0; j < keys; j++) {
    initialTable[j] = calloc(2,sizeof(GtkWidget*));
  }

  for(int row =0; row < keys; row++) 
  {
    for(int column=0; column < 3; column++) 
    {
      initialTable[row][column] = gtk_entry_new();
      gtk_entry_set_width_chars(GTK_ENTRY(initialTable[row][column]),8);
      gtk_grid_attach (GTK_GRID (tableData),initialTable[row][column] , column, row, 1, 1);

      if (row == 0){
        gtk_entry_set_text (GTK_ENTRY(initialTable[row][column]),rowHeader[column]);
        gtk_widget_set_name(initialTable[row][column],"rowHeader");
        gtk_widget_set_sensitive(initialTable[row][column],FALSE);
      }
      if(column == 2  && row >0 && row<keys-1){
      	globalIJ = (int*) malloc(2 * sizeof(int));
      	globalIJ[0] = row;
      	globalIJ[1] = column;
        
      	g_signal_connect(GTK_ENTRY(initialTable[row][column]), "activate", G_CALLBACK(print_text),(gpointer) globalIJ);
      	
      }
      if (column==0 && row>0){
        gtk_entry_set_text (GTK_ENTRY(initialTable[row][column]),alphabetNodes[row-1]);
      }
    }
  }

  for(int row =0; row < keys; row++) 
  {
    for(int column=0; column < 3; column++) 
    {
      
      
      if(column == 2  && row >0 && row<keys-1){
         gtk_widget_set_sensitive(initialTable[row+1][column-1],FALSE);
      }
    }
  }
}



void loadData(char *filename)
{
  infoFile = fopen(filename, "r");
  if(infoFile != NULL)
  {
    fscanf(infoFile, "%i", &inputNumberNode);
    
  }
}

void createSetNodeDataFile()
{
  char str[40];
  char number[10];
  for(int row =0; row < inputNumberNode + 1; row++) 
  {
    
    if (row != 0){
      fscanf(infoFile, "%s", str);
      fscanf(infoFile, "%s", number);
      gtk_entry_set_text (GTK_ENTRY(initialTable[row][0]),str);
      gtk_entry_set_text (GTK_ENTRY(initialTable[row][1]),number);
    
    }
  
  }
}


void loadFile()
{
  char *filename;
  filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(chooseFileButton));
  loadData(filename);
  gtk_widget_hide(windowInitial); 
  createSetNodeData();
  createSetNodeDataFile();
  gtk_widget_show_all(windowSetNode);
}



void createTableNode()
{
	gtk_widget_hide(windowInitial); 
	inputNumberNode= gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(spinNumberNode));
	createSetNodeData();

	gtk_widget_show_all(windowSetNode);
}
void on_window_main_destroy(){gtk_main_quit();}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
	/* code */

  myCSS();

	GtkBuilder      *builder; 
     
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/window_matrices.glade", NULL);

    windowInitial = GTK_WIDGET(gtk_builder_get_object(builder, "window_initial_matrices"));
    gtk_builder_connect_signals(builder, NULL);

    windowSetNode = GTK_WIDGET(gtk_builder_get_object(builder, "window_setNodos"));
    gtk_builder_connect_signals(builder, NULL);

    windowFinal = GTK_WIDGET(gtk_builder_get_object(builder, "windowFinal"));
    gtk_builder_connect_signals(builder, NULL);


    containerTable = GTK_WIDGET(gtk_builder_get_object(builder, "containerTable"));
    gtk_builder_connect_signals(builder, NULL);

    containerTableA = GTK_WIDGET(gtk_builder_get_object(builder, "tableA"));
    gtk_builder_connect_signals(builder, NULL);

    containerTableB = GTK_WIDGET(gtk_builder_get_object(builder, "tableB"));
    gtk_builder_connect_signals(builder, NULL);

    //containerTableFinal = GTK_WIDGET(gtk_builder_get_object(builder, "tableFinal"));
    //gtk_builder_connect_signals(builder, NULL);

    filenameEntry = GTK_WIDGET(gtk_builder_get_object(builder, "filenameEntry"));
    gtk_builder_connect_signals(builder, NULL);

    saveFile = GTK_WIDGET(gtk_builder_get_object(builder, "saveFile"));
    gtk_builder_connect_signals(builder, NULL);

    chooseFileButton = GTK_WIDGET(gtk_builder_get_object(builder, "chooseFileButton"));
    gtk_builder_connect_signals(builder, NULL);
 
 
    spinNumberNode = GTK_WIDGET(gtk_builder_get_object(builder, "spinBtn_numberNode"));
    gtk_spin_button_set_range (GTK_SPIN_BUTTON(spinNumberNode),1,100);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(spinNumberNode),1,1);





    g_object_unref(builder);
    gtk_widget_show(windowInitial); 

    gtk_main();

	return 0;
}