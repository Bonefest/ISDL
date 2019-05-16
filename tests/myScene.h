#include "../core/scene.h"
#include "../core/game.h"
#include "Block.h"

class myScene : public Scene{
	Block *block;
	Sprite *s1block;
	Sprite *s2block;
	Sprite *s3block;
	Sprite *s4block;
	Label* label;
	Camera *cam;
public:
	myScene(){
		block = new Block(Game::getMediaManager()->loadTexture("tests/block.png"), (Rect){50, 50}, (Rect){32, 32});
		s1block = new Sprite(Game::getMediaManager()->loadTexture("tests/block.png"), (Rect){0, 700}, (Rect){900, 32});
		s2block = new Sprite(Game::getMediaManager()->loadTexture("tests/block.png"), (Rect){0, 0}, (Rect){900, 32});
		s3block = new Sprite(Game::getMediaManager()->loadTexture("tests/block.png"), (Rect){0, 0}, (Rect){32, 900});
		s4block = new Sprite(Game::getMediaManager()->loadTexture("tests/block.png"), (Rect){700, 0}, (Rect){32, 900});
		label = new Label(Game::getMediaManager()->loadFont("resources/default.ttf",36),"TEST",(Rect){150,150},(SDL_Color){255,255,255});

		cam = new Camera();

		addSprite(block);
		addSprite(s1block);
		addSprite(s2block);
		addSprite(s3block);
		addSprite(s4block);
		addSprite(label);

		addCamera("camera", cam);
	}


};