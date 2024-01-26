#include "includes.h"

void ponerOpcionesACero()
{
    for (int i = 0; i < 20; i++)
        opcionesMenu[i] = 0;
}

GtkWidget *get_widget_by_name(GtkContainer *container, const gchar *name) {
    GList *children = gtk_container_get_children(container);

    while (children != NULL) {
        GtkWidget *child = GTK_WIDGET(children->data);
        if (gtk_widget_get_name(child) != NULL && g_strcmp0(gtk_widget_get_name(child), name) == 0) {
            // Se encontró el widget con el nombre deseado
            g_list_free(children);
            return child;
        }
        
        if (GTK_IS_CONTAINER(child)) {
            // Si el widget es un contenedor, realizar la búsqueda recursiva
            GtkWidget *result = get_widget_by_name(GTK_CONTAINER(child), name);
            if (result != NULL) {
                g_list_free(children);
                return result;
            }
        }

        children = g_list_next(children);
    }

    g_list_free(children);
    
    return NULL;
}


/***
 * @brief se le tiene que pasar un gpointer con un widget contenedor
*/
void runOk(GtkWidget *botonOk, gpointer data) // por algun motivo que desconozco tengo que poner GtkWidget, creo que es el widget de a lo que le has dado, en este caso el boton OK
{
    UserData *button_data = (UserData *)data;
    int opcion = -1;
    int contadorOpciones = 0;

    gchar *texto = "textOption";
    GtkWidget * textWidget = get_widget_by_name(GTK_CONTAINER(button_data->box), texto);
    

    const gchar *widget_value = gtk_entry_get_text(GTK_ENTRY(textWidget));

    for (int i = 0; i < 20; i++)
    {
        if(opcionesMenu[i] == 1)
        {
            opcion = i;
            contadorOpciones++;
        }
    }
    
    if(contadorOpciones == 1)
    {
        switch (opcion)
        {
        case NUEVA_CARPETA:
            
            if(strcmp(widget_value, ""))
            {
                char aux[2048]="";
                strcat(aux, cwd);
                strcat(aux, "/");
                strcat(aux, widget_value);
                char *aux2 = agregarBarras(aux); 

                char mkdir[2056] = "mkdir ";
                strcat(mkdir, aux2);

                system(mkdir);

                //per refrescar la pagina a veure si puc
                GtkWidget *main = gtk_widget_get_parent(textWidget);
                main = gtk_widget_get_parent(main);

                texto = "files";
                GtkWidget *grid = get_widget_by_name(GTK_CONTAINER(main), texto);
                GtkWidget *auxButton = gtk_button_new_with_label("Predeterminado");
                gtk_grid_attach(GTK_GRID(grid), auxButton, 0, 0, 50, 50);

                UserData *userdata = g_new(UserData, 1);
                strncpy(userdata->some_value, "button1", sizeof(userdata->some_value) - 1);
                userdata->some_value[sizeof(userdata->some_value) - 1] = '\0';
                userdata->box = grid;
                on_button_clicked(GTK_WIDGET(auxButton), userdata);
            }
            break;

        case NUEVO_XOURNAL:

            if(strcmp(widget_value, ""))
            {
                char aux[2048]="";
                strcat(aux, cwd);
                strcat(aux, "/");
                strcat(aux, widget_value);
                char *aux2 = agregarBarras(aux); 

                char xournalpp[2056] = "xournalpp ";
                strcat(xournalpp, aux2);
                strcat(xournalpp, " &");

                system(xournalpp);
            }
            break;
        default:
            break;
        }
    }

    gtk_entry_set_text(GTK_ENTRY(textWidget), "");
    gtk_widget_hide(textWidget);
    gtk_widget_hide(botonOk);
    ponerOpcionesACero();
    
    
    
}

void nuevaCarpeta(GtkWidget *widget, gpointer data)
{
    UserData *button_data = (UserData *)data;
    gchar *texto = "textOption";
    GtkWidget * textWidget = get_widget_by_name(GTK_CONTAINER(button_data->box), texto);
    texto = "buttonOk";
    GtkWidget * buttonOk = get_widget_by_name(GTK_CONTAINER(button_data->box), texto);

    gtk_widget_show(textWidget);
    gtk_widget_show(buttonOk);
    gtk_entry_set_text(GTK_ENTRY(textWidget), "'Nombre de directorio'");

    ponerOpcionesACero();
    opcionesMenu[NUEVA_CARPETA] = 1;
}

void reset(GtkWidget *widget, gpointer data)
{
    UserData *button_data = (UserData *)data;
    ponerOpcionesACero();
    gchar *texto = "textOption";
    GtkWidget * textWidget = get_widget_by_name(GTK_CONTAINER(button_data->box), texto);
    texto = "buttonOk";
    GtkWidget * buttonOk = get_widget_by_name(GTK_CONTAINER(button_data->box), texto);

    gtk_widget_hide(textWidget);
    gtk_widget_hide(buttonOk);
}

// Función que maneja la selección de los elementos del menú
void on_menu_item_activate(GtkMenuItem *menu_item, gpointer data) {
    g_print("Se seleccionó: %s\n", (const char *)data);
}

// Función que crea el menú y lo agrega al contenedor principal
void create_menu(GtkWidget *main_box, GtkWidget *window) {

    ponerOpcionesACero();
    
    GtkWidget *menu_bar;
    GtkWidget *menu1, *menu2, *menu3;
    GtkWidget *item1, *item2, *item3;
    GtkWidget *section1_1, *section1_2, *section2_1, *section2_2, *section3_1, *section3_2;

    // Crear la barra de menú
    menu_bar = gtk_menu_bar_new();
    gtk_box_pack_start(GTK_BOX(main_box), menu_bar, FALSE, FALSE, 0);

    // Botón 1
    menu1 = gtk_menu_new();
    item1 = gtk_menu_item_new_with_label("Archivo");

    section1_1 = gtk_menu_item_new_with_label("Nueva Carpeta");
    UserData *data = g_new(UserData, 1);
    strncpy(data->some_value, cwd, sizeof(data->some_value) - 1); 
    data->box = main_box;
    g_signal_connect(G_OBJECT(section1_1), "activate", G_CALLBACK(nuevaCarpeta), data);

    section1_2 = gtk_menu_item_new_with_label("Exportar a PDF");
    g_signal_connect(G_OBJECT(section1_2), "activate", G_CALLBACK(on_menu_item_activate), "Exportar a PDF");

    GtkWidget *section1_3 = gtk_menu_item_new_with_label("Reset");
    g_signal_connect(G_OBJECT(section1_3), "activate", G_CALLBACK(reset), data);


    gtk_menu_shell_append(GTK_MENU_SHELL(menu1), section1_1);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu1), section1_2);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu1), section1_3);

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(item1), menu1);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), item1);

    // Botón 2
    menu2 = gtk_menu_new();
    item2 = gtk_menu_item_new_with_label("Navegación");

    section2_1 = gtk_menu_item_new_with_label("Sección 2.1");
    section2_2 = gtk_menu_item_new_with_label("Sección 2.2");

    g_signal_connect(G_OBJECT(section2_1), "activate", G_CALLBACK(on_menu_item_activate), "Sección 2.1");
    g_signal_connect(G_OBJECT(section2_2), "activate", G_CALLBACK(on_menu_item_activate), "Sección 2.2");

    gtk_menu_shell_append(GTK_MENU_SHELL(menu2), section2_1);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu2), section2_2);

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(item2), menu2);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), item2);

    // Botón 3
    menu3 = gtk_menu_new();
    item3 = gtk_menu_item_new_with_label("Herramientas");

    section3_1 = gtk_menu_item_new_with_label("Sección 3.1");
    section3_2 = gtk_menu_item_new_with_label("Sección 3.2");

    g_signal_connect(G_OBJECT(section3_1), "activate", G_CALLBACK(on_menu_item_activate), "Sección 3.1");
    g_signal_connect(G_OBJECT(section3_2), "activate", G_CALLBACK(on_menu_item_activate), "Sección 3.2");

    gtk_menu_shell_append(GTK_MENU_SHELL(menu3), section3_1);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu3), section3_2);

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(item3), menu3);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), item3);

    gtk_widget_show_all(window);
}