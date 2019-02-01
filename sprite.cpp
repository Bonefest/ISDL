#include <cmath>

#include "sprite.h"
#include "game.h"

Sprite::Sprite():texture(nullptr),position{0,0},angle(0),pinned(false),currentAnimation(nullptr),animationStopped(false) { }

Sprite::Sprite(SDL_Texture* tex,Rect pos,Rect sz,double angl,SDL_RendererFlip type,bool pnd):
texture(tex),position(pos),size(sz),anchor{0.5*sz.x,0.5*sz.y},angle(angl),pinned(pnd),flip(type),
currentAnimation(nullptr),animationStopped(false) { }

void Sprite::setPosition(double x,double y) { position.x = x;position.y = y; }

void Sprite::setPosition(Rect pos) { position = pos; }

void Sprite::setSize(double w,double h) { size.x = w;size.y = h; }
void Sprite::setSize(Rect _size) { size = _size; }

SDL_Rect Sprite::getDestination() const {
	return (SDL_Rect){(int)round(position.x),(int)round(position.y),(int)round(size.x),(int)round(size.y)};
}

void Sprite::draw(SDL_Renderer* renderer,Rect cameraPosition,double cameraAngle) {

	SDL_Rect destination = {(int)round(position.x-cameraPosition.x),(int)round(position.y-cameraPosition.y),(int)round(size.x),(int)round(size.y)};
	SDL_Rect source;
	
	if(currentAnimation == nullptr) //Или || count of animations <= 0.Если анимация не задана или их нет
		source = {0,0,(int)round(size.x),(int)round(size.y)};
	else
		source = currentAnimation->getFrameSource();

	SDL_RenderCopyEx(renderer,texture,&source,&destination,angle+(int)round(cameraAngle),&anchor,flip);
	
}

void Sprite::update(double delta) {
	if(currentAnimation != nullptr && !animationStopped)
		currentAnimation->update(delta);
}


void Sprite::addAnimation(std::string key,Animation* animation) {
	animations.emplace(key,animation);
}

void Sprite::startAnimation(std::string key) {
	if(animations.find(key) != animations.end()) {
		animationStopped = false;

		if(currentAnimation != nullptr)
			currentAnimation->reset();	//Обнуляем предыдущую анимацию

		currentAnimation = animations[key];
	}
}

void Sprite::stopAnimation() {
	animationStopped = true;
}

void Sprite::resetAnimation() {
	if(currentAnimation != nullptr)
		currentAnimation->reset();
}

Animation* Sprite::getAnimation(std::string key) {
	if(animations.find(key) != animations.end())
		return animations[key];

	return nullptr;
}

void Sprite::removeAnimation(std::string key) {
	animations.erase(key);
}

bool Sprite::isClicked() const {
	SDL_Point clickedPosition = Game::getInstance()->getLastMouseClickPosition();
	SDL_Rect destination = getDestination();

	return SDL_PointInRect(&clickedPosition,&destination);

}

bool Sprite::isAnimate() const{
	return (!animationStopped && (currentAnimation != nullptr));	//Анимация не остановлена и текущая анимация задана
}