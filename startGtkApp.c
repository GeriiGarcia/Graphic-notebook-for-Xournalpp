#include <gtk/gtk.h>
#include <dirent.h> 

char cwd[10000] = "/home/gerard/Gerard/UNI/Apuntes";

typedef struct {
    int some_value;
    GtkWidget *box;
} UserData;

void create_menu(GtkWidget *, GtkWidget *);


/**
 * @brief funcio que s'executa al fer click en button1. Crea i afegeix un boto en el box passat depenent dels archius que hi ha a la carpeta
 * 
 * @param widget [GtkWidget]
 * @param data [UserData]
 */
void on_button_clicked(GtkWidget *widget, gpointer data) {

    UserData *button_data = (UserData *)data;

    if(strcmp(gtk_button_get_label(GTK_BUTTON(widget)), "button1"))
    {
        strcat(cwd,"/");
        chdir(strcat(cwd,gtk_button_get_label(GTK_BUTTON(widget))));
        printf("%s\n",cwd);
    }

   
    GList *children, *iter;

    children = gtk_container_get_children(GTK_CONTAINER(button_data->box));
    for(iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    
    g_list_free(children);

    
    

    g_print("Listando Directorios\n");

    DIR *d;
    struct dirent *dir;
    d = opendir(cwd);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            printf("%s\n", dir->d_name);

            //if( strcmp(dir->d_name,".") && strcmp(dir->d_name,"..")) // he de posar mes excepcions
            //{
                GtkWidget *normalButton = gtk_button_new_with_label(dir->d_name);
                gtk_container_add(GTK_CONTAINER(button_data->box), normalButton);

                UserData *userdata = g_new(UserData, 1);
                button_data->some_value = 42;
                userdata->box = button_data->box;
                g_signal_connect(normalButton, "clicked", G_CALLBACK(on_button_clicked), userdata);
            //}
            
        }
        closedir(d);
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