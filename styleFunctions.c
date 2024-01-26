#include "includes.h"


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

    //print de la cadena nueva
    //printf("%s\n",nuevaCadena);

    return nuevaCadena;
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
 * @brief Se usa en el qsort()
*/
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
