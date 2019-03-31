#ifndef SCENE_H_
#define SCENE_H_

#include <SDL2/SDL.h>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <SDL2/SDL_ttf.h>

#include "camera.h"
#include "sprite.h"

class Scene {
private:
	std::vector<Sprite*> 			sceneSprites;	//Объекты,которые находяться на сцене
	std::map<std::string,Camera*> 	sceneCameras;	//Камеры,которые находяться на сцене.Хранится
													//в виде Имя Камеры - Значение
	

public:
	virtual ~Scene();	//НАДО РЕАЛИЗОВАТЬ УДАЛЕНИЕ

	//Обрабатывает события
	virtual void controller(SDL_Event* event);

	//Обновляет игровые данные
	virtual void update(double delta);

	//Рисует на сцене игровые данные
	virtual void draw();

	//Добавляет спрайт на сцену
	void addSprite(Sprite* sprite);

	//Добавляет камеру в словарь
	void addCamera(std::string name,Camera* camera);

	//Удаляет камеру из словаря
	void removeCamera(std::string name);

};



/*
//НИЖЕ ТЕСТ,МОЖНО УДАЛЯТЬ
class TestScene : public Scene {
private:
	int direction;
	Sprite* animatedSprite;
	Sprite* subSpr;
	Label label;
	Animation animations[4];
	float timer;

	Camera* camera;

public:
	TestScene();
	~TestScene();

	void controller(SDL_Event* event);
	void update(double delta);
	void draw();
};
*/

/////////////////////////////////////////////
class CollisionManager {
public:
	enum {DEFAULT_LEVEL_COLLISION};
private:
	std::map< long,std::list<Sprite*> >  sprites;	//Объекты, которые хранятся на сцене (Ключ - уровень коллизии, значение - список объектов на этом уровне)
public:
	void addSprite(Sprite* sprite,long level=DEFAULT_LEVEL_COLLISION);
	void removeSprite(Sprite* sprite);
	virtual void calculate();					//Рассчитать столкновение всех объектов
	Sprite* raycast(Rect pos,double angle,long level=DEFAULT_LEVEL_COLLISION);	//Пускание луча : задается позиция,направление и уровень коллизии.Возвращает первый объект, с которым столкнулся луч.
	Sprite* touch(Rect pos,long level=DEFAULT_LEVEL_COLLISION);					//Ищет в заданной координате спрайт на заданном уровне коллизий.
};

#endif
