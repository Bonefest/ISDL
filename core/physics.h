#pragma once

#include "sprite.h"

class Physics{
	Sprite *object;
	bool usesGravity;

	Vector2 speed;
	Vector2 gravityF;
	double antiF;
	Vector2 resF;
	
	double mass;

public:
	Physics(Sprite &obj, double m = 10.0, bool gravity = true);

	void update();
};