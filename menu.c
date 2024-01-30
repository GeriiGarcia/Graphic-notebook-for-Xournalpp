#include "includes.h"
#include <math.h>

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
 * @brief que al hacer click en enter se ejecute runOk 
 * Que al hacer click en f5 se actualize el directorio
*/
gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
    UserData *button_data = (UserData *)user_data;
    
    if (event->keyval == GDK_KEY_F5) {
        refrescarDirectori(widget, user_data);
        return TRUE; // Indica que se ha manejado el evento
    }
    else if(event->keyval == GDK_KEY_Return){
        GtkWidget *box = gtk_widget_get_parent(button_data->box);
        box = gtk_widget_get_parent(box);
        box = gtk_widget_get_parent(box);
        box = get_widget_by_name(GTK_CONTAINER(box), "fijo");

        GtkWidget *botoOk = get_widget_by_name(GTK_CONTAINER(box), "buttonOk");

        UserData *userdata = g_new(UserData, 1);
        userdata->box = box;
        runOk(botoOk, userdata);
    }

    return FALSE; // Permite que otros manejadores de eventos procesen la tecla
}


long long calcularTamanoCarpeta(const char *ruta) {
    DIR *dir;
    struct dirent *entrada;
    struct stat infoArchivo;
    long long tamanoTotal = 0;

    dir = opendir(ruta);

    if (dir == NULL) {
        perror("Error al abrir el directorio");
        return -1;
    }

    while ((entrada = readdir(dir)) != NULL) {
        char rutaCompleta[2048];
        snprintf(rutaCompleta, sizeof(rutaCompleta), "%s/%s", ruta, entrada->d_name);

        if (stat(rutaCompleta, &infoArchivo) == 0) {
            if (S_ISDIR(infoArchivo.st_mode)) {
                if (strcmp(entrada->d_name, ".") != 0 && strcmp(entrada->d_name, "..") != 0) {
                    // Recursivamente calcular el tamaño de subdirectorios
                    long long tamanoSubDirectorio = calcularTamanoCarpeta(rutaCompleta);
                    if (tamanoSubDirectorio >= 0) {
                        tamanoTotal += tamanoSubDirectorio;
                    } else {
                        closedir(dir);
                        return -1;
                    }
                }
            } else {
                // Sumar el tamaño de archivos regulares
                tamanoTotal += infoArchivo.st_size;
            }
        } else {
            perror("Error al obtener información del archivo");
            closedir(dir);
            return -1;
        }
    }

    closedir(dir);
    return tamanoTotal;
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

                //per refrescar la pagina 
                //El que faig es afegir un element amb el nom Predeterminat a la grid i per-li click "artificialment"
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

        gtk_entry_set_text(GTK_ENTRY(textWidget), "");
        gtk_widget_hide(textWidget);
        gtk_widget_hide(botonOk);
        ponerOpcionesACero();
    }

    
    
    
    
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

void vaciarCache()
{
    system("rm -rf /home/$(whoami)/.libretaXournal");
    system("mkdir /home/$(whoami)/.libretaXournal");
}

//encontrar items de un submenu del menu
void encontrarHijos(GtkWidget *widget, gpointer data) {
    const gchar *nombreWidget = gtk_widget_get_name(widget);

    // Verificar si el widget tiene el nombre "vaciarCache"
    if (nombreWidget != NULL && g_strcmp0(nombreWidget, "vaciarCache") == 0) {
        const char *rutaCarpeta = "/home/gerard/.libretaXournal";
        long long tamanoEnBytes = calcularTamanoCarpeta(rutaCarpeta);
        double tamanoEnMB = (double)tamanoEnBytes / (1024 * 1024);
        char cadena[2048];
        sprintf(cadena, "Vaciar Caché: %.2f MB", round(tamanoEnMB * 100.0) / 100.);
        gtk_menu_item_set_label(GTK_MENU_ITEM(widget), cadena);
    }
}

void recargarTamaño(GtkWidget *widget, gpointer data)
{

    if (GTK_IS_MENU_ITEM(widget)) {
        const gchar *nombreWidget = gtk_widget_get_name(widget);

        // Verificar si el widget tiene el nombre "item3"
        if (nombreWidget != NULL && g_strcmp0(nombreWidget, "item3") == 0) {
            // Obtener el submenú
            GtkWidget *submenu = gtk_menu_item_get_submenu(GTK_MENU_ITEM(widget));

            // Iterar sobre los elementos del submenú
            gtk_container_foreach(GTK_CONTAINER(submenu), encontrarHijos, NULL);
        }
    }

}

void refrescarDirectori(GtkWidget *widget, gpointer data)
{
    UserData *button_data = (UserData *)data;

    GtkWidget *box = gtk_widget_get_parent(button_data->box);
    
    box = get_widget_by_name(GTK_CONTAINER(box), "files");

    //per refrescar la pagina 
    //El que faig es afegir un element amb el nom Predeterminat a la grid i per-li click "artificialment"
    GtkWidget *auxButton = gtk_button_new_with_label("Predeterminado");
    gtk_grid_attach(GTK_GRID(box), auxButton, 0, 0, 50, 50);

    UserData *userdata = g_new(UserData, 1);
    strncpy(userdata->some_value, "button1", sizeof(userdata->some_value) - 1);
    userdata->some_value[sizeof(userdata->some_value) - 1] = '\0';
    userdata->box = box;
    on_button_clicked(GTK_WIDGET(auxButton), userdata);

}

void passTo(GtkWidget *widget, gpointer data)
{
    if(data)
        system("passTo UNI &");
    else
        system("passTo &");
}

void exportarPdf(GtkWidget *widget, gpointer data)
{
    UserData *button_data = (UserData *)data;
    gchar *texto = "textOption";
    GtkWidget * textWidget = get_widget_by_name(GTK_CONTAINER(button_data->box), texto);
    texto = "buttonOk";
    GtkWidget * buttonOk = get_widget_by_name(GTK_CONTAINER(button_data->box), texto);

    gtk_widget_show(textWidget);
    //gtk_widget_show(buttonOk);
    gtk_entry_set_text(GTK_ENTRY(textWidget), "Pulsa un archivo");

    ponerOpcionesACero();
    opcionesMenu[EXPORTAR_PDF] = 1;
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
    g_signal_connect(G_OBJECT(section1_2), "activate", G_CALLBACK(exportarPdf), data);

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
    gtk_widget_set_name(item2, "item2");

    section2_1 = gtk_menu_item_new_with_label("Mostrar PDFs?");
    section2_2 = gtk_menu_item_new_with_label("Mostrar previsializaciones?");
    GtkWidget *section2_3 = gtk_menu_item_new_with_label("Cambiar ruta predeterminada");
    GtkWidget *section2_4 = gtk_menu_item_new_with_label("Tipo de orden");
    GtkWidget *section2_5 = gtk_menu_item_new_with_label("Refrescar directorio f5");
    GtkWidget *section2_6 = gtk_menu_item_new_with_label("Volver a inicio");

    g_signal_connect(G_OBJECT(section2_1), "activate", G_CALLBACK(on_menu_item_activate), NULL);
    g_signal_connect(G_OBJECT(section2_2), "activate", G_CALLBACK(on_menu_item_activate), NULL);
    g_signal_connect(G_OBJECT(section2_3), "activate", G_CALLBACK(on_menu_item_activate), NULL);
    g_signal_connect(G_OBJECT(section2_4), "activate", G_CALLBACK(on_menu_item_activate), NULL);
    g_signal_connect(G_OBJECT(section2_5), "activate", G_CALLBACK(refrescarDirectori), data);
    g_signal_connect(G_OBJECT(section2_6), "activate", G_CALLBACK(on_menu_item_activate), NULL);
    
    gtk_menu_shell_append(GTK_MENU_SHELL(menu2), section2_1);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu2), section2_2);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu2), section2_3);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu2), section2_4);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu2), section2_5);

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(item2), menu2);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), item2);

    // Botón 3
    menu3 = gtk_menu_new();
    item3 = gtk_menu_item_new_with_label("Herramientas");
    gtk_widget_set_name(item3, "item3");
    g_signal_connect(G_OBJECT(item3), "activate", G_CALLBACK(recargarTamaño), "Item 3");

    section3_1 = gtk_menu_item_new_with_label("passTo");
    g_signal_connect(G_OBJECT(section3_1), "activate", G_CALLBACK(passTo), NULL);

    section3_2 = gtk_menu_item_new_with_label("passTo UNI");
    g_signal_connect(G_OBJECT(section3_2), "activate", G_CALLBACK(passTo), "UNI");

    const char *rutaCarpeta = "/home/gerard/.libretaXournal";
    long long tamanoEnBytes = calcularTamanoCarpeta(rutaCarpeta);
    double tamanoEnMB = (double)tamanoEnBytes / (1024 * 1024);
    char cadena[2048];
    sprintf(cadena, "Vaciar Caché: %f", tamanoEnMB);
    GtkWidget *section3_3 = gtk_menu_item_new_with_label(cadena);
    gtk_widget_set_name(section3_3, "vaciarCache");
    g_signal_connect(G_OBJECT(section3_3), "activate", G_CALLBACK(vaciarCache), NULL);

    
    

    gtk_menu_shell_append(GTK_MENU_SHELL(menu3), section3_1);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu3), section3_2);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu3), section3_3);

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(item3), menu3);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), item3);

    gtk_widget_show_all(window);
}