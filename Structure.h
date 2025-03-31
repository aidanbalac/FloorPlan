#pragma once

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "Wall.h"

using namespace sf;

class Structure {
public:
    Structure();
    std::vector<Wall> walls;
    std::vector<Wall*> selectedWalls;
    sf::Image floorPlan;
    sf::Texture floorPlanTexture;
    sf::Sprite floorPlanSprite;

    void loadFloorPlan(const std::string& path);

    
    void addWall(Vector2f& point); // adds a wall to the structure
    void selectWall(Wall& wall); // selects a wall
    void unselectWall(Wall& wall); // unselects a wall
    void unselectAllWalls(); // unselects all walls
    void removeWalls(); // removes selected walls
    void editWall(Vector2f& point); // edits a point of a wall, includes rotation
    void moveWall(Vector2f& point); // moves a wall

    void splitWall(Vector2f& point);// splits wall and creates new wall
    void mergeWalls(Wall& wall1, Wall& wall2);// merges two walls and creates a new wall
};
    