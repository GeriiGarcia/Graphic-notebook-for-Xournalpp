OBJS	= startGtkApp.o
SOURCE	= startGtkApp.c menu.c crearImagen.c styleFunctions.c buttonFunctions.c
OUT	= start
CC	 = gcc
FLAGS	 = -g -Wall -g3 -pedantic -Wextra -Wshadow -Wpointer-arith \
	-Wcast-qual -Wcast-align -Wstrict-prototypes -Wmissing-prototypes -Wconversion -Wno-overlength-strings \
	-D_FILE_OFFSET_BITS=64
MKDIR_P = mkdir -p
USER = gerard
DIR_TO_CREATE = /home/$(USER)/.libretaXournal/cache
DIR_TO_CREATE_CONFIG = /home/$(USER)/.libretaXournal/config

all: create_directory
	$(CC) $(FLAGS) -o $(OUT) `pkg-config --cflags gtk+-3.0` $(SOURCE) `pkg-config --libs --cflags gtk+-3.0` `pkg-config --cflags --libs poppler-glib` `pkg-config --libs --cflags libxml-2.0` -lfreeimage -lm && ./start

create_directory:
	@if [ ! -d $(DIR_TO_CREATE) ]; then \
		echo "Creating directory: $(DIR_TO_CREATE);"; \
		$(MKDIR_P) $(DIR_TO_CREATE); \
	fi

	@if [ ! -d $(DIR_TO_CREATE_CONFIG) ]; then \
		echo "Creating directory: $(DIR_TO_CREATE_CONFIG);"; \
		$(MKDIR_P) $(DIR_TO_CREATE_CONFIG); \
	fi



# gcc -o start `pkg-config --cflags gtk+-3.0` startGtkApp.c `pkg-config --libs --cflags gtk+-3.0` `pkg-config --cflags --libs poppler-glib` && ./start 