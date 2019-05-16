#include <cmath>
#include "../vector.h"

using namespace MSDL;
void Vector2::calcToPolar() {
	dist = sqrt(posX*posX + posY*posY);
	ang = (posX == 0)?0:atan(posY/posX);
}

void Vector2::calcToRect() {
	posX = cos(ang)*dist;
	posY = sin(ang)*dist;
}

Vector2::Vector2(double x,double y){
	posX = x;
	posY = y;
	calcToPolar();
}

Vector2::Vector2(Rect rect):Vector2(rect.x,rect.y) {}

Vector2::Vector2(const Vector2& vector){
	posX = vector.posX;
	posY = vector.posY;
	ang = vector.ang;
	dist = vector.dist;
}


Vector2& Vector2::operator=(const Vector2& vec) {
	if(this == &vec) return *this;

	posX = vec.posX;
	posY = vec.posY;

	ang = vec.ang;
	dist = vec.dist;

	return *this;
}

Vector2& Vector2::operator+=(const Vector2& rightOp) {
	posX += rightOp.posX;
	posY += rightOp.posY;
	ang += rightOp.ang;
	dist += rightOp.dist;

	return *this;
}


Vector2 Vector2::operator+(const Vector2& rightOp) const {
	return Vector2(posX + rightOp.posX,posY + rightOp.posY);
}

Vector2& Vector2::operator-=(const Vector2& rightOp) {
	posX -= rightOp.posX;
	posY -= rightOp.posY;
	ang -= rightOp.ang;
	dist -= rightOp.dist;

	return *this;
}

Vector2& Vector2::operator*=(const double value){
	posX *= value;
	posY *= value;

	return *this;
}

Vector2 Vector2::operator-(const Vector2& rightOp) const {
	return Vector2(posX - rightOp.posX,posY - rightOp.posY);
}

Vector2 Vector2::operator-() const{
	return Vector2(-posX, -posY);
}

Vector2 Vector2::operator*(double value) const {
	return Vector2(posX*value,posY*value);
}

namespace MSDL {

	Vector2 operator*(double value,const Vector2& rightOp) {
		return rightOp*value;
	}

}
Vector2 Vector2::operator/(double value) const{
	return Vector2(posX/value, posY/value);
}

int Vector2::operator==(const Vector2 rightOp){
	return (posX == rightOp.posX && posY == rightOp.posY);
}

double Vector2::len(){
	return sqrt(this->sqrlen());
}

double Vector2::sqrlen(){
	return posX * posX + posY * posY;
}

Vector2 Vector2::normal(){
	if(this->len() == 0)
		return Vector2();
	return Vector2(posX / this->len(), posY / this->len());
}

Vector2 Vector2::up(){
	return Vector2(0, -1);
}

Vector2 Vector2::right(){
	return Vector2(1, 0);
}