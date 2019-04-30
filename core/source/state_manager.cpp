#include "../state_manager.h"

using namespace MSDL;
using namespace MSDL::StandardActions;

MoveTo::MoveTo(Object* obj, Rect position, double duration, v_callback finishCallback ):Action(duration,finishCallback),object(obj),curTime(0) {
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

/////////////////////////////////////////////////////////////////////////////////////////////////

RotateTo::RotateTo(Sprite* spr, double angle, double duration, v_callback finishCallback):Action(duration,finishCallback),sprite(spr),curTime(0) {
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
/////////////////////////////////////////////////////////////////////////////////////////////////
/*
PaintTo::PaintTo(Sprite* spr, SDL_Color color , double duration, v_callback finishCallback):Action(duration,finishCallback) {
	//НЕ РЕАЛИЗОВАНО
}
*/
/////////////////////////////////////////////////////////////////////////////////////////////////
Call::Call(double duration,v_callback finishCallback):Action(duration,finishCallback),curTime(0) {}

bool Call::isFinished() { return curTime >= getDuration(); }

void Call::update(double delta) {
	if(!isFinished())
		curTime += delta;
}

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

void StateManager::rotateTo(Sprite* sprite, double angle, double duration, v_callback finishCallback) {
	actionList.push_back(new RotateTo(sprite,angle,duration,finishCallback));
}

void StateManager::call(double duration,v_callback finishCallback) {
	actionList.push_back(new Call(duration,finishCallback));
}