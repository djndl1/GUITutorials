#include <glib.h>
#include <gtk/gtk.h>

#undef G_LOG_DOMAIN
#define G_LOG_DOMAIN "gtk_c.helloworld"

static void destroy(GtkWidget *widget, gpointer data) {
  g_debug("Destroying the main window\n");
  gtk_main_quit();
}

static gboolean delete_event(GtkWidget *widget, GdkEvent *event,
                             gpointer data) {
  g_debug("Deleting the main window\n");

  GtkWidget *dialog = gtk_dialog_new_with_buttons("Staying?",
                                                  GTK_WINDOW(widget),
                                                  GTK_DIALOG_MODAL,
                                                  "_Yes", GTK_RESPONSE_ACCEPT,
                                                  "_No", GTK_RESPONSE_REJECT,
                                                  NULL);
  gint response = gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);

  return response == GTK_RESPONSE_ACCEPT;
}

static void on_window_title_notify(GObject *gobject,
                                   GParamSpec *pspec,
                                   gpointer user_data)
{
  gchar *title = NULL;
  g_object_get(gobject, "title", &title, NULL);
  g_message("window %s set: %s",
            g_param_spec_get_name(pspec),
            title);

  g_free(title);
}

static gboolean key_press_callback(GtkWidget *widget, GdkEventKey *event,
                                   GtkWidget *user_data) {
  static gboolean pressed = FALSE;
  static gchar *previous = NULL;
  if (event->keyval == GDK_KEY_F1 && !pressed) {
    const gchar *label_text = gtk_label_get_text(GTK_LABEL(user_data));
    if (previous != NULL) {
      g_free(previous);
    }
    previous = g_strdup(label_text);

    g_object_set(GTK_LABEL(user_data), "use-markup", TRUE, NULL);
    g_object_set(GTK_LABEL(user_data), "label", "<i>My Name</i>", NULL);
    g_object_set(widget, "title", "Deng Hongzhi", NULL);
    pressed = TRUE;
    return TRUE;
  } else if (event->keyval == GDK_KEY_F1 && pressed && previous != NULL) {
    g_object_set(GTK_LABEL(user_data), "label", previous, NULL);
    pressed = FALSE;

    return TRUE;
  }
  return FALSE;
}

int main(int argc, char *argv[]) {
  // set up GTK environment: obtaining GDK display,
  // preparing the Glib main event loop
  // basic signal handling
  gtk_init(&argc, &argv);

  // top level windows got WM decorations and a border frame, managed by the WM
  // any window visually seen as a window should be a top level window.
  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_object_set(window, "title", "Hans Meyer", NULL);
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
  // sets the minimum size of a widget
  g_object_set(window, "height-request", 100, NULL);
  g_object_set(window, "width-request", 300, NULL);

  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(destroy), NULL);
  g_signal_connect(G_OBJECT(window), "delete-event", G_CALLBACK(delete_event),
                   NULL);
  g_signal_connect(G_OBJECT(window), "notify::title", G_CALLBACK(on_window_title_notify), NULL);

  // often used to label other widgets, but also to create large blocks of
  // uneditable, formatted or wrapped text
  GtkWidget *label = gtk_label_new("Meyer");
  gtk_label_set_selectable(GTK_LABEL(label), TRUE);
  // so that the text can be selected
  // error messages shoudl be set as selectable
  g_signal_connect(G_OBJECT(window), "key-press-event",
                   G_CALLBACK(key_press_callback), label);

  // add the label as a child widget of the window
  gtk_container_add(GTK_CONTAINER(window), label);

  // GTK queues the widget until all preprocessing is complete before it
  // is drawn onto the screen. Not drawn if the parent is not drawn.
  // The children will be drawn.
  // the function recursively draws the window, its children and so on
  gtk_widget_show_all(window);

  // enter the main loop and start processing events
  gtk_main();
  return 0;
}
