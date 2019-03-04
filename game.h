#ifndef GAME_H_
#define GAME_H_
#include <SDL2/SDL.h>

#include <map>
#include <utility>

#include "scene.h"
#include "logger.h"
#include "media_manager.h"

class Game {
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	Logger* logger;

	std::string gameName; 	//Имя игры

	Scene* currentScene; 	//Хранит текущую сцену

	bool initialized;		//Хранит состояние инициализации
	bool fullscreen;
	bool exitStatus;

	SDL_Point screenSize;

	Uint64 lastTime;		//Время последнего кадра

	std::map<SDL_Keycode,Uint64> pressedKeys;	//Нажатые клавиши клавиатуры
	std::map<Uint8,Uint64> pressedButtons;		//Нажатые клавиши мыши
	
	SDL_Point lastMouseClickPosition;

	//Обрабатывает некоторые события и делегирует их текущей сцене
	void controller();

	//Возвращает время,которое прошло от прошлого кадра
	double getDeltaTime();

	Game():lastMouseClickPosition{0,0}  { }
	Game(const Game&)=delete;
	Game& operator=(const Game&)=delete;

public:
	void initGame(int width,int height,std::string name, Uint32 flags,Uint32 windowFlags,Uint32 rendererFlags = SDL_RENDERER_ACCELERATED,bool fullscreen=false,const char* logFileName = "default.log");

	//Завершает игру,очищает память,сбрасывает логи,сохраняет информацию.
	void closeGame();

	static Game* getInstance();

	void startGame();	//Начинает игровой цикл

	bool isFullscreen() const { return fullscreen; }
	bool isInitialized() const { return initialized; }

	void draw();

	void log(std::string message);

	void switchScene(Scene* scene) { currentScene = scene;}

	static MediaManager* getMediaManager() { return MediaManager::getInstance();}

	//Возвращает состояние переданной клавиши (нажата или нет)
	bool isPressed(SDL_Keycode key);
	bool isPressed(Uint8 key);

	SDL_Point getLastMouseClickPosition() const { return lastMouseClickPosition; }

	//Возвращает время,которое прошло с момента нажатия клавиши
	long long getPressedTime(SDL_Keycode key);

	SDL_Point getScreenSize() { return screenSize; }

	SDL_Renderer* getRenderer() { return renderer; }
};

#endif