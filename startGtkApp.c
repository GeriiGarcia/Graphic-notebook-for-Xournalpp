#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h> 

char cwd[10000] = "/home/gerard/Gerard/UNI/Apuntes";

typedef struct {
    int some_value;
    GtkWidget *box;
} UserData;

void create_menu(GtkWidget *, GtkWidget *);

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


/**
 * @brief funcio que s'executa al fer click en button1. Crea i afegeix un boto en el box passat depenent dels archius que hi ha a la carpeta
 * 
 * @param widget [GtkWidget]
 * @param data [UserData]
 */
void on_button_clicked(GtkWidget *widget, gpointer data) {

    UserData *button_data = (UserData *)data;

    //obrir fitxer en el cas que sigui d'extensio .xopp
    if(!strcmp(obtenerExtension(gtk_button_get_label(GTK_BUTTON(widget))), ".xopp") || !strcmp(obtenerExtension(gtk_button_get_label(GTK_BUTTON(widget))), ".pdf"))
    {
        char comando[1024] = "";
        char xournal[11] = "xournalpp "; 
        strcat(comando, cwd);
        strcat(comando, "/");
        strcat(comando, gtk_button_get_label(GTK_BUTTON(widget)));
        system(strcat(xournal, agregarBarras(comando))); 
    }
    else if(!strcmp(obtenerExtension(gtk_button_get_label(GTK_BUTTON(widget))), "Sin extensión")) // en cas que no tingui extensio (directori)
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

        }


        // borro tots els fills del box de fitxers
        GList *children, *iter;
        children = gtk_container_get_children(GTK_CONTAINER(button_data->box));
        for(iter = children; iter != NULL; iter = g_list_next(iter))
            gtk_widget_destroy(GTK_WIDGET(iter->data));

        g_list_free(children);



        //Llistar directoris
        g_print("Listando Directorios\n");

        DIR *d;
        struct dirent *dir;
        d = opendir(cwd);
        if (d) {
            while ((dir = readdir(d)) != NULL) 
            {
                printf("%s\n", dir->d_name);

                //en cas que no es digui "." el directori y que tingui format .pdf o .xopp o que no tingui format (directori) llavors es mostraran
                if(strcmp(dir->d_name,".") && (!strcmp(obtenerExtension(dir->d_name), "Sin extensión") || (!strcmp(obtenerExtension(dir->d_name), ".xopp") || !strcmp(obtenerExtension(dir->d_name), ".pdf")) )) /*&& strcmp(dir->d_name,"..")) */ // he de posar mes excepcions
                {
                    
                    GtkWidget *normalButton = gtk_button_new_with_label(dir->d_name);
                    gtk_container_add(GTK_CONTAINER(button_data->box), normalButton);
                    
                    UserData *userdata = g_new(UserData, 1);
                    button_data->some_value = 42;
                    userdata->box = button_data->box;
                    g_signal_connect(normalButton, "clicked", G_CALLBACK(on_button_clicked), userdata);
                    
                }

            }
            closedir(d);
        }
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

    GtkWidget *view;

    view = gtk_label_new(cwd);
    gtk_label_set_markup(GTK_LABEL(view),result);


    window = gtk_application_window_new(app); //decimos que window sera una ventana de aplicacion
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit),NULL);
    gtk_window_set_title(GTK_WINDOW(window), "LlunaLaMejor&HarryStyles<3");
    gtk_window_set_default_size(GTK_WINDOW(window), 700, 500);

    //creamos y metemos en window el contenedor main
    main = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);   
    files = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
    gtk_container_add(GTK_CONTAINER(window), main);
    

    //gtk_box_pack_start(GTK_BOX(window),main,TRUE,TRUE, 50);
    gtk_box_pack_end(GTK_BOX(main),files, TRUE, TRUE, 50);

    //la primera pantalla la deixare per si vull posar més d'una opcio quan s'inicii l'aplicacio, com opcions de configuracio o algo aixi.
    button1 = gtk_button_new_with_label("button1"); //creamos el boton 1

    create_menu(main, window);

    //Declaramos UserData y decimos que cuando se haga click en el boton 1 se ejecute la funcion on_button_clicked
    UserData *userdata = g_new(UserData, 1);
    userdata->some_value = 42;
    userdata->box = files;
    g_signal_connect(button1, "clicked", G_CALLBACK(on_button_clicked), userdata);
    

    gtk_box_pack_start(GTK_BOX(main), view, TRUE, TRUE, 50);
    gtk_box_pack_end(GTK_BOX(files), button1, TRUE, TRUE, 50);

 
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