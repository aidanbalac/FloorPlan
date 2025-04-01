
#include "../include/Structure.hpp"
#include "../include/Inputs.hpp" 

Structure::Structure() {
    currentMode = SELECT;
    BUILDING_WALL = false;
    PANNING = false;
    ZOOMING = false;
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
void Structure::addWall(sf::Vector2f& point) {
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
void Structure::editWall(sf::Vector2f& point) {
    if (selectedWalls.size() != 1) 
        throw std::runtime_error("Selected walls size is not 1, something is amiss");
    selectedWalls[0]->edit(point);
}

// moves the wall based on mouse position
void Structure::moveWall(sf::Vector2f& point) {
    if (selectedWalls.size() != 1) 
        throw std::runtime_error("Selected walls size is not 1, something is amiss");
    selectedWalls[0]->move(point);
}

// splits the wall at the mouse position
void Structure::splitWall(sf::Vector2f& point) {
       return;
}

// merges two walls
void Structure::mergeWalls(Wall& wall1, Wall& wall2) {
    return;
}

// returns the mode as a string
std::string Structure::getModeString(Mode mode) {
    if (mode == SELECT)          return "Select";
    else if (mode == EDIT_WALL)  return "Editing Wall";
    else if (mode == MOVE_WALL)  return "Moving Wall";
    else if (mode == BUILD_WALL) return "Building Wall";
    else return "Unknown";
}

// Structure Logic
void Structure::update(sf::RenderWindow& window, sf::View& mainView, Inputs& inputs) {

    // // check if the mouse is in menu area, if so, handle the mouse click
    // if (menu.getGlobalBounds().contains(worldPos)){
    //     if (!leftReleased) // if left is not released, user hasn't clicked on anything yet
    //         continue;
    //     else if (open.getGlobalBounds().contains(worldPos))
    //         std::cout << "Open button pressed" << std::endl; //structure.open();
    //     else if (save.getGlobalBounds().contains(worldPos)) 
    //         std::cout << "Save button pressed" << std::endl; //structure.save();
    //     else if (scale.getGlobalBounds().contains(worldPos))
    //         std::cout << "Scale button pressed" << std::endl; //structure.scale();
    //     else if (import.getGlobalBounds().contains(worldPos))
    //         std::cout << "Import button pressed" << std::endl; //structure.import();
    //     else if (buildWall.getGlobalBounds().contains(worldPos)){
    //         // toggle the build wall mode
    //         if (currentMode == BuildWall)
    //             currentMode =  Select;
    //         else
    //             currentMode = BuildWall;
    //         std::cout << "Build wall button pressed" << std::endl;
    //     }
    // }


    // Handle panning mode. stop when the left button is released
    if (PANNING){
        if (inputs.rightReleased || inputs.leftReleased)
            PANNING = false;
        else{
            mainView.setCenter(mainView.getCenter() + inputs.worldPosOld - inputs.worldPos);
            window.setView(mainView);
            inputs.worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window)); // recaculate because of view change
            //inputs.worldPosOld = window.mapPixelToCoords(sf::Mouse::getPosition(window)); // recaculate because of view change
        }
    }     

    // handle the build wall mode
    if (currentMode == BUILD_WALL){
        if (inputs.spaceReleased){
            selectedWalls[0]->flip();
        }
        // update the wall
        if (BUILDING_WALL)
            editWall(inputs.worldPos);
        // start a new wall, finishes building current wall if one is active.
        if (inputs.leftReleased){
            BUILDING_WALL = true;
            unselectAllWalls();
            addWall(inputs.worldPos);}

        if (inputs.rightReleased && BUILDING_WALL){ // right click while building wall to remove the current wall
            removeWalls();
            BUILDING_WALL = false;}
        else if (inputs.rightReleased && !BUILDING_WALL){ // right click while not building wall to leave build wall mode
            BUILDING_WALL = false;
            unselectAllWalls();
            currentMode = SELECT;}
    }

    // Handle editWall mode. stop when the left button is released
    else if (currentMode == EDIT_WALL){
        if (inputs.leftReleased)   
            currentMode = SELECT;
        else
            editWall(inputs.worldPos);
    }

    // Handle moveWall mode. stop when the left button is released
    else if (currentMode == MOVE_WALL){
        if (PANNING)
            throw std::runtime_error("MoveWall mode and panning mode cannot be active at the same time. add a check for this.");
        if (inputs.leftReleased)   
            currentMode = SELECT;
        else{
            sf::Vector2f delta = inputs.worldPos - inputs.worldPosOld;
            moveWall(delta);
            inputs.worldPosOld = inputs.worldPos;
        }
    }
    
    if (inputs.rightPressed)
        PANNING = true;
    
    // user left clicked on *something*, decide what based on click location and current mode
    if (inputs.leftPressed){
        bool wallPressed = false;
        // check if the user clicked on a wall
        for (Wall& wall : walls){
                if (wall.contains(inputs.worldPos)){
                    wallPressed = true;
                    break;
                }
        }   
        if (!wallPressed)
            PANNING = true;      
        else if (selectedWalls.size() == 1){
            Wall* wall = selectedWalls[0];
            if (wall->selectPoint(inputs.worldPos))
                currentMode = EDIT_WALL;
            else if (wall->contains(inputs.worldPos))
                currentMode = MOVE_WALL;
        }
    }

    // if left released, user may be selecting item, check if they are
    // if shift is held, user could be selecting subsequent items
    if (inputs.leftReleased && currentMode == SELECT){
        for (Wall& wall : walls){
            if (wall.contains(inputs.worldPos)){
                if (inputs.Shift_held && wall.selected)
                    unselectWall(wall);
                else if (!inputs.Shift_held)
                    unselectAllWalls();
                selectWall(wall);     
                break;               
            }
        }
    }

    // user space key to split a wall
    if (inputs.spaceReleased && selectedWalls.size() == 1){
        splitWall(inputs.worldPos);
    }

    if (inputs.B_Released){ // Toggle build wall mode
        unselectAllWalls();
        if (currentMode == BUILD_WALL)
            currentMode =  SELECT;
        else 
            currentMode = BUILD_WALL;
    }
    // user D key to delete a wall, does nothing if no walls are selected
    if (inputs.D_Released){
        if (selectedWalls.size() > 0)
            removeWalls();
        currentMode = SELECT;
    }

    // reset the pressed and released flags so events are not triggered multiple times
    inputs.rightPressed = false;   
    inputs.rightReleased = false;
    inputs.leftPressed = false;
    inputs.leftReleased = false;
    inputs.spaceReleased = false;
    inputs.A_Released = false;
    inputs.B_Released = false;
    inputs.C_Released = false;
    inputs.D_Released = false; 
}