#include "../camera.h"
#include "../game.h"

Camera::Camera(Type t,Sprite* obj,bool actv):Object((Rect){0,0}),type(t),object(obj),border(),active(actv)  { }

Camera::Camera(Type t,Sprite* obj,bool actv,Rect pos,Rect brdr,double angl):
Object(pos),type(t),object(obj),border(brdr),angle(angl),active(actv) { }


void Camera::update(double delta) {
	if(!fixed && active) {
		SDL_Point screenSize = Game::getInstance()->getScreenSize();
		Rect objectPosition;
		Rect objectSize;

		if(object != nullptr) { 
			objectPosition = object->getPosition();
			objectSize = object->getSize();
		}
		else { 
			objectPosition = (Rect){0,0};
			objectSize = (Rect){0,0};
		}

		switch(type) {
			case None:break;
			case Centered:
				Rect newPosition;
				newPosition.x = objectPosition.x - screenSize.x/2 + objectSize.x/2;
				newPosition.y = objectPosition.y - screenSize.y/2 + objectSize.y/2;
				setPosition(newPosition);
				break;
			case Sleeper:break;
			case Rectangle:break;
		}	
	}
}

void Camera::setAngle(double angl) { 
	angle = angl; 
}