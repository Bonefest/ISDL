#ifndef SPRITE_H_
#define SPRITE_H_

#include <SDL2/SDL.h>
#include <map>
#include <list>
#include <string>

#include "vector.h"
#include "animation.h"
#include "media_manager.h"

//Реализует логику базового графического объекта 
class Sprite {
private:
	SDL_Texture* texture;	//Главное отображение спрайта - если у спрайта нет анимаций вовсе или нет активной анимации,то эта текстура является его отображением.

	Image image;			//Главное отображение спрайта - если у спрайта нет анимаций вовсе или нет активной анимации, то это изображение является его отображением.В отличии от текстуры,является более предпочтительным из-за того,что может быть частью текстуры,а не одним целым и как следствие,можно легко менять,по сути одну и ту же текстуру.Если image не задан,а texture - задано (не указывает на nullptr),то отображается вся текстура

	Rect absolutePosition;	//Абсолютная позиция в мире
	Rect relativePosition;	//Позиция на экране (Warning: позиция обновляется при отрисовке --> при наличие двух активных камер,позиция будет оставаться за последней)
	Rect size;
	SDL_Point anchor;	//Точка вращения

	float angle;		//Угол поворота

	bool pinned;		//Состояние прикреплен ли спрайт к экрану (перемещается ли он относительно игровой камеры) (??? должна ли быть возможность быть прикрепленным относительно одного и неприкрепленным относительно другого?)

	bool alreadyHovered;	//Состояние,наведена ли уже мышка на объект (нужно для того,чтобы не вызывать повторно событие наведения)

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
	Sprite(Image image,Rect absolutePosition,Rect size,double angle = 0.0,SDL_RendererFlip type=SDL_FLIP_NONE,bool pinned = false);
	virtual ~Sprite() {}

	Rect getAbsolutePosition() const { return absolutePosition; }

	Rect getSize() const { return size;}

	void setTexture(SDL_Texture* texture);	//Меняет текстуру
	void setImage(Image image);				//Меняет изображение

	void setPosition(double x,double y);
	void setPosition(Rect position);
	
	void addPosition(double x,double y);	//Добавляет к текущей позиции заданые координаты

	void setSize(double w,double h);
	void setSize(Rect size);

	void setAngle(float angle);
	float getAngle();

	void setFlip(SDL_RendererFlip type) { flip = type; }

	//int addChild(Sprite* child);	//int - это id нового объекта
	//void addChild(Sprite* child,std::string name);
	void addChild(Sprite* child);
	void setParent(Sprte* parent);
	void removeParent();

	//Рисует изображение относительно камеры.Рисует кадр анимации,если она есть,иначе рисует просто текстуру
	virtual void draw(SDL_Renderer* renderer,Rect cameraPosition,double cameraAngle);
	virtual void update(double delta);

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

	virtual void onClick(SDL_MouseButtonEvent event) {}			//Событие при нажатии мыши
	virtual void onRelease() {}									//Событие при отпускании мыши
	virtual void onHover() { alreadyHovered = true;}			//Событие при наведении мыши
	virtual void onUnhover() { }

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
	std::string lastText;		//Последнее сообщение.Если оно и цве

public:
	Label(TTF_Font* font=nullptr);
	Label(TTF_Font* font,const std::string& text, Rect position , SDL_Color textColor );

	void setText(const std::string& text);
	void setText(const std::string& text,Rect position,SDL_Color textColor);	//Не рисует в прямом смысле,а просто создает текстуру из заданного текста и цвета

	virtual void onHover() { setText("hovered"); }
	virtual void onUnhover() { setText("unhovered"); }

	void setColor(SDL_Color textColor);

	void setFont(TTF_Font* font);
};

class Button: public Sprite {
private:
	Label label;	//Текст,который содержит кнопка
	Image pressedImage;
	Image hoveredImage;

	//short borderSize;
	//SDL_Color borderColor;


public:
	void setClickedImage(Image image);
	void setHoveredImage(Image image);
};

#endif