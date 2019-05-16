#pragma once

#include "sprite.h"

namespace MSDL {
	class Sprite;

	class Physics{
		Sprite *object;
		bool usesGravity;

		Vector2 speed;
		Vector2 gravityF;
		double antiF;
		Vector2 antiFV;
		Vector2 curF;
		Vector2 constF;

		double sqrSpeedLimit;
		
		double mass;
		double bounce;

		//Collision info
		bool isCollision;
		bool mySide[4];
		Sprite *that;

	public:
		Physics(Sprite *obj, bool gravity = true, double m = 1.0, double b = 0.7);

		void update(double delta);

		void toggleGrav();

		void toggleGrav(int state);

		void onPhysicsCollision(Sprite *sprite);

		void addForce(Vector2 force);

		void addConstForce(Vector2 force);

		void setAntiF(double d);

		
	};
}