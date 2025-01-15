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
#include <glib.h>


#include <sys/wait.h>
#include <fcntl.h>










#define ANCHO_PREV (59.5+80)
#define ALTURA_PREV (84.1+80)
#define MARGIN 10


extern char rutaPredeterminada[10000];
extern char cwd[10000];  // Definición de cwd
extern char guardarPrevisualizaciones[1024];  // Definición de guardarPrevisualizaciones
extern char directorioMas[1024];  // Definición de directorioMas
extern char *CSS;
extern char *nombreDeFicheroAntiguo; // Para el cambio de nombre de un archivo

#define NUEVA_CARPETA 0
#define NUEVO_XOURNAL 19
#define VACIAR_CACHE 1
#define EXPORTAR_PDF 18
#define CAMBIAR_RUTA_PREDETERMINADA 2
#define CONFIRMAR_SUPRIMIR 3
#define MOVER_ARCHIVO 4
#define CAMBIAR_NOMBRE 5
/***
 * @param opciones[0] Nueva carpeta
 * @param opciones[19] Nuevo Xournal
 * @param opciones[1] Vaciar Caché
 * @param opciones[2] cambiar ruta predeterminada
 * @param opciones[3] confirmar suprimir
 * @param opciones[4] mover archivo
*/
extern int opcionesMenu[20];

/***
 * @param 0 xournals primero 
 * @param 1 PDF primero
*/ 
extern int ordenarArchivos;

/***
 * @param 0 no 
 * @param 1 si
*/ 
extern int mostrarPdf;

/***
 * @param 0 no 
 * @param 1 si
*/ 
extern int mostrarPrevisualizaciones;

typedef struct Recientes Recientes;
struct Recientes{
    char recientes[20][1024];
    int numRecientes;
    int recientesActivado;
};

typedef struct {
    GtkWidget *output_label;
} AppData;

extern Recientes *recientesAplicacion;


typedef struct {
    char some_value[1024];
    GtkWidget *box;
} UserData;


void create_menu(GtkWidget *, GtkWidget *);
void pdf_to_image(const char *, const char *);
void on_button_clicked(GtkWidget *, gpointer );
void base64_to_image(const char *, const char *);
xmlChar* copiar_y_extraer_preview(const char *, const char *);
GtkWidget *create_file_button(const char *, gpointer , char *, int);
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
void ponerOpcionesACero(void);
char *cambiarExtension(const char *, const char *);
GtkWidget *get_widget_by_name(GtkContainer *, const gchar *);
int directorio_existe(const char *);
gboolean on_button_right_click(GtkWidget *, GdkEventButton *, gpointer );
const char* obtener_nombre_directorio(const char* );
const char* obtener_nombre_archivo(const char* );
void ordenar(struct Recientes *, const char *, int );
void borrarRecientes(GtkWidget *, gpointer );
void volverAInicio(GtkWidget *, gpointer );
void on_drag_data_received(GtkWidget *, GdkDragContext *, int , int , GtkSelectionData *, guint , guint , gpointer );
void alCierre(void);
void cargarConfig(void);
long long calcularTamanoCarpeta(const char *); 
void nuevaCarpeta(GtkWidget *, gpointer );
void reset(GtkWidget *, gpointer );
void vaciarCache(void);
void encontrarHijos(GtkWidget *, void *);
void recargarTamaño(GtkWidget * );
void passTo(GtkWidget *, gpointer );
void exportarPdf(GtkWidget *, gpointer );
void ordenarXopp(GtkWidget *, gpointer );
void ordenarPdf(GtkWidget *, gpointer );
void cambiarRutaPredeterminada(GtkWidget *, gpointer );
void mostrar_pdf(GtkWidget *, gpointer );
void mostrar_previsualizaciones(GtkWidget *, gpointer );
void suprimir(GtkWidget *, gpointer );
void moverArchivo(GtkWidget *, gpointer );
void cambiarNombre(GtkWidget *, gpointer );
void exportarAPdf(GtkWidget *, gpointer );
void abrirPdf(GtkWidget *, gpointer );
void on_button_hover(GtkWidget *, GdkEvent *, gpointer );
void on_button_unhover(GtkWidget * );
void on_passText_destroy(GtkWidget *, gpointer );
gboolean ocultar_ventana(gpointer );
void conectar_a_wifi(const char *);
void montar_sshfs(GtkWidget *, gpointer );
void copiarArchivo(const char *, const char *);
void copiarDirectorio(const char *, const char *, gpointer );
void desmontar_sshfs(GtkWidget *, gpointer );
void abrir_directorio_actual(GtkWidget *, gpointer );
GtkWidget* crear_menu_contextual(void);
gboolean on_window_button_press(GtkWidget *widget, GdkEventButton *, gpointer );
void on_drag_data_get(GtkWidget *, GdkDragContext *, GtkSelectionData *, guint , guint , gpointer );
void setup_drag_source(GtkWidget *, UserData *);

#endif // INCLUDES_H