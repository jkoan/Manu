CC	= g++
CFLAGS	= -O2 $(shell sdl2-config --cflags) -c -Wall
LFLAGS  = -O2 $(shell sdl2-config --cflags --libs) -lSDL2_image -lSDL2_ttf -Wall
LDFLAGS = $(shell sdl2-config --cflags --libs) -lSDL2_image -lSDL2_ttf -Wall

OBJS = 08_geometry_rendering.o

all: test

build:  $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o 08_geometry_rendering

clean:
	rm -f *.o 08_geometry_rendering

test: build
	./08_geometry_rendering

%.o: %.cpp
	$(CC) $(LFLAGS) -c $< -o $@
