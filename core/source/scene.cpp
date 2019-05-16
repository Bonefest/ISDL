#include "../scene.h"
#include "../game.h"

#include <sstream>
#include <utility>


using namespace MSDL;

Scene::~Scene() {
	/*for(auto spriteIter = sceneSprites.begin();spriteIter!=sceneSprites.end();spriteIter++) {

	}*/
	/*for(unsigned int counter = sceneSprites.size()-1; counter >= 0; counter--){
		Sprite *spr = sceneSprites[counter];
		delete spr;
	}
	sceneSprites.clear();
	for(auto cameraIter=sceneCameras.begin();cameraIter!=sceneCameras.end();cameraIter++){
		Camera *cam = cameraIter->second;
		delete cam;
	}
	sceneCameras.clear();*/
}


void Scene::controller(SDL_Event* event) {
	for(auto uiIter = UILayer.begin();uiIter != UILayer.end();uiIter++)
		(*uiIter)->controller(event);

	for(auto spriteIter = sceneSprites.begin();spriteIter!=sceneSprites.end();spriteIter++) {
		(*spriteIter)->controller(event);
	}
}

void Scene::draw() {
	//Для каждой камеры рисуем все объекты
	for(auto cameraIter=sceneCameras.begin();cameraIter!=sceneCameras.end();cameraIter++) {
		if(cameraIter->second->isActive()) {
			if(cameraIter->second->isActiveViewport()) {
				SDL_Rect viewportRect = cameraIter->second->getViewport().toSDLRect();
				SDL_RenderSetViewport(Game::getInstance()->getRenderer(),&viewportRect);
			} else SDL_RenderSetViewport(Game::getInstance()->getRenderer(),NULL);

			for(auto spriteIter = sceneSprites.begin();spriteIter!=sceneSprites.end();spriteIter++)
				(*spriteIter)->draw(Game::getInstance()->getRenderer(),cameraIter->second->getPosition(),cameraIter->second->getAngle());

		}
	}

	//Рисуем объекты пользовательского интерфейса
	for(auto uiIter = UILayer.begin();uiIter != UILayer.end();uiIter++) {
		SDL_RenderSetViewport(Game::getInstance()->getRenderer(),NULL);
		(*uiIter)->draw(Game::getInstance()->getRenderer(),(Rect){0,0},0);
	}
}

void Scene::update(double delta) {
	collisionManager.calculate();	//Рассчитывает коллизии

	for(auto uiIter = UILayer.begin();uiIter != UILayer.end();uiIter++)
		(*uiIter)->update(delta);

	for(auto cameraIter=sceneCameras.begin();cameraIter!=sceneCameras.end();cameraIter++) 
		cameraIter->second->update(delta);

	for(auto spriteIter = sceneSprites.begin();spriteIter!=sceneSprites.end();spriteIter++)
		(*spriteIter)->update(delta);
}

//Добавляет спрайт на сцену(фактически в очередь)
void Scene::addSprite(Sprite* sprite,long collisionLevel) {
	sceneSprites.push_back(sprite);
	collisionManager.addSprite(sprite,collisionLevel);
}

void Scene::removeSprite(Sprite* sprite,long collisionLevel) {
	sceneSprites.remove(sprite);
	collisionManager.removeSprite(sprite,collisionLevel);
}

//Добавляет спрайт на слой пользовательского интерфейса
void Scene::addUIElement(Sprite* sprite) {
	UILayer.push_back(sprite);
}


//Добавляет камеру в словарь,где ключ - это фактически её название.	
void Scene::addCamera(std::string name,Camera* camera) {
	sceneCameras.insert(std::pair<std::string,Camera*>(name,camera));
}


//Удаляет камеру из сцены
void Scene::removeCamera(std::string name){
	Camera *cam = sceneCameras[name];
	sceneCameras.erase(name);
	delete cam;
}


////////////////////////////////////

/*

TestScene::TestScene() {
	timer = 0.0f;
	SDL_Point scrSize = Game::getInstance()->getScreenSize();

	label = Label(Game::getMediaManager()->loadFont("default.ttf",8));
	label.setText("Test",Rect{100,100},SDL_Color{255,255,255});
	label.setPinned(true);


	Game::getMediaManager()->scanJsonFile("spritesheet.json");

	camera = new Camera();
	animatedSprite = new Sprite(Game::getMediaManager()->loadTexture("test.png"),(Rect){int(scrSize.x)/2,0},(Rect){64,64},0);
	subSpr = new Sprite(Game::getMediaManager()->getImage("RunRight04.png"),(Rect){int(scrSize.x)/2,10});

	animatedSprite->addChild(subSpr);

	addSprite(animatedSprite);
	addSprite(&label);


	
	animations = new Animation[4];

	animations[0] = Animation(Game::getMediaManager()->loadTexture("test.png"),64,64,0,0,20);
	animations[1] = Animation(Game::getMediaManager()->loadTexture("test.png"),64,64,0,64,20);
	animations[2] = Animation(Game::getMediaManager()->loadTexture("test.png"),64,64,0,128,20);
	animations[3] = Animation(Game::getMediaManager()->loadTexture("test.png"),64,64,0,192,20);

	animatedSprite->addAnimation("up",Game::getMediaManager()->getAnimation("up"));
	animatedSprite->addAnimation("left",Game::getMediaManager()->getAnimation("left"));
	animatedSprite->addAnimation("down",Game::getMediaManager()->getAnimation("down"));
	animatedSprite->addAnimation("right",Game::getMediaManager()->getAnimation("right"));

	animatedSprite->startAnimation("up");
	direction = 0;

	camera->setObject(animatedSprite);

	camera->setType(Camera::Centered);

	addCamera("test",camera);


}

TestScene::~TestScene() {
	delete animatedSprite;
	delete camera;
	delete subSpr;
	delete [] animations;
}

void TestScene::controller(SDL_Event* event) {
	Scene::controller(event);

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
			label.setText("left",label.getAbsolutePosition(),SDL_Color{rand()%255,rand()%255,rand()%255});
			animatedSprite->setImage(Game::getMediaManager()->getImage("left_staying"));
		}
		if(Game::getInstance()->isPressed(SDLK_d) && direction != 3) {
			animatedSprite->startAnimation("right");
			direction = 3;
			label.setText("right",label.getAbsolutePosition(),SDL_Color{rand()%255,rand()%255,rand()%255});
		}
		if(Game::getInstance()->isPressed(SDLK_s) && direction != 2) {
			animatedSprite->startAnimation("down");
			direction = 2;
			label.setText("down",label.getAbsolutePosition(),SDL_Color{rand()%255,rand()%255,rand()%255});
		}
		if(Game::getInstance()->isPressed(SDLK_w) && direction != 0) {
			animatedSprite->startAnimation("up");
			direction = 0;
			label.setText("up",label.getAbsolutePosition(),SDL_Color{rand()%255,rand()%255,rand()%255});

		}
	} else if(event->type == SDL_MOUSEBUTTONDOWN) {
		if(animatedSprite->isClicked()) {
			Rect s = animatedSprite->getSize();
			animatedSprite->setSize(s.x*2,s.y*2);
			Game::getInstance()->log("Clicked");
		}
		if(label.isClicked()) {
			Rect s = label.getSize();
			label.setSize(s.x*2,s.y*2);
		}
	}



	if(Game::getInstance()->isPressed(SDLK_a)) {
		Rect rect = animatedSprite->getAbsolutePosition();
		rect.x -= 1;
		animatedSprite->setPosition(rect);

	}
	if(Game::getInstance()->isPressed(SDLK_d)) {
		Rect rect = animatedSprite->getAbsolutePosition();
		rect.x += 1;
		animatedSprite->setPosition(rect);
	}
	if(Game::getInstance()->isPressed(SDLK_s)) {
		Rect rect = animatedSprite->getAbsolutePosition();
		rect.y += 1;
		animatedSprite->setPosition(rect);
	}
	if(Game::getInstance()->isPressed(SDLK_w)) {
		Rect rect = animatedSprite->getAbsolutePosition();
		rect.y -= 1;
		animatedSprite->setPosition(rect);

	}



}

void TestScene::update(double delta) {
	Scene::update(delta);
	timer += delta;

}

void TestScene::draw() {
	Scene::draw();


}

*/