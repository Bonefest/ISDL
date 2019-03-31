#include "../collision_manager.h"
#include "../game.h"

#include <iostream>
void CollisionManager::addSprite(Sprite* sprite,long level) {
	sprites[level].push_front(sprite);
}

void CollisionManager::simpleCollisionAlgorithm() {
	for(auto levelIterPair=sprites.begin();levelIterPair!=sprites.end();levelIterPair++) {	//Проходим по всем уровням коллизий

		for(auto objOneIter=levelIterPair->second.begin();objOneIter!=levelIterPair->second.end();objOneIter++) {
			for(auto objTwoIter=levelIterPair->second.begin();objTwoIter!=levelIterPair->second.end();objTwoIter++) {
				if(*objOneIter==*objTwoIter) continue;	//Если это один и тот же объект

				SDL_Rect oneSDLPos = (*objOneIter)->getAbsolutePosition().toSDLRect();
				SDL_Rect twoSDLPos = (*objTwoIter)->getAbsolutePosition().toSDLRect();

				SDL_Rect oneSDLSize = (*objOneIter)->getSize().toSDLRect();
				SDL_Rect twoSDLSize = (*objTwoIter)->getSize().toSDLRect();

				oneSDLPos.w = oneSDLSize.x;
				oneSDLPos.h = oneSDLSize.y;

				twoSDLPos.w = twoSDLSize.x;
				twoSDLPos.h = twoSDLSize.y;

				if(SDL_HasIntersection(&oneSDLPos,&twoSDLPos)) {
					(*objOneIter)->onCollision(*objTwoIter);
					(*objTwoIter)->onCollision(*objOneIter);
				}

			}

		}

	}
}

void CollisionManager::calculate() {
	simpleCollisionAlgorithm();
}