#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define gtk_spin_button_get_value_as_float gtk_spin_button_get_value
#define MINAUX(a, b) ((a) < (b) ? a : b) // return min a , b
#define path(a, b, c, d) ((a) != (b) ? d : c) 


GtkBuilder    *myBuilder; 
GtkWidget     *windowInitial;
GtkWidget     *windowCreateData;
GtkWidget     *windowFinal;
GtkWidget     *chooseFileButton;
GtkWidget     *spinButtonNode;

//CreateData 
GtkWidget     ***tableD0;
GtkWidget 		*scrolledTable;
GtkWidget 		*columnD0;

GtkWidget     ***tableP;
GtkWidget 		*columnP;
GtkWidget 		*scrolledTableP;

GtkWidget     *label_table_DNumber;
GtkWidget     *label_betterPath;

const char **header;
char bufferForFile[9];
int loadFileFlag = 0;
char *filename;

int begin = -1;
int end = -1;


//final
GtkWidget       *combobox_from;
GtkWidget       *combobox_to;
GtkWidget       *container_for_combobox_from;
GtkWidget       *container_for_combobox_to;
GtkWidget       *filenameEntry;
GtkWidget       *windowSave;


//Guardar Archivo
FILE *infoFile;
FILE *infoFileAux;

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
			if (loadFileFlag == 1){
				gtk_entry_set_text (GTK_ENTRY(tableP[i][j]),header[j]);
			}
			else{
				gtk_entry_set_text (GTK_ENTRY(tableP[i][j]),alphabetNodes[j-1]);
			}
		}
		if (j ==0 && i != 0){
			if (loadFileFlag == 1){
				gtk_entry_set_text (GTK_ENTRY(tableP[i][j]),header[i]);
			}
			else{
				gtk_entry_set_text (GTK_ENTRY(tableP[i][j]),alphabetNodes[i-1]);  
			}

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
	sprintf(tableNumber, "%d", totalCycles);
	strcat(labelInstruction, tableNumber);
	strcat(labelInstruction, ")");

	gtk_label_set_text(GTK_LABEL(label_table_DNumber), labelInstruction);

	for(int i =0; i < nodes; i++)
	{ 
		for(int j=0; j < nodes; j++) 
		{
			char str[10];
			gtk_widget_set_sensitive(tableD0[i][j],FALSE);

			if (i == 0 && j != 0){
				gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]), header[j]);
			}
			if (j ==0 && i!=0){
				gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),header[i]);
			}

			if (j !=0 && i!=0){
				sprintf(str, "%d", MatrixD[i-1][j-1]);

				if (strcmp(str,gtk_entry_get_text(GTK_ENTRY(tableD0[i][j]))) == 0) {
					gtk_widget_set_name (tableD0[i][j],"oldValue");
				} else {
					gtk_widget_set_name (tableD0[i][j],"newValue");
				}
				gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),str);
			}
		}
	}

	gtk_widget_show_all(scrolledTable); 
}

void setTableP(int matrixP[][numberNodes-1]){
	int nodes = numberNodes;

	for(int i =0; i < nodes; i++) 
	{
		for(int j=0; j < nodes; j++) 
		{
			char str[10];

			if (i == 0 && j != 0){
				gtk_entry_set_text (GTK_ENTRY(tableP[i][j]),header[j]);
			}
			if (j ==0 && i!=0){
				gtk_entry_set_text (GTK_ENTRY(tableP[i][j]),header[i]);
			}
			if (j !=0 && i!=0){
				sprintf(str, "%d", matrixP[i-1][j-1]);
				if (strcmp(str,gtk_entry_get_text(GTK_ENTRY(tableP[i][j])))==0) {
					gtk_widget_set_name (tableP[i][j],"oldValue");
				} else {
					gtk_widget_set_name (tableP[i][j],"newValueP");
				}
				gtk_entry_set_text (GTK_ENTRY(tableP[i][j]),str);
			}
		}
	}
	gtk_widget_show_all(scrolledTableP); 
}

void setTableFile(int Matriz[][numberNodes-1]){
	int nodes=numberNodes;
	tableD0 = calloc(nodes,sizeof(GtkWidget**));


	columnD0 = gtk_grid_new ();
	gtk_container_add (GTK_CONTAINER (scrolledTable), columnD0);

	for(int j = 0; j < nodes; j++) {
		tableD0[j] = calloc(nodes,sizeof(GtkWidget*));
	}

	for(int i =0; i < nodes; i++) 
	{
		for(int j=0; j < nodes; j++) 
		{
			char str[10];
			tableD0[i][j] = gtk_entry_new();
			gtk_entry_set_width_chars(GTK_ENTRY(tableD0[i][j]),8);
			gtk_grid_attach (GTK_GRID (columnD0),tableD0[i][j] , j, i, 1, 1);

			if(i==j && i != 0 && j != 0){
				gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),"0");
				gtk_widget_set_sensitive(tableD0[i][j],FALSE);
			}
			if (i == 0 && j != 0){
				gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),header[j]);
			}
			if (j ==0 && i!=0){
				gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),header[i]);
			}
			if (j !=0 && i!=0 && i!=j) {
				sprintf(str, "%d", Matriz[i-1][j-1]);
				gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),str);
			}
		}
	}

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
	if(infoFile != NULL)
	{
		while(feof(infoFile) == 0) {  
			ch = fgetc(infoFile);
			if (ch == '\n'){
				break;
			}
			if (ch ==';'){
				numberNodes ++;
			}
		}

		fclose(infoFile);
		return 1;
	}
	return 0;
}

void createTablesFromFile(){
	int matrizAux[numberNodes-1][numberNodes-1];
	header = malloc(numberNodes * sizeof(char*));
	infoFileAux = fopen(filename, "r");

	int i = 0;
	int j = 0;
	int flag = 0;
	int accion = getNext();
	int var = 0;

	while (var < numberNodes) {
		
		if (accion == 0){
			break;
		}
		char * data = malloc(1000);
		strcpy(data,bufferForFile);

		header[var] = data;

		var ++;
		accion = getNext();
	}

	while (i < numberNodes-1) {
		while (j < numberNodes-1) {
			accion = getNext();
			if (flag == 0) {
				flag = 1;
				accion = getNext();
			}
			if (accion == 1) {
				int valor = atoi(bufferForFile);
				matrizAux[i][j] = valor;
				j ++;
			}	
		}
		j = 0;
		i ++;
		flag = 0;
		accion = getNext();
	}

	createTableP();
	setTableFile(matrizAux);
}
void createInfoFile(char *filename) {
    infoFile = fopen(filename,"w+");
    for(int i =0; i < numberNodes; i++) 
  {
        for(int j=0; j < numberNodes; j++) 
        {
          
          fprintf(infoFile,"%s;",(gtk_entry_get_text(GTK_ENTRY(tableD0[i][j]))));
        }
         fprintf(infoFile,"\n");
  }
  fclose(infoFile);
}

void saveFile()
{


    char filename[1000] = "examples/floyd/";

    int len = gtk_entry_get_text_length (GTK_ENTRY(filenameEntry));
    if(len != 0 )
    {
    	
    	strcat(filename,gtk_entry_get_text (GTK_ENTRY(filenameEntry)));
    	strcat(filename,".txt");
    	
    	createInfoFile(filename);
    	
    	gtk_entry_set_text(GTK_ENTRY(filenameEntry),"");

    	gtk_widget_show_all(windowSave);
    }
}



void destroy()
{
	gtk_widget_hide(windowSave);
}

int loadFile()
{

	filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(chooseFileButton));

	int flag = loadData(filename);

	return flag;

}

void createMatrixFile()
{

	if(loadFile() == 1)
	{
		gtk_widget_hide(windowInitial);
		loadFileFlag = 1;
		createTablesFromFile();

		gtk_widget_show_all(windowCreateData);
	} 
}

void printMatrix(int matrix[numberNodes-1][numberNodes-1], int totalNode)
{

	for (int i = 0; i < totalNode; i++)
	{
		for (int j = 0; j < totalNode; j++)
		{
			printf("%d\t", matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n\n\n");

	}

void floydAux(int matrix[numberNodes-1][numberNodes-1], int matrixAux[numberNodes-1][numberNodes-1], int matrixP[numberNodes-1][numberNodes-1], int totalNode, int node)
{

	for (int i = 0; i < totalNode; i++)
		{ 
			int p;
			for (int j = 0; j < totalNode; j++)
			{ 
				p = MINAUX(matrix[i][j], matrix[i][node - 1] + matrix[node - 1][j]);
				matrixAux[i][j] = p;

				matrixP[i][j] = path(p, matrix[i][j], matrixP[i][j], node);


			}
		}

		for (int i = 0; i < totalNode; i++)
		{ 
			for (int j = 0; j < totalNode; j++)
			{ 
				matrix[i][j] = matrixAux[i][j];     
			}
		}
}



void createMatrix()
{
	gtk_widget_hide(windowInitial); 
	inputNumberNodes = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(spinButtonNode));	
	inputNumberNodes++; 
	createTableD0(inputNumberNodes);
	gtk_widget_show_now(windowCreateData);
}

void createHeader() {
	header = malloc(numberNodes * sizeof(char*));
	for(int i =0; i < numberNodes; i++) 
	{
		for(int j=0; j < numberNodes; j++) 
		{
			if (i == 0){
				header[j] = gtk_entry_get_text(GTK_ENTRY(tableD0[i][j]));
			}
		}
	}
}

void applyFloyd() {
	int MatrizD[numberNodes-1][numberNodes-1];
	int matrixP[numberNodes-1][numberNodes-1];

	for(int i =0; i < numberNodes; i++) 
	{
		for(int j=0; j < numberNodes; j++) 
		{
			if(i != 0 && j != 0){
				MatrizD[i-1][j-1] = atoi(gtk_entry_get_text(GTK_ENTRY(tableD0[i][j])));;
				matrixP[i-1][j-1] = atoi(gtk_entry_get_text(GTK_ENTRY(tableP[i][j])));
			}
		}
	}

	int matrixAux[numberNodes-1][numberNodes-1];;
	floydAux(MatrizD, matrixAux, matrixP,numberNodes-1, totalCycles);

	printf("%s\n","matriz post" );
	printMatrix(MatrizD, numberNodes-1);
	setTableD(MatrizD);
	setTableP(matrixP);
	totalCycles++;
}

void fillCombobox() {
	combobox_from = gtk_combo_box_text_new ();
	combobox_to = gtk_combo_box_text_new();

	for (int i = 0; i < numberNodes; ++i)
	{
		gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(combobox_from),NULL,header[i]);
		gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(combobox_to),NULL,header[i]);
	}
	gtk_container_add (GTK_CONTAINER (container_for_combobox_from), combobox_from);
	gtk_container_add (GTK_CONTAINER (container_for_combobox_to), combobox_to);
}

void execFloyd()
{
	if (totalCycles == 0) {
		totalCycles++;
		createHeader();
		applyFloyd();
	} else {
		if (totalCycles < numberNodes) {
			applyFloyd();
		}
		if (totalCycles == numberNodes) {
			fillCombobox();
			gtk_widget_show_all(windowFinal);
		}
	}
}

void getOptimalPath(int begin,int end, char label []){
	char arrow[7] = " --> ";
	printf("%s\n", gtk_entry_get_text(GTK_ENTRY(tableP[begin][end])));
	int medium = atoi(gtk_entry_get_text(GTK_ENTRY(tableP[begin][end])));

	if (medium == 0){
		printf("Ruta directa de %d a %d \n",begin,end);
		strcat(label,arrow);
		strcat(label, header[end]);
	}
	else{
		strcat(label,arrow);
		strcat(label, header[medium]);
		printf("Tome %d y pase por %d \n",begin,medium);
		getOptimalPath(medium,end, label);
	}
	gtk_label_set_text (GTK_LABEL(label_betterPath),label);
}


void getOptimalPath_button() {
	char resultPre [1000] = "";
	
	//trcpy(resultPreOptimal,"");
	for (int i = 0; i < numberNodes; ++i)
	{
		if(strcmp(header[i],gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combobox_from))) == 0) {
			begin = i;
		}
		if(strcmp(header[i],gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combobox_to))) == 0) {
			end = i;
		}
	}

	printf("%d, %d\n",begin, end );
	if((begin != -1)  && (end != -1)){
		strcat(resultPre,header[begin]);
		//resultPreOptimal = header[begin];
		printf("Inicio: %d, Salida: %d\n",begin,end);
		getOptimalPath(begin,end, resultPre);
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
	gtk_spin_button_set_range (GTK_SPIN_BUTTON(spinButtonNode),1,26);
	gtk_spin_button_set_increments (GTK_SPIN_BUTTON(spinButtonNode),1,3);

	label_table_DNumber = GTK_WIDGET(gtk_builder_get_object(myBuilder, "label_tableD_number"));

	scrolledTable = GTK_WIDGET(gtk_builder_get_object(myBuilder, "scrolledwindow_table"));
	scrolledTableP = GTK_WIDGET(gtk_builder_get_object(myBuilder, "scrolledwindow_tableP"));
	combobox_from = GTK_WIDGET(gtk_builder_get_object(myBuilder, "combobox_from"));
	combobox_to = GTK_WIDGET(gtk_builder_get_object(myBuilder, "combobox_to"));
	container_for_combobox_from = GTK_WIDGET(gtk_builder_get_object(myBuilder,"container_from"));
	container_for_combobox_to = GTK_WIDGET(gtk_builder_get_object(myBuilder,"container_to"));
	label_betterPath = GTK_WIDGET(gtk_builder_get_object(myBuilder, "label_betterPath"));
	chooseFileButton = GTK_WIDGET(gtk_builder_get_object(myBuilder, "chooseFileButton"));
    filenameEntry = GTK_WIDGET(gtk_builder_get_object(myBuilder, "filenameEntry"));
    windowSave = GTK_WIDGET(gtk_builder_get_object(myBuilder, "windowSave"));

	gtk_builder_connect_signals(myBuilder, NULL);


	g_object_unref(myBuilder);

	gtk_widget_show_all(windowInitial);                
	gtk_main();
	return 0;
}

