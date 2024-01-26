#include "includes.h"

// Función que maneja la selección de los elementos del menú
void on_menu_item_activate(GtkMenuItem *menu_item, gpointer data) {
    g_print("Se seleccionó: %s\n", (const char *)data);
}

// Función que crea el menú y lo agrega al contenedor principal

void create_menu(GtkWidget *main_box, GtkWidget *window) {
    GtkWidget *menu_bar;
    GtkWidget *menu1, *menu2, *menu3;
    GtkWidget *item1, *item2, *item3;
    GtkWidget *section1_1, *section1_2, *section2_1, *section2_2, *section3_1, *section3_2;

    // Crear la barra de menú
    menu_bar = gtk_menu_bar_new();
    gtk_box_pack_start(GTK_BOX(main_box), menu_bar, FALSE, FALSE, 0);

    // Botón 1
    menu1 = gtk_menu_new();
    item1 = gtk_menu_item_new_with_label("Botón 1");

    section1_1 = gtk_menu_item_new_with_label("Sección 1.1");
    section1_2 = gtk_menu_item_new_with_label("Sección 1.2");

    g_signal_connect(G_OBJECT(section1_1), "activate", G_CALLBACK(on_menu_item_activate), "Sección 1.1");
    g_signal_connect(G_OBJECT(section1_2), "activate", G_CALLBACK(on_menu_item_activate), "Sección 1.2");

    gtk_menu_shell_append(GTK_MENU_SHELL(menu1), section1_1);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu1), section1_2);

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(item1), menu1);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), item1);

    // Botón 2
    menu2 = gtk_menu_new();
    item2 = gtk_menu_item_new_with_label("Botón 2");

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
    item3 = gtk_menu_item_new_with_label("Botón 3");

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