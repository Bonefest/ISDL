#ifndef MEDIA_MANAGER_H_
#define MEDIA_MANAGER_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "animation.h"

#include <map>
#include <string>

SDL_Texture* loadTextureFromFile(const std::string& path);

//Структура,которая хранит расположение фрагмента текстуры.Необходимо для представления
//изображений на огромных текстурах(атласах).

//Запланированное применение : имя конкретного куска атласа.Возможность зная имя изображения
//получить текстуру и позицию на текстуре
struct Image {
	SDL_Rect source;
	SDL_Texture* texture;
};

class MediaManager {
private:
	//Словарь загруженных текстур,где ключ - путь к текстуре
	std::map<std::string,SDL_Texture*> loadedTextures;	

	//Словарь загруженных изображений,где ключ - имя изображения
	std::map<std::string,Image> loadedImages;

	//Словарь загруженных анимаций,где ключ - название анимации
	std::map<std::string,Animation> loadedAnimations;

	//Словарь загруженных шрифтов,где ключ - имя шрифта
	std::map<std::string,TTF_Font*> loadedFonts;

	MediaManager() {}
	MediaManager(const MediaManager& )=delete;
	MediaManager& operator=(const MediaManager& )=delete;
public:
	static MediaManager* getInstance();

	//Загружает текстуру и добавляет её в словарь загруженных текстур,если
	//текстура уже загружена,просто возвращает указатель
	SDL_Texture* loadTexture(const std::string& path);

	//Загружает текстуру (если ещё не загружена) и добавляет в словарь загруженных
	//изображений,если изображение уже загружено - просто возвращает структуру
	Image loadImage(const std::string& path,const std::string& imageName,SDL_Rect source);

	//Загружает анимацию и добавляет в словарь загруженных анимаций.Если такая анимация уже существует -
	//возвращает её копию.
	Animation loadAnimation(const std::string& path,const std::string& animationName,SDL_Rect source,
		float speed=0.0,bool loop=true,bool complicated=false);

	//Возвращает ранее загруженную анимацию.В случае отсутствия - анимация с нулевой текстурой
	Animation getAnimation(const std::string& animationName);
	/*//Сохраняет переданную анимацию под заданным именем в словаре
	Animation loadAnimation(const Animation& animation,const std::string& animationName);*/

	//Возвращает ранее загруженное изображение.В случае,если изображение ещё не загружено -
	//возвращает структуру с нулевой текстурой.
	Image getImage(const std::string& imageName);

	//Возвращает загружено ли изображение в данный момент.
	bool isImageLoaded(const std::string& imageName);

	//Загружает шрифт и добавляет его в словарь загруженных шрифтов,если шрифт уже добавлен -
	//просто возвращает указатель
	TTF_Font* loadFont(const std::string& path,int ptsize);

	//Получает на вход json файл, который содержит метаданные (информация про изображения на атласе)
	//автоматически добавляет различные изображения в словарь
	void scanJsonFile(const std::string& path);

	//Удаляет все текстуры и завершает работу синглтона
	void close();

};


#endif