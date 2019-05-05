#include "../core/scene.h"
#include "../core/game.h"
#include "../core/state_manager.h"
#include "Block.h"

using namespace MSDL;
using namespace MSDL::StandardCameras;

class myScene : public Scene{
	Block *block;
	Sprite *sblock;
	Sprite *sblocks[10];
	Label* labelCenter;
	Label* labelSleeper;
	Label* labelRect;
	Label* labelNone;

	Camera *center;
	Camera *sleeper;
	CameraRectangle *rect;
	Camera *none;
public:
	myScene(){
		block = new Block(Game::getMediaManager()->loadTexture("tests/block.png"), (Rect){50, 50}, (Rect){32, 32});
		sblock = new Sprite(Game::getMediaManager()->loadTexture("tests/block.png"), (Rect){0, 100}, (Rect){32, 32});
		labelCenter = new Label(Game::getMediaManager()->loadFont("resources/default.ttf",36),"Label center",Rect(0,0),(SDL_Color){255,255,255});
		labelSleeper = new Label(Game::getMediaManager()->loadFont("resources/default.ttf",36),"Label sleeper",Rect(640,0),(SDL_Color){255,255,255});
		labelRect = new Label(Game::getMediaManager()->loadFont("resources/default.ttf",36),"Label rect",Rect(0,360),(SDL_Color){255,255,255});
		labelNone = new Label(Game::getMediaManager()->loadFont("resources/default.ttf",36),"Label none",Rect(640,360),(SDL_Color){255,255,255});

		for(int i = 0;i<10;++i) {
			sblocks[i] = new Sprite(Game::getMediaManager()->loadTexture("tests/block.png"),(Rect){i*50.0,200.0},Rect(8*(i+1),8*(i+1)));
			sblocks[i]->setColor(i*25.0,i*25.0,i*25.0,255.0);
			addSprite(sblocks[i]);
		}


		center = new CameraCenter();
		center->setViewport((Rect){0,0,640,360});	
		center->setObject(block);
		center->setPosition((Rect){0,0});

		sleeper = new CameraSleeper(100,1000);
		sleeper->setViewport((Rect){640,0,640,360});
		sleeper->setObject(block);

		rect = new CameraRectangle(Rect(100,100));
		rect->setObject(block);
		rect->setViewport((Rect){0,360,640,360});

		none = new Camera();
		none->setViewport((Rect){640,360,640,360});

		addSprite(block);
		addSprite(sblock);
		addUIElement(labelCenter);
		addUIElement(labelRect);
		addUIElement(labelSleeper);
		addUIElement(labelNone);


		addCamera("camera", center);
		addCamera("camer2", sleeper);
		addCamera("cam", rect);
		addCamera("af",none);
	}

	void draw() {
		Scene::draw();
		SDL_Renderer* render = Game::getInstance()->getRenderer();
		SDL_SetRenderDrawColor(render,255,0,0,255);
		Rect borderPosition = rect->getPosition();
		Rect border = rect->getBorder();
		Rect objectPosition = block->getPosition();

		borderPosition.y = 360 + 180 - border.y/2;
		borderPosition.x = 320 - border.x/2;

		borderPosition.width = border.x;
		borderPosition.height = border.y;
		SDL_Rect sdlRect = borderPosition.toSDLRect();
		SDL_RenderDrawRect(render,&sdlRect);

		Rect sleeperPosition = sleeper->getPosition();
		Rect relativePosition = {objectPosition.x - sleeperPosition.x + 640 + 16,objectPosition.y - sleeperPosition.y + 16};
		SDL_Point center = {320+640,180};

		SDL_RenderDrawLine(render,center.x,center.y,int(relativePosition.x),int(relativePosition.y));

		SDL_SetRenderDrawColor(render,0,0,0,255);

	}

	~myScene() {
		for(int i = 0;i<10;++i)
			delete sblocks[i];

		delete sblock;
		delete block;
		delete labelCenter;
		delete labelSleeper;
		delete labelRect;
		delete labelNone;
	}


};

//304 164