#include "../core/scene.h"
#include "../core/game.h"
#include "../core/input_manager.h"
#include "Block.h"

#include <cctype>
#include <list>
#include <string>

using namespace MSDL;

const int SCREEN_HEIGHT = 720;
const int SCREEN_WIDTH = 1280;

class myScene : public Scene{

	int shiftX;
	std::list<Label*> symbols;

	Label* label;
	Camera *cam;
public:
	myScene():shiftX(0) {
		//s4block = new Sprite(Game::getMediaManager()->loadTexture("tests/block.png"), (Rect){700, 0}, (Rect){32, 900});
		label = new Label(Game::getMediaManager()->loadFont("resources/default.ttf",36),"TEST",(Rect){150,150},(SDL_Color){255,255,255});

		cam = new Camera();


		addSprite(label);

		addCamera("camera", cam);
	}

	void controller(SDL_Event* event) {
		Scene::controller(event);
		if(event->type == SDL_KEYDOWN) {
			const char* key = SDL_GetKeyName(event->key.keysym.sym);
			if(strlen(key) <= 2 && isalpha(key[0])) {
				Label* sym =  new Label(Game::getMediaManager()->loadFont("resources/default.ttf",36),SDL_GetKeyName(event->key.keysym.sym),(Rect){double(shiftX),0},(SDL_Color){255,255,255});
				sym->physics = new Physics(sym,true,1.0,1.0);
				addSprite(sym);	

				shiftX = (shiftX + 48) % SCREEN_WIDTH;
			}
		}
	}

	void update(double delta) {
		Scene::update(delta);
		for(auto symIter = symbols.begin();symIter != symbols.end();) {
			if((*symIter)->getPosition().y > SCREEN_HEIGHT + SCREEN_HEIGHT/15) {
				removeSprite(*symIter);
				symIter = symbols.erase(symIter);
			} else
				symIter++;
		}
	}


};