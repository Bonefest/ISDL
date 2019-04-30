#include "../core/scene.h"
#include "../core/game.h"
#include "../core/state_manager.h"
#include "Block.h"
#include <iostream>

void myCallback() {
	std::cout << "Finish!" << std::endl;
}

void myCallback2() {
	std::cout << "FINISHSHSHS" << std::endl;
}

void timer() {
	static int score = 0;
	if(score == 100) return;
	score++;
	std::cout << score << std::endl;
	return MSDL::StateManager::getInstance()->call(1000,timer);
}

class myScene : public Scene{
	Block *block;
	Sprite *sblock;
	Label* label;
	Camera *cam;

public:
	myScene(){

		block = new Block(Game::getMediaManager()->loadTexture("tests/block.png"), (Rect){50, 50}, (Rect){32, 32});
		sblock = new Sprite(Game::getMediaManager()->loadTexture("tests/block.png"), (Rect){50, 250}, (Rect){32, 32});
		label = new Label(Game::getMediaManager()->loadFont("resources/default.ttf",36),"TEST",(Rect){150,150},(SDL_Color){255,255,255});

		cam = new Camera();

		addSprite(block);
		addSprite(sblock);
		addSprite(label);

		MSDL::StateManager::getInstance()->moveTo(block,(Rect){500,0},1000,myCallback);
		MSDL::StateManager::getInstance()->rotateTo(sblock,90,5000,myCallback2);
		MSDL::StateManager::getInstance()->call(1000,timer);

		addCamera("camera", cam);
	}



};