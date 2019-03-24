#include <fstream>
#include <string>
#include <iostream>

using namespace std;

bool createScene(string sceneName);

bool createObject(string objectName);

int main(){
	cout << "Chose option from list:\n1: create new Scene\n2: create new Object\n\n0: quit\n";
	int choice;
	string name;
	string exit = "q";
	cin >> choice;
	while(choice){
		switch(choice){
			case 1:
				cout << "Enter name of new scene or \"q\" to cancel:\n";
				cin >> name;
				if(name != exit){
					if(createScene(name)){
						cout << "Scene created successfully!\n";
					}else{
						cout << "Error, try again\n";
					}
				}
				break;
			case 2:
				cout << "Enter name of new object or \"q\" to cancel:\n";
				cin >> name;
				if(name != exit){
					if(createObject(name)){
						cout << "Object created successfully!\n";
					}else{
						cout << "Error, try again\n";
					}
				}
				break;
			default:
				cout << "Something went wrong, try again!\n";
				break;
		}
		cout << "\n\nChose option from list:\n1: create new Scene\n2: create new Object\n\n0: quit\n";
		cin >> choice;
	}
	
	return 0;
}

bool createScene(string sceneName){
	ofstream fout;
	fout.open(sceneName + ".h");
	if(!fout.is_open())
		return false;
	string ftext = 
	"#pragma once\n\n#include \"game.h\"\n#include \"scene.h\"\n\n\nclass " + sceneName + " : public Scene{\n	//private variables here\n	Camera *cam;\npublic:\n	//public - here\n	" + sceneName + "(){\n		addCamera(\"camera\", cam);\n	}\n};";
	fout << ftext;
	fout.close();
	return true;
}

bool createObject(string objectName){
	ofstream fout;
	fout.open(objectName + ".h");
	if(!fout.is_open())
		return false;
	string ftext = 
	"#pragma once\n\n#include \"game.h\"\n#include \"sprite.h\"\n\n\nclass " + objectName + " : public Sprite{\n	//private variables here\npublic:\n	//public - here\n	" + objectName + "(SDL_Texture* texture, Rect absolutePosition, Rect size, double angle = 0.0, SDL_RendererFlip type=SDL_FLIP_NONE, bool pinned = false):Sprite(texture, absolutePosition, size, angle, type, pinned){\n	}\n};";
	fout << ftext;
	fout.close();
	return true;
}