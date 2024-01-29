#ifndef INCLUDES_H
#define INCLUDES_H

#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h> 
#include <libxml2/libxml/parser.h>

#include <cairo.h>
#include <poppler/glib/poppler.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>
#include <FreeImage.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>



#define ANCHO_PREV (59.5+80)
#define ALTURA_PREV (84.1+80)
#define MARGIN 10



extern char cwd[10000];  // Definición de cwd
extern char guardarPrevisualizaciones[1024];  // Definición de guardarPrevisualizaciones
extern char directorioMas[1024];  // Definición de directorioMas
extern char *css;

#define NUEVA_CARPETA 0
#define NUEVO_XOURNAL 19
#define VACIAR_CACHE 1
/***
 * @param opciones[0] Nueva carpeta
 * @param opciones[19] Nuevo Xournal
 * @param opciones[1] Vaciar Caché
*/
extern int opcionesMenu[20];

typedef struct {
    char some_value[1024];
    GtkWidget *box;
} UserData;


void create_menu(GtkWidget *, GtkWidget *);
void pdf_to_image(const char *, const char *);
void on_button_clicked(GtkWidget *, gpointer );
void base64_to_image(const char *, const char *);
xmlChar* copiar_y_extraer_preview(const char *, const char *);
GtkWidget *create_file_button(const char *, gpointer , char *);
void quitarDesdeUltimaBarra(char *);
const char *obtenerExtension(const char *);
char* agregarBarras(char *);
void css_add(char *);
int compararArchivos(const void *, const void *);
void afegirPredeterminat(GtkWidget *);
void box_add(GtkWidget *, char *, const char * , gpointer , char * , int , int , int );
void runOk(GtkWidget *, gpointer ); 
gboolean on_key_press(GtkWidget *, GdkEventKey *, gpointer );
void refrescarDirectori(GtkWidget *, gpointer );

#endif // INCLUDES_H