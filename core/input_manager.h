#ifndef INPUT_MANAGER_H_
#define INPUT_MANAGER_H_

#include <map>
#include <SDL2/SDL.h>

namespace MSDL {

	const Uint32 NONE_TIME = -1;

	class InputManager {
	private:
		//Словарь нажатых клавиш.Хранит : Код нажатой клавиши - Время нажатия
		std::map<SDL_Keycode,Uint32> pressedKeyboardKeys;	//Можно хранить целый Keyboard ивент, но в этом нет смысла
		
		//Словарь нажатые клавиш мыши.Хранит : код нажатой клавиши - Событие мыши
		std::map<Uint8,SDL_MouseButtonEvent> pressedMouseButtons;

		SDL_MouseButtonEvent lastPressedButton;		//Последняя нажатая клавиша мыши

		InputManager() {}
		InputManager(const InputManager&) {}
		InputManager& operator=(const InputManager&)=delete;
		~InputManager() {}

	public:
		static InputManager* getInstance();

		void processEvent(SDL_Event event);	//Обрабатывает событие и при необходимости сохраняет данные

		bool isKeyPressed(SDL_Keycode) const;
		bool isButtonPressed(Uint8) const;

		Uint32 getPressedKeyTime(SDL_Keycode) const;
		Uint32 getPressedButtonTime(Uint8) const;

		SDL_MouseButtonEvent getLastPressedMouseButton() const;		//Возвращает последнюю нажатую клавишу мыши
		SDL_Point getLastPressedMouseButtonPoint() const;			//Возвращает точку нажатия последней клавиши мыши

		void close();	//Очищает менеджер входных событий
	};

}

#endif