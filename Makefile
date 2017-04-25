CC	= g++
CFLAGS	= -O2 $(shell sdl2-config --cflags) -c -Wall
LFLAGS  = -O2 $(shell sdl2-config --cflags --libs) -lSDL2_image -lSDL2_ttf -Wall
LDFLAGS = $(shell sdl2-config --cflags --libs) -lSDL2_image -lSDL2_ttf -Wall

OBJS = Manu.o

all: test

build:  $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o Manu

clean:
	rm -f *.o Manu

test: build
	./Manu

%.o: %.cpp
	$(CC) $(LFLAGS) -c $< -o $@
