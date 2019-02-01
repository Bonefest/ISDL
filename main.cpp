#include <iostream>
#include <SDL2/SDL.h>

#include "game.h"
#include "scene.h"

#define DEFAULT_WIDTH 1280
#define DEFAULT_HEIGHT 720

int main(int argc, char const *argv[]) {
	Game::getInstance()->initGame(DEFAULT_WIDTH,DEFAULT_HEIGHT,"Game",SDL_INIT_VIDEO,SDL_WINDOW_SHOWN);
	TestScene* scene = new TestScene();
	Game::getInstance()->switchScene(scene);

	if(Game::getInstance()->isInitialized()) {
		Game::getInstance()->startGame();
	}

	Game::getInstance()->closeGame();

	delete scene;
	return 0;
}