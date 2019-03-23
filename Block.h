#include "sprite.h"
#include "game.h"
#include <iostream>

class Block: public Sprite{
	int speed;
public:
	void controller(SDL_Event* event){
		if(event->type == SDL_KEYDOWN) {

			if(Game::getInstance()->isPressed(SDLK_a)) {
				addPosition(-1*speed, 0);
			}
			if(Game::getInstance()->isPressed(SDLK_d)) {
				addPosition(1*speed, 0);
			}
			if(Game::getInstance()->isPressed(SDLK_s)) {
				addPosition(0, 1*speed);
			}
			if(Game::getInstance()->isPressed(SDLK_w)) {
				addPosition(0, -1*speed);
			}
		}
		//std::cout << getAbsolutePosition().x << " " << getAbsolutePosition().y << std::endl;
	}

	Block(SDL_Texture* texture,Rect absolutePosition,Rect size,double angle = 0.0,SDL_RendererFlip type=SDL_FLIP_NONE,bool pinned = false){
		Sprite(texture, absolutePosition, size, angle, type, pinned);
		speed = 5;
	}
};