CC=g++

SDL_LIBS=-lSDL2 -lSDL2_image -lSDL2_ttf -std=c++11
CFLAGS=-Wall
FILES=main.cpp game.cpp vector.cpp animation.cpp camera.cpp logger.cpp sprite.cpp scene.cpp media_manager.cpp

test: main.h vector.h animation.h sprite.h logger.h scene.h camera.h media_manager.h
	$(CC) $(CFLAGS) $(FILES)  -o game $(SDL_LIBS)