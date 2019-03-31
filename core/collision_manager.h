#ifndef COLLISION_MANAGER_H_
#define COLLISION_MANAGER_H_

#include <map>
#include <list>
#include "sprite.h"

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