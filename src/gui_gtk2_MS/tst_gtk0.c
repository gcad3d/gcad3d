// test gtk-only - simple window.





#include <gtk/gtk.h>

// dummy-funcs
int GUI_Tip (char *key) { return 0; }
int GUI_update__ () { return 0; }


//================================================================
  int b1_CB (void *parent, void **data) {
//================================================================
// called when button is clicked ..
 
  printf("callback_button1\n");

  return 1;

}


//================================================================
int main (int   argc, char *argv[]) {
//================================================================

  GtkWidget *window, *wb1, *box1;

  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

  box1 = gtk_vbox_new (FALSE, 1);
  gtk_container_add (GTK_CONTAINER (window), box1);
  gtk_widget_show (box1);

  wb1 = gtk_button_new_with_label ("......go 1.......");
  gtk_signal_connect (GTK_OBJECT (wb1), "clicked",
                      GTK_SIGNAL_FUNC(b1_CB), NULL);
  gtk_container_add (GTK_CONTAINER (box1), wb1);
  gtk_widget_show (wb1);

  gtk_widget_show (window);

  gtk_main ();

  return 0;
}

// EOF

