#include "scene.h"
#include "game.h"
#include "Block.h"

class myScene : public Scene{
	Block *block;
	Sprite *sblock;
	Label* label;
	Camera *cam;
public:
	myScene(){
		block = new Block(Game::getMediaManager()->loadTexture("block.png"), (Rect){50, 50}, (Rect){32, 32});
		sblock = new Sprite(Game::getMediaManager()->loadTexture("block.png"), (Rect){50, 100}, (Rect){32, 32});
		label = new Label(Game::getMediaManager()->loadFont("default.ttf",36),"TEST",(Rect){150,150},(SDL_Color){255,255,255});

		cam = new Camera();

		addSprite(block);
		addSprite(sblock);
		addSprite(label);

		addCamera("camera", cam);
	}
};