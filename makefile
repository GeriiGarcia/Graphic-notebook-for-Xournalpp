OBJS	= startGtkApp.o
SOURCE	= startGtkApp.c menu.c crearImagen.c styleFunctions.c buttonFunctions.c
HEADER	= 
OUT	= start
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all:
	$(CC) -o $(OUT) `pkg-config --cflags gtk+-3.0` $(SOURCE) `pkg-config --libs --cflags gtk+-3.0` `pkg-config --cflags --libs poppler-glib` `pkg-config --libs --cflags libxml-2.0` -lfreeimage -lm && ./start



# gcc -o start `pkg-config --cflags gtk+-3.0` startGtkApp.c `pkg-config --libs --cflags gtk+-3.0` `pkg-config --cflags --libs poppler-glib` && ./start 