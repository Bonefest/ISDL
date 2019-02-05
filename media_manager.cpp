#include "texture_manager.h"
#include "game.h"

SDL_Texture* loadTextureFromFile(std::string path) {
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
	static TextureManager* textureManager = new TextureManager();

	return textureManager;
}

SDL_Texture* MediaManager::loadTexture(std::string path) {
	if(loadedTextures.find(path) == loadedTextures.end()) {
		SDL_Texture* texture = loadTextureFromFile(path);
		if(texture == NULL) return NULL;

		loadedTextures.emplace(path,texture);

		return texture;
	}

	return loadedTextures[path];
}

void MediaManager::close() {
	for(auto textureIter = loadedTextures.begin();textureIter!=loadedTextures.end();textureIter++) {
		SDL_DestroyTexture(textureIter->second);
	}

	delete getInstance();
}