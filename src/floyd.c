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

//CreateData 
GtkWidget       ***tableD0;
GtkWidget 		*scrolledTable;
GtkWidget 		*columnD0;

GtkWidget       ***tableP;
GtkWidget 		*columnP;
GtkWidget 		*scrolledTableP;

GtkWidget       *label_table_DNumber;


//initiial
GtkWidget 		*loadFileButton;
GtkWidget 		*chooseFileButton;
GtkWidget  		*spinButtonNode;
GtkWidget 		*spinButtonGamePH;
GtkWidget 		*spinButtonGamePV;
GtkWidget 		*saveFileButton;
GtkWidget 		*filenameEntry;
GtkWidget 		*windowSave;



//create Data
GtkWidget 		*execGameButton;
GtkWidget 		*scrolleGameSerieH;
GtkWidget 		**tableHV0;
GtkWidget 		*tableHV;

//Guardar Archivo
FILE *infoFile;
const char *alphabetNodes[27]={"A","B","C","D","E","F","G","H","I","J",
"K","L","M","N","Ñ","O","P","Q","R","S","T","U","V","W","X","Y","Z"};

int inputNumberNodes;
int numberNodes = 0;
int totalNodes = 0;
int totalCycles = 0;


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



/*void createInfoFile(char *filename) {
  infoFile = fopen(filename,"w+");
  fprintf(infoFile,"%d\n",inputNumberGames);
  fprintf(infoFile,"%f\n",inputPH);
  fprintf(infoFile,"%f\n",inputPV);

  for(int i = 0; i < inputNumberGames; i++) 
  {
    fprintf(infoFile,"%d",juegosCasa[i]);
  }
  
  fclose(infoFile);
}*/

void createTableP() {
  tableP = calloc(numberNodes,sizeof(GtkWidget**));

  columnP = gtk_grid_new ();
  gtk_container_add (GTK_CONTAINER (scrolledTableP), columnP);

  for(int j = 0; j < numberNodes; j++) {
    tableP[j] = calloc(numberNodes,sizeof(GtkWidget*));
  }

  for(int i =0; i < numberNodes; i++) 
  {
    for(int j=0; j < numberNodes; j++) 
    {
      tableP[i][j] = gtk_entry_new();
      gtk_entry_set_width_chars(GTK_ENTRY(tableP[i][j]),8);
      gtk_widget_set_sensitive(tableP[i][j],FALSE);
      gtk_grid_attach (GTK_GRID (columnP),tableP[i][j] , j, i, 1, 1);

      if(i != 0 && j != 0){
        gtk_entry_set_text (GTK_ENTRY(tableP[i][j]),"0");
      }
      if (i == 0 && j != 0){
        /*if (loadFile == 1){
          gtk_entry_set_text (GTK_ENTRY(tableP[row][column]),header[column]);
        }
        else{*/
        gtk_entry_set_text (GTK_ENTRY(tableP[i][j]),alphabetNodes[j-1]);
      //}
      }
      if (j ==0 && i != 0){
        //if (loadFile ==1){
        //  gtk_entry_set_text (GTK_ENTRY(tableP[row][column]),header[row]);
        //}
        //else{
          gtk_entry_set_text (GTK_ENTRY(tableP[i][j]),alphabetNodes[i-1]);  
        //}
        
      }
    }
  }

}

 	
void createTableD0 (int nodes)
{
	numberNodes = nodes;
	createTableP();
	tableD0 = calloc(nodes,sizeof(GtkWidget**));
    columnD0 = gtk_grid_new ();
    

    for(int j = 0; j < nodes; j++) {
    	tableD0[j] = calloc(10,sizeof(GtkWidget*));
  	}

   	gtk_container_add(GTK_CONTAINER(scrolledTable), columnD0);

   	for(int i = 0; i < nodes; i++)
   	{
   		for(int j = 0; j < nodes; j++)
   		{	
   			tableD0[i][j] = gtk_entry_new();
   			gtk_entry_set_width_chars(GTK_ENTRY(tableD0[i][j]),8);
   			gtk_grid_attach (GTK_GRID(columnD0),tableD0[i][j], j, i, 1, 1);
   			

   			if(i == j && i != 0 && j != 0){
	        	gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),"0");
	        	gtk_widget_set_sensitive(tableD0[i][j],FALSE);
	      	}
	      	if (i == 0 && j != 0){
	        	gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),alphabetNodes[j-1]);
	      	}
	      	if (j ==0 && i!=0){
	        	gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),alphabetNodes[i-1]);
	      	}
	      	if (j !=0 && i!=0 && i!=j){
	        	gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),"10000000");
	      	}
    	}
  }

  gtk_widget_show_all(windowCreateData); 
}

void setTableD(int MatrixD[][numberNodes-1]){
  int nodes = numberNodes;
  char labelInstruction[100] = "Resultados para D(";
  char tableNumber[4];
  sprintf(tableNumber, "%d", totalCycles+1);
  strcat(labelInstruction, tableNumber);
  strcat(labelInstruction, ")");

  gtk_label_set_text(GTK_LABEL(label_table_DNumber), labelInstruction);

  for(int i =0; i < nodes; i++)
  { 
    for(int j=0; j < nodes; j++) 
    {
      char str[10];
      gtk_widget_set_sensitive(tableD0[i][j],FALSE);

      /*if (i == 0 && j != 0){
        gtk_entry_set_text (GTK_ENTRY(tableD0[row][column]),header[column]);
      }
      if (column ==0 && row!=0){
        gtk_entry_set_text (GTK_ENTRY(entrada[row][column]),header[row]);
      }
      if (column !=0 && row!=0){
        sprintf(str, "%d", MatrizD[row-1][column-1]);
        
        if (strcmp(str,gtk_entry_get_text(GTK_ENTRY(entrada[row][column])))==0) {
          gtk_widget_set_name (entrada[row][column],"oldValue");
        } else {
          gtk_widget_set_name (entrada[row][column],"newValue");
        }
        gtk_entry_set_text (GTK_ENTRY(entrada[row][column]),str);
      }*/
    }
  }

  gtk_widget_show_all(scrolledTable); 
}

/*int loadData(char *filename)
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
*/

/*void saveFile()
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

}*/

void destroy()
{
	gtk_widget_hide(windowSave);
}

/*int loadFile()
{
	char *filename;
	filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(chooseFileButton));

	int flag = loadData(filename);

	return flag;

}*/

/*void createGameFile()
{
	

	
	if(loadFile() == 1)
	{
		gtk_widget_hide(windowInitial);
		//createTableHV1();

		gtk_widget_show_all(windowCreateData);
	}
	else
	{

	}
	 

}*/



void createMatrix()
{
	gtk_widget_hide(windowInitial); 
	inputNumberNodes = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(spinButtonNode));	
	inputNumberNodes++; 
	createTableD0(inputNumberNodes);
  gtk_widget_hide(windowInitial);
  gtk_widget_show_now(windowCreateData);
}




void execFloyd()
{
	if (totalCycles == 0) {
    	totalNodes = numberNodes-1;
    	//applyFloyd();

  	} else {
    	if (totalCycles < numberNodes-1) {
      		//applyFloyd();
    	}
    	if (totalCycles == numberNodes-1) {
      		//fillCombobox();
      		//gtk_widget_show_all(dialogFloyd);
    	}
  	}
}


int main(int argc, char *argv[])
{
   

    gtk_init(&argc, &argv);
    myCSS();


 
    myBuilder = gtk_builder_new();
    gtk_builder_add_from_file (myBuilder, "glade/window_floyd.glade", NULL);
 
    windowInitial = GTK_WIDGET(gtk_builder_get_object(myBuilder, "window_initial_floyd"));

    windowCreateData = GTK_WIDGET(gtk_builder_get_object(myBuilder, "window_floyd"));

    windowFinal  = GTK_WIDGET(gtk_builder_get_object(myBuilder, "get_path_between"));


  

    spinButtonNode = GTK_WIDGET(gtk_builder_get_object(myBuilder, "spinButtonNode"));
    gtk_spin_button_set_range (GTK_SPIN_BUTTON(spinButtonNode),1,99);
	gtk_spin_button_set_increments (GTK_SPIN_BUTTON(spinButtonNode),1,3);

	label_table_DNumber = GTK_WIDGET(gtk_builder_get_object(myBuilder, "label_tableD_number"));
    /*spinButtonGamePH = GTK_WIDGET(gtk_builder_get_object(myBuilder, "spinButtonGamePH"));
    gtk_spin_button_set_range (GTK_SPIN_BUTTON(spinButtonGamePH),0,1);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(spinButtonGamePH),0.01,1);

    spinButtonGamePV = GTK_WIDGET(gtk_builder_get_object(myBuilder, "spinButtonGamePV"));
    gtk_spin_button_set_range (GTK_SPIN_BUTTON(spinButtonGamePV),0,1);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(spinButtonGamePV),0.01,1);

    execGameButton =  GTK_WIDGET(gtk_builder_get_object(myBuilder, "execute_serie"));*/





	scrolledTable = GTK_WIDGET(gtk_builder_get_object(myBuilder, "scrolledwindow_table"));
    scrolledTableP = GTK_WIDGET(gtk_builder_get_object(myBuilder, "scrolledwindow_tableP"));



/*
    
    saveFileButton = GTK_WIDGET(gtk_builder_get_object(myBuilder, "saveFileButton"));
    filenameEntry = GTK_WIDGET(gtk_builder_get_object(myBuilder, "filenameEntry"));
    
	scrolledTableSerie = GTK_WIDGET(gtk_builder_get_object(myBuilder, "gtkScrolledWindowTableSerie"));
    
    chooseFileButton = GTK_WIDGET(gtk_builder_get_object(myBuilder, "chooseFileButton"));
    loadFileButton = GTK_WIDGET(gtk_builder_get_object(myBuilder, "loadFileButton"));

    windowSave = GTK_WIDGET(gtk_builder_get_object(myBuilder, "windowSave"));

    labelA = GTK_WIDGET(gtk_builder_get_object(myBuilder, "PH1"));
    labelB = GTK_WIDGET(gtk_builder_get_object(myBuilder, "PH2"));
*/
    gtk_builder_connect_signals(myBuilder, NULL);
  
    
    g_object_unref(myBuilder);
 
    gtk_widget_show_all(windowInitial);                
    gtk_main();
 
    return 0;
}
