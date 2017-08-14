#include <gtk/gtk.h>
#include <string.h>

int main() {
    GtkBuilder      *myBuilder;
    GtkWidget       *myWinPending;

    gtk_init(NULL, NULL);

    myBuilder = gtk_builder_new();
    GError *err = NULL; /* It is mandatory to initialize to NULL */
    if(0 ==   gtk_builder_add_from_file (myBuilder, "glade/pending.glade", &err))
    {
        /* Print out the error. You can use GLib's message logging */
        fprintf(stderr, "Error adding build from file. Error: %s\n", err->message);
        /* Your error handling code goes here */
    }


    myWinPending = GTK_WIDGET(gtk_builder_get_object(myBuilder, "win_pending"));
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
