CC=g++
CC_WINDOWS=i686-w64-mingw32-g++

SDL_LIBS=-lSDL2 -lSDL2_image -lSDL2_ttf -std=c++11 -static-libgcc -static-libstdc++
CFLAGS=-Wall
H_PATH=core
SOURCE_PATH=$(H_PATH)/source

_SFILES=main.cpp game.cpp vector.cpp animation.cpp camera.cpp logger.cpp sprite.cpp scene.cpp media_manager.cpp collision_manager.cpp input_manager.cpp state_manager.cpp physics.cpp
SFILES=$(patsubst %,$(SOURCE_PATH)/%,$(_SFILES))

_HFILES=main.h vector.h animation.h sprite.h logger.h scene.h camera.h media_manager.h collision_manager.h input_manager.h state_manager.h physics.h
HFILES=$(patsubst %,$(H_PATH)/%,$(_HFILES))

%.o: %.cpp %(HFILES) $(CC) -c -o $@ $< $(CLFAGS)

all: $(HFILES)
	$(CC) $(CFLAGS) $(SFILES) -o game $(SDL_LIBS)
windows: $(HFILES)
	$(CC_WINDOWS) $(CFLAGS) $(SFILES) -o game.exe -lmingw32 -lSDL2main $(SDL_LIBS)
	