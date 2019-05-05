#ifndef CAMERA_H_
#define CAMERA_H_

#include <SDL2/SDL.h>
#include "sprite.h"
#include "vector.h"

#define MAX_ANGLE 360

namespace MSDL {

	class Camera: public Object {
	private:
		Sprite* object;		//Объект,за которым следит камера

		//Rect border;		//Граница,после которой активируется перемещение камеры (для 2 и 3 поведения)
		Rect viewport;		//Прямоугольник в который камера будет рисовать изображение

		double angle; 		//Угол поворота камеры по часовой стрелке в градусах (DEPRICATED)

		bool active;		//Состояние камеры,которое определяет активна ли она
		bool fixed;			//Состояние камеры,которое определяет фиксирована ли её позиция.
							//меняется ли с течением времени
		bool activeViewport;//Состояние камеры,которое определяет есть ли вьюпорт у камеры
	public:
		Camera(Sprite* object=nullptr,bool active=true);
		Camera(Sprite* object,bool active,Rect position,Rect viewport);

		virtual ~Camera() {}

		virtual void update(double delta);	//Обновляет положение камеры в зависимости от её поведения

		void setObject(Sprite* _object) { object=_object;}
		Sprite* getObject() { return object; }

		void setActive(bool state) { active = state; }
		void setFixed(bool state) { fixed = state; }

		bool isActive() const { return active; }
		bool isFixed() const { return fixed; }
		bool isActiveViewport() const { return activeViewport; }

		void setViewport(Rect viewport) { this->viewport = viewport;activeViewport=true; }
		void disableViewport() { activeViewport=false; }
		Rect getViewport() const { return viewport; }


		//DEPRICATED
		void setAngle(double value);
		double getAngle() const { return angle; }
	};

	namespace StandardCameras {

		//Камера, которая центрирует объект за которым наблюдает
		class CameraCenter : public Camera {
		public:
			//Наследуем конструкторы
			using Camera::Camera;

			virtual ~CameraCenter() {}

			virtual void update(double delta);

		};

		//Камера, которая доганяет объект,если он выходит за некоторую границу
		class CameraSleeper : public Camera {
		private:
			//Максимальное расстояние,после которого камера доганяет объект
			double maximalDistance;

			//Время за которое камера доганяет объект
			double runningTime;

			//Время,которое прошло с последней погони за объектом
			double currentRunningTime;

			//Определяет состояние доганяет ли камера объект
			bool running;
		public:

			CameraSleeper(double maximalDistance,double runningTime,Sprite* object=nullptr,bool active=true);
			CameraSleeper(double maximalDistance,double runningTime,Sprite* object,bool active,Rect position,Rect viewport);

			virtual ~CameraSleeper() {}

			virtual void update(double delta);

			void setMaximalDistance(double distance) { maximalDistance = distance; }
			void setRuningTime(double time) { runningTime = time; }

			bool isRunning() const { return running; }
		};

		//Камера, которая двигается за объектом только если он выходит за границу заданного прямоугольника
		class CameraRectangle : public Camera {
		private:
			Rect border;
		public:
			virtual ~CameraRectangle() {}

			CameraRectangle(Rect border,Sprite* object=nullptr,bool active=true);
			CameraRectangle(Rect border,Sprite* object,bool active,Rect position,Rect viewport);
		
			virtual void update(double delta);

			Rect getBorder() const { return border; }
			void setBorder(Rect border) { this->border = border; }
		};
	}
}

#endif