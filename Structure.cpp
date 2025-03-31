
#include "Structure.h"

Structure::Structure() {
    walls = std::vector<Wall>();
    selectedWalls = std::vector<Wall*>();
    floorPlan = sf::Image();
    floorPlanTexture = sf::Texture();
    floorPlanSprite = sf::Sprite();
}

// loads the floor plan from a file
void Structure::loadFloorPlan(const std::string& path) {
    floorPlan.loadFromFile(path);
    floorPlanTexture.loadFromImage(floorPlan);
    floorPlanSprite.setPosition(0, 0);
    floorPlanSprite.setTexture(floorPlanTexture);
}

// adds a wall to the structure, only used in build wall mode. 
void Structure::addWall(Vector2f& point) {
    walls.push_back(Wall(point, 10));
    selectedWalls.push_back(&walls.back());
}

// selects a wall 
void Structure::selectWall(Wall& wall) {
    wall.select();
    selectedWalls.push_back(&wall);
    
}
// unselects a wall 
void Structure::unselectWall(Wall& wall) {
    wall.unselect();
    selectedWalls.erase(std::remove(selectedWalls.begin(), selectedWalls.end(), &wall), selectedWalls.end());
    
}
// unselects all walls
void Structure::unselectAllWalls() {
    for (Wall* w : selectedWalls) 
        w->unselect();
    selectedWalls.clear();
}

// deletes all walls in selectedWalls
void Structure::removeWalls() {  
    for (Wall* wallpointer : selectedWalls) 
        walls.erase(walls.begin() + (wallpointer - &walls[0]));
    selectedWalls.clear();
}

// edits the wall based on mouse position
void Structure::editWall(Vector2f& point) {
    if (selectedWalls.size() != 1) 
        throw std::runtime_error("Selected walls size is not 1, something is amiss");
    selectedWalls[0]->edit(point);
}

// moves the wall based on mouse position
void Structure::moveWall(Vector2f& point) {
    if (selectedWalls.size() != 1) 
        throw std::runtime_error("Selected walls size is not 1, something is amiss");
    selectedWalls[0]->move(point);
}

// splits the wall at the mouse position
void Structure::splitWall(Vector2f& point) {
       return;
}

// merges two walls
void Structure::mergeWalls(Wall& wall1, Wall& wall2) {
    return;
}