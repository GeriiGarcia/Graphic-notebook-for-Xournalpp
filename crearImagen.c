#include <stdio.h>
#include <cairo.h>
#include <poppler/glib/poppler.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>
#include <FreeImage.h>

void pdf_to_image(const char *pdf_filename, const char *image_filename) {
    GError *error = NULL;
    gchar *uri = g_filename_to_uri(pdf_filename, NULL, &error);
    if (uri == NULL) {
        printf("Error al convertir la ruta del archivo en URI: %s\n", error->message);
        g_error_free(error);
        return;
    }

    PopplerDocument *document = poppler_document_new_from_file(uri, NULL, &error);
    g_free(uri);
    if (document == NULL) {
        printf("Error al abrir el documento PDF: %s\n", error->message);
        g_error_free(error);
        return;
    }

    PopplerPage *page = poppler_document_get_page(document, 0);
    if (page == NULL) {
        printf("Error al obtener la primera página del documento PDF.\n");
        g_object_unref(document);
        return;
    }

    double width, height;
    poppler_page_get_size(page, &width, &height);

    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cairo_t *cr = cairo_create(surface);

    // Llenar la superficie con blanco
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0); // RGB para blanco
    cairo_paint(cr);

    // Renderizar la página
    poppler_page_render(page, cr);
    cairo_destroy(cr);

    cairo_status_t status = cairo_surface_write_to_png(surface, image_filename);
    if (status != CAIRO_STATUS_SUCCESS) {
        printf("Error al escribir la imagen PNG: %s\n", cairo_status_to_string(status));
    }

    cairo_surface_destroy(surface);
    g_object_unref(page);
    g_object_unref(document);
}



/*----------------------XOURNALS----------------------------------*/

void base64_to_image(const char *base64, const char *image_filename) {
    gsize output_length;
    guchar *data = g_base64_decode(base64, &output_length);

    FILE *file = fopen(image_filename, "wb");
    if (file == NULL) {
        printf("No se pudo abrir el archivo para escribir.\n");
        return;
    }

    fwrite(data, 1, output_length, file);
    fclose(file);

    g_free(data);
}

xmlChar* copiar_y_extraer_preview(const char *ruta_origen, const char *ruta_destino) {
    char comando[1024];

    char *preview = "";

    // Copiar el fichero al directorio destino y cambiar la extensión a .gz
   
    sprintf(comando, "cp %s %s.gz", ruta_origen, ruta_destino);
    system(comando);

    // Descomprimir el fichero
    sprintf(comando, "gunzip %s.gz -f", ruta_destino);
    system(comando);

  

    // Parsear el fichero XML y extraer el contenido del tag <preview>
    //xmlDocPtr doc = xmlParseFile(ruta_destino);

    xmlDocPtr doc = xmlReadFile(ruta_destino, NULL, 0);
    if (doc == NULL) {
        fprintf(stderr, "Error al cargar el archivo XML.\n");
        return NULL;
    }

    xmlNodePtr nodo = xmlDocGetRootElement(doc);
    xmlNodePtr previewNode = xmlFirstElementChild(nodo);

  
    while (previewNode != NULL) {
        if (xmlStrcmp(previewNode->name, (const xmlChar *)"preview") == 0) {
            // Obtener y mostrar el contenido del tag <preview>
            xmlChar *previewValue = xmlNodeListGetString(doc, previewNode->xmlChildrenNode, 1);
            //printf("Valor del tag <preview>: %s\n", previewValue);
            xmlFreeDoc(doc);  // Liberar el documento antes de salir de la función
            return previewValue;
            
        }
        previewNode = xmlNextElementSibling(previewNode);
    }

    xmlFreeDoc(doc);
    fprintf(stderr, "Tag <preview> no encontrado en el archivo XML.\n");
    return NULL;
}





