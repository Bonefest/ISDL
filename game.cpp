#include <string>
#include <sstream>
#include "game.h"
#include <SDL2/SDL_ttf.h>


#include "vector.h"
Game* Game::getInstance() {
	static Game* game = new Game();
	return game;
}


void Game::initGame(int width,int height,std::string name, Uint32 flags,Uint32 windowFlags,Uint32 rendererFlags,bool _fullscreen,const char* logFileName) {
	logger = new Logger(logFileName);
	currentScene = nullptr;

	gameName = name;
	fullscreen = _fullscreen;
	lastTime = SDL_GetPerformanceCounter();
	screenSize = (SDL_Point){width,height};

	logger->log("Starting initializing of '" + gameName + "'");

	if(SDL_Init(flags) != 0) {
		logger->log(std::string("SDL_Init has been failed with error : ") + SDL_GetError());
		initialized = false;
		return;
	}

	if(fullscreen) windowFlags = windowFlags | SDL_WINDOW_FULLSCREEN;

	window = SDL_CreateWindow(gameName.c_str(),SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,windowFlags);
	if(window == NULL) {
		logger->log(std::string("Window can't be created : ") + SDL_GetError());
		initialized = false;
		return;
	}

	renderer = SDL_CreateRenderer(window,-1,rendererFlags);
	if(renderer == NULL) {
		logger->log(std::string("Renderer can't be loaded : ") + SDL_GetError());
		initialized = false;
		return;
	}

	int imageFlags = IMG_INIT_PNG;
	int initted = IMG_Init(imageFlags);
	if( (initted&imageFlags) != imageFlags) {
		logger->log(std::string("Image subsystem can't be loaded : ") + SDL_GetError());
		initialized = false;
		return;
	}

	if(TTF_Init() == -1) {
		logger->log(std::string("TTF subsystem can't be loaded : ") + SDL_GetError());
		initialized = false;
		return;

	}

	initialized = true;
	logger->log("'" + gameName + "' has successfully initialized.");
}


	#include <iostream>
void Game::closeGame() {

	logger->log("Destroying a render ...");
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;

	logger->log("Destroying a window ...");
	SDL_DestroyWindow(window);
	window = nullptr;

	MediaManager::getInstance()->close();
	logger->log("MediaManager has successfully destroyed.");

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	delete logger;
	delete Game::getInstance();

}

void Game::controller() {
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		if(event.type == SDL_QUIT) {
			exitStatus = true;
			return;
		} 

		//Добавление нажатой клавиши мыши
		else if(event.type == SDL_MOUSEBUTTONDOWN) {
			if(!isPressed(event.button.button)) {
				pressedButtons.insert(std::pair<Uint8,Uint64>(event.button.button,SDL_GetTicks()));
				
				lastMouseClickPosition.x = event.button.x;
				lastMouseClickPosition.y = event.button.y;
			}
		}

		//Если нажата клавиша и её нет в словаре - добавляем её в словарь.
		//Добавляется до обработки сценой нажатий.
		else if(event.type == SDL_KEYDOWN) {
			if(!isPressed(event.key.keysym.sym))
				pressedKeys.insert(std::pair<SDL_Keycode,Uint64>(event.key.keysym.sym,SDL_GetTicks()));	
		}

		currentScene->controller(&event);

		//Если клавиша отпущена и она есть в словаре - удаляем её из словаря.
		//Удаляется после обработки сценой прекращения нажатия клавиши
		if(event.type == SDL_KEYUP) {
			if(isPressed(event.key.keysym.sym)) {
				pressedKeys.erase(event.key.keysym.sym);
			}
		} else if(event.type == SDL_MOUSEBUTTONUP) {
			if(isPressed(event.button.button))
				pressedButtons.erase(event.button.button);
		}
	}
}

double Game::getDeltaTime() {
	Uint64 nowTime = SDL_GetPerformanceCounter();
	double delta = (nowTime-lastTime)*1000.0/double(SDL_GetPerformanceFrequency());
	lastTime = nowTime;
	return delta;
}

void Game::startGame() {
	exitStatus = false;
//	Vector2 vect(100,100);

//	Game::getInstance()->log(std::to_string(vect.getPosition().x));
	while(!exitStatus) {
		controller();
		currentScene->update(getDeltaTime());
		draw();
	}

}

void Game::draw() {
	SDL_RenderClear(renderer);
	currentScene->draw();
	SDL_RenderPresent(renderer);
}

void Game::log(std::string message) {
	logger->log(message);
}

bool Game::isPressed(SDL_Keycode key) {
	return pressedKeys.find(key) != pressedKeys.end();
}

bool Game::isPressed(Uint8 key) {
	return pressedButtons.find(key) != pressedButtons.end();
}

long long Game::getPressedTime(SDL_Keycode key) {
	if(isPressed(key)) {
		return SDL_GetTicks() - pressedKeys[key];
	}

	return -1ll;	//Лучше пусть это будет константа аля NONE
}