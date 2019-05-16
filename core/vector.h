#ifndef VECTOR_H_
#define VECTOR_H_

#include <SDL2/SDL.h>
#include <cmath>

namespace MSDL {

	struct Rect {
		double x;
		double y;

		double width;
		double height;

		Rect(const SDL_Rect& rect):x(rect.x),y(rect.y),width(rect.w),height(rect.h) {}
		Rect(double _x=0,double _y=0,double _width=0,double _height=0):x(_x),y(_y),width(_width),height(_height) {}

		void setSDLRect(const SDL_Rect& rect) {
			x = rect.x;
			y = rect.y;
			width = rect.w;
			height = rect.h;
		}
		
		double distanceTo(Rect rect) { return sqrt(pow(x-rect.x,2) + pow(y-rect.y,2) ); }

		SDL_Rect toSDLRect() const { return (SDL_Rect){int(round(x)),int(round(y)),int(round(width)),int(round(height)) }; }
		SDL_Point toSDLPoint() const { return (SDL_Point){int(round(x)),int(round(y))}; }
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
		Vector2(const Vector2 &vector);

		Vector2& operator=(const Vector2 &vec);

		Vector2 operator+(const Vector2 &rightOp) const;
		Vector2& operator+=(const Vector2 &rightOp);

		Vector2 operator-(const Vector2 &rightOp) const;
		Vector2 operator-() const;
		Vector2& operator-=(const Vector2 &rightOp);
		Vector2& operator*=(const double value);
		Vector2 operator/(double value) const;

		Vector2 operator*(double value) const;

		friend Vector2 operator*(double value,const Vector2 &rightOp);

		int operator==(const Vector2 rightOp);

		//Возвращает позицию в прямоугольной системе
		Rect getPosition() const { return (Rect){posX,posY};}

		double getX() const { return posX; }
		double getY() const { return posY; }

		void setX(double x) { posX = x; }
		void setY(double y) { posY = y; }

		void setPosition(double x,double y) {
			posX = x;
			posY = y;

			calcToPolar();
		}

		void setPosition(Rect position) { 
			setPosition(position.x,position.y);
		}

		double len();

		double sqrlen();

		Vector2 normal();

		static Vector2 up();

		static Vector2 right();

		//Возвращает позицию в полярной системе
		Polar getPolarPosition() const { return (Polar){ang,dist};}

		bool isNull() { return !(posX || posY); }
	};
}
#endif