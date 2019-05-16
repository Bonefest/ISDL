#include <cmath>

#include "../sprite.h"
#include "../game.h"
#include "../input_manager.h"

using namespace MSDL;

Sprite::Sprite():Object(Rect()),texture(nullptr),image{},spriteColor(255,255,255,255),relativePosition{0,0},angle(0),pinned(false),alreadyHovered(false),pressed(false),dragged(false),flip(SDL_FLIP_NONE),currentAnimation(nullptr),animationStopped(false) { }

Sprite::Sprite(SDL_Texture* tex,Rect pos,Rect sz,double angl,SDL_RendererFlip type,bool pnd):
Object(pos),texture(tex),image{},spriteColor(255,255,255,255),relativePosition{0,0},size(sz),angle(angl),pinned(pnd),alreadyHovered(false),pressed(false),dragged(false),flip(type),
currentAnimation(nullptr),animationStopped(false) { }

Sprite::Sprite(Image img,Rect pos,double angl,SDL_RendererFlip type,bool pnd):
Object(pos),texture(nullptr),image(img),spriteColor(255,255,255,255),relativePosition{0,0},angle(angl),pinned(pnd),alreadyHovered(false),dragged(false),flip(type),
currentAnimation(nullptr),animationStopped(false) { 
	size.setSDLRect(image.source);
}

void Sprite::addPosition(double x,double y) {
	for(auto childIter = children.begin();childIter != children.end();childIter++)
		(*childIter)->addPosition(x,y);

	Object::addPosition(x,y);
}

void Sprite::addPosition(Vector2 xy) : Sprite::addPosition(xy.getX(), xy.getY()){ };

void Sprite::setPosition(double x,double y) {
	Rect absolutePosition = getPosition();

	for(auto childIter = children.begin();childIter!=children.end();childIter++)
		(*childIter)->addPosition(x-absolutePosition.x,y-absolutePosition.y);

	Object::setPosition(x,y);

}

void Sprite::setPosition(Rect pos) { setPosition(pos.x,pos.y); }

void Sprite::setSize(double w,double h) { 
	size.x = w;size.y = h; 
	image.source.w = w;
	image.source.h = h;
}
void Sprite::setSize(Rect _size) { 
	size = _size; 
	image.source.w = _size.x;
	image.source.h = _size.y;
}

void Sprite::addChild(Sprite* child) {
	children.push_back(child);
}

void Sprite::setParent(Sprite* prnt) {
	if(prnt == this) return;

	parent = prnt;
}

void Sprite::removeParent() {
	setParent(nullptr);
}

SDL_Rect Sprite::getAbsoluteDestination() const {
	Rect absolutePosition = getPosition();

	return (SDL_Rect){(int)round(absolutePosition.x),(int)round(absolutePosition.y),(int)round(size.x),(int)round(size.y)};
}

SDL_Rect Sprite::getRelativeDestination() const {
	return (SDL_Rect){(int)round(relativePosition.x),(int)round(relativePosition.y),(int)round(size.x),(int)round(size.y)};
}

void Sprite::setTexture(SDL_Texture* _texture) {
	texture = _texture;
}

void Sprite::setImage(Image _image) {
	image = _image;
}

#include <iostream>
void Sprite::draw(SDL_Renderer* renderer,Rect cameraPosition,double cameraAngle) {
	if(pinned) { cameraPosition.x = 0; cameraPosition.y = 0; } //Если спрайт прикреплен,меняем позицию камеры в 0 (по сути удаляем позицинирование относительно камеры)

	Rect absolutePosition = getPosition();

	relativePosition.x = absolutePosition.x-cameraPosition.x;	//Устанавливаем относительную
	relativePosition.y = absolutePosition.y-cameraPosition.y;	//позицию.

	SDL_Rect destination = {(int)round(relativePosition.x),(int)round(relativePosition.y),(int)round(size.x),(int)round(size.y)};
	SDL_Rect source;

	SDL_Texture* sourceTexture = texture;
	
	if(currentAnimation == nullptr) {//Или || count of animations <= 0.Если анимация не задана или их нет
		//Если изображение задано - используем его
		if(image.texture != nullptr) {
			sourceTexture = image.texture;
			source = image.source;
		}
		//Иначе используем стандартную текстуру
		else {
			source = {0,0,(int)round(size.x),(int)round(size.y)};	//Берем изображение из верхнего левого угла
		}
	}
	else
		source = currentAnimation->getFrameSource();

	//Переводим относительные координаты центра вращения в абсолютные
	SDL_Point sdlAnchor = Rect(anchor.x*source.w,anchor.y*source.h).toSDLPoint();

	//Смещаем позицию объекта в зависимости от центра вращения
	destination.x -= sdlAnchor.x;
	destination.y -= sdlAnchor.y;

	SDL_SetTextureBlendMode(sourceTexture,SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(sourceTexture,Uint8(round(spriteColor.a)));
	SDL_SetTextureColorMod(sourceTexture,Uint8(round(spriteColor.r)),Uint8(round(spriteColor.g)),Uint8(round(spriteColor.b)));
	SDL_RenderCopyEx(renderer,sourceTexture,&source,&destination,angle,&sdlAnchor,flip);

	//Рисуем всех детей
	for(auto childIter = children.begin();childIter != children.end();childIter++)
		(*childIter)->draw(renderer,cameraPosition,cameraAngle);
}

void Sprite::update(double delta) {
	if(currentAnimation != nullptr && !animationStopped)
		currentAnimation->update(delta);

	//Обновляем всех детей
	for(auto childIter = children.begin();childIter != children.end();childIter++)
		(*childIter)->update(delta);
}

void Sprite::controller(SDL_Event* event){
	
	if(event->type == SDL_MOUSEBUTTONDOWN && this->isClicked()) {	//Объект нажат - нажатие
		if(pressed == false) {
			onClick(event->button);
			pressed = true;
		}
	} else if(event->type == SDL_MOUSEBUTTONUP) {
		if(pressed) {
			onRelease(event->button);
			pressed = false;
		}
		if(dragged) {
			onDrop(event->button);
			dragged= false;
		}
	} 
	else if(event->type == SDL_MOUSEMOTION) {
		if(pressed) {	//Объект нажат и мышь двигается - перетягивание
			onDrag(event->motion);
			if(!dragged) dragged = true;
		}
		else if(this->isHovered()) {	//Мышь была перемещена и данный объект не был наведен - ховер
			if(alreadyHovered == false) {
				onHover(event->motion);
				alreadyHovered = true;
			}
		} else {					//Мышь была перемещена и данный объект был наведен - анховер
			if(alreadyHovered) {
				onUnhover(event->motion);
				alreadyHovered = false;
			}
		}
	}

}

void Sprite::addAnimation(std::string key,Animation animation) {
	animations.emplace(key,animation);
}

void Sprite::startAnimation(std::string key) {
	if(animations.find(key) != animations.end()) {
		animationStopped = false;

		if(currentAnimation != nullptr)
			currentAnimation->reset();	//Обнуляем предыдущую анимацию

		currentAnimation = &animations[key];
	}
}

void Sprite::stopAnimation() {
	animationStopped = true;
}

void Sprite::resetAnimation() {
	if(currentAnimation != nullptr)
		currentAnimation->reset();
}

Animation Sprite::getAnimation(std::string key) {
	if(animations.find(key) != animations.end())
		return animations[key];

	return nullptr;
}

void Sprite::removeAnimation(std::string key) {
	animations.erase(key);
}

bool Sprite::isClicked() const {
	SDL_Point clickedPosition = MSDL::InputManager::getInstance()->getLastPressedMouseButtonPoint();
	SDL_Rect destination = getRelativeDestination();

	return SDL_PointInRect(&clickedPosition,&destination) ;

}

bool Sprite::isHovered() const {
	SDL_Point mousePosition;
	SDL_GetMouseState(&mousePosition.x,&mousePosition.y);

	SDL_Rect destination = getRelativeDestination();
	return SDL_PointInRect(&mousePosition,&destination);
}



bool Sprite::isAnimate() const{
	return (!animationStopped && (currentAnimation != nullptr));	//Анимация не остановлена и текущая анимация задана
}

bool Sprite::isPinned() const { return pinned; }

void Sprite::setPinned(bool value) { pinned = value; }

Label::Label(TTF_Font* _font):Sprite(),currentTexture(nullptr),font(_font),lastColor{255,255,255} {}

Label::Label(TTF_Font* _font,const std::string& text, Rect position , SDL_Color textColor ):Label(_font) {

	setText(text,position,textColor);
}

void Label::setText(const std::string& text,Rect position,SDL_Color textColor) {
	if(font != nullptr ) {
		setPosition(position);

		if(textColor.r == lastColor.r && textColor.g == lastColor.g && textColor.b == lastColor.b && currentText == text) return;	//Если текст и цвет не изменились - ничего не делаем

		lastColor = textColor;
		currentText = text;

		SDL_Surface* surface = TTF_RenderText_Solid(font,text.c_str(),textColor);

		//Если текущая текстура существу - удаляем её (для новой)
		if(currentTexture!= nullptr) {
			SDL_DestroyTexture(currentTexture);
			currentTexture = nullptr;
		}

		currentTexture = SDL_CreateTextureFromSurface(Game::getInstance()->getRenderer(),surface);

		setTexture(currentTexture);
		setSize(surface->w,surface->h);

		SDL_FreeSurface(surface);
	}
}

void Label::setText(const std::string& text) {
	setText(text,getPosition(),lastColor);

}

void Label::setColor(SDL_Color textColor) {
	setText(currentText,getPosition(),textColor);
}

//////////////////////////////////////////////

Button::Button(Rect position,Rect size,Image image,SDL_Color color,std::string text,TTF_Font* font):Sprite(image,position) {
	//Если шрифт не задан
	if(font==nullptr) {
		label = Label(Game::getInstance()->getMediaManager()->loadFont("default.ttf",12),text,position,color);
	} else
		label = Label(font,text,position,color);

	setSize(size);
}