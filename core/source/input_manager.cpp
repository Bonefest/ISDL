#include "../input_manager.h"

using MSDL::InputManager;

InputManager* InputManager::getInstance() {
	static InputManager* manager = new InputManager();
	return manager;
}


bool InputManager::isKeyPressed(SDL_Keycode key) const {
	return pressedKeyboardKeys.find(key) != pressedKeyboardKeys.end();
}

bool InputManager::isButtonPressed(Uint8 button) const {
	return pressedMouseButtons.find(button) != pressedMouseButtons.end();
}

Uint32 InputManager::getPressedKeyTime(SDL_Keycode key) const {
	auto keyIter = pressedKeyboardKeys.find(key);
	if(keyIter == pressedKeyboardKeys.end()) return MSDL::NONE_TIME;

	return SDL_GetTicks()-keyIter->second;
}

Uint32 InputManager::getPressedButtonTime(Uint8 button) const {
	auto buttonIter = pressedMouseButtons.find(button);
	if(buttonIter == pressedMouseButtons.end()) return MSDL::NONE_TIME;

	return SDL_GetTicks()-buttonIter->second.timestamp;
}

void InputManager::processEvent(SDL_Event event) {
		if(event.type == SDL_MOUSEBUTTONDOWN) {
			if(!isButtonPressed(event.button.button)) {
				pressedMouseButtons.insert(std::pair<Uint8,SDL_MouseButtonEvent>(event.button.button,event.button));
				
				lastPressedButton = event.button;
			}
		}

		//Если нажата клавиша и её нет в словаре - добавляем её в словарь.
		else if(event.type == SDL_KEYDOWN) {
			if(!isKeyPressed(event.key.keysym.sym))
				pressedKeyboardKeys.insert(std::pair<SDL_Keycode,Uint64>(event.key.keysym.sym,SDL_GetTicks()));	
		}

		//Если клавиша отпущена и она есть в словаре - удаляем её из словаря.
		else if(event.type == SDL_KEYUP) {
			if(isKeyPressed(event.key.keysym.sym)) {
				pressedKeyboardKeys.erase(event.key.keysym.sym);
			}
		} 

		else if(event.type == SDL_MOUSEBUTTONUP) {
			if(isButtonPressed(event.button.button))
				pressedMouseButtons.erase(event.button.button);
		}
}


SDL_MouseButtonEvent InputManager::getLastPressedMouseButton() const {
	return lastPressedButton;
}

SDL_Point InputManager::getLastPressedMouseButtonPoint() const {
	return (SDL_Point){lastPressedButton.x,lastPressedButton.y};
}

void InputManager::close() {
	delete getInstance();
}