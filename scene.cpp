//НИЖЕ ТЕСТ,МОЖНО УДАЛЯТЬ

#include "scene.h"
#include "game.h"

#include <sstream>
#include <utility>


Scene::~Scene() {
	clickedSprites.clear();
	hoveredSprites.clear();
	for(unsigned int counter = sceneSprites.size()-1; counter >= 0; counter--){
		Sprite *spr = sceneSprites[counter];
		delete spr;
	}
	sceneSprites.clear();
	for(auto cameraIter=sceneCameras.begin();cameraIter!=sceneCameras.end();cameraIter++){
		Camera *cam = cameraIter->second;
		delete cam;
	}
	sceneCameras.clear();
}


void Scene::controller(SDL_Event* event) {
	if(event->type == SDL_MOUSEBUTTONDOWN) {
		for(auto spriteIter = sceneSprites.begin();spriteIter != sceneSprites.end();spriteIter++) {

			if((*spriteIter)->isClicked()) {
				(*spriteIter)->onClick(event->button);
				clickedSprites.push_back(*spriteIter);
			}
		}
	} else if(event->type == SDL_MOUSEBUTTONUP) {
		for(auto clickedSpriteIter = clickedSprites.begin();clickedSpriteIter != clickedSprites.end();) {
			
			(*clickedSpriteIter)->onRelease();
			clickedSpriteIter = clickedSprites.erase(clickedSpriteIter);

		}

	} else if(event->type == SDL_MOUSEMOTION) {
		for(auto spriteIter = sceneSprites.begin();spriteIter != sceneSprites.end();spriteIter++) {

			//Удаляем из списка все указатели на спрайты,на которые больше не наведена мышка
			for(auto hoveredSpriteIter = hoveredSprites.begin();hoveredSpriteIter != hoveredSprites.end(); ) {
				if( !(*hoveredSpriteIter)->isHovered()) {

					(*hoveredSpriteIter)->onUnhover();
					hoveredSpriteIter = hoveredSprites.erase(hoveredSpriteIter);
				} else hoveredSpriteIter++;
			}


			//Если на спрайт наведа мышка и это первичное событие
			if((*spriteIter)->isHovered() && !(*spriteIter)->isAlreadyHovered()) {
				(*spriteIter)->onHover();
				hoveredSprites.push_back(*spriteIter);
			}

			
		}
	}
	for(unsigned int counter = 0; counter < sceneSprites.size(); counter++){
		sceneSprites[counter]->controller(event);
	}
}

void Scene::draw() {
	//Для каждой камеры рисуем все объекты
	for(auto cameraIter=sceneCameras.begin();cameraIter!=sceneCameras.end();cameraIter++) {
		if(cameraIter->second->isActive()) {
			for(size_t counter=0;counter!=sceneSprites.size();counter++) {
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

	label = Label(Game::getMediaManager()->loadFont("slkscr.ttf",8));
	label.setText("Test",Rect{100,100},SDL_Color{255,255,255});
	label.setPinned(true);


	Game::getMediaManager()->scanJsonFile("spritesheet.json");

	camera = new Camera();
	animatedSprite = new Sprite(Game::getMediaManager()->loadTexture("test.png"),(Rect){int(scrSize.x)/2,0},(Rect){64,64},0);
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