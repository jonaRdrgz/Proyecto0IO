#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "equipos.h"
#include "Reemplazo.c"

GtkWidget       *windowInitialReplace;
GtkWidget       *windowTableData;
GtkWidget       *windowFinalTable;
GtkWidget       ***tableData;
GtkWidget       *g_tableData;
GtkWidget       *g_frame_manualEntry;
GtkWidget       *g_frame_fileEntry;
GtkWidget       *g_filechooser_btn;
GtkWidget       *g_entry_fileName;
GtkWidget       *g_spin_usefulLife;
GtkWidget       *g_spinbutton_timeLimit;
GtkWidget       *g_spinbutton_precioEquipo;
GtkWidget       *windowSave;

GtkWidget       *g_scrolledwindow_initialTableData;
GtkWidget       *g_scrolledwindow_finalTable;
GtkWidget       *g_scrolledwindow_optimalSolution;
FILE            *file_tableData;

const char *rowHeaderInitialTable[3] = {"Año","Precio de Reventa","Mantenimiento"};
const char *rowHeaderFinalTable[4] = {"t","G(t)","Próximo","Ganancia"};
int usefulLife = 0;


plans planesPosibles[100];
int cont_plans = 0;

InitialTable *initialData;

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

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    myCSS();


 	GtkBuilder      *builder; 
     
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/window_equipos.glade", NULL);
 
    windowInitialReplace = GTK_WIDGET(gtk_builder_get_object(builder, "window_initial_equipo"));
    gtk_builder_connect_signals(builder, NULL);

    windowTableData = GTK_WIDGET(gtk_builder_get_object(builder, "window_tableData"));
    gtk_builder_connect_signals(builder, NULL);

    windowFinalTable = GTK_WIDGET(gtk_builder_get_object(builder, "window_finalTable"));
    gtk_builder_connect_signals(builder, NULL);

    windowSave = GTK_WIDGET(gtk_builder_get_object(builder, "windowSave"));

    g_scrolledwindow_initialTableData = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow_initialTableData"));
    g_scrolledwindow_finalTable = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow_finalTable"));
    g_scrolledwindow_optimalSolution = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow_optimalSolution"));


    g_filechooser_btn = GTK_WIDGET(gtk_builder_get_object(builder, "filechooser_btn"));
    GtkFileFilter *filter = gtk_file_filter_new ();
    gtk_file_filter_add_pattern (filter, "*.txt");
    gtk_file_chooser_add_filter (GTK_FILE_CHOOSER(g_filechooser_btn),filter);

    g_spin_usefulLife = GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton_usefulLife"));
    gtk_spin_button_set_range (GTK_SPIN_BUTTON(g_spin_usefulLife),1,10);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(g_spin_usefulLife),1,3);

    g_spinbutton_timeLimit = GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton_timeLimit"));
    gtk_spin_button_set_range (GTK_SPIN_BUTTON(g_spinbutton_timeLimit),1,30);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(g_spinbutton_timeLimit),1,3);

    g_spinbutton_precioEquipo = GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton_precioEquipo"));
    gtk_spin_button_set_range (GTK_SPIN_BUTTON(g_spinbutton_precioEquipo),1,1000000000);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(g_spinbutton_precioEquipo),1,3);

    g_entry_fileName = GTK_WIDGET(gtk_builder_get_object(builder, "entry_fileName"));

    g_object_unref(builder);
    gtk_widget_show(windowInitialReplace);                
    gtk_main();
 
    return 0;
}
 
void on_window_initial_replace_destroy() {
  gtk_main_quit();
}

void on_window_tableData_destroy() {
  gtk_main_quit();
}

void on_window_finalTable_destroy() {
  free(tableData);
  gtk_main_quit();
}
void destroy()
{
	gtk_widget_hide(windowSave);
}


void createTableData() {
  tableData = calloc(usefulLife,sizeof(GtkWidget**));

  g_tableData = gtk_grid_new ();
  gtk_container_add (GTK_CONTAINER (g_scrolledwindow_initialTableData), g_tableData);

  char rowNumber[4];

  for(int j = 0; j < usefulLife; j++) {
    tableData[j] = calloc(3,sizeof(GtkWidget*));
  }

  for(int row =0; row < usefulLife; row++) 
  {
    for(int column=0; column < 3; column++) 
    {
      tableData[row][column] = gtk_entry_new();
      gtk_grid_attach (GTK_GRID (g_tableData),tableData[row][column] , column, row, 1, 1);
      if (row == 0){
        gtk_entry_set_text (GTK_ENTRY(tableData[row][column]),rowHeaderInitialTable[column]);
        gtk_widget_set_name(tableData[row][column],"header");
        gtk_widget_set_sensitive(tableData[row][column],FALSE);
      }
      if (column ==0 && row!=0){
        sprintf(rowNumber, "%d", row);
        gtk_entry_set_text (GTK_ENTRY(tableData[row][column]),rowNumber);
        gtk_widget_set_name(tableData[row][column],"header");
        gtk_widget_set_sensitive(tableData[row][column],FALSE);
      }
    }
  }
  gtk_widget_show_all(windowTableData);
}

void createTableDataFile(int Matriz[usefulLife-1][2]){

  gtk_spin_button_set_value (GTK_SPIN_BUTTON(g_spinbutton_precioEquipo),Amount);

  gtk_spin_button_set_value (GTK_SPIN_BUTTON(g_spinbutton_timeLimit),timeLimitEquipment);

  tableData = calloc(usefulLife,sizeof(GtkWidget**));

  g_tableData = gtk_grid_new ();
  gtk_container_add (GTK_CONTAINER (g_scrolledwindow_initialTableData), g_tableData);

  char rowNumber[4];

  for(int j = 0; j < usefulLife; j++) {
    tableData[j] = calloc(3,sizeof(GtkWidget*));
  }

  for(int row =0; row < usefulLife; row++) 
  {
    for(int column=0; column < 3; column++) 
    {
      char str[10];
      tableData[row][column] = gtk_entry_new();
      gtk_entry_set_width_chars(GTK_ENTRY(tableData[row][column]),10);
      gtk_grid_attach (GTK_GRID (g_tableData),tableData[row][column] , column, row, 1, 1);
      if (row == 0){
        gtk_entry_set_text (GTK_ENTRY(tableData[row][column]),rowHeaderInitialTable[column]);
        gtk_widget_set_name(tableData[row][column],"header");
        gtk_widget_set_sensitive(tableData[row][column],FALSE);
      }
      if (column ==0 && row!=0){
        sprintf(rowNumber, "%d", row);
        gtk_entry_set_text (GTK_ENTRY(tableData[row][column]),rowNumber);
        gtk_widget_set_name(tableData[row][column],"header");
        gtk_widget_set_sensitive(tableData[row][column],FALSE);
      }
      if (column !=0 && row!=0) {
        sprintf(str, "%d", Matriz[row-1][column-1]);
        gtk_entry_set_text (GTK_ENTRY(tableData[row][column]),str);
      }
    }
  }
  gtk_widget_show_all(windowTableData);
}

void createFinalTableData(int timeLimit, GX Ftable[timeLimit + 1]) {
  free(tableData);
  int ptimeLimit = timeLimit + 2;
  tableData = calloc(ptimeLimit,sizeof(GtkWidget**));

  g_tableData = gtk_grid_new ();
  gtk_container_add (GTK_CONTAINER (g_scrolledwindow_finalTable), g_tableData);

  char rowNumber[3];

  for(int j = 0; j < ptimeLimit; j++) {
    tableData[j] = calloc(4,sizeof(GtkWidget*));
  }

  for(int row = 0; row < ptimeLimit; row++) 
  {
    for(int column=0; column < 4; column++) 
    {
      tableData[row][column] = gtk_entry_new();
      gtk_entry_set_width_chars(GTK_ENTRY(tableData[row][column]),10);
      gtk_widget_set_sensitive(tableData[row][column],FALSE);
      gtk_grid_attach (GTK_GRID (g_tableData),tableData[row][column] , column, row, 1, 1);
      if (row == 0) {
        gtk_entry_set_text (GTK_ENTRY(tableData[row][column]),rowHeaderFinalTable[column]);
        gtk_widget_set_name(tableData[row][column],"header");
        gtk_widget_set_sensitive(tableData[row][column],FALSE);
      }
    }
    if (row!=0) {
      
      char gt[50];
      char prox[50];
      char profit[50];

      strcpy(prox," ");

      sprintf(rowNumber, "%d", row -1);
      gtk_entry_set_text (GTK_ENTRY(tableData[row][0]),rowNumber);
      gtk_widget_set_name(tableData[row][0],"header");

      sprintf(gt, "%d", Ftable[timeLimit + 1 - row].valor);
      gtk_entry_set_text (GTK_ENTRY(tableData[row][1]), gt);
      gtk_widget_set_name(tableData[row][1],"allEntries");
        
      
      for (int i=0;i<Ftable[timeLimit + 1 - row].numeroLlaves ;i++){
        char aux[3];
        sprintf(aux, "%d",Ftable[timeLimit + 1 - row].llaves[i]);
        strcat(prox,aux);
        if (i<Ftable[timeLimit + 1 - row].numeroLlaves-1){
          strcat(prox,",");
        }

      }
      
      gtk_entry_set_text (GTK_ENTRY(tableData[row][2]),prox);
      gtk_widget_set_name(tableData[row][2],"allEntries");

      sprintf(profit, "%d", Ftable[timeLimit + 1 - row].ganancia);
      gtk_entry_set_text (GTK_ENTRY(tableData[row][3]), profit);
      gtk_widget_set_name(tableData[row][3],"allEntries");
      memset(prox,'\0',strlen(prox));
      memset(profit,'\0',strlen(profit));
    }
  }
  gtk_widget_show_all(windowFinalTable);
}

void createFile() {
  int lenName = strlen(gtk_entry_get_text (GTK_ENTRY(g_entry_fileName))) + 23;
  
  char fileName[lenName]; 
  strcpy(fileName,"examples/equipos/");
  strcat(fileName, gtk_entry_get_text (GTK_ENTRY(g_entry_fileName)));
  strcat(fileName, ".txt");

  file_tableData = fopen(fileName,"w+");

  int inputAmount = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(g_spinbutton_precioEquipo));
  int inputTimeLimit = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(g_spinbutton_timeLimit));

  
  for(int row =0; row < usefulLife; row++) 
  {
    for(int column=0; column < 3; column++) 
    {
      fprintf(file_tableData,"%s;",(gtk_entry_get_text(GTK_ENTRY(tableData[row][column]))));
    }
    fprintf(file_tableData,"\n");

  }
  fprintf(file_tableData,"%d\n",inputAmount);
  fprintf(file_tableData,"%d\n",inputTimeLimit);
  fclose(file_tableData);

  gtk_entry_set_text(GTK_ENTRY(g_entry_fileName),"");
  gtk_widget_show_all(windowSave);
}

void createObjects() {
  initialData = calloc(usefulLife,sizeof(InitialTable));
  for (int row = 1; row < usefulLife;row ++)
  {
    InitialTable data;
    data.year = atoi(gtk_entry_get_text(GTK_ENTRY(tableData[row][0])));
    data.sale = atoi(gtk_entry_get_text(GTK_ENTRY(tableData[row][1])));
    data.maintenance = atoi(gtk_entry_get_text(GTK_ENTRY(tableData[row][2])));
    initialData[row-1] = data;
  }
}

void createOptimalSolution(plans planesPosibles[300]) {
  char text[1000];
  char number[3];
  strcpy(text, " ");
  for (int i =0; i < cont_plans;i++){
    plans aux = planesPosibles[i];
    if (repeat(planesPosibles,i) ==0) {
      for (int x = 0;x<=aux.position;x++) {
        sprintf(number, "%d", aux.year[x]);
        strcat(text,number);
        if (x<aux.position){
          strcat(text,"   ->   ");
        }
        else{
          strcat(text," \n");
        }
      }
    }
  }
  GtkWidget *g_lblSolution = gtk_label_new (text);
  gtk_container_add (GTK_CONTAINER (g_scrolledwindow_optimalSolution), g_lblSolution);
  gtk_widget_set_name(g_lblSolution,"label");

  gtk_widget_show_all(windowFinalTable);
  memset(text,'\0',strlen(text));
}
void on_btn_manualEntry_clicked() {
  gtk_widget_hide(g_frame_fileEntry);
  gtk_widget_show(g_frame_manualEntry);
}

void on_btn_fileEntry_clicked() {
  gtk_widget_hide(g_frame_manualEntry);
  gtk_widget_show(g_frame_fileEntry);
}

void on_btn_usefulLife_clicked() {
  usefulLife = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(g_spin_usefulLife)) + 1;
  createTableData();
  gtk_widget_hide(windowInitialReplace);
  gtk_widget_show_now(windowTableData);
}

void on_btn_getFile_clicked() {
  usefulLife = countUsefulLifeFiles (gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(g_filechooser_btn)));
  int matrixData[usefulLife-1][2];
  startFill(matrixData,gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(g_filechooser_btn)));
  createTableDataFile(matrixData);

  gtk_widget_hide(windowInitialReplace);
  gtk_widget_show_now(windowTableData);
}

void changeTableData(InitialTable data[usefulLife], int pManteSale[usefulLife - 1][2])
{
  for(int i = 0; i < usefulLife -1; i++)
  {
    
    pManteSale[i][0] = data[i].sale;
    pManteSale[i][1] = data[i].maintenance;
    
  }
 


}

void on_btn_getTableData_clicked() {
  int initialCost = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(g_spinbutton_precioEquipo));
  timeLimit = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(g_spinbutton_timeLimit));

  
  setTotalObjectsCount(usefulLife-1, initialCost, timeLimit);
  createObjects();

  FinalTable *finalData = calloc (timeLimit+1,sizeof(FinalTable));
  int manteSale[usefulLife - 1][2];

  changeTableData(initialData, manteSale);

   for(int i = 0; i < usefulLife - 1; i++)
  {
    
    printf("%d \n", manteSale[i][0]);
    printf("%d \n", manteSale[i][1]);
    
  }

  vidaUtilEquipo = usefulLife - 1;
  plazoProyecto = timeLimit;
  GX tablaFinal[timeLimit + 1];
  reemplazo(usefulLife - 1, manteSale, initialCost, tablaFinal);
 


  createFinalTableData(timeLimit, tablaFinal);
  

  gtk_widget_hide(windowTableData);
  gtk_widget_show_now(windowFinalTable);
  free(finalData);
}
