# Graphic Notebook for Xournal++

## Introduction

This application has been developed in C with GTK3+ graphic library. It shows .xopp and .pdf files in a table of 4 columns and any number of rows. 

I started to develop this application because I do my unisersity notes with Xournal++. What I found missing was an application that unifys all my notes like Ipad applications, like goodnotes or OneNote of Microsoft. I did not like to open my file manager to go search my notes.

## Description

### General

![preview](IMG/preview.png)

This application is a file management interface that displays folders and files in a grid layout. Each button, when clicked, enters into a folder or opens a file in Xournal++. PDF files are indicated with a blue frame and can be opened directly. The xopp files would be indicated with an orange frame (not visible in this image). The interface is designed to be scrollable both horizontally and vertically, ensuring that users can navigate through the content even if it extends beyond the screen size. This allows for efficient organization and access to various file types within the application. In addition to the features previously mentioned, the files in this application also include a preview function. This allows users to see a thumbnail or a glimpse of the content within the files, enhancing the user experience by providing a quick way to identify and access the desired documents without having to open them fully. The preview feature is particularly useful for quickly locating a specific file among many, especially when dealing with a large number of documents. Also we can see the path in the current directory that we are in all moment.

If we left click in a directory we will see a remove option, it will remove the directory if we click.

If we do the same but in a file will apear the same button and depending the type of file:

    - PDF: We will see an option for opening the file with predeterminate application.
    - XOPP: We will see an option for exporting the file to PDF.

### Menu
The section for the menu has 3 submenus:

![preview](IMG/write_text.png)

    - Archivo:
        - New folder: we can create a new folder with specific name filling the new space avilitated for.
        - Export to pdf: if we click in this option, we have to click in a XOPP file to export it to PDF. It is like do left click option in XOPP.
        - Reset: to hide and reset the section to fill a name.
        - Remove recents: To remove the recent screen files. 
    - Navegación: 
        -
        -
        -
        -
        -
        -
    - Herramientas: The most important option in the remove cache. It is for removing the cache files in ~/.libretaXournal .

### Initial screen
