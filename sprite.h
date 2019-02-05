#ifndef SPRITE_H_
#define SPRITE_H_

#include <SDL2/SDL.h>
#include <map>
#include <string>


#include "vector.h"
#include "animation.h"

//Реализует логику базового графического объекта 
class Sprite {
private:
	SDL_Texture* texture;
	Rect absolutePosition;	//Абсолютная позиция в мире
	Rect relativePosition;	//Позиция на экране (Warning: позиция обновляется при отрисовке --> при наличие двух активных камер,позиция будет оставаться за последней)
	Rect size;
	SDL_Point anchor;	//Точка вращения

	float angle;		//Угол поворота

	bool pinned;		//Состояние прикреплен ли спрайт к экрану (перемещается ли он относительно игровой камеры) (??? должна ли быть возможность быть прикрепленным относительно одного и неприкрепленным относительно другого?)

	SDL_RendererFlip flip;	//Хранит отражение,которое используется при отрисовке

	std::map<std::string, Animation*> animations;	//Словарь анимаций,ключ - это имя анимации (idle,moving,etc)
	Animation* currentAnimation;

	bool animationStopped;	//Остановлено обновление анимации

	SDL_Rect getAbsoluteDestination() const;	//Возвращает абсолютную позицию,переведенную из Rect в SDL_Rect (с округлением к int)
	SDL_Rect getRelativeDestination() const;	//Возвращает позицию на экране
public:
	Sprite();
	Sprite(SDL_Texture* texture,Rect absolutePosition,Rect size,double angle = 0.0,SDL_RendererFlip type=SDL_FLIP_NONE,bool pinned = false);
	virtual ~Sprite() {}

	Rect getAbsolutePosition() const { return absolutePosition; }

	Rect getSize() const { return size;}

	void setPosition(double x,double y);
	void setPosition(Rect position);
	
	void setSize(double w,double h);
	void setSize(Rect size);

	void setAngle(float angle);
	float getAngle();

	void setFlip(SDL_RendererFlip type) { flip = type; }

	//Рисует изображение относительно камеры.Рисует кадр анимации,если она есть,иначе рисует просто текстуру
	virtual void draw(SDL_Renderer* renderer,Rect cameraPosition,double cameraAngle);
	virtual void update(double delta);

	void addAnimation(std::string key,Animation* animation);	//Добавляет новую анимацию в словарь
	void startAnimation(std::string key);						//Запускает анимацию
	void stopAnimation();										//Останавливает обновление анимации
	void resetAnimation();										//Возвращает анимацию в начальное состояние
	Animation* getAnimation(std::string key);					//возвращает адрес заданой анимации
	void removeAnimation(std::string key);						//Удаляет заданую анимацию

	bool isClicked() const;										//Возвращает нажат ли заданный спрайт
	bool isSelected() const;									//Возвращает выбран ли заданый спрайт

	bool isAnimate() const;										//Возвращает активна ли в данный момент
																//анимация
};

#endif