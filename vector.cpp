#include <cmath>
#include "vector.h"


void Vector2::calcToPolar() {
	dist = sqrt(posX*posX + posY*posY);
	ang = (posX == 0)?0:atan(posY/posX);
}

void Vector2::calcToRect() {
	posX = cos(ang)*dist;
	posY = sin(ang)*dist;
}

Vector2::Vector2(double x,double y): posX(x),posY(y) {
	calcToPolar();
}

Vector2::Vector2(Rect rect):Vector2(rect.x,rect.y) {}

Vector2::Vector2(const Vector2& vector):posX(vector.posX),posY(vector.posY),ang(vector.ang),dist(vector.dist) { }


Vector2& Vector2::operator=(const Vector2& vec) {
	if(this == &vec) return *this;

	posX = vec.posX;
	posY = vec.posY;

	ang = vec.ang;
	dist = vec.dist;

	return *this;
}

Vector2& operator+=(Vector2& left,const Vector2& rightOp) {
	left.posX += rightOp.posX;
	left.posY += rightOp.posY;
	left.ang += rightOp.ang;
	left.dist += rightOp.dist;

	return left;
}


Vector2 Vector2::operator+(const Vector2& rightOp) const {
	return Vector2(posX + rightOp.posX,posY + rightOp.posY);
}

Vector2& operator-=(Vector2& left,const Vector2& rightOp) {
	left.posX -= rightOp.posX;
	left.posY -= rightOp.posY;
	left.ang -= rightOp.ang;
	left.dist -= rightOp.dist;

	return left;
}

Vector2 Vector2::operator-(const Vector2& rightOp) const {
	return Vector2(posX - rightOp.posX,posY - rightOp.posY);
}


Vector2 Vector2::operator*(double value) const {
	return Vector2(posX*value,posY*value);
}

Vector2 operator*(double value,const Vector2& rightOp) {
	return rightOp*value;
}
