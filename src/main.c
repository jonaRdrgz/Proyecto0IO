/*
 * Copyright 2017 Samantha Arburola <> 
 */
#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>

void display_pending() {
  system("./pending &"); 
}

void on_window_main_destroy()
{
    gtk_main_quit();
}

void run_program1(){
    display_pending();
}
void run_program2(){
    display_pending();
}
void run_program3(){
    display_pending();
}
void run_program4(){
    display_pending();
}
void run_program5(){
    display_pending();
}

int main(int argc, char *argv[])
{
    GtkBuilder      *myBuilder; 
    GtkWidget       *window;
 
    gtk_init(&argc, &argv);
 
    myBuilder = gtk_builder_new();
    gtk_builder_add_from_file (myBuilder, "glade/window_main.glade", NULL);
 
    window = GTK_WIDGET(gtk_builder_get_object(myBuilder, "window_main"));
    gtk_builder_connect_signals(myBuilder, NULL);
    
    g_object_unref(myBuilder);
 
    gtk_widget_show(window);                
    gtk_main();
 
    return 0;
}

