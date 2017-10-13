#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "matricesAlgoritmo.c"

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
GtkWidget     *label_betterPath;




char val[300000];





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

      fprintf(infoFile,"%d",atoi(gtk_entry_get_text(GTK_ENTRY(initialTable[i][1]))));
      fprintf(infoFile,"\n");
  }
  fprintf(infoFile,"%d",atoi(gtk_entry_get_text(GTK_ENTRY(initialTable[inputNumberNode][2]))));
  fclose(infoFile);
}







void saveFilebtn()
{


    char filename[1000] = "examples/matrices/";

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



void createMatrizDi(int matrixDi[inputNumberNode+1]) 
{
  
  for (int row = 0; row < inputNumberNode; row ++)
  {
   
    matrixDi[row] = atoi(gtk_entry_get_text(GTK_ENTRY(initialTable[row+1][1])));
    //printf(" %d %d\n",inputNumberNode+1, matrixDi[row] );
  }
  matrixDi[inputNumberNode] = atoi(gtk_entry_get_text(GTK_ENTRY(initialTable[inputNumberNode][2])));
  //printf(" %d %d\n",inputNumberNode, matrixDi[inputNumberNode] );

}

void createTableA() 
{
  int keys = inputNumberNode + 1;

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
      //gtk_widget_set_name(initialTableA[row][column],"newValueP");

      if(row == 0 && column == 0)
      {
      	sprintf(str, "%d", 0);
        gtk_entry_set_text (GTK_ENTRY(initialTableA[row][column]),str);
        gtk_widget_set_name(initialTableA[row][column],"rowHeader2");
      }

      else if(row == 0)
      {
      	sprintf(str, "%d", column);
        gtk_entry_set_text (GTK_ENTRY(initialTableA[row][column]),str);

        gtk_widget_set_name(initialTableA[row][column],"rowHeader2");
      }

      else if(column == 0)
      {
      	sprintf(str, "%d", row);
        gtk_entry_set_text (GTK_ENTRY(initialTableA[row][column]),str);
        gtk_widget_set_name(initialTableA[row][column],"rowHeader2");
      }

      else
      {
      	int  number;
      	number = matrixMul[row - 1][column - 1 ];
      	sprintf(str, "%d", number);
      	gtk_entry_set_text (GTK_ENTRY(initialTableA[row][column]),str);
        if(row == 1 && column == keys-1){
            gtk_widget_set_name(initialTableA[row][column],"newValueP");
        }
      }
        
      
    }
  }
}


void createTableB() 
{
  int keys = inputNumberNode + 1;

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
        gtk_widget_set_name(initialTableB[row][column],"rowHeader2");
      }

      else if(row == 0)
      {
      	sprintf(str, "%d", column);
        gtk_entry_set_text (GTK_ENTRY(initialTableB[row][column]),str);
        gtk_widget_set_name(initialTableB[row][column],"rowHeader2");
      }

      else if(column == 0)
      {
      	sprintf(str, "%d", row);
        gtk_entry_set_text (GTK_ENTRY(initialTableB[row][column]),str);
        gtk_widget_set_name(initialTableB[row][column],"rowHeader2");
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

void respuesta(int i,int j){
    if(i==j){
        strcat(val,alphabetNodes[i]);
    }else{
        strcat(val,"(");
        respuesta(i,matrixKeyAux[i][j]);
        strcat(val,"x");
        respuesta(matrixKeyAux[i][j]+1,j);
        strcat(val,")");
    }
}



void getTree()
{


  numberDi = inputNumberNode;
  int matrixDi[inputNumberNode+1];
  createMatrizDi(matrixDi);


  matrixMul =  fillMatrixMul(matrixMul, numberDi, matrixDi);
  matrixKey   = fillMatrixKey(matrixKey, numberDi);

  matrixMul = finalMatrix(matrixMul, matrixKey, matrixDi);
  matrixKeyAux = matrixKey;


  createTableA();
  createTableB();

  for(int i = 0;i< inputNumberNode;i++){
        for(int j=0;j< inputNumberNode;j++){
            if(matrixKey[i][j]>0){
                matrixKey[i][j]-=1; 
            }
        }
    }
  strcpy(val,"");
  respuesta(0,inputNumberNode-1);
  //printf("%s\n",val );
  gtk_label_set_text (GTK_LABEL(label_betterPath),val);
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
        gtk_widget_set_sensitive(initialTable[row][column],FALSE);
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
  fscanf(infoFile, "%s", number);
  gtk_entry_set_text (GTK_ENTRY(initialTable[inputNumberNode][2]),number);
}


void loadFile()
{
  char *filename;
  filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(chooseFileButton));
  loadData(filename);
  createSetNodeData();
  createSetNodeDataFile();
  gtk_widget_hide(windowInitial); 
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
    label_betterPath = GTK_WIDGET(gtk_builder_get_object(builder, "labelSolution"));

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