#include "../animation.h"

Animation::Animation(SDL_Texture* text,short w,short h,short begX,short begY,float speed,bool lp,bool cmpl):
	texture(text),begFrameX(begX),begFrameY(begY),curFrameX(begX),curFrameY(begY),
	frameWidth(w),frameHeight(h),frameSpeed(speed),loop(lp),finished(false),complicated(cmpl) {

		curTime = 0.0f;

		if(speed < 0.0f) frameSpeed = 0.0f;

		SDL_QueryTexture(texture,nullptr,nullptr,&textureWidth,&textureHeight);	//Получаем размер текстуры
}

void Animation::setSpeed(float speed) {
	if(speed < 0.0f) frameSpeed = 0.0f;
}

void Animation::nextFrame() {
	//Смещаем кадр вправо,если вышли за размер текстуры и тип анимации усложнённый - переходим на
	//следующий ряд,иначе просто переходим в начальный кадр.

	curFrameX += frameWidth;
	if(curFrameX >= textureWidth) {
		if(complicated) curFrameY += frameHeight;

		curFrameX = begFrameX;

		//Ставим флаг,что анимация завершилась,если она не усложнённая
		if(!complicated) finished=true;
	}

	//Если вышли за границу размера текстуры по высоте - обнуляем в начальную позицию.
	if(curFrameY > textureHeight) {
		curFrameY = begFrameY;
		finished = true;
	}
}

void Animation::update(float delta) {
	if(frameSpeed > 0) {
		curTime += delta;

		//Если прошел кадр
		if(curTime >= 1000/frameSpeed) {
			curTime = 0.0f;

			if(loop || !finished) nextFrame();
		}
	}
}

void Animation::reset() {
	curFrameY = begFrameY;
	curFrameX = begFrameX;

	curTime = 0.0f;
	finished = false;
}

SDL_Rect Animation::getFrameSource() { return (SDL_Rect){curFrameX,curFrameY,frameWidth,frameHeight}; }