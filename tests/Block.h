#include "../core/sprite.h"
#include "../core/game.h"
#include <iostream>

class Block: public Sprite{
	int speed;
public:
	void controller(SDL_Event* event){
		Sprite::controller(event);
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
		
	}

	virtual void onDrag(SDL_MouseMotionEvent event) {
		setPosition(event.x,event.y);
		speed = 0;
	}

	virtual void onDrop(SDL_MouseButtonEvent event) {
		
		speed = 1;
	}

	virtual void onCollision(Sprite* sprite) {
		speed = 0;
	}
	
	virtual void update(double delta) {
		Sprite::update(delta);
		addPosition(0,0.098*delta*speed);
	}

	Block(SDL_Texture* texture,Rect absolutePosition,Rect size,double angle = 0.0,SDL_RendererFlip type=SDL_FLIP_NONE,bool pinned = false):Sprite(texture, absolutePosition, size, angle, type, pinned){
		
		speed = 1;
	}
};