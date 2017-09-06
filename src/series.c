#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>


GtkBuilder      *myBuilder; 
GtkWidget       *windowInitial;
GtkWidget       *windowCreateData;
GtkWidget       *windowFinal;

GtkWidget       ***tableP0;
GtkWidget 		*scrolledTableSerie;
GtkWidget 		*tableP;
 	
void createTable()
{
	tableP0 = calloc(10,sizeof(GtkWidget**));
    tableP = gtk_grid_new ();
    for(int j = 0; j < 10; j++) {
    	tableP0[j] = calloc(10,sizeof(GtkWidget*));
  	}

   	gtk_container_add(GTK_CONTAINER(scrolledTableSerie), tableP);

   	for(int i = 0; i < 10; i++)
   	{
   		for(int j = 0; j < 10; j++)
   		{	tableP0[i][j] = gtk_entry_new();
   			gtk_entry_set_text (GTK_ENTRY(tableP0[i][j]), "izcar :v");
   			gtk_entry_set_width_chars(GTK_ENTRY(tableP0[i][j]),8);
   			gtk_grid_attach (GTK_GRID(tableP),tableP0[i][j], i, j, 1, 1);
   			gtk_widget_set_sensitive(tableP0[i][j],FALSE);

   		}
   	}
}
int main(int argc, char *argv[])
{
   

    gtk_init(&argc, &argv);
    //myCSS();
 
    myBuilder = gtk_builder_new();
    gtk_builder_add_from_file (myBuilder, "glade/window_series.glade", NULL);
 
    windowFinal = GTK_WIDGET(gtk_builder_get_object(myBuilder, "window_final_series"));
    
	scrolledTableSerie = GTK_WIDGET(gtk_builder_get_object(myBuilder, "gtkScrolledWindowTableSerie"));
    
    createTable();

    gtk_builder_connect_signals(myBuilder, NULL);

  
    
    g_object_unref(myBuilder);
 
    gtk_widget_show_all(windowFinal);                
    gtk_main();
 
    return 0;
}


