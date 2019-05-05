#include "../camera.h"
#include "../game.h"
#include "../state_manager.h"


using namespace MSDL;
using namespace MSDL::StandardCameras;

Camera::Camera(Sprite* obj,bool actv):Object((Rect){0,0}),object(obj),viewport(),active(actv),fixed(false),activeViewport(false)  { }

Camera::Camera(Sprite* obj,bool actv,Rect pos,Rect viewp):
Object(pos),object(obj),viewport(viewp),active(actv),fixed(false),activeViewport(true) { }


void Camera::update(double delta) {

}

void Camera::setAngle(double angl) { 
	angle = angl; 
}

///////////////////////////////////////////////////////////////////////////////////////////////

void CameraCenter::update(double delta) {
	if(!isFixed() && isActive() ) {
		Rect newPosition;
		Rect objectPosition;
		Rect objectSize;

		if(getObject() != nullptr) { 
			objectPosition = getObject()->getPosition();
			objectSize = getObject()->getSize();
		}
		else { 
			objectPosition = (Rect){0,0};
			objectSize = (Rect){0,0};
		}

		if(isActiveViewport()) {
			Rect viewport = getViewport();
			newPosition.x = objectPosition.x - viewport.width/2 + objectSize.x/2;
			newPosition.y = objectPosition.y - viewport.height/2 + objectSize.y/2;
		} else {
			SDL_Point screenSize = Game::getInstance()->getScreenSize();
			newPosition.x = objectPosition.x - screenSize.x/2 + objectSize.x/2;
			newPosition.y = objectPosition.y - screenSize.y/2 + objectSize.y/2;
		}

		setPosition(newPosition);

	}

}

///////////////////////////////////////////////////////////////////////////////////////////////

CameraSleeper::CameraSleeper(double distance,double time,Sprite* object,bool active):Camera(object,active),maximalDistance(distance),runningTime(time),currentRunningTime(0),running(false) {}

CameraSleeper::CameraSleeper(double distance,double time,Sprite* object,bool active,Rect position,Rect viewport):Camera(object,active,position,viewport),maximalDistance(distance),runningTime(time),currentRunningTime(0),running(false) {}

#include <iostream>
void CameraSleeper::update(double delta) {
	if(!isFixed() && isActive()) {
		if( !isRunning() ) {
			Rect objectPosition;
			Rect objectSize;

			if(getObject() != nullptr) {
				objectPosition = getObject()->getPosition();
				objectSize = getObject()->getSize();
			}
			else  {
				objectPosition = (Rect){0,0};
				objectSize = (Rect){0,0};
			}


			Rect centerPosition = getPosition();
			Rect newPosition;

			if(isActiveViewport()) {
				Rect viewport = getViewport();
				centerPosition.x += viewport.width/2-objectSize.x/2;
				centerPosition.y += viewport.height/2-objectSize.x/2;
				newPosition.x = objectPosition.x - viewport.width/2 + objectSize.x/2;
				newPosition.y = objectPosition.y - viewport.height/2 + objectSize.y/2;
			} else {
				SDL_Point screenSize = Game::getInstance()->getScreenSize();
				centerPosition.x += screenSize.x/2-objectSize.x/2;
				centerPosition.y += screenSize.y/2-objectSize.y/2;
				newPosition.x = objectPosition.x - screenSize.x/2 + objectSize.x/2;
				newPosition.y = objectPosition.y - screenSize.y/2 + objectSize.y/2;
			}

			if(centerPosition.distanceTo(objectPosition) >= maximalDistance) {
				StateManager::getInstance()->moveTo(this, newPosition, runningTime);
				running = true;
				currentRunningTime=0.0;
			}



		} else {
			if(currentRunningTime > runningTime) {
				currentRunningTime = 0;
				running = false;
			}

			currentRunningTime += delta;
		}
	}
}


CameraRectangle::CameraRectangle(Rect size,Sprite* object,bool active):Camera(object,active),border(size) {}

CameraRectangle::CameraRectangle(Rect size,Sprite* object,bool active,Rect position,Rect viewport):Camera(object,active,position,viewport),border(size) {}

#include <iostream>
void CameraRectangle::update(double delta) {
	if(!isFixed() && isActive()) {
		Rect objectPosition;
		Rect objectSize;

		if(getObject() != nullptr) {
			objectPosition = getObject()->getPosition();
			objectSize = getObject()->getSize();
		} else {
			objectPosition = (Rect){0,0};
			objectSize = (Rect){0,0};
		}

		Rect cameraPosition = getPosition();
		Rect borderPosition = cameraPosition;

		Rect screenRect;

		if(isActiveViewport()) {
			screenRect = getViewport();
		} else {
			SDL_Point screenSize = Game::getInstance()->getScreenSize();
			screenRect.width = screenSize.x;
			screenRect.height = screenSize.y;
		}

		borderPosition.x += screenRect.width/2 - border.x/2;
		borderPosition.y += screenRect.height/2 - border.y/2;

		Rect newPosition = cameraPosition;
		if(objectPosition.x < borderPosition.x)
			newPosition.x = cameraPosition.x + objectPosition.x - borderPosition.x;
		else if(objectPosition.x > borderPosition.x + border.x - objectSize.x)
			newPosition.x = cameraPosition.x + objectPosition.x - borderPosition.x - border.x + objectSize.x;

		if(objectPosition.y < borderPosition.y)
			newPosition.y = cameraPosition.y + objectPosition.y - borderPosition.y;
		else if(objectPosition.y > borderPosition.y + border.y - objectSize.y)
			newPosition.y = cameraPosition.y + objectPosition.y - borderPosition.y - border.y + objectSize.y;

		setPosition(newPosition);

		
	}
}