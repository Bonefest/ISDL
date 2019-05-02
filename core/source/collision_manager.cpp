#include "../collision_manager.h"
#include "../game.h"

#include <iostream>
#include <cmath>

void CollisionManager::addSprite(Sprite* sprite,long level) {
	sprites[level].push_front(sprite);
}

void CollisionManager::simpleCollisionAlgorithm() {
	for(auto levelIterPair=sprites.begin();levelIterPair!=sprites.end();levelIterPair++) {	//Проходим по всем уровням коллизий

		for(auto objOneIter=levelIterPair->second.begin();objOneIter!=levelIterPair->second.end();objOneIter++) {
			for(auto objTwoIter=levelIterPair->second.begin();objTwoIter!=levelIterPair->second.end();objTwoIter++) {
				if(*objOneIter==*objTwoIter) continue;	//Если это один и тот же объект

				SDL_Rect oneSDLPos = (*objOneIter)->getPosition().toSDLRect();
				SDL_Rect twoSDLPos = (*objTwoIter)->getPosition().toSDLRect();

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

#include <iostream>

Sprite* CollisionManager::raycast(Rect startPos,Rect endPos, long level) {
	Sprite* closerSprite = nullptr;

	//Берем список всех объектов на данном уровне коллизий с карты коллизий
	for(auto spriteIter = sprites[level].begin();spriteIter != sprites[level].end();spriteIter++) {
		SDL_Point startPoint = startPos.toSDLPoint();
		SDL_Point endPoint = endPos.toSDLPoint();

		Rect iterSpritePos = (*spriteIter)->getPosition();
		SDL_Rect iterSpriteSDLPos = iterSpritePos.toSDLRect();
		SDL_Rect iterSpriteSize = (*spriteIter)->getSize().toSDLRect();
		iterSpriteSDLPos.w = iterSpriteSize.x;
		iterSpriteSDLPos.h = iterSpriteSize.y;

		//Проверяем сталкивается ли проверяемый объект с лучем
		if(SDL_IntersectRectAndLine( &iterSpriteSDLPos, &startPoint.x, &startPoint.y, &endPoint.x, &endPoint.y ) ) {
			if(closerSprite==nullptr) closerSprite=(*spriteIter);
			else {
				Rect currentSpritePos = closerSprite->getPosition();
				double distanceCurrent = sqrt(pow(currentSpritePos.x-startPos.x,2) + pow(currentSpritePos.y-startPos.y,2));
				double distanceIter = sqrt(pow(iterSpritePos.x-startPos.x,2) + pow(iterSpritePos.y-startPos.y,2));

				//Смотрим какой из объектов ближе к началу : ранее найденный или текущий
				if(distanceIter<distanceCurrent) closerSprite = (*spriteIter);
			}
		}
	}

	return closerSprite;
}

Sprite* CollisionManager::raycast(Rect pos,double angle,double length,long level) {
	return raycast(pos,(Rect){cos(angle)*level,sin(angle)*length},level);
}


void CollisionManager::calculate() {
	simpleCollisionAlgorithm();
}