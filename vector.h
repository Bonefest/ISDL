#ifndef VECTOR_H_
#define VECTOR_H_

struct Rect {
	double x;
	double y;

	double width;
	double height;
};

struct Polar {
	double angle;
	double distance;
};

//Представляет вектор в двумерном пространстве
class Vector2 {
private:
	double posX;
	double posY;

	double ang;
	double dist;

	//Переводит координаты с прямоугольной системы координат в полярную
	void calcToPolar();

	//Переводит координаты с полярной системы координат в прямоугольную
	void calcToRect();
public:
	Vector2(double x=0.0,double y=0.0);
	Vector2(Rect position);
	Vector2(const Vector2& vector);

	Vector2& operator=(const Vector2& vec);

	Vector2 operator+(const Vector2& rightOp) const;
	friend Vector2& operator+=(Vector2& left,const Vector2& rightOp);

	Vector2 operator-(const Vector2& rightOp) const;
	friend Vector2& operator-=(Vector2& left,const Vector2& rightOp);

	Vector2 operator*(double value) const;

	friend Vector2 operator*(double value,const Vector2& rightOp);

	//Возвращает позицию в прямоугольной системе
	Rect getPosition() const { return (Rect){posX,posY};}

	double getX() const { return posX; }
	double getY() const { return posY; }

	void setPosition(double x,double y) {
		posX = x;
		posY = y;

		calcToPolar();
	}

	void setPosition(Rect position) { 
		setPosition(position.x,position.y);
	}

	//Возвращает позицию в полярной системе
	Polar getPolarPosition() const { return (Polar){ang,dist};}
};


#endif