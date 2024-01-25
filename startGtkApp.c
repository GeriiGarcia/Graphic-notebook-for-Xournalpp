#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h> 
#include <libxml2/libxml/parser.h>

char cwd[10000] = "/home/gerard/Gerard/UNI/Apuntes";
char guardarPrevisualizaciones[1024] = "/home/gerard/.libretaXournal/";
gchar *nombre = "pdf";
char *border_class = "border";
#define ANCHO_PREV (59.5+80)
#define ALTURA_PREV (84.1+80)
#define MARGIN 10

typedef struct {
    char some_value[1024];
    GtkWidget *box;
} UserData;

char *css =
    ".border          { border-color: #cc7700; border-style: solid; border-width: 10px; padding:5px;}\n"
    ".border_solid    { border-style: solid; }\n"
    ".border_margin   { margin: 10px; border-radius: 5%; opacity: 0.9; }\n"
    ".border_margin_pdf   { margin: 10px; border-radius: 5%; opacity: 0.9; border-style: solid; border-color: #add8e6; }\n";


void create_menu(GtkWidget *, GtkWidget *);
void pdf_to_image(const char *, const char *);
void on_button_clicked(GtkWidget *, gpointer );
void base64_to_image(const char *, const char *);
xmlChar* copiar_y_extraer_preview(const char *, const char *);
GtkWidget *create_file_button(const char *, gpointer , char *);

/***
 * @brief funcion que elimina desde la última barra encontrada en esa cadena hasta el final
 * 
*/
void quitarDesdeUltimaBarra(char *cadena) {
    int longitud = strlen(cadena);

    // Verificar si la cadena es vacía o tiene un solo carácter (no hay barras que quitar)
    if (longitud <= 1) {
        return;
    }
    // Empezar desde el último carácter
    int i = longitud - 1;

    // Buscar la última barra desde el final
    while (i >= 0 && cadena[i] != '/') {
        i--;
    }
    // Verificar si se encontró una barra
    if (i >= 0 && cadena[i] == '/') {
        // Eliminar la última barra
        cadena[i] = '\0';
    }
}

const char *obtenerExtension(const char *nombreArchivo) {
    const char *punto = strrchr(nombreArchivo, '.');
    // Verificar si se encontró un punto y si no es el último carácter
    if (punto != NULL && punto != nombreArchivo + strlen(nombreArchivo) - 1) {
        return punto;
    } else {
        // No se encontró extensión o el punto está al final
        return "Sin extensión";
    }
}

char* agregarBarras(char *cadena) {
    int longitud = strlen(cadena);

    // Contar la cantidad de espacios en la cadena
    int contadorEspacios = 0;
    for (int i = 0; i < longitud; i++) {
        if (cadena[i] == ' ') {
            contadorEspacios++;
        }
    }

    // Calcular la nueva longitud de la cadena con barras adicionales
    int nuevaLongitud = longitud + contadorEspacios;

    // Crear un nuevo arreglo para almacenar la cadena modificada
    char *nuevaCadena = (char *)malloc((nuevaLongitud + 1) * sizeof(char));

    // Copiar la cadena original con barras adicionales en el nuevo arreglo
    int indiceOriginal = 0;
    int indiceNuevo = 0;

    while (indiceOriginal < longitud) {
        if (cadena[indiceOriginal] == ' ') {
            nuevaCadena[indiceNuevo++] = '\\'; // Agregar barra invertida
        }

        nuevaCadena[indiceNuevo++] = cadena[indiceOriginal++];
    }

    // Agregar el carácter nulo al final de la nueva cadena
    nuevaCadena[indiceNuevo] = '\0';

    printf("%s\n",nuevaCadena);

    return nuevaCadena;
}

void abrirXournal()
{
    system("xournalpp &");
}


void css_add(char *css)
{
    /* CSS */
    GError *error = NULL;
    GtkStyleContext *context;
    GtkCssProvider *provider = gtk_css_provider_new ();
    gtk_css_provider_load_from_data (provider, css, strlen (css), &error);
    if (error != NULL)
    {
        fprintf (stderr, "CSS: %s\n", error->message);
    }
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}


/***
 * @brief sirve para añadir un pdf o xopp a la grid, teniendo en cuenta que cada uno tiene un marco diferente
*/
void box_add(GtkWidget *parent_box, char *class, const char * auxPrev, gpointer data, char * d_name, int i, int j, int esPdf)
{  
    // Crear el botón

    UserData *button_data = (UserData *)data;

    GtkWidget *normalButton = create_file_button(auxPrev, data, d_name);
    gtk_style_context_add_class(gtk_widget_get_style_context(normalButton), border_class);
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
    // Crear el botón
    GtkWidget *normalButton = gtk_button_new_with_label("+");

    gtk_widget_set_size_request(normalButton, ANCHO_PREV, ALTURA_PREV);

    gtk_grid_attach(GTK_GRID(main_box), normalButton, 0, 0, 1, 1);

    g_signal_connect(normalButton, "clicked", G_CALLBACK(abrirXournal), NULL);

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
    gtk_widget_set_name(button, nombre);
    gtk_widget_set_size_request(button, 50, 50);  // Establecer el tamaño del botón
    gtk_container_add(GTK_CONTAINER(button), image);
    gtk_container_add(GTK_CONTAINER(box), button);

    UserData *userdata = g_new(UserData, 1);
    
    strncpy(userdata->some_value, d_name, sizeof(userdata->some_value) - 1);
    userdata->some_value[sizeof(userdata->some_value) - 1] = '\0';

    userdata->box = button_data->box;
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), userdata);
    g_signal_connect(button, "enter-notify-event", G_CALLBACK(on_button_hover), userdata);
    g_signal_connect(button, "leave-notify-event", G_CALLBACK(on_button_unhover), userdata);


    return box;
}


int compararArchivos(const void *a, const void *b) {
    const char *nombreA = *(const char **)a;
    const char *nombreB = *(const char **)b;

    // Obtener extensiones
    const char *extensionA = obtenerExtension(nombreA);
    const char *extensionB = obtenerExtension(nombreB);

    // Comparar directorios primero
    if (!strcmp(extensionA, "Sin extensión") && strcmp(extensionB, "Sin extensión")) {
        return -1;
    } else if (strcmp(extensionA, "Sin extensión") && !strcmp(extensionB, "Sin extensión")) {
        return 1;
    }

    /* si quiero listar los pdf antes que los xopp
    // Comparar extensiones .xopp
    int comparacion = strcmp(extensionA, extensionB);
    if (comparacion != 0) {
        return comparacion;
    }

    return strcmp(nombreA, nombreB);
    */
    
    // Comparar extensiones .xopp primero
    if (strcmp(extensionA, ".xopp") == 0 && strcmp(extensionB, ".xopp") != 0) {
        return -1;
    } else if (strcmp(extensionA, ".xopp") != 0 && strcmp(extensionB, ".xopp") == 0) {
        return 1;
    }

    // Comparar extensiones .pdf
    int comparacion = strcmp(extensionA, extensionB);
    if (comparacion != 0) {
        return comparacion;
    }

    // Comparar nombres
    
    return strcmp(nombreA, nombreB);
}



/**
 * @brief funcio que s'executa al fer click en button1. Crea i afegeix un boto en el box passat depenent dels archius que hi ha a la carpeta
 * 
 * @param widget [GtkWidget]
 * @param data [UserData]
 */
void on_button_clicked(GtkWidget *widget, gpointer data) {

    UserData *button_data = (UserData *)data;
    
    // en cas que haigi fet click a un .pdf o un .xopp, s'obrira el xournalpp
    if(strcmp(gtk_widget_get_name(widget), nombre) == 0 || strcmp(gtk_widget_get_name(widget), "xournal") == 0)
    {
        char comando[1024] = "";
        char xournal[1024] = "xournalpp "; 
        strcat(comando, cwd);
        strcat(comando, "/");
        strcat(comando, button_data->some_value);
        strcat(xournal, agregarBarras(comando));
        system(strcat(xournal, " &")); //para que se vaya al background
    }
    else if(!strcmp(obtenerExtension(gtk_button_get_label(GTK_BUTTON(widget))), "Sin extensión")) // en cas que no tingui extensio (directori) he de llistar
    {
        
        // si no he pulsat button1, cambiar de directori al que marqui cwd
        if(strcmp(gtk_button_get_label(GTK_BUTTON(widget)), "button1"))
        {
            if(!strcmp(gtk_button_get_label(GTK_BUTTON(widget)),".."))
                quitarDesdeUltimaBarra(cwd);
            else
            {
                strcat(cwd,"/");
                chdir(strcat(cwd,gtk_button_get_label(GTK_BUTTON(widget))));
                printf("%s\n",cwd);
            }

            //agafo el pare del widget i canvio el text del directori actual
            GtkWidget *parent = gtk_widget_get_parent(button_data->box);
            parent = gtk_widget_get_parent(parent);
            parent = gtk_widget_get_parent(parent);

            GList *children, *iter;
            children = gtk_container_get_children(GTK_CONTAINER(parent));
            
            for(iter = children; iter != NULL; iter = g_list_next(iter))
            {
                
                if(strcmp(gtk_widget_get_name((iter->data)), "fijo") == 0)
                {   

                    GList *children2, *iter2;
                    children2 = gtk_container_get_children(GTK_CONTAINER(iter->data));

                    for(iter2 = children2; iter2 != NULL; iter2 = g_list_next(iter2))
                    {
                        if(GTK_IS_LABEL(iter2->data))
                        {   
                            char result[1000] = "<big><b> ";
                            strcat(result, cwd);
                            strcat(result, " </b></big>");
                            gtk_label_set_markup(GTK_LABEL(iter2->data),result);
                            break;
                        }
                    }
                    break;
                }
            }

            g_list_free(children);

        }


        // borro tots els fills del box de fitxers
        GList *children, *iter;
        children = gtk_container_get_children(GTK_CONTAINER(button_data->box));
        for(iter = children; iter != NULL; iter = g_list_next(iter))
            gtk_widget_destroy(GTK_WIDGET(iter->data));

        g_list_free(children);

        

        //Llistar directoris
        g_print("Listando Directorios\n");

        char *archivosDirectorios[1024];

        for (int i = 0; i < 1024; i++) {
            archivosDirectorios[i] = NULL;
        }

        int n = 0;

        DIR *d;
        struct dirent *dir;
        d = opendir(cwd);
        if (d) 
        {
            
            while ((dir = readdir(d)) != NULL) 
            {
                printf("%s\n", dir->d_name);

                //en cas que no es digui "." el directori y que tingui format .pdf o .xopp o que no tingui format (directori) llavors es MOSTRARAN
                if(strcmp(dir->d_name,".") && (!strcmp(obtenerExtension(dir->d_name), "Sin extensión") || (!strcmp(obtenerExtension(dir->d_name), ".xopp") || !strcmp(obtenerExtension(dir->d_name), ".pdf")) )) /*&& strcmp(dir->d_name,"..")) */ // he de posar mes excepcions
                {
                    archivosDirectorios[n] = (char *)malloc(strlen(dir->d_name) + 1); // Asignar memoria
                    archivosDirectorios[n][0] = '\0';
                    strcat(archivosDirectorios[n], dir->d_name);
                    n++;
                }

            }
            closedir(d);
        }

        //ordenar archivosDirectorios[n]   
        qsort(archivosDirectorios, n, sizeof(char *), compararArchivos);

        int i = 1;
        int j = 0;
        for (int k = 0; k < n; k++) //mostrare els artxius un cop ordenats
        {
            
            if(!strcmp(obtenerExtension(archivosDirectorios[k]), "Sin extensión")) // si es un directorio
            {
                //posar widgets al box   
                GtkWidget *normalButton = gtk_button_new_with_label(archivosDirectorios[k]);
                gtk_widget_set_margin_start(normalButton, MARGIN);
                gtk_widget_set_margin_end(normalButton, MARGIN);
                gtk_widget_set_margin_top(normalButton, MARGIN);
                gtk_widget_set_margin_bottom(normalButton, MARGIN);

                gtk_widget_set_size_request(normalButton, ANCHO_PREV, ALTURA_PREV);

                gtk_grid_attach(GTK_GRID(button_data->box), normalButton, i, j, 1, 1);

                UserData *userdata = g_new(UserData, 1);
                strncpy(button_data->some_value, archivosDirectorios[k], sizeof(button_data->some_value) - 1);
                button_data->some_value[sizeof(button_data->some_value) - 1] = '\0';

                userdata->box = button_data->box;
                g_signal_connect(normalButton, "clicked", G_CALLBACK(on_button_clicked), userdata);
            }
            else if(!strcmp(obtenerExtension(archivosDirectorios[k]), ".pdf")) // si es pdf
            {
                char auxPdf[1024] = "";
                char auxPrev[1024] = "";

                strcat(auxPdf, cwd);
                strcat(auxPdf, "/");
                strcat(auxPdf, archivosDirectorios[k]);

                strcat(auxPrev, guardarPrevisualizaciones);
                strcat(auxPrev, archivosDirectorios[k]);
                strcat(auxPrev, ".png");

                pdf_to_image(auxPdf, auxPrev);

                css_add(css);
                box_add(button_data->box, "border_margin_pdf", auxPrev, data, archivosDirectorios[k], i, j, 1);



            }
            else // si es xopp
            {

                char auxXopp[1024] = "";
                char auxPrev[1024] = "";

                strcat(auxXopp, cwd);
                strcat(auxXopp, "/");
                strcat(auxXopp, archivosDirectorios[k]);

                strcat(auxPrev, guardarPrevisualizaciones);
                strcat(auxPrev, archivosDirectorios[k]);
                strcat(auxPrev, ".png");

                base64_to_image(copiar_y_extraer_preview(agregarBarras(auxXopp), "/home/gerard/.libretaXournal/previewXournal.xml" /*guardarPrevisualizaciones*/), auxPrev);

                css_add(css);
                box_add(button_data->box, "border_margin", auxPrev, data, archivosDirectorios[k], i, j, 0);


            }

                    
            //nombre de columnes q vull que tingui
            if(i == 3 ){
                i = 0;
                j += 1;
            }
            else
                i += 1;
        }

        for (int i = 0; i < n; i++) {
            free(archivosDirectorios[i]);
        }

        afegirPredeterminat(button_data->box);    


    }

    // recarrega la vista
    gtk_widget_show_all(button_data->box);

}

static void activate (GtkApplication *app, gpointer user_data){
    
    GtkWidget *window;
    GtkWidget *button1;
    GtkWidget *files;
    GtkWidget *main;

    char result[100] = "<big><b> ";
    strcat(result, cwd);
    strcat(result, " </b></big>");


    GtkWidget *container = gtk_fixed_new();
    gtk_widget_set_name(container, "fijo");
   

    GtkWidget *view;

    view = gtk_label_new(cwd);
    gtk_label_set_markup(GTK_LABEL(view),result);
    gtk_label_set_xalign(GTK_LABEL(view), 0.0); // Alineación a la izquierda
    gtk_label_set_yalign(GTK_LABEL(view), 0.0); // Alineación en la parte superior
    gtk_widget_set_size_request(view, 300, 20);
    
    


    window = gtk_application_window_new(app); //decimos que window sera una ventana de aplicacion
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit),NULL);
    gtk_window_set_title(GTK_WINDOW(window), "Libreta Para Xournalpp");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 600);

    //creamos y metemos en window el contenedor main
    main = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);   
    files = gtk_grid_new();

    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

    
    gtk_container_add(GTK_CONTAINER(window), main);
    

    //gtk_box_pack_start(GTK_BOX(window),main,TRUE,TRUE, 50);
    gtk_container_add(GTK_CONTAINER(scrolled_window), files);
    gtk_box_pack_end(GTK_BOX(main), scrolled_window, TRUE, TRUE, 10);


    gtk_widget_set_halign(files, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(files, GTK_ALIGN_CENTER);



    //la primera pantalla la deixare per si vull posar més d'una opcio quan s'inicii l'aplicacio, com opcions de configuracio o algo aixi.
    button1 = gtk_button_new_with_label("button1"); //creamos el boton 1
    gtk_grid_attach(GTK_GRID(files), button1, 0, 0, 50, 50);

    create_menu(main, window);

    //Declaramos UserData y decimos que cuando se haga click en el boton 1 se ejecute la funcion on_button_clicked
    UserData *userdata = g_new(UserData, 1);
    strncpy(userdata->some_value, "button1", sizeof(userdata->some_value) - 1);
    userdata->some_value[sizeof(userdata->some_value) - 1] = '\0';
    userdata->box = files;
    g_signal_connect(button1, "clicked", G_CALLBACK(on_button_clicked), userdata);
    
    gtk_fixed_put(GTK_FIXED(container), view, 5, 5);
    
    gtk_container_add(GTK_CONTAINER(main), container);
    //gtk_box_pack_start(GTK_BOX(main), view, TRUE, TRUE, 5);

 
    gtk_widget_show_all(window);

    
    
    gtk_main();
    g_free(userdata);
}


int main( int argc, char **argv){
    GtkApplication *app;

    chdir(cwd);

    getcwd(cwd, sizeof(cwd));

    app = gtk_application_new("in.aducators",G_APPLICATION_FLAGS_NONE);

    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    

    g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);

    return 0;
}   