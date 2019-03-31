#ifndef CAMERA_H_
#define CAMERA_H_

#include <SDL2/SDL.h>
#include "sprite.h"
#include "vector.h"

#define MAX_ANGLE 360

class Camera {
public:
	//Типы движения камеры : (Во всех случаях объект будет в центре камеры)
	//None - камера двигается только под влиянием соответствующих функций
	//Centered - камера всегда двигается за объектом.
	//Sleeper - камера "доганяет" объект,когда он достигает некой условной границы
	//Rectangle - камера двигается,если объект выходит за границы заданого прямоугольника
	enum Type {None,Centered,Sleeper,Rectangle};
private:
	Type type;		//Тип движения
	Sprite* object;	//Объект,за которым следит камера

	Rect position;
	Rect border;	//Граница,после которой активируется перемещение камеры (для 2 и 3 поведения)

	double angle; 	//Угол поворота камеры по часовой стрелке в градусах (DEPRICATED)

	bool active;	//Состояние камеры,которое определяет активна ли она
	bool fixed;		//Состояние камеры,которое определяет фиксирована ли её позиция.
					//меняется ли с течением времени

public:
	Camera(Type type=None,Sprite* object=nullptr,bool active=true);
	Camera(Type type,Sprite* object,bool active,Rect position,Rect border,double angle);

	void setPosition(double x,double y);
	void setPosition(Rect position);

	void update(float delta);	//Обновляет положение камеры в зависимости от её поведения

	void setObject(Sprite* _object) { object=_object;}

	void setType(Type _type) { type = _type;}

	Rect getPosition() const { return position; }

	void setActive(bool state) { active = state; }
	void setFixed(bool state) { fixed = state; }

	bool isActive() const { return active; }
	bool isFixed() const { return fixed; }


	//DEPRICATED
	void setAngle(double value);
	double getAngle() const { return angle; }
};

#endif