#include <cmath>

#include "sprite.h"
#include "game.h"

Sprite::Sprite():texture(nullptr),absolutePosition{0,0},relativePosition{0,0},angle(0),pinned(false),currentAnimation(nullptr),animationStopped(false) { }

Sprite::Sprite(SDL_Texture* tex,Rect apos,Rect sz,double angl,SDL_RendererFlip type,bool pnd):
texture(tex),absolutePosition(apos),relativePosition{0,0},size(sz),anchor{int(0.5*sz.x),int(0.5*sz.y)},angle(angl),pinned(pnd),flip(type),
currentAnimation(nullptr),animationStopped(false) { }

void Sprite::setPosition(double x,double y) { absolutePosition.x = x;absolutePosition.y = y; }

void Sprite::setPosition(Rect pos) {absolutePosition = pos; }

void Sprite::setSize(double w,double h) { size.x = w;size.y = h; }
void Sprite::setSize(Rect _size) { size = _size; }

SDL_Rect Sprite::getAbsoluteDestination() const {
	return (SDL_Rect){(int)round(absolutePosition.x),(int)round(absolutePosition.y),(int)round(size.x),(int)round(size.y)};
}

SDL_Rect Sprite::getRelativeDestination() const {
	return (SDL_Rect){(int)round(relativePosition.x),(int)round(relativePosition.y),(int)round(size.x),(int)round(size.y)};
}

void Sprite::draw(SDL_Renderer* renderer,Rect cameraPosition,double cameraAngle) {
	if(pinned) { cameraPosition.x = 0; cameraPosition.y = 0; } //Если спрайт прикреплен,меняем позицию камеры в 0 (по сути удаляем позицинирование относительно камеры)

	relativePosition.x = absolutePosition.x-cameraPosition.x;	//Устанавливаем относительную
	relativePosition.y = absolutePosition.y-cameraPosition.y;	//позицию.

	SDL_Rect destination = {(int)round(relativePosition.x),(int)round(relativePosition.y),(int)round(size.x),(int)round(size.y)};
	SDL_Rect source;
	
	if(currentAnimation == nullptr) //Или || count of animations <= 0.Если анимация не задана или их нет
		source = {0,0,(int)round(size.x),(int)round(size.y)};	//Берем изображение из верхнего левого угла
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
	SDL_Rect destination = getRelativeDestination();

	return SDL_PointInRect(&clickedPosition,&destination);

}

bool Sprite::isAnimate() const{
	return (!animationStopped && (currentAnimation != nullptr));	//Анимация не остановлена и текущая анимация задана
}