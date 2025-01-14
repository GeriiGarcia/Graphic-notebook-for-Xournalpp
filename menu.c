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
    else if(event->keyval == GDK_KEY_F6)
    {
        volverAInicio(widget, user_data);
        return TRUE;
    }
    else if(event->keyval == GDK_KEY_Return || event->keyval == GDK_KEY_KP_Enter){
        GtkWidget *box = gtk_widget_get_parent(button_data->box);
        box = gtk_widget_get_parent(box);
        box = gtk_widget_get_parent(box);
        box = get_widget_by_name(GTK_CONTAINER(box), "fijo");

        GtkWidget *botoOk = get_widget_by_name(GTK_CONTAINER(box), "buttonOk");

        UserData *userdata = g_new(UserData, 1);
        userdata->box = box;
        runOk(botoOk, userdata);
        g_free(userdata);
    }
    else if(event->keyval == GDK_KEY_Escape)
    {
        GtkWidget *box = gtk_widget_get_parent(button_data->box);
        box = gtk_widget_get_parent(box);
        box = gtk_widget_get_parent(box);
        box = get_widget_by_name(GTK_CONTAINER(box), "fijo");
        UserData *userdata = g_new(UserData, 1);
        userdata->box = box;
        reset(widget, userdata);
    }
    else if((event->keyval == 78)) 
    {
        GtkWidget *box = gtk_widget_get_parent(button_data->box);
        box = gtk_widget_get_parent(box);
        box = gtk_widget_get_parent(box);
        UserData *userdata = g_new(UserData, 1);
        userdata->box = box;
        nuevaCarpeta(widget, userdata);
        return TRUE;
    }
//HarryStyles
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
    
    texto = "passText";
    GtkWidget *passText = get_widget_by_name(GTK_CONTAINER(button_data->box), texto);
    const gchar *label_text = gtk_label_get_text(GTK_LABEL(passText));

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
                //char *aux2 = agregarBarras(aux); 

                mkdir(aux, 0777);

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

        case CAMBIAR_RUTA_PREDETERMINADA:

            if(directorio_existe(widget_value) == 1)
                strcpy(rutaPredeterminada, widget_value);
            
            borrarRecientes(botonOk, data);
            break;
        case CONFIRMAR_SUPRIMIR:            
            if(strcmp(widget_value, "") == 0)
            {
                char *prefix = "Confirmar eliminación ";
                char *nombre = label_text + strlen(prefix);

                char aux[1024] = "";
                strcat(aux, agregarBarras(cwd));
                strcat(aux, "/");
                strcat(aux, agregarBarras(nombre));
                char comando[1024] = "rm -rf ";
                strcat(comando, aux);

                system(comando);

                gtk_widget_hide(passText);
            }
            break;
        default:
            break;
        }

        refrescarDirectori(botonOk, data);

        gtk_entry_set_text(GTK_ENTRY(textWidget), "");
        gtk_widget_hide(textWidget);
        gtk_widget_hide(botonOk);
        ponerOpcionesACero();
    }
    
}

void nuevaCarpeta(GtkWidget *widget, gpointer data)
{
    (void)widget;
    UserData *button_data = (UserData *)data;
    gchar *texto = "textOption";
    GtkWidget * textWidget = get_widget_by_name(GTK_CONTAINER(button_data->box), texto);
    texto = "buttonOk";
    GtkWidget * buttonOk = get_widget_by_name(GTK_CONTAINER(button_data->box), texto);

    gtk_widget_grab_focus(textWidget);

    gtk_widget_show(textWidget);
    gtk_widget_show(buttonOk);
    gtk_entry_set_text(GTK_ENTRY(textWidget), "'Nombre de directorio'");

    ponerOpcionesACero();
    opcionesMenu[NUEVA_CARPETA] = 1;
}

void reset(GtkWidget *widget, gpointer data)
{
    (void)widget;
    UserData *button_data = (UserData *)data;
    ponerOpcionesACero();
    gchar *texto = "textOption";
    GtkWidget * textWidget = get_widget_by_name(GTK_CONTAINER(button_data->box), texto);
    texto = "buttonOk";
    GtkWidget * buttonOk = get_widget_by_name(GTK_CONTAINER(button_data->box), texto);
    texto = "passText";
    GtkWidget * passText = get_widget_by_name(GTK_CONTAINER(button_data->box), texto);

    gtk_widget_hide(textWidget);
    gtk_widget_hide(buttonOk);
    gtk_widget_hide(passText);
}

void vaciarCache()
{

    char *comando_inicial = "rm -rf ";
    char *comandoRm = (char *)malloc(strlen(guardarPrevisualizaciones) + 1 +strlen(comando_inicial)); // Asignar memoria
    strcpy(comandoRm, comando_inicial);
    strcat(comandoRm, guardarPrevisualizaciones);

    system(comandoRm);

    mkdir(guardarPrevisualizaciones, 0777);

    free(comandoRm);
}

//encontrar items de un submenu del menu
void encontrarHijos(struct _GtkWidget *widget, void *data) 
{
    (void)data;
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

void recargarTamaño(GtkWidget *widget)
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
    (void)widget;
    UserData *button_data = (UserData *)data;

    GtkWidget *box = gtk_widget_get_parent(button_data->box);
    
    box = get_widget_by_name(GTK_CONTAINER(box), "files");

    //per refrescar la pagina 
    //El que faig es afegir un element amb el nom Predeterminat a la grid i per-li click "artificialment"
    GtkWidget *auxButton;
    if(recientesAplicacion->recientesActivado == 0)
        auxButton = gtk_button_new_with_label("Predeterminado");
    else if(recientesAplicacion->recientesActivado == 1)
        auxButton = gtk_button_new_with_label("Recientes");
    
    gtk_grid_attach(GTK_GRID(box), auxButton, 0, 0, 1, 1);

    UserData *userdata = g_new(UserData, 1);
    strncpy(userdata->some_value, "button1", sizeof(userdata->some_value) - 1);
    userdata->some_value[sizeof(userdata->some_value) - 1] = '\0';
    userdata->box = box;
    on_button_clicked(GTK_WIDGET(auxButton), userdata);
    g_free(userdata);

}


void on_passText_destroy(GtkWidget *widget, gpointer data) {
    guint temporizador_id = GPOINTER_TO_UINT(data);
    // Desconecta la señal "destroy" y elimina el temporizador
    g_signal_handler_disconnect(widget, temporizador_id);
}

gboolean ocultar_ventana(gpointer data) {
    GtkWidget *ventana = GTK_WIDGET(data);
    guint temporizador_id = GPOINTER_TO_UINT(g_object_get_data(G_OBJECT(ventana), "temporizador_id"));

    // Verifica si el temporizador aún existe y si el identificador es mayor que cero
    if (temporizador_id > 0 && g_source_remove(temporizador_id)) {
        gtk_widget_hide(ventana);
    }

    return FALSE;
}

void conectar_a_wifi(const char *nombre_conexion) {
    // Construir el comando nmcli
    gchar *comando = g_strdup_printf("nmcli connection up uuid \"%s\"", nombre_conexion);
    // Ejecutar el comando
    int resultado = system(comando);
    // Liberar la memoria
    g_free(comando);
    // Verificar el resultado
    if (resultado == 0) {
        printf("Conectado a la red WiFi: %s\n", nombre_conexion);
    } else {
        fprintf(stderr, "Error al conectar a la red WiFi: %s\n", nombre_conexion);
    }
}


void montar_sshfs(GtkWidget *widget, gpointer data) {
    
    UserData *app_data = (UserData *)data;

    if(!strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(widget)), "passTo UNI"))
    {
        conectar_a_wifi("0e689f4e-376f-4f1f-bde9-4b48c775afbd");
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("Error al crear el proceso hijo");
    } else if (pid == 0) {  // Proceso hijo
        // Ejecuta el comando sshfs en el proceso hijo
        execlp("sshfs", "sshfs", "-p", "32556", "gerard@gerardgarcia2003.hopto.me:/sftp/gerard/Casa/Gerard/UNI/3er/3.2/Apuntes", "/home/gerard/Gerard/pass", (char *)NULL);

        // Si execlp() falla, muestra un mensaje de error
        perror("Error al ejecutar el comando sshfs");
        exit(EXIT_FAILURE);
    } else {  // Proceso padre
        // Espera a que el proceso hijo termine
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            gtk_label_set_text(GTK_LABEL(app_data->box), "Montado");
        } else {
            gtk_label_set_text(GTK_LABEL(app_data->box), "Error al montar SSHFS");
        }
    }
}

void copiarArchivo(const char *origen, const char *destino) {
    FILE *file_origen = fopen(origen, "rb");
    FILE *file_destino = fopen(destino, "wb");

    if (file_origen == NULL || file_destino == NULL) {
        perror("Error al abrir archivos");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFSIZ];
    size_t bytes_leidos;

    while ((bytes_leidos = fread(buffer, 1, sizeof(buffer), file_origen)) > 0) {
        fwrite(buffer, 1, bytes_leidos, file_destino);
    }

    fclose(file_origen);
    fclose(file_destino);
}

// Función para copiar directorio recursivamente
void copiarDirectorio(const char *origen, const char *destino, gpointer data) {
    
    DIR *dir_origen = opendir(origen);

    if (dir_origen == NULL) {
        perror("Error al abrir el directorio de origen");
        exit(EXIT_FAILURE);
    }

    // Crea el directorio de destino si no existe
    mkdir(destino, 0700);

    struct dirent *entrada;
    while ((entrada = readdir(dir_origen)) != NULL) {
        if (strcmp(entrada->d_name, ".") == 0 || strcmp(entrada->d_name, "..") == 0) {
            continue;  // Ignora las entradas . y ..
        }

        char ruta_origen[2048];
        char ruta_destino[2048];

        snprintf(ruta_origen, sizeof(ruta_origen), "%s/%s", origen, entrada->d_name);
        snprintf(ruta_destino, sizeof(ruta_destino), "%s/%s", destino, entrada->d_name);

        struct stat st;
        if (stat(ruta_origen, &st) == 0 && S_ISDIR(st.st_mode)) {
            // Si es un directorio, copia recursivamente
            copiarDirectorio(ruta_origen, ruta_destino, data);
        } else {
            // Si es un archivo, simplemente cópialo
            copiarArchivo(ruta_origen, ruta_destino);
        }
    }

    closedir(dir_origen);
}

void desmontar_sshfs(GtkWidget *widget, gpointer data) {
    (void)widget;
    UserData *app_data = (UserData *)data;

    pid_t pid = fork();

    if (pid == -1) {
        perror("Error al crear el proceso hijo");
    } else if (pid == 0) {  // Proceso hijo
        // Ejecuta el comando fusermount en el proceso hijo
        execlp("fusermount", "fusermount", "-u", "/home/gerard/Gerard/pass", (char *)NULL);

        // Si execlp() falla, muestra un mensaje de error
        perror("Error al ejecutar el comando fusermount");
        exit(EXIT_FAILURE);
    } else {  // Proceso padre
        // Espera a que el proceso hijo termine
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            gtk_label_set_text(GTK_LABEL(app_data->box), "¡Hecho!");
        } else {
            gtk_label_set_text(GTK_LABEL(app_data->box), "Error al desmontar SSHFS");
        }
    }
}

void passTo(GtkWidget *widget, gpointer data)
{
     int status;
    
    if(!strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(widget)), "passTo UNI"))
        status = system("passTo UNI &");
    
    else
        status = system("passTo &");
    
    
    UserData *button_data = (UserData *)data;
    gchar *texto = "passText";
    GtkWidget * passText = get_widget_by_name(GTK_CONTAINER(button_data->box), texto);

    gtk_widget_show(passText);

    // UserData *pass = g_new(UserData, 1);
    // pass->box = passText;

    // montar_sshfs(widget,pass);
    // const char *origen = "/home/gerard/Gerard/UNI/Apuntes";
    // const char *destino = "/home/gerard/Gerard/pass/Apuntes";
    // copiarDirectorio(origen, destino, pass);
    // desmontar_sshfs(widget,pass);

    if(status == -1)
        gtk_label_set_text(GTK_LABEL(passText), "¡Error!");
    else
        gtk_label_set_text(GTK_LABEL(passText), "¡Hecho!");

    guint temporizador_id = g_timeout_add(15000, ocultar_ventana, passText);
    g_object_set_data(G_OBJECT(passText), "temporizador_id", GUINT_TO_POINTER(temporizador_id));
    g_signal_connect(passText, "destroy", G_CALLBACK(on_passText_destroy), GUINT_TO_POINTER(temporizador_id));
    g_free(button_data);

}

void exportarPdf(GtkWidget *widget, gpointer data)
{
    (void)widget;
    UserData *button_data = (UserData *)data;
    gchar *texto = "passText";
    GtkWidget * textWidget = get_widget_by_name(GTK_CONTAINER(button_data->box), texto);

    gtk_widget_show(textWidget);
    
    gtk_label_set_text(GTK_LABEL(textWidget), "Pulsa un archivo");

    ponerOpcionesACero();
    opcionesMenu[EXPORTAR_PDF] = 1;
}

void ordenarXopp(GtkWidget *widget, gpointer data)
{
    ordenarArchivos = 0;
    refrescarDirectori(widget, data);
}

void ordenarPdf(GtkWidget *widget, gpointer data)
{
    ordenarArchivos = 1;
    refrescarDirectori(widget, data);
}

void cambiarRutaPredeterminada(GtkWidget *widget, gpointer data)
{
    (void)widget;
    UserData *button_data = (UserData *)data;
    gchar *texto = "textOption";
    GtkWidget * textWidget = get_widget_by_name(GTK_CONTAINER(button_data->box), texto);
    texto = "buttonOk";
    GtkWidget * buttonOk = get_widget_by_name(GTK_CONTAINER(button_data->box), texto);

    gtk_widget_grab_focus(textWidget);

    gtk_widget_show(textWidget);
    gtk_widget_show(buttonOk);
    gtk_entry_set_text(GTK_ENTRY(textWidget), rutaPredeterminada);


    ponerOpcionesACero();
    opcionesMenu[CAMBIAR_RUTA_PREDETERMINADA] = 1;

}

void volverAInicio(GtkWidget *widget, gpointer data)
{
    strcpy(cwd, rutaPredeterminada);
    chdir(rutaPredeterminada);
    refrescarDirectori(widget, data);    

    UserData *button_data = (UserData *)data;
    GtkWidget *box = gtk_widget_get_parent(button_data->box);
    
    box = get_widget_by_name(GTK_CONTAINER(box), "files");

    GList *children2, *iter2;
    children2 = gtk_container_get_children(GTK_CONTAINER(box));
    for(iter2 = children2; iter2 != NULL; iter2 = g_list_next(iter2))
        gtk_widget_destroy(GTK_WIDGET(iter2->data));

    GtkWidget *auxButton = gtk_button_new_with_label("Predeterminado");
    gtk_widget_set_margin_top(auxButton, MARGIN);
    gtk_widget_set_margin_bottom(auxButton, MARGIN);
    gtk_grid_attach(GTK_GRID(box), auxButton, 0, 0, 1, 1);

    GtkWidget *button2 = gtk_button_new_with_label("Recientes");
    gtk_grid_attach(GTK_GRID(box), button2, 0, 1, 1, 1);

    UserData *userdata = g_new(UserData, 1);
    strncpy(userdata->some_value, "button1", sizeof(userdata->some_value) - 1);
    userdata->some_value[sizeof(userdata->some_value) - 1] = '\0';
    userdata->box = box;
    g_signal_connect(auxButton, "clicked", G_CALLBACK(on_button_clicked), userdata);

    strncpy(userdata->some_value, "button2", sizeof(userdata->some_value) - 1);
    userdata->some_value[sizeof(userdata->some_value) - 1] = '\0';
    g_signal_connect(button2, "clicked", G_CALLBACK(on_button_clicked), userdata);

    gtk_widget_show_all(box);
    
}

void mostrar_pdf(GtkWidget *widget, gpointer data)
{
    if(gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget)))
        mostrarPdf = 1;
    else
        mostrarPdf = 0;

    refrescarDirectori(widget, data);
}

void mostrar_previsualizaciones(GtkWidget *widget, gpointer data)
{
    if(gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget)))
        mostrarPrevisualizaciones = 1;
    else
        mostrarPrevisualizaciones = 0;
    
    refrescarDirectori(widget, data);
}

void borrarRecientes(GtkWidget *widget, gpointer data)
{
    for (int i = 0; i < recientesAplicacion->numRecientes; i++)
        strcpy(recientesAplicacion->recientes[i], "");
    
    recientesAplicacion->numRecientes = 0;

    if(recientesAplicacion->recientesActivado == 1)
        refrescarDirectori(widget, data);
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

    section1_1 = gtk_menu_item_new_with_label("Nueva Carpeta\tMayús+Ctrl+N");
    UserData *data = g_new(UserData, 1);
    strncpy(data->some_value, cwd, sizeof(data->some_value) - 1); 
    data->box = main_box;
    g_signal_connect(G_OBJECT(section1_1), "activate", G_CALLBACK(nuevaCarpeta), data);

    section1_2 = gtk_menu_item_new_with_label("Exportar a PDF");
    g_signal_connect(G_OBJECT(section1_2), "activate", G_CALLBACK(exportarPdf), data);

    GtkWidget *section1_3 = gtk_menu_item_new_with_mnemonic("_Reset\t\t\tESC");
    g_signal_connect(G_OBJECT(section1_3), "activate", G_CALLBACK(reset), data);

    GtkWidget *section1_4 = gtk_menu_item_new_with_label("Borrar recientes");
    g_signal_connect(G_OBJECT(section1_4), "activate", G_CALLBACK(borrarRecientes), data);



    gtk_menu_shell_append(GTK_MENU_SHELL(menu1), section1_1);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu1), section1_2);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu1), section1_3);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu1), section1_4);

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(item1), menu1);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), item1);

    // Botón 2
    menu2 = gtk_menu_new();
    item2 = gtk_menu_item_new_with_label("Navegación");
    gtk_widget_set_name(item2, "item2");

    section2_1 =  gtk_check_menu_item_new_with_label("Mostrar PDFs?");
    if(mostrarPdf == 1)
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(section2_1), TRUE);
    else if(mostrarPdf == 0)
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(section2_1), FALSE);
    
    section2_2 =  gtk_check_menu_item_new_with_label("Mostrar previsializaciones?");
    if(mostrarPrevisualizaciones == 1)
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(section2_2), TRUE);
    else if(mostrarPrevisualizaciones == 0)
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(section2_2), FALSE);

    GtkWidget *section2_3 = gtk_menu_item_new_with_label("Cambiar ruta predeterminada");

    GtkWidget *section2_4 = gtk_menu_item_new_with_label("Ordenar");
    GtkWidget *section2_4_0 = gtk_menu_new();
    //GSList *group = NULL;
    // Crear las opciones como elementos de menú de radio
    GtkWidget *section2_4_1 = gtk_radio_menu_item_new_with_label(NULL, "Xopp primero");
    GtkWidget *section2_4_2 = gtk_radio_menu_item_new_with_label_from_widget(GTK_RADIO_MENU_ITEM(section2_4_1), "Pdf primero");
    if(ordenarArchivos == 0)
    {
        gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(section2_4_1));
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(section2_4_1), TRUE);
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(section2_4_2), FALSE);
    }
    else if(ordenarArchivos == 1)
    {
        gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(section2_4_1));
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(section2_4_1), FALSE);
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(section2_4_2), TRUE);
    }
         

    GtkWidget *section2_5 = gtk_menu_item_new_with_mnemonic("_Refrescar directorio\t\t\tF5");
    GtkWidget *section2_6 = gtk_menu_item_new_with_mnemonic("_Volver a inicio\t\t\t\t\tF6");

    g_signal_connect(G_OBJECT(section2_1), "activate", G_CALLBACK(mostrar_pdf), data);
    g_signal_connect(G_OBJECT(section2_2), "activate", G_CALLBACK(mostrar_previsualizaciones), data);
    g_signal_connect(G_OBJECT(section2_3), "activate", G_CALLBACK(cambiarRutaPredeterminada), data);
    g_signal_connect(G_OBJECT(section2_4_1), "activate", G_CALLBACK(ordenarXopp), data);
    g_signal_connect(G_OBJECT(section2_4_2), "activate", G_CALLBACK(ordenarPdf), data);
    g_signal_connect(G_OBJECT(section2_5), "activate", G_CALLBACK(refrescarDirectori), data);
    g_signal_connect(G_OBJECT(section2_6), "activate", G_CALLBACK(volverAInicio), data);
    
    gtk_menu_shell_append(GTK_MENU_SHELL(menu2), section2_1);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu2), section2_2);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu2), section2_3);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu2), section2_4);
    gtk_menu_shell_append(GTK_MENU_SHELL(section2_4_0), section2_4_1);
    gtk_menu_shell_append(GTK_MENU_SHELL(section2_4_0), section2_4_2);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu2), section2_5);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu2), section2_6);

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(section2_4), section2_4_0);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(item2), menu2);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), item2);

    // Botón 3
    menu3 = gtk_menu_new();
    item3 = gtk_menu_item_new_with_label("Herramientas");
    gtk_widget_set_name(item3, "item3");
    g_signal_connect(G_OBJECT(item3), "activate", G_CALLBACK(recargarTamaño), "Item 3");

    section3_1 = gtk_menu_item_new_with_label("passTo");
    g_signal_connect(G_OBJECT(section3_1), "activate", G_CALLBACK(passTo), data);

    section3_2 = gtk_menu_item_new_with_label("passTo UNI");
    g_signal_connect(G_OBJECT(section3_2), "activate", G_CALLBACK(passTo), data);

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