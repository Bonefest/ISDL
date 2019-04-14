#ifndef MAZE_SCENE_H_
#define MAZE_SCENE_H_

#include "../core/scene.h"
#include <vector>

const int GRID_SIZE = 5;

enum direction {LEFT,UP,RIGHT,DOWN,WRONG};

struct Wall {
	enum state {ACTIVATED,DISACTIVATED,DESTROYED};
	state direction[4];
};

struct Point {
	int x;
	int y;
	Point(int _x,int _y):x(_x),y(_y) {}
};


class Maze {
private:
	Wall grid[GRID_SIZE][GRID_SIZE];
	int lastX;
	int lastY;

	std::vector<Point> visitedPoints;	//Клетки, которые уже были посещены

	bool isAvailable(int x,int y,int max=1);	//Клетка является доступной если количество разрушенных стен меньше или равно 2
	int getDestroyedCount(int x,int y);
	bool step();
	direction getNextDirection();
	bool generateNextStartPoint();	//Ищет наиболее подходящую клетку из уже имеющихся для дальнейшей генерации (если такой нет - false)
	void clearUnusedPoints();	//Ищет все точки, которые не могут быть использованы(сверху,снизу,слева и справа - недоступны)
	bool deleteWall(int& x,int& y,direction dir);

	void initialize();
public:
	Maze();
	void generateMaze();
	Wall getWall(int x,int y) { return grid[y][x]; }
};

class Analyzer {
private:
	//Хранит точку и направления которые были исследованы
	struct Step {
		Point point;
		bool usedDirections[4];
	};
	int fX;
	int fY;

	std::vector<Step> activatedSteps;	//Шаги, которые были выполнены
	Step currentStep;
	direction getNextDirection(Wall wall);
	bool isVisited(int x,int y);		//Ищет заданную точку в массиве посещенных
public:
	Analyzer(int startX,int startY,int finishX,int finishY,Maze& m);

	void analyze(int startX,int startY,Maze& m);
}

class MyScene: public Scene {
private:
	Maze maze;
public:
	virtual void draw();
	virtual void controller(SDL_Event* event);

};

#endif

/*

Алгоритм поиска пути:
Начинаем с заданной точки:

Сохраняем каждую точку в массиве(с уже выбранными путями).
Пытаемся идти вверх : если не получается,пытаемся вправо, иначе вниз иначе влево.(назад нельзя идти)
Если у нас не получается зайти на следующую клетку (она уже была исследована или у нас тупик)

В случае, если мы не можем идти дальше, возвращаемся на самую последнюю точку, в которой мы могли выбрать
путь.

*/