#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <SDL2/SDL.h>

namespace MSDL {

	//Класс представляет анимацию,хранит необходимую информацию касательно её отображения
	//и отображает саму текстуру.
	class Animation {
	private:
		SDL_Texture* texture;
		
		short begFrameX;	//Позиция начального кадра	
		short begFrameY;

		short endFrameX;	//Позиция конечного кадра
		short endFrameY;

		short curFrameX;	//Позиция текущего кадра		
		short curFrameY;

		short frameWidth;	//Ширина кадра
		short frameHeight;	//Высота кадра
		float frameSpeed;	//Скорость смены кадра (в кадрах/c).0 - никогда не менять автоматически
		
		float curTime;		//Время,которое прошло от смены прошлого кадра

		bool loop;			//Повторяется ли анимация
		bool finished;		//Завершена ли анимация

		bool complicated;	//Усложнённая анимация - при достижении конца ряда - переходит на следующий,
							//либо на начало

		int textureWidth;
		int textureHeight;

	public:
		Animation(SDL_Texture* texture=nullptr,short width=0,short height=0,short begX=0,short begY=0,float speed=0.0f,bool loop=true,bool complicated=false);

		//Изменяет скорость смены кадров анимации
		void setSpeed(float speed);

		//Переходит на следующий кадр
		void nextFrame();

		//Обнуляет анимацию,переводя её состояние в начальное
		void reset();

		void update(float delta);	//Обновляет анимацию
		SDL_Rect getFrameSource();	//Возвращает расположение кадра на текстуре 
	};

}

#endif