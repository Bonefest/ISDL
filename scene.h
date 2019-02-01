#ifndef SCENE_H_
#define SCENE_H_

#include <SDL2/SDL.h>
#include <vector>
#include <map>
#include <string>

#include "camera.h"

class Scene {
private:
	std::vector<Sprite*> 			sceneSprites;	//Объекты,которые находяться на сцене
	std::map<std::string,Camera*> 	sceneCameras;	//Камеры,которые находяться на сцене.Хранится
													//в виде Имя Камеры - Значение
public:
	virtual ~Scene()=0;	//НАДО РЕАЛИЗОВАТЬ УДАЛЕНИЕ

	//Обрабатывает события
	virtual void controller(SDL_Event* event)=0;

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



#include "sprite.h"
//НИЖЕ ТЕСТ,МОЖНО УДАЛЯТЬ
class TestScene : public Scene {
private:
	int direction;
	Sprite* animatedSprite;
	Animation* animations;
	float timer;

	Camera* camera;
public:
	TestScene();
	~TestScene();

	void controller(SDL_Event* event);
	void update(double delta);
	void draw();
};

#endif