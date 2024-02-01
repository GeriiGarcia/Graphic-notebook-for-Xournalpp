#include "includes.h"
#include <X11/Xw32defs.h>
#include <fcntl.h>
#include "../../../../../usr/include/i386-linux-gnu/sys/stat.h"


/***
 * @brief funcion que elimina desde la última barra encontrada en esa cadena hasta el final
 * 
*/
void quitarDesdeUltimaBarra(char *cadena) {
    int longitud = (int)strlen(cadena);

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

char *cambiarExtension(const char *nombreArchivo, const char *nuevaExtension) {
    char *resultado = malloc(strlen(nombreArchivo) + strlen(nuevaExtension) + 1);

    if (resultado != NULL) {
        strcpy(resultado, nombreArchivo);

        char *punto = strrchr(resultado, '.');

        if (punto != NULL && punto != resultado) {
            // Se encontró un punto y no es el primer carácter
            *punto = '\0';  // Elimina la extensión actual
            strcat(resultado, nuevaExtension);  // Agrega la nueva extensión
        }
    }

    return resultado;
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

//para cuando necesito añadir contrabarras
char* agregarBarras(char *cadena) {
    int longitud = (int)strlen(cadena);

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
    char *nuevaCadena = (char *)malloc((size_t)(nuevaLongitud + 1) * sizeof(char));

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

    //print de la cadena nueva
    //printf("%s\n",nuevaCadena);

    return nuevaCadena;
}


void css_add(char *css)
{
    /* CSS */
    GError *error = NULL;
    GtkCssProvider *provider = gtk_css_provider_new ();
    gtk_css_provider_load_from_data (provider, css, (gssize)strlen(css), &error);
    if (error != NULL)
    {
        fprintf (stderr, "CSS: %s\n", error->message);
    }
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}


/***
 * @brief Se usa en el qsort()
*/
int compararArchivos(const void *a, const void *b) {
    const char *nombreA = *(const char * const *)a;
    const char *nombreB = *(const char * const *)b;


    // Obtener extensiones
    const char *extensionA = obtenerExtension(nombreA);
    const char *extensionB = obtenerExtension(nombreB);

    // Comparar directorios primero
    if (!strcmp(extensionA, "Sin extensión") && strcmp(extensionB, "Sin extensión")) {
        return -1;
    } else if (strcmp(extensionA, "Sin extensión") && !strcmp(extensionB, "Sin extensión")) {
        return 1;
    }

    // si quiero listar los pdf antes que los xopp
    // Comparar extensiones .xopp
    if(ordenarArchivos == 1)
    {
        int comparacion = strcmp(extensionA, extensionB);
        if (comparacion != 0) {
            return comparacion;
        }

        return strcmp(nombreA, nombreB);
        }
    else if(ordenarArchivos == 0)
    {
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
    }

    // Comparar nombres
    return strcmp(nombreA, nombreB);
}


int directorio_existe(const char *ruta) {
    DIR* dir = opendir(ruta);
    if (dir) {
        // El directorio existe
        closedir(dir);
        return 1;
    } else {
        // El directorio no existe
        return 0;
    }
}

const char* obtener_nombre_directorio(const char* ruta) {
    /*const char* subruta = ruta;
    for (int i = 0; i < 6; i++) {
        subruta = strchr(subruta, '/');
        if (subruta) subruta += 1; // Avanza el puntero para saltar el '/'
        else return "/"; // No se encontró la quinta '/', devuelve NULL
    }
    return subruta;*/

    char* subruta = NULL;
    
    // Verificar si la cadena base está presente en la ruta completa
    const char* inicio = strstr(ruta, rutaPredeterminada);
    
    if (inicio != NULL) {
        // Calcular la longitud de la subruta
        size_t longitudCadenaBase = strlen(rutaPredeterminada);
        size_t longitudSubruta = strlen(inicio) - longitudCadenaBase;
        
        // Asignar memoria para la subruta y copiarla
        subruta = (char*)malloc((longitudSubruta + 1) * sizeof(char));
        strncpy(subruta, inicio + longitudCadenaBase, longitudSubruta);
        subruta[longitudSubruta] = '\0';
    }
    
    return subruta;
}

const char* obtener_nombre_archivo(const char* ruta) {
    const char* nombre = strrchr(ruta, '/');
    if (nombre) nombre += 1; // Avanza el puntero para saltar el '/'
    else nombre = ruta; // No se encontró '/', la ruta es el nombre del archivo
    return nombre;
}