#include "../core/sprite.h"
#include "../core/game.h"
#include "../core/physics.h"
#include <iostream>

class Block: public Sprite{
	double speed;
public:
	void controller(SDL_Event* event){
		Sprite::controller(event);
		if(event->type == SDL_KEYDOWN) {

			if(Game::getInstance()->isPressed(SDLK_a)) {
				physics->addForce(-Vector2::right() * speed);
			}
			if(Game::getInstance()->isPressed(SDLK_d)) {
				physics->addForce(Vector2::right() * speed);
			}
			if(Game::getInstance()->isPressed(SDLK_s)) {
				physics->addForce(-Vector2::up() * speed);
			}
			if(Game::getInstance()->isPressed(SDLK_w)) {
				physics->addForce(Vector2::up() * speed);
			}
		}
	}

	virtual void onCollision(Sprite* sprite) {

	}
	
	virtual void update(double delta) {
		Sprite::update(delta);
	}

	Block(SDL_Texture* texture,Rect absolutePosition,Rect size,double angle = 0.0,SDL_RendererFlip type=SDL_FLIP_NONE,bool pinned = false):Sprite(texture, absolutePosition, size, angle, type, pinned){
		physics = new Physics(this, false, 1.0, 1.0);
		physics->setAntiF(0);
		speed = 0.3;
		//physics->addConstForce(Vector2::right() * 0.0001);
	}
};