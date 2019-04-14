CC=g++

SDL_LIBS=-lSDL2 -lSDL2_image -lSDL2_ttf -std=c++11
CFLAGS=-Wall
H_PATH=core
SOURCE_PATH=$(H_PATH)/source

_SFILES=main.cpp game.cpp vector.cpp animation.cpp camera.cpp logger.cpp sprite.cpp scene.cpp media_manager.cpp collision_manager.cpp input_manager.cpp
SFILES=$(patsubst %,$(SOURCE_PATH)/%,$(_SFILES))

_HFILES=main.h vector.h animation.h sprite.h logger.h scene.h camera.h media_manager.h collision_manager.h input_manager.h
HFILES=$(patsubst %,$(H_PATH)/%,$(_HFILES))

%.o: %.cpp %(HFILES) $(CC) -c -o $@ $< $(CLFAGS)

test: $(HFILES)
	$(CC) $(CFLAGS) $(SFILES) -o game $(SDL_LIBS)