#include <gtk/gtk.h>
#include <string.h>

int main() {
    GtkBuilder      *myBuilder; 
    GtkWidget       *myWinPending;
 
    gtk_init(NULL, NULL);
 
    myBuilder = gtk_builder_new();
    gtk_builder_add_from_file (myBuilder, "glade/pending.glade", NULL);
 
    myWinPending = GTK_WIDGET(gtk_builder_get_object(myBuilder, "pending"));
    gtk_builder_connect_signals(myBuilder, NULL);

    g_object_unref(myBuilder);
 
    gtk_widget_show(myWinPending);                
    gtk_main();
 
    return 0;
}
 
// called when window is closed
void pending_destroy() {
    gtk_main_quit();
}