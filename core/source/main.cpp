#include <iostream>
#include <SDL2/SDL.h>

#include "../game.h"
#include "../scene.h"
#include "../../tests/myScene.h"

#define DEFAULT_WIDTH 1280
#define DEFAULT_HEIGHT 720

int main(int argv, char** args) {
	MSDL::Game::getInstance()->initGame(DEFAULT_WIDTH,DEFAULT_HEIGHT,"Game",SDL_INIT_VIDEO,SDL_WINDOW_SHOWN);
	myScene* scene = new myScene();
	MSDL::Game::getInstance()->switchScene(scene);

	if(MSDL::Game::getInstance()->isInitialized()) {
		MSDL::Game::getInstance()->startGame();
	}

	MSDL::Game::getInstance()->closeGame();

	return 0;
}