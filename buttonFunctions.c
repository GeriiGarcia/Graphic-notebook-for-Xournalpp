#include "includes.h"


void abrirXournal();
void on_button_clicked(GtkWidget *, gpointer );
void afegirPredeterminat(GtkWidget *);
GtkWidget *create_file_button(const char *, gpointer , char *);

/***
 * @brief sirve para añadir un pdf o xopp a la grid, teniendo en cuenta que cada uno tiene un marco diferente
 * 
 * @param parent_box box al que añadir el widget
 * @param class CSS class para añadir estilo
 * @param auxPrev ruta de la que queremos listar la imagen
 * @param data data que se pasa en la funcion on_button_clicked()
 * @param d_name se usa para la etiqueta
 * @param i columna 
 * @param j fila
 * @param esPdf 1 si es PDF. 0 si no es PDF
*/
void box_add(GtkWidget *parent_box, char *class, const char * auxPrev, gpointer data, char * d_name, int i, int j, int esPdf)
{  
    // Crear el botón

    UserData *button_data = (UserData *)data;

    GtkWidget *normalButton = create_file_button(auxPrev, data, d_name);
    gtk_style_context_add_class(gtk_widget_get_style_context(normalButton), "border");
    gtk_style_context_add_class(gtk_widget_get_style_context(normalButton), class);

    gtk_widget_set_margin_start(normalButton, MARGIN);
    gtk_widget_set_margin_end(normalButton, MARGIN);
    gtk_widget_set_margin_top(normalButton, MARGIN);
    gtk_widget_set_margin_bottom(normalButton, MARGIN);

    if(esPdf == 0)
        gtk_widget_set_name(normalButton, "xournal");
    else
        gtk_widget_set_name(normalButton, "pdf");

    //poner nombre abajo
    GtkWidget *label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label), d_name);
    gtk_container_add(GTK_CONTAINER(normalButton), label);
    gtk_grid_attach(GTK_GRID(button_data->box), normalButton, i, j, 1, 1);
    
}

void afegirPredeterminat(GtkWidget *main_box)
{
    // Crear un contenedor para el botón y la vista previa
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    // Cargar la imagen de vista previa del PDF en un GtkImage
    GError *error = NULL;
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(directorioMas, &error);

    if (!pixbuf) {
        g_printerr("Error cargando la imagen: %s\n", error->message);
        g_error_free(error);
        return;
    }

    GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple(pixbuf, 20, 20, GDK_INTERP_BILINEAR);

    if (!scaled_pixbuf) {
        g_printerr("Error escalando la imagen.\n");
        g_object_unref(pixbuf);
        return;
    }

    g_object_unref(pixbuf);  // Liberar el pixbuf original después de escalar

    GtkWidget *image = gtk_image_new_from_pixbuf(scaled_pixbuf);

    // Crear un GtkButton con la vista previa como contenido
    GtkWidget *button = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(button), image);
    gtk_container_add(GTK_CONTAINER(box), button);

    gtk_widget_set_size_request(button, ANCHO_PREV, ALTURA_PREV);

    g_signal_connect(button, "clicked", G_CALLBACK(abrirXournal), NULL);

    gtk_widget_set_margin_start(box, MARGIN);
    gtk_widget_set_margin_end(box, MARGIN);
    gtk_widget_set_margin_top(box, MARGIN);
    gtk_widget_set_margin_bottom(box, MARGIN);

    gtk_grid_attach(GTK_GRID(main_box), box, 0, 0, 1, 1);



    // Liberar el pixbuf escalado después de usarlo
    g_object_unref(scaled_pixbuf);

    gtk_widget_show_all(main_box);
}




void on_button_hover(GtkWidget *widget, GdkEvent *event, gpointer data) {
    UserData *button_data = (UserData *)data;
    gchar *tooltip = gtk_widget_get_tooltip_text(widget);

    if (!tooltip) {
        // Si no hay tooltip, establecer el texto
        gtk_widget_set_tooltip_text(widget, button_data->some_value);
    }
}

void on_button_unhover(GtkWidget *widget, gpointer data) {
    // Limpiar el tooltip cuando el ratón deja el área del widget
    gtk_widget_set_tooltip_text(widget, NULL);
}


GtkWidget *create_file_button(const char *filename, gpointer data, char *d_name) {
    UserData *button_data = (UserData *)data;

    // Crear un contenedor para el botón y la vista previa
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    // Cargar la imagen de vista previa del PDF en un GtkImage
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
    GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple( pixbuf, ANCHO_PREV, ALTURA_PREV, GDK_INTERP_BILINEAR);
    g_object_unref(pixbuf);  // Liberar el pixbuf original después de escalar
    GtkWidget *image = gtk_image_new_from_pixbuf(scaled_pixbuf);

    // Crear un GtkButton con la vista previa como contenido
    GtkWidget *button = gtk_button_new();
    gtk_widget_set_name(button, "pdf");
    gtk_widget_set_size_request(button, 50, 50);  // Establecer el tamaño del botón
    gtk_container_add(GTK_CONTAINER(button), image);
    gtk_container_add(GTK_CONTAINER(box), button);

    UserData *userdataNew = g_new(UserData, 1);
    
    strncpy(userdataNew->some_value, d_name, sizeof(userdataNew->some_value) - 1);
    userdataNew->some_value[sizeof(userdataNew->some_value) - 1] = '\0';

    userdataNew->box = button_data->box;
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), userdataNew);
    g_signal_connect(button, "enter-notify-event", G_CALLBACK(on_button_hover), userdataNew);
    g_signal_connect(button, "leave-notify-event", G_CALLBACK(on_button_unhover), userdataNew);

    return box;
}


void abrirXournal()
{
    system("xournalpp &");
}