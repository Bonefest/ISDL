#include "scene.h"
#include "game.h"
#include "Block.h"

class myScene : public Scene{
	Block *block;
	Sprite *sblock;
	Camera *cam;
public:
	myScene(){
		block = new Block(Game::getMediaManager()->loadTexture("block.png"), (Rect){50, 50}, (Rect){32, 32});
		sblock = new Sprite(Game::getMediaManager()->loadTexture("block.png"), (Rect){50, 100}, (Rect){32, 32});

		cam = new Camera();

		addSprite(block);
		addSprite(sblock);

		addCamera("camera", cam);
	}
};