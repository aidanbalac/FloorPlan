#pragma once

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "../include/Wall.hpp"
#include "../include/Inputs.hpp"   

class Structure {
public:
    enum Mode {
    SELECT,
    BUILD_WALL,
    EDIT_WALL, 
    MOVE_WALL};

    Structure();

    Mode currentMode;
    bool BUILDING_WALL;
    bool PANNING;
    bool ZOOMING;
    std::vector<Wall> walls;
    std::vector<Wall*> selectedWalls;
    sf::Image floorPlan;
    sf::Texture floorPlanTexture;
    sf::Sprite floorPlanSprite;

    void loadFloorPlan(const std::string& path);

    
    void addWall(sf::Vector2f& point); // adds a wall to the structure
    void selectWall(Wall& wall); // selects a wall
    void unselectWall(Wall& wall); // unselects a wall
    void unselectAllWalls(); // unselects all walls
    void removeWalls(); // removes selected walls
    void editWall(sf::Vector2f& point, bool shiftHeld); // edits a point of a wall, includes rotation
    void moveWall(sf::Vector2f& point); // moves a wall
    sf::Vector2f alignWall(sf::Vector2f& point); // aligns a wall to the grid
    sf::Vector2f snapCorner(sf::Vector2f& point); // snaps a corner to the grid
    void splitWall(sf::Vector2f& point);// splits wall and creates new wall
    void mergeWalls(Wall& wall1, Wall& wall2);// merges two walls and creates a new wall
    std::string getModeString(Mode mode);
    void update(sf::RenderWindow& window, sf::View& mainView, Inputs& inputs);
};
    