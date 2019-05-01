#ifndef STATE_MANAGER_H_
#define STATE_MANAGER_H_

#include <list>
#include "sprite.h"


namespace MSDL {

	typedef void(*v_callback)();

	//Абстрактный класс любого действия
	class Action {
	private:

		double duration;

		v_callback finishCallback;

	public:
		Action(double drtn,v_callback fnshCallback=nullptr):duration(drtn),finishCallback(fnshCallback) {}
		virtual ~Action() {}

		virtual void finish() { if(finishCallback!=nullptr) finishCallback(); }
		virtual void update(double delta)=0;
		virtual bool isFinished()=0;

		double getDuration() const { return duration; }
	};

	//Набор стандартных действий
	namespace StandardActions {

		//Действие перемещения в точку
		class MoveTo : public Action {
		private:
			Object* object;
			Rect speed;

			double curTime;

		public:
			MoveTo(Object* object, Rect position, double duration, v_callback finishCallback=nullptr );
			virtual ~MoveTo() {}

			void update(double delta);
			bool isFinished();
		};

		//Действие поворота
		class RotateTo : public Action {
		private:
			Sprite* sprite;
			double speed;

			double curTime;
		public:
			RotateTo(Sprite* object, double angle, double duration, v_callback finishCallback=nullptr );
			virtual ~RotateTo() {}

			void update(double delta);
			bool isFinished();
		};

		//Действие покраски в заданный цвет
		class PaintTo : public Action {
		private:
			Sprite* sprite;
			Color speedColor;

			double curTime;
		public:
			PaintTo(Sprite* sprite,Color color, double duration, v_callback finishCallback=nullptr );
			virtual ~PaintTo() {}

			void update(double delta);
			bool isFinished();
		};

		//Действие вызова функтора
		class Call : public Action {
		private:
			double curTime;
		public:
			Call(double duration,v_callback finishCallback=nullptr );
			virtual ~Call() {}

			void update(double delta);
			bool isFinished();
		};

	}

	class StateManager {
	private:
		std::list<Action*> actionList;

		StateManager() {}
		StateManager(const StateManager& ) {}
		StateManager& operator=(const StateManager& )=delete;
		~StateManager() {}
	public:
		static StateManager* getInstance();
		void close();

		//Обновляет список всех действий
		void update(double delta);

		//Создает действие перемещения объекта в точку
		void moveTo(Object* object, Rect position, double duration, v_callback finishCallback=nullptr );
		
		//Создает действие перемещения объекта относительно чего-то в точку
		void moveBy(Object* movable, Object* comp, Rect position , double duration, v_callback finishCallback=nullptr );

		//Создает действие поворота объекта
		void rotateTo(Sprite* object, double angle, double duration, v_callback finishCallback=nullptr );

		//Cоздает действие поворота объекта относительно чего-то
		void rotateBy(Sprite* rotateable, Sprite* comp, double angle , double duration, v_callback finishCallback=nullptr );

		//Создает действие покраски объекта в цвет
		void paintTo(Sprite* sprite, Color color, double time, v_callback finishCallback=nullptr );

		//Создает действие вызова функтора
		void call(double time,v_callback finishCallback=nullptr );

		//Добавляет заданное действие в список действий
		void addAction(Action* action);

	};

}

#endif
