#ifndef MEDIA_MANAGER_H_
#define MEDIA_MANAGER_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <map>
#include <string>

SDL_Texture* loadTextureFromFile(std::string path);

//Структура,которая хранит расположение фрагмента текстуры.Необходимо для представления
//изображений на огромных текстурах(атласах).
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

	MediaManager() {}
	MediaManager(const MediaManager& )=delete;
	MediaManager& operator=(const MediaManager& )=delete;
public:
	static MediaManager* getInstance();

	//Загружает текстуру и добавляет её в словарь загруженных текстур,если
	//текстура уже загружена,просто возвращает указатель
	SDL_Texture* loadTexture(std::string path);

	//Загружает текстуру (если ещё не загружена) и добавляет в словарь загруженных
	//изображений,если изображение уже загружено - просто возвращает указатель
	Image loadImage(std::string path,std::string imageName,SDL_Rect source);

	//Удаляет все текстуры и завершает работу синглтона
	void close();

};


#endif