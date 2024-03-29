#include <gtk/gtk.h>

#undef G_LOG_DOMAIN
#define G_LOG_DOMAIN "gtk_c.button"

static void on_window_destroy(GtkWidget *widget, gpointer data) { gtk_main_quit(); }
static gboolean on_window_delete_event(GtkWidget *widget, GdkEvent *event,
                             gpointer data) {
  return FALSE;
}

int main(int argc, char *argv[]) {
  gtk_init(&argc, &argv);

  // top level windows got WM decorations and a border frame, managed by the WM
  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Hello world");
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);
  gtk_widget_set_size_request(window, 200,
                              100);  // sets the minimum size of a widget
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(on_window_destroy), NULL);
  g_signal_connect(G_OBJECT(window), "delete-event", G_CALLBACK(on_window_delete_event),
                   NULL);

  GtkWidget *closeButton = gtk_button_new_with_mnemonic("_Close");
  gtk_button_set_relief(GTK_BUTTON(closeButton), GTK_RELIEF_NORMAL);
  g_signal_connect_swapped(G_OBJECT(closeButton), "clicked",
                           G_CALLBACK(gtk_widget_destroy), window);
  // pass the data into gtk_widget_destory instead of creating a custom callback for the button
  // just to destroy the window

  gtk_container_add(GTK_CONTAINER(window), closeButton);

  // GTK queues the widget until all preprocessing is complete before it
  // is drawn onto the screen. Not drawn if the parent is not drawn.
  // The children will be drawn.
  // the function recursively draws the window, its children and so on
  gtk_widget_show_all(window);

  // enter the main loop and start processing events
  gtk_main();
  return 0;
}
