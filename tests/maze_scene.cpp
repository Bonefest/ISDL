#include "maze_scene.h"
#include "../core/game.h"

#include <iostream>
Maze::Maze() {
    initialize();
}


bool Maze::isAvailable(int x,int y,int max) {
    int count = getDestroyedCount(x,y);
    if(count < 0) return false;

    for(auto iter = visitedPoints.begin();iter!=visitedPoints.end();iter++) {
        if(iter->x == x && iter->y == y) return false;
    }

    return count <= max;
}

void Maze::initialize() {
    for(int i = 0;i<GRID_SIZE;++i) {
        for(int j = 0;j<GRID_SIZE;++j) {
            for(int s = 0;s<4;++s)
                grid[i][j].direction[s] = Wall::ACTIVATED;
        }
    }

    for(int i = 0;i<GRID_SIZE;++i) {
        grid[0][i].direction[UP]=Wall::DISACTIVATED;
        grid[i][0].direction[LEFT]=Wall::DISACTIVATED;
        grid[GRID_SIZE-1][i].direction[DOWN]=Wall::DISACTIVATED;
        grid[i][GRID_SIZE-1].direction[RIGHT]=Wall::DISACTIVATED;
    }

    lastX = rand() % GRID_SIZE;
    lastY = GRID_SIZE-1;
}


int Maze::getDestroyedCount(int x,int y) {
    if(x < 0 || y < 0 || x >= GRID_SIZE || y >= GRID_SIZE) return -1;

    int decount = 0;
    for(int i = 0;i<4;++i)
        if(grid[y][x].direction[i] == Wall::DESTROYED || grid[y][x].direction[i] == Wall::DISACTIVATED) decount++;

    return decount;
}


bool Maze::generateNextStartPoint() {
    std::cout << "NEXT POINT\n";
    std::vector<Point> points;
    for(auto iter = visitedPoints.begin();iter!=visitedPoints.end();iter++)
        if(isAvailable(iter->x,iter->y),2) points.push_back(*iter);

    if(points.size() <= 0) return false;

    Point point = points[rand()%points.size()];
    lastX = point.x;
    lastY = point.y;

    return true;
}

void Maze::clearUnusedPoints() {
    std::cout << "here\n";
    for(auto iter = visitedPoints.begin();iter!=visitedPoints.end();) {
        if(!isAvailable(iter->x-1,iter->y) && !isAvailable(iter->x,iter->y-1) &&
            !isAvailable(iter->x+1,iter->y) && !isAvailable(iter->x,iter->y+1))
                iter = visitedPoints.erase(iter);
        else iter++;
    }
    std::cout << "here2\n";
}

direction Maze::getNextDirection() {
    int decount = 4;
    std::vector<direction> directions;
    directions.push_back(WRONG);

    int tdecount = getDestroyedCount(lastX-1,lastY);
    if(isAvailable(lastX-1,lastY) && grid[lastY][lastX].direction[LEFT] != Wall::DESTROYED && tdecount >= 0) {
        if(tdecount < decount) {
            directions.clear();
            directions.push_back(LEFT);

            decount = tdecount;
        } else if(tdecount == decount) {
            directions.push_back(LEFT);

            decount = tdecount;
        }
    }

    tdecount = getDestroyedCount(lastX,lastY-1);
    if(isAvailable(lastX,lastY-1) && grid[lastY][lastX].direction[UP] != Wall::DESTROYED && tdecount >= 0) {
         if(tdecount < decount) {
            directions.clear();
            directions.push_back(UP);

            decount = tdecount;
        } else if(tdecount == decount) {
            directions.push_back(UP);

            decount = tdecount;
        }
    }

    tdecount = getDestroyedCount(lastX+1,lastY);
    if(isAvailable(lastX+1,lastY) && grid[lastY][lastX].direction[RIGHT] != Wall::DESTROYED && tdecount >= 0) {
       if(tdecount < decount) {
            directions.clear();
            directions.push_back(RIGHT);

            decount = tdecount;
        } else if(tdecount == decount) {
            directions.push_back(RIGHT);

            decount = tdecount;
        }
    }

    tdecount = getDestroyedCount(lastX,lastY+1);
    if(isAvailable(lastX,lastY+1) && grid[lastY][lastX].direction[DOWN] != Wall::DESTROYED && tdecount >= 0) { 
        if(tdecount < decount) {
            directions.clear();
            directions.push_back(DOWN);

        } else if(tdecount == decount)
            directions.push_back(DOWN);

    }

    return directions[rand() % directions.size()];
}

bool Maze::deleteWall(int& x,int& y,direction dir) {
    switch(dir) {
        case LEFT:
            grid[y][x].direction[LEFT] = Wall::DESTROYED;
            grid[y][--x].direction[RIGHT] = Wall::DESTROYED;
            return true;
    
        case UP:
            grid[y][x].direction[UP] = Wall::DESTROYED;
            grid[--y][x].direction[DOWN] = Wall::DESTROYED;
            return true;
        
        case RIGHT:
            grid[y][x].direction[RIGHT] = Wall::DESTROYED;
            grid[y][++x].direction[LEFT] = Wall::DESTROYED;
            return true;
        
        case DOWN:
            grid[y][x].direction[DOWN] = Wall::DESTROYED;
            grid[++y][x].direction[UP] = Wall::DESTROYED;
            return true;
        case WRONG:
            return false;
        
    }

    return false;

}

bool Maze::step() {
    std::cout << lastX << " " << lastY << std::endl;
    if(!isAvailable(lastX-1,lastY) && !isAvailable(lastX,lastY-1) && !isAvailable(lastX+1,lastY) && !isAvailable(lastX,lastY+1)) return false;
 
    direction dir = getNextDirection();
    if(dir != WRONG) visitedPoints.push_back(Point(lastX,lastY));

   return deleteWall(lastX,lastY,dir);

}

void Maze::generateMaze() {
    initialize();
    while(true) {
        while(step());
        clearUnusedPoints();
        if(!generateNextStartPoint()) break;
    }
  
    int x,y;
    x=y=0;
    deleteWall(x,y,direction(rand()%2+2));  
    x=0;
    y=GRID_SIZE-1;
    deleteWall(x,y,direction(rand()%2+1));
    x=y=GRID_SIZE-1;
    deleteWall(x,y,direction(rand()%2));
    x=GRID_SIZE-1;
    y=0;
    deleteWall(x,y,direction(3*(rand()%2)));

}


Analyzer::Analyzer(int startX,int startY,int finishX,int finishY,Maze& m) {
    fX = finishX;
    fY = finishY;
}

bool Analyzer::isVisited(int x,int y) {
    if(x < 0 || y < 0 || x >= GRID_SIZE || y>= GRID_SIZE) return true;

    
}
direction Analyzer::getNextDirection(Wall wall) {
    direction next;
    if(wall.direction[UP] == DESTROYED 
        && !isVisited(currentStep.point.x,currentStep.point.y-1)
        && !currentStep.usedDirections[UP]) { //Если вверху стенка разрушена и мы эту клетку ещё не посетили и по этому направлению не ходили
            Step nextStep = currentStep;
            nextStep.point.y--;
            currentStep.usedDirections[UP]=true;
            activatedSteps.push_back(nextStep);
            currentStep = nextStep;
            return UP;
    }
    else if(wall.direction[RIGHT] == DESTROYED 
        && !isVisited(currentStep.point.x+1,currentStep.point.y)
        && !currentStep.usedDirections[RIGHT]) { //Если вверху стенка разрушена и мы эту клетку ещё не посетили и по этому направлению не ходили
         

            return RIGHT;
    }
    else if(wall.direction[DOWN] == DESTROYED 
        && !isVisited(currentStep.point.x,currentStep.point.y+1)
        && !currentStep.usedDirections[DOWN]) { //Если вверху стенка разрушена и мы эту клетку ещё не посетили и по этому направлению не ходили
           

            return DOWN;
    }
    else if(wall.direction[LEFT] == DESTROYED 
        && !isVisited(currentStep.point.x-1,currentStep.point.y)
        && !currentStep.usedDirections[LEFT]) { //Если вверху стенка разрушена и мы эту клетку ещё не посетили и по этому направлению не ходили
        
        return LEFT;
    }

    return WRONG;   //Если все клетки заняты
}   

void Analyzer::analyze(int startX,int startY,Maze& m) {
    Step step = {Point(startX,startY)};
    activatedSteps.push_back(step);
    currentStep = step;
    while(!activatedSteps.empty()) {
        direction dir = getNextDirection(m.getWall(currentStep.point.x,currentStep.point.y)); 
        switch(dir) {
            case UP:
                Step nextStep = currentStep;
                nextStep.point.y--;
                currentStep.usedDirections[UP]=true;
                activatedSteps.push_back(nextStep);
                currentStep = nextStep;
                break;
            case RIGHT:
                Step nextStep = currentStep;
                nextStep.point.x++;
                currentStep.usedDirections[RIGHT]=true;
                activatedSteps.push_back(nextStep);
                currentStep = nextStep;
                break;
            case DOWN:
                Step nextStep = currentStep;
                nextStep.point.y++;
                currentStep.usedDirections[DOWN]=true;
                activatedSteps.push_back(nextStep);
                currentStep = nextStep;
                break;
            case LEFT:
                Step nextStep = currentStep;
                nextStep.point.x--;
                currentStep.usedDirections[LEFT]=true;
                activatedSteps.push_back(nextStep);
                currentStep = nextStep;
                break;
            case WRONG:
                //Пока массив посещенных клеток не пуст - получаем предыдущую клетку и смотрим,
                //Есть ли доступные направления
                while(!activatedSteps.empty()) {
                    Step lastStep = activatedSteps.pop();
                    if(getNextDirection(lastStep.point.x,lastStep.point.y) != WRONG)
                        break;
                }

        }
    }


}


void MyScene::draw() {
    SDL_SetRenderDrawColor(Game::getInstance()->getRenderer(),255,255,255,255);
    for(int i = 0;i<GRID_SIZE;++i) {
        for(int j = 0;j<GRID_SIZE;++j) {
            if(maze.getWall(j,i).direction[LEFT] != Wall::DESTROYED) {
                SDL_Rect rect = {j*32,i*32,1,32};
                SDL_RenderFillRect(Game::getInstance()->getRenderer(),&rect);
            }
            if(maze.getWall(j,i).direction[RIGHT] != Wall::DESTROYED) {
                SDL_Rect rect ={j*32 + 32,i*32,1,32};
                SDL_RenderFillRect(Game::getInstance()->getRenderer(),&rect);
            }
            if(maze.getWall(j,i).direction[DOWN] != Wall::DESTROYED) {
                SDL_Rect rect = {j*32,i*32 + 32,32,1};
                SDL_RenderFillRect(Game::getInstance()->getRenderer(),&rect);
            }
            if(maze.getWall(j,i).direction[UP] != Wall::DESTROYED) {
                SDL_Rect rect = {j*32,i*32,32,1};
                SDL_RenderFillRect(Game::getInstance()->getRenderer(),&rect);
            }
               
        }
    }

    SDL_SetRenderDrawColor(Game::getInstance()->getRenderer(),0,0,0,255);
}

void MyScene::controller(SDL_Event* event) {
    if(event->type == SDL_KEYDOWN) {
        if(Game::getInstance()->isPressed(SDLK_a))
            maze.generateMaze();
    }
}