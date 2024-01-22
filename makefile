OBJS	= startGtkApp.o
SOURCE	= startGtkApp.c menu.c
HEADER	= 
OUT	= start
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all:
	$(CC) -o $(OUT) `pkg-config --cflags gtk+-3.0` $(SOURCE) `pkg-config --libs --cflags gtk+-3.0` && ./start



