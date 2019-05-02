#include "../core/scene.h"
#include "../core/game.h"
#include "../core/input_manager.h"
#include "../core/collision_manager.h"
#include "Block.h"
#include <iostream>

class myScene : public Scene{
	Block *block;
	Sprite *sblock;
	Label* label;
	Camera *cam;

	Sprite* element;
public:
	myScene(){
		element = nullptr;

		block = new Block(Game::getMediaManager()->loadTexture("tests/block.png"), (Rect){50, 50}, (Rect){32, 32});
		sblock = new Sprite(Game::getMediaManager()->loadTexture("tests/block.png"), (Rect){80, 65}, (Rect){32, 32});
		label = new Label(Game::getMediaManager()->loadFont("resources/default.ttf",36),"TeSt",(Rect){80,80},(SDL_Color){255,255,255});

		cam = new Camera();

		addSprite(block);
		addSprite(sblock);
		addSprite(label);

		addCamera("camera", cam);
	}

	void update(double delta) {
		Scene::update(delta);
		SDL_Point cursor = MSDL::InputManager::getInstance()->getCursorPosition();
		Sprite* sprite = getCollisionManager()->raycast((Rect){0,0},(Rect){double(cursor.x),double(cursor.y)});
		if(sprite != nullptr) {
			if(element != nullptr) {
				element->setColor(255,255,255,255);
				sprite->setColor(255,0,0,255);
				element = sprite;
			}
			else {
				sprite->setColor(255,0,0,255);
				element = sprite;
			}
		} else if(element != nullptr) {
			element->setColor(255,255,255,255);
			element = nullptr;
		}

	}

	void draw() {
		Scene::draw();
		SDL_Point cursor = MSDL::InputManager::getInstance()->getCursorPosition();
		SDL_SetRenderDrawColor(Game::getInstance()->getRenderer(),255,255,255,255);
		SDL_RenderDrawLine(Game::getInstance()->getRenderer(),0,0,cursor.x,cursor.y);
		SDL_SetRenderDrawColor(Game::getInstance()->getRenderer(),0,0,0,255);
	}

};

