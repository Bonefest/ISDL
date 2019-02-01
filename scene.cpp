//НИЖЕ ТЕСТ,МОЖНО УДАЛЯТЬ

#include "scene.h"
#include "game.h"

#include <sstream>
#include <utility>


Scene::~Scene() {}


void Scene::draw() {
	//Для каждой камеры рисуем все объекты
	for(auto cameraIter=sceneCameras.begin();cameraIter!=sceneCameras.end();cameraIter++) {
		if(cameraIter->second->isActive()) {
			for(unsigned int counter=0;counter!=sceneSprites.size();counter++) {
				sceneSprites[counter]->draw(Game::getInstance()->getRenderer(),cameraIter->second->getPosition(),cameraIter->second->getAngle());
					
			}

		}
	}

}

void Scene::update(double delta) {
	for(auto cameraIter=sceneCameras.begin();cameraIter!=sceneCameras.end();cameraIter++) 
		cameraIter->second->update(delta);

	for(unsigned int counter = 0;counter<sceneSprites.size();counter++)
		sceneSprites[counter]->update(delta);
}

//Добавляет спрайт на сцену(фактически в очередь)
void Scene::addSprite(Sprite* sprite) {
	sceneSprites.push_back(sprite);
}

//Добавляет камеру в словарь,где ключ - это фактически её название.	
void Scene::addCamera(std::string name,Camera* camera) {
	sceneCameras.insert(std::pair<std::string,Camera*>(name,camera));
}


//Удаляет камеру из сцены
void Scene::removeCamera(std::string name) {
	sceneCameras.erase(name);
}


////////////////////////////////////

TestScene::TestScene() {
	timer = 0.0f;
	SDL_Point scrSize = Game::getInstance()->getScreenSize();


	camera = new Camera();
	animatedSprite = new Sprite(Game::getTextureManager()->loadTexture("test.png"),(Rect){scrSize.x/2,0},(Rect){64,64},0);
	addSprite(animatedSprite);
	
	animations = new Animation[4];

	animations[0] = Animation(Game::getTextureManager()->loadTexture("test.png"),64,64,0,0,20);
	animations[1] = Animation(Game::getTextureManager()->loadTexture("test.png"),64,64,0,64,20);
	animations[2] = Animation(Game::getTextureManager()->loadTexture("test.png"),64,64,0,128,20);
	animations[3] = Animation(Game::getTextureManager()->loadTexture("test.png"),64,64,0,192,20);

	animatedSprite->addAnimation("up",animations);
	animatedSprite->addAnimation("left",animations+1);
	animatedSprite->addAnimation("down",animations+2);
	animatedSprite->addAnimation("right",animations+3);

	animatedSprite->startAnimation("up");
	direction = 0;

	camera->setObject(animatedSprite);

	camera->setType(Camera::Centered);

	addCamera("test",camera);


}

TestScene::~TestScene() {
	delete animatedSprite;
	delete camera;
	delete [] animations;
}

void TestScene::controller(SDL_Event* event) {
	if(event->type == SDL_KEYUP) {
		animatedSprite->stopAnimation();
		animatedSprite->resetAnimation();
		direction = -1;


		std::stringstream stream;
		stream << Game::getInstance()->getLastMouseClickPosition().x << " " << Game::getInstance()->getLastMouseClickPosition().y;
		Game::getInstance()->log(stream.str());
	}

	if(event->type == SDL_KEYDOWN) {

		if(Game::getInstance()->isPressed(SDLK_a) && direction != 1) {
			animatedSprite->startAnimation("left");
			direction = 1;

		}
		if(Game::getInstance()->isPressed(SDLK_d) && direction != 3) {
			animatedSprite->startAnimation("right");
			direction = 3;
		}
		if(Game::getInstance()->isPressed(SDLK_s) && direction != 2) {
			animatedSprite->startAnimation("down");
			direction = 2;
		}
		if(Game::getInstance()->isPressed(SDLK_w) && direction != 0) {
			animatedSprite->startAnimation("up");
			direction = 0;

		}
	}

	if(Game::getInstance()->isPressed(SDLK_a)) {
		Rect rect = animatedSprite->getPosition();
		rect.x -= 1;
		animatedSprite->setPosition(rect);

	}
	if(Game::getInstance()->isPressed(SDLK_d)) {
		Rect rect = animatedSprite->getPosition();
		rect.x += 1;
		animatedSprite->setPosition(rect);
	}
	if(Game::getInstance()->isPressed(SDLK_s)) {
		Rect rect = animatedSprite->getPosition();
		rect.y += 1;
		animatedSprite->setPosition(rect);
	}
	if(Game::getInstance()->isPressed(SDLK_w)) {
		Rect rect = animatedSprite->getPosition();
		rect.y -= 1;
		animatedSprite->setPosition(rect);

	}

	if(animatedSprite->isClicked()) {
		animatedSprite->setSize(150,150);
		Game::getInstance()->log("Clicked");
	}
}

void TestScene::update(double delta) {
	Scene::update(delta);
	timer += delta;

}

void TestScene::draw() {
	Scene::draw();

}