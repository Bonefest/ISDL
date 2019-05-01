#include "../core/scene.h"
#include "../core/game.h"
#include "../core/state_manager.h"
#include "Block.h"
#include <iostream>

bool finish = false;

void myCallback() {
	finish = true;
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

		MSDL::StateManager::getInstance()->call(1000,timer);
		MSDL::StateManager::getInstance()->paintTo(sblock,(SDL_Color){0,0,0,0},1000,myCallback);

		addCamera("camera", cam);
	}

	void update(double delta) {
		Scene::update(delta);

		if(finish) {

			Color color = sblock->getColor();
			if(color.a == 255) {
				MSDL::StateManager::getInstance()->paintTo(sblock,(SDL_Color){0,0,0,0},1000,myCallback);
			} else
				MSDL::StateManager::getInstance()->paintTo(sblock,(SDL_Color){255,255,255,255},1000,myCallback);
			MSDL::StateManager::getInstance()->moveBy(block,nullptr,(Rect){10,10},500);
			MSDL::StateManager::getInstance()->rotateBy(block,nullptr,45,500);

			finish = false;
		}
	}


};

