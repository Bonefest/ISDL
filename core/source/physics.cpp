#include "../physics.h"
#include "../sprite.h"

Physics::Physics(Sprite &obj, double m, bool gravity){
	object = &obj;
	mass = m;
	usesGravity = gravity;
	gravityF = m * Vector2(0, 0.98);
	antiF = m * 0.2;
	if(gravity){
		resF = gravityF;
		resF += antiF * (-gravityF.normal());
	}else{
		resF = Vector2();
	}


} 

void Physics::update(){
	speed += resF/m;
	object->addPosition(speed);
}