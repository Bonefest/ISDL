#ifndef SPRITE_H_
#define SPRITE_H_

#include <SDL2/SDL.h>
#include <map>
#include <list>
#include <string>

#include "vector.h"
#include "animation.h"
#include "media_manager.h"

//Структура для определения цвета (Для более точного и плавного цвета в отличии от SDL_Color)
struct Color {

	double r;
	double g;
	double b;
	double a;

	Color(double red=0,double green=0,double blue=0,double alpha=0):r(red),g(green),b(blue),a(alpha) { setSafeColor(); }
	Color(SDL_Color color):r(color.r),g(color.g),b(color.b),a(color.a) { setSafeColor(); }

	SDL_Color getSDLColor() const {
		return (SDL_Color){Uint8(round(r)),Uint8(round(g)),Uint8(round(b)),Uint8(round(a))};
	}

	Color& operator=(const Color& color) {
		r = color.r;
		g = color.g;
		b = color.b;
		a = color.a;

		setSafeColor();

		return *this;
	}

	Color& operator=(const SDL_Color& color) {
		r = color.r;
		g = color.g;
		b = color.b;
		a = color.a;

		setSafeColor();

		return *this;
	}

private:
	void setSafeColor() {
		if(r > 255) r = 255;
		else if(r < 0) r = 0;

		if(g > 255) g = 255;
		else if(g < 0) g = 0;

		if(b > 255) b = 255;
		else if(b < 0) b = 0;

		if(a > 255) a = 255;
		else if(a < 0) a = 0;

	}
};

class Object {
private:
	Rect position;

public:
	Object(Rect pos):position(pos) {}

	virtual void update(double delta)=0;

	virtual void setPosition(double x,double y) { position.x = x; position.y = y; }
	virtual void setPosition(Rect position) { this->position = position; }

	virtual void addPosition(double x,double y) { position.x += x; position.y += y; }
	virtual void addPosition(Rect position) { this->position.x += position.x; this->position.y += position.y; }

	virtual Rect getPosition() const { return position; }
};

//Реализует логику базового графического объекта 
class Sprite : public Object{
private:
	SDL_Texture* texture;	//Главное отображение спрайта - если у спрайта нет анимаций вовсе или нет активной анимации,то эта текстура является его отображением.

	Image image;			//Главное отображение спрайта - если у спрайта нет анимаций вовсе или нет активной анимации, то это изображение является его отображением.В отличии от текстуры,является более предпочтительным из-за того,что может быть частью текстуры,а не одним целым и как следствие,можно легко менять,по сути одну и ту же текстуру.Если image не задан,а texture - задано (не указывает на nullptr),то отображается вся текстура

	Color spriteColor;		//Цвет графического объекта

	Rect relativePosition;	//Позиция на экране (Warning: позиция обновляется при отрисовке --> при наличие двух активных камер,позиция будет оставаться за последней)
	Rect size;
	SDL_Point anchor;	//Точка вращения

	float angle;		//Угол поворота

	bool pinned;		//Состояние прикреплен ли спрайт к экрану (перемещается ли он относительно игровой камеры) (??? должна ли быть возможность быть прикрепленным относительно одного и неприкрепленным относительно другого?)

	bool alreadyHovered;	//Состояние,наведена ли уже мышка на объект (нужно для того,чтобы не вызывать повторно событие наведения)
	bool pressed;			//Состояние, был ли объект нажат (необходимо для определения события отпускания)
	bool dragged;			//Состояние, перетаскивается ли объект в данный момент

	SDL_RendererFlip flip;	//Хранит отражение,которое используется при отрисовке

	std::map<std::string, Animation> animations;	//Словарь анимаций,ключ - это имя анимации (idle,moving,etc)
	Animation* currentAnimation;

	bool animationStopped;	//Остановлено обновление анимации

	Sprite* parent;								//Родительский объект
	std::list<Sprite*> children;				//Дочерние объекты, необходимы для относительного позиционирования. (Башня танка позицинируется относительно его тела)

	SDL_Rect getAbsoluteDestination() const;	//Возвращает абсолютную позицию,переведенную из Rect в SDL_Rect (с округлением к int)
	SDL_Rect getRelativeDestination() const;	//Возвращает позицию на экране
public:
	Sprite();
	Sprite(SDL_Texture* texture,Rect absolutePosition,Rect size,double angle = 0.0,SDL_RendererFlip type=SDL_FLIP_NONE,bool pinned = false);
	Sprite(Image image,Rect absolutePosition,double angle = 0.0,SDL_RendererFlip type=SDL_FLIP_NONE,bool pinned = false);
	virtual ~Sprite() {}

	Rect getSize() const { return size;}

	void setTexture(SDL_Texture* texture);	//Меняет текстуру
	void setImage(Image image);				//Меняет изображение

	void setColor(SDL_Color color) { spriteColor = color; }
	void setColor(Color color) { spriteColor = color; }
	Color getColor() const { return spriteColor; }

	void setPosition(double x,double y);
	void setPosition(Rect position);
	
	void addPosition(double x,double y);	//Добавляет к текущей позиции заданые координаты

	void setSize(double w,double h);
	void setSize(Rect size);

	void setAngle(float angle) { this->angle = angle; }
	void addAngle(float angle) { this->angle += angle; }
	float getAngle() { return angle; }

	void setFlip(SDL_RendererFlip type) { flip = type; }

	//int addChild(Sprite* child);	//int - это id нового объекта
	//void addChild(Sprite* child,std::string name);
	void addChild(Sprite* child);
	void setParent(Sprite* parent);
	void removeParent();

	//Рисует изображение относительно камеры.Рисует кадр анимации,если она есть,иначе рисует просто текстуру
	virtual void draw(SDL_Renderer* renderer,Rect cameraPosition,double cameraAngle);
	virtual void update(double delta);
	virtual void controller(SDL_Event* event);

	void addAnimation(std::string key,Animation animation);		//Добавляет новую анимацию в словарь
	void startAnimation(std::string key);						//Запускает анимацию
	void stopAnimation();										//Останавливает обновление анимации
	void resetAnimation();										//Возвращает анимацию в начальное состояние
	Animation getAnimation(std::string key);					//возвращает адрес заданой анимации
	void removeAnimation(std::string key);						//Удаляет заданую анимацию

	bool isClicked() const; 									//Возвращает нажат ли заданный спрайт
	bool isHovered() const;										//Возвращает выбран ли заданый спрайт (наведена ли на него мышка)

	bool isAlreadyHovered() const { return alreadyHovered; }

	bool isAnimate() const;										//Возвращает активна ли в данный момент анимация

	virtual void onClick(SDL_MouseButtonEvent event) { }		//Событие при нажатии мыши
	virtual void onRelease(SDL_MouseButtonEvent event) {}		//Событие при отпускании мыши
	virtual void onHover(SDL_MouseMotionEvent event) { }		//Событие при наведении мыши
	virtual void onUnhover(SDL_MouseMotionEvent event) { }
	virtual void onDrag(SDL_MouseMotionEvent event) { }         //Событие при перетягивании объекта
	virtual void onDrop(SDL_MouseButtonEvent event) { }			//Событие при завершении перетягивания
	virtual void onCollision(Sprite* object) {	}				//Событие при столкновении

	bool isPinned() const;										//Прикреплен ли к экрану
	void setPinned(bool value);

};


//ГРАФ.ИНТЕРФЕЙС(Позже нужно перенести в отдельный файлы)
#include <SDL2/SDL_ttf.h>

class Label : public Sprite {
private:
	SDL_Texture* currentTexture;	//Текущая текстура отображаемого текста.Необходимо хранить,адрес,чтобы можно было его освободить в случае изменения текста.

	TTF_Font* font;
	SDL_Color lastColor;
	std::string currentText;		//Последнее сообщение.Если оно и цве

public:
	Label(TTF_Font* font=nullptr);
	Label(TTF_Font* font,const std::string& text, Rect position , SDL_Color textColor );

	void setText(const std::string& text);
	void setText(const std::string& text,Rect position,SDL_Color textColor);	//Не рисует в прямом смысле,а просто создает текстуру из заданного текста и цвета
/*
	virtual void onHover(SDL_MouseMotionEvent event) { setText("hovered"); }
	virtual void onUnhover(SDL_MouseMotionEvent event) { setText("unhovered"); }*/


	void setColor(SDL_Color textColor);
	SDL_Color getColor() const;

	void setFont(TTF_Font* font);

	std::string getCurrentText() { return currentText; }
};

class Button: public Sprite {
private:
	Label label;	//Текст,который содержит кнопка
	Image image;		//Стандартное изображение
	Image pressedImage;
	Image hoveredImage;

	//short borderSize;
	//SDL_Color borderColor;

public:

	Button(Rect position,Rect size,Image image,SDL_Color textColor,std::string text="",TTF_Font* font=nullptr);

	Button(Rect position,Rect size,Image image,const Label& label);

	void setClickedImage(Image image);
	void setHoveredImage(Image image);

	void setFont(TTF_Font* font);
	std::string getCurrentText();

	void setColor(SDL_Color color);
	SDL_Color getColor();

};

class Scrollbar : public Sprite {

};

#endif