#include "../physics.h"
#include "../sprite.h"
#include <cmath>
#include <iostream>

using namespace MSDL;

enum {up=0, right, down, left};

Physics::Physics(Sprite *obj, bool gravity, double m, double b){
	object = obj;
	mass = m;
	usesGravity = gravity;
	bounce = b;
	constF = Vector2();
	curF = Vector2();
	gravityF = mass * Vector2(0, 0.001);
	antiF = mass * 0.0005;
	
	isCollision = false;
	sqrSpeedLimit = 1;
	speed = Vector2();

} 

void Physics::update(double delta){

	Vector2 prevSpeed = speed;
	

	if(isCollision){ //Bounce logic

		if(speed.sqrlen() > 0.0001){

			if(mySide[left]){
				speed.setX(-speed.getX());
				speed *= bounce;
				// wasSide[left] = true;
				
			}

			if(mySide[right]){
				speed.setX(-speed.getX());
				speed *= bounce;
				// std::cout << speed.getY() << std::endl;
				// wasSide[right] = true;
				
			}

			if(mySide[up]){
				speed.setY(-speed.getY());
				speed *= bounce;
				// wasSide[up] = true;
				
			}

			if(mySide[down]){
				speed.setY(-speed.getY());
				speed *= bounce;
				// wasSide[down] = true;
				
			}
		}		
	}

	
	speed += (constF/mass) * delta;
	if(!curF.isNull()){
		speed += (curF/mass) * delta;
		curF = Vector2();
	}
	if(usesGravity)
		speed += (gravityF/mass) * delta;
	antiFV = Vector2(speed.normal());
	antiFV = -antiFV;
	antiFV *= antiF;
	if(antiF == 0.0)
		antiFV = Vector2();
	//std::cout << antiFV.getX() << " " << antiFV.getY() << std::endl;

	if(abs(antiFV.getX()) > abs(speed.getX())){
		speed.setX(0.0);
	}else{
		speed.setX(speed.getX() + antiFV.getX());
	}
	if(abs(antiFV.getY()) > abs(speed.getY())){
		speed.setY(0.0);
	}else{
		speed.setX(speed.getX() + antiFV.getX());
	}

	//std::cout << speed.getX() << " " << speed.getY() << std::endl;

	if(speed.sqrlen() > sqrSpeedLimit)
		speed = prevSpeed;



	if(isCollision){ //Bounce logic

		if(mySide[left]){
			if(speed.getX() < 0)
				speed.setX(0.0);
			mySide[left] = false;
			
		}
		if(mySide[right]){
			if(speed.getX() > 0)
				speed.setX(0.0);
			// std::cout << speed.getY() << std::endl << std::endl;
			mySide[right] = false;
			
		}
		if(mySide[up]){
			if(speed.getY() < 0)
				speed.setY(0.0);
			mySide[up] = false;
			
		}
		if(mySide[down]){
			if(speed.getY() > 0)
				speed.setY(0.0);
			mySide[down] = false;
			
		}

		isCollision = false;

		
	}

	
	object->addPosition(speed);

}

void Physics::toggleGrav(){
	usesGravity = !usesGravity;
}

void Physics::toggleGrav(int state){
	usesGravity = state;
}

void Physics::onPhysicsCollision(Sprite* sprite){
	isCollision = true;
	that = sprite;

	std::cout << "Collision\n";

	Vector2 thisDot(object->getPosition());
	Vector2 thatDot(that->getPosition());
	thatDot += Vector2(that->getSize());
	if(thatDot.getX() >= thisDot.getX()-2 && thatDot.getX() <= thisDot.getX()+2){ //Left
		mySide[left] = true;
	}
	if(thatDot.getY() >= thisDot.getY()-2 && thatDot.getY() <= thisDot.getY()+2){ //Up
		mySide[up] = true;
	}
	thisDot += Vector2(object->getSize());
	thatDot -= Vector2(that->getSize());
	if(thatDot.getX() >= thisDot.getX()-2 && thatDot.getX() <= thisDot.getX()+2){ //Right
		mySide[right] = true;
	}
	if(thatDot.getY() >= thisDot.getY()-2 && thatDot.getY() <= thisDot.getY()+2){ //Down
		mySide[down] = true;
	}
}

void Physics::addForce(Vector2 force){
	curF += force;
}

void Physics::addConstForce(Vector2 force){
	constF += force;
}

void Physics::setAntiF(double d){
	antiF = d;
}