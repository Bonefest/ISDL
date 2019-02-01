CC=g++

SDL_LIBS=-lSDL2 -lSDL2_image -std=c++11
CFLAGS=-Wall
FILES=main.cpp game.cpp vector.cpp animation.cpp camera.cpp logger.cpp sprite.cpp scene.cpp texture_manager.cpp

test: main.h vector.h animation.h sprite.h logger.h scene.h camera.h texture_manager.h
	$(CC) $(CFLAGS) $(FILES)  -o game $(SDL_LIBS)