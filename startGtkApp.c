#include "includes.h"


char rutaPredeterminada[10000] = "/home/gerard/Gerard/UNI/Apuntes";
char cwd[10000] = "/home/gerard/Gerard/UNI/Apuntes";
char guardarPrevisualizaciones[1024] = "/home/gerard/.libretaXournal/";
char directorioMas[1024] = "/home/gerard/Gerard/Projects/libreriaGrafica/IMG/plus_icon.png";
char *css =
    ".border          { border-color: #cc7700; border-style: solid; border-width: 10px; padding:5px;}\n"
    ".border_margin   { margin: 10px; border-radius: 5%; opacity: 0.9; border-style: solid; }\n"
    ".border_margin_pdf   { margin: 10px; border-radius: 5%; opacity: 0.9; border-style: solid; border-color: #add8e6; }\n";
int opcionesMenu[20];
int ordenarArchivos = 0;
int mostrarPrevisualizaciones = 1;
int mostrarPdf = 1;
Recientes *recientesAplicacion;

void ordenar(struct Recientes *recientes, const char *archivo, int activado) {
    
    if (activado == 0) 
    {
        // Mover todos los elementos una posición hacia la derecha
        for (int i = recientes->numRecientes; i > 0; i--) {
            strcpy(recientes->recientes[i], recientes->recientes[i - 1]);
        }
        // Agregar el nuevo archivo en la posición 0
        strcpy(recientes->recientes[0], archivo);

        
    } else if (activado == 1) 
    {
        // Buscar el archivo y eliminarlo
        for (int i = 0; i < recientes->numRecientes; i++) {
            if (!strcmp(recientes->recientes[i], archivo)) {
                // Eliminar el archivo
                for (int j = i; j < recientes->numRecientes-1; j++) {
                    strcpy(recientes->recientes[j], recientes->recientes[j + 1]);
                }
                break;
            }
        }
        
        // Mover todos los elementos una posición hacia la derecha
        for (int i = recientes->numRecientes; i > 0; i--)
            strcpy(recientes->recientes[i], recientes->recientes[i - 1]);
        
        // Agregar el nuevo archivo en la posición 0
        strcpy(recientes->recientes[0], archivo);

    }
    
    strcpy(recientes->recientes[recientes->numRecientes], "");
    
}


void on_drag_data_received(GtkWidget *widget, GdkDragContext *context, int x, int y, GtkSelectionData *sel_data, guint info, guint time, gpointer data) {
    gchar **uris = gtk_selection_data_get_uris(sel_data);
    for (int i = 0; uris[i] != NULL; i++) {
        gchar *path = g_filename_from_uri(uris[i], NULL, NULL);
        g_print("Nombre del archivo: %s\n", path);
        char aux[2048] = "";

        strcat(aux, agregarBarras(path));
        strcat(aux, " ");
        strcat(aux, agregarBarras(cwd));

        char cp[2056] = "cp -r ";
        strcat(cp, aux);
        system(cp);

        g_free(path);

        //refrescar

        refrescarDirectori(widget, data);
    }
    g_strfreev(uris);
}


/**
 * @brief funcio que s'executa al fer click en button1. Crea i afegeix un boto en el box passat depenent dels archius que hi ha a la carpeta
 * 
 * @param widget [GtkWidget]
 * @param data [UserData]
 */
void on_button_clicked(GtkWidget *widget, gpointer data) {

    UserData *button_data = (UserData *)data;

    GtkWidget *box = NULL;
    GtkWidget *main;
    //en caso que se le haya dado a exportar
    if(opcionesMenu[EXPORTAR_PDF] == 1 && recientesAplicacion->recientesActivado == 0)
    {
        ponerOpcionesACero();
        printf("Exportar: %s\n", button_data->some_value);

        if(strcmp(obtenerExtension(button_data->some_value), "xopp"))
        {
            //exportar
            char pathArchivoXopp[2048] = "";
            strcat(pathArchivoXopp, agregarBarras(cwd));
            strcat(pathArchivoXopp, "/");
            strcat(pathArchivoXopp, button_data->some_value);

            char pathArchivoPdf[2048] = "";
            strcat(pathArchivoPdf, agregarBarras(cwd));
            strcat(pathArchivoPdf, "/");
            char *nombreArchivoModificado = cambiarExtension(button_data->some_value, ".pdf");
            strcat(pathArchivoPdf, nombreArchivoModificado);
            
            char comando[8000] = "xournalpp --export-no-ruling --create-pdf=";
            strcat(comando, pathArchivoPdf);
            strcat(comando, " ");
            strcat(comando, pathArchivoXopp);
            strcat(comando, "");

            system(comando);

            free(nombreArchivoModificado);

            box = gtk_widget_get_parent(widget);
            box = gtk_widget_get_parent(box);
            box = gtk_widget_get_parent(box);
            
            box = gtk_widget_get_parent(box);
            main = box;
            box = gtk_widget_get_parent(box);
            box = get_widget_by_name(GTK_CONTAINER(box), "textOption");
            
        }

        gtk_widget_hide(box);       

        //refrescar directorio
        UserData *refrescar = g_new(UserData, 1);
        refrescar->box = main;
        refrescarDirectori(widget, refrescar);
        return;
    }

    // en cas que haigi fet click a un .pdf o un .xopp, s'obrira el xournalpp
    if(strcmp(gtk_widget_get_name(widget), "pdf") == 0 || strcmp(gtk_widget_get_name(widget), "xournal") == 0)
    {
        printf("RECIENTES ACTIVADO: %d\n", recientesAplicacion->recientesActivado);
        if(recientesAplicacion->recientesActivado == 0)
        {
            strcpy(recientesAplicacion->recientes[recientesAplicacion->numRecientes], obtener_nombre_directorio(cwd));
            strcat(recientesAplicacion->recientes[recientesAplicacion->numRecientes], "/");
            strcat(recientesAplicacion->recientes[recientesAplicacion->numRecientes], button_data->some_value);
            if(recientesAplicacion->numRecientes < 19)
                recientesAplicacion->numRecientes++;
            ordenar(recientesAplicacion, recientesAplicacion->recientes[recientesAplicacion->numRecientes], recientesAplicacion->recientesActivado);
        }
        else
        {   
            GtkWidget *box = gtk_widget_get_parent(widget);

            GList *childrenArchivo, *iterArchivo;
            childrenArchivo = gtk_container_get_children(GTK_CONTAINER(box));
            
            for(iterArchivo = childrenArchivo; iterArchivo != NULL; iterArchivo = g_list_next(iterArchivo))
            {

                if(GTK_IS_LABEL(iterArchivo->data))
                {
                    ordenar(recientesAplicacion, gtk_label_get_text(iterArchivo->data), recientesAplicacion->recientesActivado);
                    break;
                }            
            }
            
        }

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

        if(!strcmp(gtk_button_get_label(GTK_BUTTON(widget)),".."))
            quitarDesdeUltimaBarra(cwd);
        else if(strcmp(gtk_button_get_label(GTK_BUTTON(widget)), "Predeterminado") && strcmp(gtk_button_get_label(GTK_BUTTON(widget)), "Recientes")) // si no li he fet click a Predeterminado
        {
            strcat(cwd,"/");
            chdir(strcat(cwd,gtk_button_get_label(GTK_BUTTON(widget))));
            printf("%s\n",cwd);
        }
        else if(!strcmp(gtk_button_get_label(GTK_BUTTON(widget)), "Predeterminado"))
            recientesAplicacion->recientesActivado = 0;
        else if(!strcmp(gtk_button_get_label(GTK_BUTTON(widget)), "Recientes"))
            recientesAplicacion->recientesActivado = 1;
        

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

        //Llistar directoris
        g_print("Listando Directorios\n");
        char *archivosDirectorios[1024];

        for (int i = 0; i < 1024; i++) {
            archivosDirectorios[i] = NULL;
        }
        int n = 0;
        
        if(!strcmp(gtk_button_get_label(GTK_BUTTON(widget)), "Recientes") && strcmp(gtk_button_get_label(GTK_BUTTON(widget)), "Predeterminado")) // Si tengo que listar Recientes
        {
            // borro tots els fills del box de fitxers
            GList *destroyChildren, *destroyIter;
            destroyChildren = gtk_container_get_children(GTK_CONTAINER(button_data->box));
            for(destroyIter = destroyChildren; destroyIter != NULL; destroyIter = g_list_next(destroyIter))
                gtk_widget_destroy(GTK_WIDGET(destroyIter->data));

            g_list_free(destroyChildren);

            for (int i = 0; i < 20; i++)
            {
                archivosDirectorios[i] = (char *)malloc(strlen(recientesAplicacion->recientes[i]) + 1); // Asignar memoria
                archivosDirectorios[i][0] = '\0';
                strcat(archivosDirectorios[i], recientesAplicacion->recientes[i]);
            }

            n = recientesAplicacion->numRecientes;
        }
        else // Si tengo que listar un directorio
        {
            // borro tots els fills del box de fitxers
            GList *destroyChildren, *destroyIter;
            destroyChildren = gtk_container_get_children(GTK_CONTAINER(button_data->box));
            for(destroyIter = destroyChildren; destroyIter != NULL; destroyIter = g_list_next(destroyIter))
                gtk_widget_destroy(GTK_WIDGET(destroyIter->data));

            g_list_free(destroyChildren);

            DIR *d;
            struct dirent *dir;
            d = opendir(cwd);
            if (d) 
            {

                while ((dir = readdir(d)) != NULL) 
                {
                    printf("%s\n", dir->d_name);

                    //en cas que no es digui "." el directori y que tingui format .pdf o .xopp o que no tingui format (directori) llavors es MOSTRARAN
                    if(strcmp(dir->d_name,".") && (!strcmp(obtenerExtension(dir->d_name), "Sin extensión") || (!strcmp(obtenerExtension(dir->d_name), ".xopp") || (!strcmp(obtenerExtension(dir->d_name), ".pdf")) && mostrarPdf == 1) )) /*&& strcmp(dir->d_name,"..")) */ // he de posar mes excepcions
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
        }

        
        int i = 1;
        int j = 0;
        for (int k = 0; k < n; k++) //mostrare els artxius un cop ordenats
        {
            
            if(!strcmp(obtenerExtension(archivosDirectorios[k]), "Sin extensión") || mostrarPrevisualizaciones == 0) // si es un directorio
            {
                //posar widgets al box   
                GtkWidget *normalButton = gtk_button_new_with_label(archivosDirectorios[k]);

                if(!strcmp(obtenerExtension(archivosDirectorios[k]), ".xopp"))
                {
                    css_add(css);
                    gtk_style_context_add_class(gtk_widget_get_style_context(normalButton), "border");
                    gtk_style_context_add_class(gtk_widget_get_style_context(normalButton), "border_margin");
                    gtk_widget_set_name(normalButton, "xournal");
                } 
                else if(!strcmp(obtenerExtension(archivosDirectorios[k]), ".pdf"))
                {
                    css_add(css);
                    gtk_style_context_add_class(gtk_widget_get_style_context(normalButton), "border");
                    gtk_style_context_add_class(gtk_widget_get_style_context(normalButton), "border_margin_pdf");
                    gtk_widget_set_name(normalButton, "pdf");
                }

                gtk_widget_set_margin_start(normalButton, MARGIN);
                gtk_widget_set_margin_end(normalButton, MARGIN);
                gtk_widget_set_margin_top(normalButton, MARGIN);
                gtk_widget_set_margin_bottom(normalButton, MARGIN);

                gtk_widget_set_size_request(normalButton, ANCHO_PREV + 50, ALTURA_PREV + 50);

                gtk_grid_attach(GTK_GRID(button_data->box), normalButton, i, j, 1, 1);


                UserData *userdataNew = g_new(UserData, 1);
                strncpy(userdataNew->some_value, archivosDirectorios[k], sizeof(userdataNew->some_value) - 1);
                userdataNew->some_value[sizeof(userdataNew->some_value) - 1] = '\0';
                userdataNew->box = button_data->box;

                g_signal_connect(normalButton, "clicked", G_CALLBACK(on_button_clicked), userdataNew);
                g_signal_connect(normalButton, "button-press-event", G_CALLBACK(on_button_right_click), userdataNew);
            }
            else if(!strcmp(obtenerExtension(archivosDirectorios[k]), ".pdf")) // si es pdf
            {
                
                char auxPdf[1024] = "";
                char auxPrev[1024] = "";

                strcat(auxPdf, cwd);
                strcat(auxPdf, "/");
                strcat(auxPdf, archivosDirectorios[k]);

                strcat(auxPrev, guardarPrevisualizaciones);
                strcat(auxPrev, obtener_nombre_archivo(archivosDirectorios[k])); //obtener_nombre_archivo es para Recientes y no cambia nada en Predeterminado
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
                strcat(auxPrev, obtener_nombre_archivo(archivosDirectorios[k]));
                strcat(auxPrev, ".png");
                base64_to_image(copiar_y_extraer_preview(agregarBarras(auxXopp), "/home/gerard/.libretaXournal/previewXournal.xml" /*guardarPrevisualizaciones*/), auxPrev);
    
                css_add(css);
                box_add(button_data->box, "border_margin", auxPrev, data, archivosDirectorios[k], i, j, 0);
    
                system("rm /home/gerard/.libretaXournal/previewXournal.xml");
                
            }

                    
            //nombre de columnes q vull que tingui
            if(i == 3 ){
                i = 0;
                j += 1;
            }
            else
                i += 1;
        }

        for (int i = 0; i < n; i++) 
            free(archivosDirectorios[i]);
        
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


    GtkWidget *containerPath = gtk_fixed_new();
    gtk_widget_set_name(containerPath, "fijo");


    

    GtkWidget *text = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(text), "");
    gtk_widget_set_name(text, "textOption");
    gtk_widget_set_size_request(text, 170, 20);

    GtkWidget *ok = gtk_button_new_with_label("OK");
    gtk_widget_set_name(ok, "buttonOk");

    UserData *runData = g_new(UserData, 1);
    runData->box = containerPath;
    g_signal_connect(ok, "clicked", G_CALLBACK(runOk), runData);
    

   
    GtkWidget *view;

    view = gtk_label_new(cwd);
    gtk_label_set_markup(GTK_LABEL(view),result);
    gtk_label_set_xalign(GTK_LABEL(view), 0.0); // Alineación a la izquierda
    gtk_label_set_yalign(GTK_LABEL(view), 0.0); // Alineación en la parte superior
    gtk_widget_set_size_request(view, 300, 20);
    
    
    window = gtk_application_window_new(app); //decimos que window sera una ventana de aplicacion
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit),NULL);
    gtk_window_set_title(GTK_WINDOW(window), "Libreta Para Xournalpp");
    gtk_window_set_default_size(GTK_WINDOW(window), 1600, 900);
    gtk_window_maximize(GTK_WINDOW(window));

    //creamos y metemos en window el contenedor main
    main = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);   
    files = gtk_grid_new();  
    gtk_widget_set_name(files, "files");  
    gtk_widget_set_name(main, "main");  

    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    
    gtk_container_add(GTK_CONTAINER(window), main);
    

    //gtk_box_pack_start(GTK_BOX(window),main,TRUE,TRUE, 50);
    gtk_container_add(GTK_CONTAINER(scrolled_window), files);
    gtk_box_pack_end(GTK_BOX(main), scrolled_window, TRUE, TRUE, 10);


    gtk_widget_set_halign(files, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(files, GTK_ALIGN_CENTER);



    //la primera pantalla la deixare per si vull posar més d'una opcio quan s'inicii l'aplicacio, com opcions de configuracio o algo aixi.
    button1 = gtk_button_new_with_label("Predeterminado"); //creamos el boton 1
    gtk_grid_attach(GTK_GRID(files), button1, 0, 0, 1, 1);
    gtk_widget_set_margin_top(button1, MARGIN);
    gtk_widget_set_margin_bottom(button1, MARGIN);

    GtkWidget *button2 = gtk_button_new_with_label("Recientes");
    gtk_grid_attach(GTK_GRID(files), button2, 0, 1, 1, 1);

    create_menu(main, window);

    //Declaramos UserData y decimos que cuando se haga click en el boton 1 se ejecute la funcion on_button_clicked
    UserData *userdata = g_new(UserData, 1);
    strncpy(userdata->some_value, "button1", sizeof(userdata->some_value) - 1);
    userdata->some_value[sizeof(userdata->some_value) - 1] = '\0';
    userdata->box = files;

    g_signal_connect(button1, "clicked", G_CALLBACK(on_button_clicked), userdata);
    strncpy(userdata->some_value, "button2", sizeof(userdata->some_value) - 1);
    userdata->some_value[sizeof(userdata->some_value) - 1] = '\0';
    g_signal_connect(button2, "clicked", G_CALLBACK(on_button_clicked), userdata);
    
    gtk_fixed_put(GTK_FIXED(containerPath), view, 5, 5);

    gtk_fixed_put(GTK_FIXED(containerPath), text, 1050,2);
    gtk_fixed_put(GTK_FIXED(containerPath), ok, 1050+175, 2);
    
    gtk_container_add(GTK_CONTAINER(main), containerPath);
    //gtk_box_pack_start(GTK_BOX(main), view, TRUE, TRUE, 5);

    gtk_drag_dest_set(window, GTK_DEST_DEFAULT_ALL, NULL, 0, GDK_ACTION_COPY);
    gtk_drag_dest_add_uri_targets(window);
    g_signal_connect(window, "drag-data-received", G_CALLBACK(on_drag_data_received), userdata);
    
    g_signal_connect(G_OBJECT(window), "key-press-event", G_CALLBACK(on_key_press), userdata);
    gtk_widget_show_all(window);

    
    gtk_widget_hide(text);
    gtk_widget_hide(ok);

    gtk_main();
    g_free(userdata);
}


int main( int argc, char **argv){
    GtkApplication *app;

    recientesAplicacion = g_new(Recientes, 1);
    recientesAplicacion->numRecientes = 0;
    recientesAplicacion->recientesActivado = 0;

    chdir(cwd);

    getcwd(cwd, sizeof(cwd));

    app = gtk_application_new("in.aducators",G_APPLICATION_FLAGS_NONE);

    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    

    g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);

    return 0;
}   



        
        
    


