#include "camera.h"
#include "game.h"

Camera::Camera(Type t,Sprite* obj,bool actv):type(t),object(obj),position{0,0},border(),active(actv)  { }

Camera::Camera(Type t,Sprite* obj,bool actv,Rect pos,Rect brdr,double angl):
type(t),object(obj),position(pos),border(brdr),angle(angl),active(actv) { }


void Camera::setPosition(double x,double y) { position.x = x; position.y = y;}

void Camera::setPosition(Rect pos) { position = pos; }

void Camera::update(float delta) {
	if(!fixed && active) {
		SDL_Point screenSize = Game::getInstance()->getScreenSize();
		Rect objectPosition;
		Rect objectSize;

		if(object != nullptr) { 
			objectPosition = object->getAbsolutePosition();
			objectSize = object->getSize();
		}
		else { 
			objectPosition = (Rect){0,0};
			objectSize = (Rect){0,0};
		}

		switch(type) {
			case None:break;
			case Centered:
				position.x = objectPosition.x - screenSize.x/2 + objectSize.x/2;
				position.y = objectPosition.y - screenSize.y/2 + objectSize.y/2;
				break;
			case Sleeper:break;
			case Rectangle:break;
		}	
	}
}

void Camera::setAngle(double angl) { 
	angle = angl; 
}