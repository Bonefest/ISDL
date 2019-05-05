#include "../state_manager.h"

using namespace MSDL;
using namespace MSDL::StandardActions;

#include <iostream>
MoveTo::MoveTo(Object* obj, Rect position, double duration, v_callback finishCallback ):Action(duration,finishCallback),object(obj),endPos(position),curTime(0) {
	if(duration <= 0) duration = 1;

	Rect objectPosition = obj->getPosition();

	speed.x = (position.x - objectPosition.x)/duration;
	speed.y = (position.y - objectPosition.y)/duration;
}

bool MoveTo::isFinished() { return curTime >= getDuration(); }

void MoveTo::update(double delta) {
	if( !isFinished() ) {
		object->addPosition(speed.x*delta,speed.y*delta);
		curTime += delta;
	}
}

void MoveTo::finish() {
	Action::finish();
	object->setPosition(endPos);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

RotateTo::RotateTo(Sprite* spr, double angle, double duration, v_callback finishCallback):Action(duration,finishCallback),sprite(spr),endAngle(angle),curTime(0) {
	if(duration <= 0) duration = 1;

	double objectAngle = spr->getAngle();

	speed = (angle-objectAngle)/duration;
} 

bool RotateTo::isFinished() { return curTime >= getDuration(); }

void RotateTo::update(double delta) {
	if(!isFinished() ) {
		sprite->addAngle(speed*delta);
		curTime += delta;
	}
}

void RotateTo::finish() {
	Action::finish();
	sprite->setAngle(endAngle);
}
/////////////////////////////////////////////////////////////////////////////////////////////////

PaintTo::PaintTo(Sprite* spr, Color color , double duration, v_callback finishCallback):Action(duration,finishCallback),sprite(spr),endColor(color),curTime(0) {
	if(duration <= 0) duration = 1;

	Color spriteColor = spr->getColor();
	speedColor.r = (color.r - spriteColor.r)/duration;
	speedColor.g = (color.g - spriteColor.g)/duration;
	speedColor.b = (color.b - spriteColor.b)/duration;
	speedColor.a = (color.a - spriteColor.a)/duration;

}

bool PaintTo::isFinished() { return curTime >= getDuration(); }

void PaintTo::update(double delta) {
	if(!isFinished() ) {
		Color spriteColor = sprite->getColor();
		spriteColor.r += speedColor.r*delta;
		spriteColor.g += speedColor.g*delta;
		spriteColor.b += speedColor.b*delta;
		spriteColor.a += speedColor.a*delta;

		sprite->setColor(spriteColor);

		curTime += delta;
	}
}

void PaintTo::finish() {
	Action::finish();
	sprite->setColor(endColor);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
Call::Call(double duration,v_callback finishCallback):Action(duration,finishCallback),curTime(0) {}

bool Call::isFinished() { return curTime >= getDuration(); }

void Call::update(double delta) {
	if(!isFinished())
		curTime += delta;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

StateManager* StateManager::getInstance() {
	static StateManager* stateManager = new StateManager();

	return stateManager;
}

void StateManager::close() {
	for(auto actionIter = actionList.begin();actionIter!=actionList.end();actionIter++)
		delete (*actionIter);

	delete getInstance();
}

void StateManager::update(double delta) {
	for(auto actionIter = actionList.begin();actionIter!=actionList.end();) {
		if( (*actionIter)->isFinished() ) {

			(*actionIter)->finish();
			delete (*actionIter);

			actionIter = actionList.erase(actionIter);
		}
		else {
			(*actionIter)->update(delta);
			actionIter++;
		}
	}
}

void StateManager::moveTo(Object* object, Rect position, double duration, v_callback finishCallback) {
	actionList.push_back(new MoveTo(object,position,duration, finishCallback));
}

void StateManager::moveBy(Object* movable, Object* comp, Rect position , double duration, v_callback finishCallback) {
	Rect relativePosition;
	if(comp == nullptr) relativePosition = movable->getPosition();
	else relativePosition = comp->getPosition();

	position.x += relativePosition.x;
	position.y += relativePosition.y;

	actionList.push_back(new MoveTo(movable,position, duration, finishCallback ));
}

void StateManager::rotateTo(Sprite* sprite, double angle, double duration, v_callback finishCallback) {
	actionList.push_back(new RotateTo(sprite,angle,duration,finishCallback));
}

void StateManager::rotateBy(Sprite* rotateable, Sprite* comp, double angle , double duration, v_callback finishCallback) {
	double relativeAngle;
	if(comp == nullptr) relativeAngle = rotateable->getAngle();
	else relativeAngle = comp->getAngle();

	actionList.push_back(new RotateTo(rotateable, angle+relativeAngle , duration, finishCallback));
}

void StateManager::paintTo(Sprite* sprite,Color color, double time, v_callback finishCallback) {
	actionList.push_back(new PaintTo(sprite,color,time,finishCallback));
}

void StateManager::call(double duration,v_callback finishCallback) {
	actionList.push_back(new Call(duration,finishCallback));
}
