#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[])
{
    GtkBuilder      *myBuilder; 
    GtkWidget       *window ;
 	

    gtk_init(&argc, &argv);
    //myCSS();
 
    myBuilder = gtk_builder_new();
    gtk_builder_add_from_file (myBuilder, "glade/window_initial_series.glade", NULL);
 
    window = GTK_WIDGET(gtk_builder_get_object(myBuilder, "window_final_series"));
    gtk_builder_connect_signals(myBuilder, NULL);

  
    
    g_object_unref(myBuilder);
 
    gtk_widget_show(window);                
    gtk_main();
 
    return 0;
}