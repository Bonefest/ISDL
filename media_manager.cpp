#include <fstream>
#include "media_manager.h"
#include "game.h"
#include "json.hpp"

using Json=nlohmann::json;

SDL_Texture* loadTextureFromFile(const std::string& path) {
	SDL_Surface* image = IMG_Load(path.c_str());
	if(!image) {
		Game::getInstance()->log("Can't load image '" + path + "' : " + IMG_GetError());
		return nullptr;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::getInstance()->getRenderer(),image);
	if(texture == NULL)
		Game::getInstance()->log("Can't load image '" + path + "' : " + IMG_GetError());

	SDL_FreeSurface(image);
	return texture;

}

MediaManager* MediaManager::getInstance() {
	static MediaManager* mediaManager = new MediaManager();

	return mediaManager;
}

SDL_Texture* MediaManager::loadTexture(const std::string& path) {
	if(loadedTextures.find(path) == loadedTextures.end()) {
		SDL_Texture* texture = loadTextureFromFile(path);
		if(texture == NULL) return NULL;

		loadedTextures.emplace(path,texture);

		return texture;
	}

	return loadedTextures[path];
}

Image MediaManager::loadImage(const std::string& path, const std::string& imageName , SDL_Rect source) {
	if(loadedImages.find(imageName) == loadedImages.end()) {
		Image image;
		image.source = source;
		image.texture = loadTexture(path);

		loadedImages[imageName] = image;

		return image;
	}

	return loadedImages[imageName];

}

Image MediaManager::getImage(const std::string& imageName) {
	if(loadedImages.find(imageName) == loadedImages.end()) {
		return (Image){};
	}

	return loadedImages[imageName];
}


Animation MediaManager::loadAnimation(const std::string& path,const std::string& animationName,SDL_Rect source,
		float speed,bool loop,bool complicated) {
	if(loadedAnimations.find(animationName) == loadedAnimations.end()) {
		loadedAnimations[animationName] = Animation(loadTexture(path),source.w,source.h,source.x,source.y,
			speed,loop,complicated);
	}

	return loadedAnimations[animationName];
}

Animation MediaManager::getAnimation(const std::string& animationName) {
	if(loadedAnimations.find(animationName) == loadedAnimations.end())
		return Animation();
	return loadedAnimations[animationName];
}

TTF_Font* MediaManager::loadFont(const std::string& path,int ptsize) {
	if(loadedFonts.find(path) == loadedFonts.end()) {
		TTF_Font* font = TTF_OpenFont(path.c_str(),ptsize);
		if(!font) {
			Game::getInstance()->log("Can't load font '" + path + "' : " + TTF_GetError());
			return nullptr;
		}
		loadedFonts.emplace(path,font);
		return font;
	}
	return loadedFonts[path];
}

void MediaManager::scanJsonFile(const std::string& path) {
	std::ifstream jsonFile(path);
	if(jsonFile.is_open()) {
		Json parser;
		jsonFile >> parser;

		//Если не можем получить информацию про атлас
		if(parser["meta"]["image"].is_null()) {
			Game::getInstance()->log("Can't get 'meta' in '" + path + "'.");
			return;
		}

		std::string texturePath = parser["meta"]["image"];
		
		//Загрузка изображений из атласа
		for(auto& frame : parser["frames"].items()) {

/*			//Если кадр не содержит информации(не является объектом)
			if(!frame.value()["frame"].is_object()) {
				Game::getInstance()->log("Can't load json data from '" + texturePath +
					" frame '" + frame.key() + " is not an object!\nFrame skipped!");
			}
*/
			SDL_Rect rect = {
				frame.value()["frame"].value("x",0),	//Получаем информацию
				frame.value()["frame"].value("y",0),	//из кадра
				frame.value()["frame"].value("w",0),
				frame.value()["frame"].value("h",0)
			};

			loadImage(texturePath,frame.key(),rect);

		}


		for(auto& animation : parser["animations"].items()) {
			SDL_Rect rect = {
				animation.value()["frame"].value("x",0),
				animation.value()["frame"].value("y",0),
				animation.value()["frame"].value("w",0),
				animation.value()["frame"].value("h",0)
			};
			float speed = animation.value().value("speed",0.0);
			bool loop = animation.value().value("loop",true);
			bool complicated = animation.value().value("complicated",false);

			loadAnimation(texturePath,animation.key(),rect,speed,loop,complicated);
		}

	}
}

void MediaManager::close() {
	for(auto textureIter = loadedTextures.begin();textureIter != loadedTextures.end();textureIter++) {
		SDL_DestroyTexture(textureIter->second);
	}

	for(auto fontIter = loadedFonts.begin();fontIter != loadedFonts.end();fontIter++)
		TTF_CloseFont(fontIter->second);

	delete getInstance();
}
