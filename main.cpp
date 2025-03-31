#include <string>           
#include "Wall.h"
#include "Structure.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <iostream>

// Define operation modes
enum Mode {
    Select,
    BuildWall,
    EditWall, MoveWall,
    Panning,
    Zooming
};
Mode currentMode = Select;

std::string getModeString(Mode mode) {
    if (mode == Select)         return "Select";
    else if (mode == EditWall)  return "Editing Wall";
    else if (mode == MoveWall)  return "Moving Wall";
    else if (mode == BuildWall) return "Building Wall";
    else if (mode == Panning)   return "Panning";
    else if (mode == Zooming)   return "Zooming";
    else return "Unknown";
};

int main(){

    Structure structure = Structure();
    structure.loadFloorPlan("2-house-plan.png");

    // Create a window with a resolution of 800x600 and a title
    sf::RenderWindow window(sf::VideoMode(800, 600), "FloorPlan");
    window.setFramerateLimit(60);
    sf::Clock clock;
    sf::Font font;
    sf::View mainView= window.getDefaultView();
    sf::View textView = window.getDefaultView();
    

    // load font
    if(!font.loadFromFile("MarkerFelt.ttc")) 
        return -1; // Handle font loading error

    // 1 unit = 10 gameworld units
    

    
    // // Create menu area
    // sf::RectangleShape menu(sf::Vector2f(mainView.getSize().x, mainView.getSize().y*0.1));
    // menu.setPosition(0.f, 0.f);
    // menu.setFillColor(sf::Color::Yellow);

    // // Create Menubuttons
    // sf::Text openText("Open", font, 20);
    // openText.setFillColor(sf::Color::Black);
    // sf::RectangleShape open(sf::Vector2f(70.f, 30.f));
    // open.setFillColor(sf::Color::Blue);
    // open.setPosition(0.f, 0.f); 
    // open.setOutlineColor(sf::Color::Black);
    // open.setOutlineThickness(-2);
    // openText.setPosition(open.getPosition().x + 5, open.getPosition().y + 2);

    // sf::Text saveText("Save", font, 20);
    // saveText.setFillColor(sf::Color::Black);
    // sf::RectangleShape save(sf::Vector2f(70.f, 30.f));
    // save.setFillColor(sf::Color::Red);
    // save.setPosition(70.f, 0.f);
    // save.setOutlineColor(sf::Color::Black);
    // save.setOutlineThickness(-2);
    // saveText.setPosition(save.getPosition().x + 5, save.getPosition().y + 2);

    // sf::Text scaleText("Scale", font, 20);
    // scaleText.setFillColor(sf::Color::Black);
    // sf::RectangleShape scale(sf::Vector2f(70.f, 30.f)); 
    // scale.setFillColor(sf::Color::Yellow);
    // scale.setOutlineColor(sf::Color::Black);
    // scale.setOutlineThickness(-2);
    // scale.setPosition(140.f, 0.f);
    // scaleText.setPosition(scale.getPosition().x + 5, scale.getPosition().y + 2);

    // sf::Text importText("Import", font, 20);
    // importText.setFillColor(sf::Color::Black);
    // sf::RectangleShape import(sf::Vector2f(70.f, 30.f)); 
    // import.setFillColor(sf::Color::Green);
    // import.setOutlineColor(sf::Color::Black);
    // import.setOutlineThickness(-2);
    // import.setPosition(210.f, 0.f);
    // importText.setPosition(import.getPosition().x + 5, import.getPosition().y + 2);

    // sf::Text buildWallText("Build", font, 20);
    // buildWallText.setFillColor(sf::Color::Black);
    // sf::RectangleShape buildWall(sf::Vector2f(70.f, 30.f)); 
    // buildWall.setFillColor(sf::Color::Magenta);
    // buildWall.setOutlineColor(sf::Color::Black);
    // buildWall.setOutlineThickness(-2);
    // buildWall.setPosition(280.f, 0.f);
    // buildWallText.setPosition(buildWall.getPosition().x + 5, buildWall.getPosition().y + 2);

    // Mode text display
    sf::Text modeText("Mode: Select", font, 20);
    modeText.setFillColor(sf::Color::White);
    modeText.setOutlineColor(sf::Color::Black);
    modeText.setOutlineThickness(-1);
    modeText.setPosition(window.getSize().x - 200.f, 25.f);

    //Debug text
    sf::Text debugText("Debug Text", font, 20);
    debugText.setFillColor(sf::Color::White);
    debugText.setOutlineColor(sf::Color::Black);
    debugText.setOutlineThickness(-1);
    debugText.setPosition(window.getSize().x - 300.f, 50.f);

    // world variables and logic
    
    //left click variables
    float timeSinceLastLeftClick = 0.0f;
    bool leftPressed = false;
    bool leftReleased = false;  
    bool leftHeld = false;
    //right click variables
    float timeSinceLastRightClick = 0.0f;
    bool rightPressed = false;
    bool rightReleased = false;
    bool rightHeld = false; 
    //keyboard variables
    bool spaceReleased = false;
    bool A_Released = false;
    bool B_Released = false;
    bool C_Released = false;
    bool D_Released = false;
    bool Shift_held = false;
    
    bool panWhileBuilding = false;
    bool buildingWall = false; // true if the user is building a wall

    sf::Vertex line[2];
    Vector2f moveOldPos; // old position of the mouse for moving and panning
    sf::Vector2f worldPos; // mouse position in world coordinates
    sf::Vector2i windowPos; // mouse position in window coordinates

    std::cout << "Window created" << std::endl;
    // Main loop to keep the window open and handle events
    while (window.isOpen()){
        // Event handling
        sf::Event event;
        windowPos = sf::Mouse::getPosition(window);
        worldPos = window.mapPixelToCoords(windowPos);
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed)
                window.close();  // Close the window when the close event is triggered
            
            // catch the resize events
            if (event.type == sf::Event::Resized){
                // update the view to the new size of the window
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                mainView.setSize(visibleArea.width, visibleArea.height);
                textView.setSize(visibleArea.width, visibleArea.height);
                window.setView(mainView);
            }
            // catch the mouse wheel events
            if (event.type == sf::Event::MouseWheelMoved){
                if (event.mouseWheel.delta > 0)
                    mainView.zoom(1.1f);
                else if (event.mouseWheel.delta < 0)
                    mainView.zoom(0.9f);
            }

            // catch the mouse left button pressed events 
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
                leftPressed = true;
                timeSinceLastLeftClick = 0;
                clock.restart();
            }
            // catch the mouse left button released events 
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left){
                leftPressed = false;
                leftHeld = false;
                leftReleased = true;
                timeSinceLastLeftClick = 0;
            }
            // catch the mouse right button pressed events 
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right){
                rightPressed = true;
                timeSinceLastRightClick = 0;
                clock.restart();
            }
            // catch the mouse right button released events 
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right){
                rightPressed = false;
                rightHeld = false;
                rightReleased = true;
                timeSinceLastRightClick = 0;
            }
            // catch the key pressed events 
            if (event.type == sf::Event::KeyPressed){
                if (event.key.code == sf::Keyboard::LShift) Shift_held = true;
            }
            // catch the key released events 
            if (event.type == sf::Event::KeyReleased){
                if ( event.key.code == sf::Keyboard::Space) spaceReleased = true;
                if (event.key.code == sf::Keyboard::A) A_Released = true;
                if (event.key.code == sf::Keyboard::B)B_Released = true;
                if (event.key.code == sf::Keyboard::C) C_Released = true;
                if (event.key.code == sf::Keyboard::D) D_Released = true; 
                if (event.key.code == sf::Keyboard::LShift) Shift_held = false;
            }
        }
    

        // determine if mouse held 
        if (timeSinceLastLeftClick > 0.1f || leftPressed)
            timeSinceLastLeftClick += clock.getElapsedTime().asSeconds();
        if (timeSinceLastRightClick > 0.1f || rightPressed)
            timeSinceLastRightClick += clock.getElapsedTime().asSeconds();
        if (timeSinceLastLeftClick > 0.1f)
            leftHeld = true;
        if (timeSinceLastRightClick > 0.1f)
            rightHeld = true;
        

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

        // handle the build wall mode
        else if (currentMode == BuildWall){
            
            // update the wall
            if (buildingWall)
                structure.editWall(worldPos);
            // start a new wall, finishes building current wall if one is active.
            if (leftReleased){
                buildingWall = true;
                structure.unselectAllWalls();
                structure.addWall(worldPos);}

            if (rightReleased && buildingWall){ // right click while building wall to remove the current wall
                structure.removeWalls();
                buildingWall = false;}
            else if (rightReleased && !buildingWall){ // right click while not building wall to leave build wall mode
                buildingWall = false;
                structure.unselectAllWalls();
                currentMode = Select;}
        }

        // Handle editWall mode. stop when the left button is released
        else if (currentMode == EditWall){
            if (leftReleased)   
                currentMode = Select;
            else
                structure.editWall(worldPos);
        }

        // Handle moveWall mode. stop when the left button is released
        else if (currentMode == MoveWall){
            if (leftReleased)   
                currentMode = Select;
            else{
                Vector2f delta = worldPos - moveOldPos;
                structure.moveWall(delta);
                moveOldPos = worldPos;
            }
        }

        // Handle panning mode. stop when the left button is released
        else if (currentMode == Panning){
            if (leftReleased)
                currentMode = Select;
            else{
                mainView.setCenter(mainView.getCenter() + moveOldPos - worldPos);
                window.setView(mainView);
                moveOldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window)); // recaculate because of view change
            }
        }       

        // user left clicked on *something*, decide what based on click location and current mode
        else if (leftPressed){
            bool wallPressed = false;
            moveOldPos = worldPos;
            // check if the user clicked on a wall
            for (Wall& wall : structure.walls){
                    if (wall.contains(worldPos)){
                        wallPressed = true;
                        break;
                    }
            }   
            if (!wallPressed)
                currentMode = Panning;  
            else if (structure.selectedWalls.size() == 1){
                Wall* wall = structure.selectedWalls[0];
                if (wall->selectPoint(worldPos))
                    currentMode = EditWall;
                else if (wall->contains(worldPos))
                    currentMode = MoveWall;
            }
        }

        // if left released, user may be selecting item, check if they are
        // if shift is held, user could be selecting subsequent items
        if (leftReleased && (currentMode == Select || currentMode == Panning)){
            for (Wall& wall : structure.walls){
                if (wall.contains(worldPos)){
                    if (Shift_held && wall.selected)
                        structure.unselectWall(wall);
                    else if (!Shift_held)
                        structure.unselectAllWalls();
                    structure.selectWall(wall);     
                    break;               
                }
            }
        }
        

        // user right click to reset to select mode, doesn't happen if in build wall mode.
        // build wall mode right clicks are handled separately in the build wall handler
        if (rightPressed && currentMode != BuildWall){
            structure.unselectAllWalls();
            currentMode = Select;
        }

        // user space key to split a wall
        if (spaceReleased && structure.selectedWalls.size() == 1){
            structure.splitWall(worldPos);
        }

        // user B key to build walls
        if (B_Released){
            structure.unselectAllWalls();
            if (currentMode == BuildWall)
                currentMode =  Select;
            else 
                currentMode = BuildWall;
        }
        // user D key to delete a wall, does nothing if no walls are selected
        if (D_Released){
            if (structure.selectedWalls.size() > 0)
                structure.removeWalls();
            currentMode = Select;
        }
        // Clear the window
        window.clear(sf::Color::White);

        //Draw all
        window.setView(mainView);

        // // Create grid lines
        // int numVerticalLines = 101;
        // int numHorizontalLines = 101;
        // float xmin = mainView.getCenter().x - mainView.getSize().x/2;
        // float xmax = mainView.getCenter().x + mainView.getSize().x/2;
        // float ymin = mainView.getCenter().y - mainView.getSize().y/2;
        // float ymax = mainView.getCenter().y + mainView.getSize().y/2;
        // // Create vertical lines
        // for (int i = 0; i < numVerticalLines; i++) {
        //     float xloc = 10*i;
        //     if (xloc > xmin && xloc < xmax){
        //         line[0].position = sf::Vector2f(xloc, ymin);
        //         line[0].color = sf::Color::Black;
        //         line[1].position = sf::Vector2f(xloc, ymax);
        //         line[1].color = sf::Color::Black;
        //         window.draw(line, 2, sf::Lines);
        //     }  
        // }
        // // Create horizontal lines
        // for (int i = 0; i < numHorizontalLines; i++) {
        //     float yloc = 10*i;
        //     if (yloc > ymin && yloc < ymax){
        //         line[0].position = sf::Vector2f(xmin, yloc);
        //         line[0].color = sf::Color::Black;
        //         line[1].position = sf::Vector2f(xmax, yloc);
        //         line[1].color = sf::Color::Black;
        //         window.draw(line, 2, sf::Lines);
        //     }  
        // }
        
        window.draw(structure.floorPlanSprite);
    
        // draw all walls
        for (Wall& wall : structure.walls)
        {
            window.draw(wall.shape);
        }
        // draw selected walls
        if (structure.selectedWalls.size() == 1){
            Wall* wall = structure.selectedWalls[0];
            window.draw(wall->p1shape);
            window.draw(wall->p2shape);
            window.draw(wall->topshape);
            window.draw(wall->bottomshape);
        }

        // draw menu
         window.setView(textView);
        // window.draw(menu);
        // window.draw(open);
        // window.draw(openText);
        // window.draw(save);
        // window.draw(saveText);  
        // window.draw(scale);
        // window.draw(scaleText);
        // window.draw(import);
        // window.draw(importText);
        // window.draw(buildWall);
        // window.draw(buildWallText);
        std::string modeString = getModeString(currentMode);
        modeText.setString("Mode: " + modeString);
        debugText.setString("view: " + std::to_string(mainView.getCenter().x) + " " + std::to_string(mainView.getCenter().y) + "\n" + "size: " + std::to_string(mainView.getSize().x) + " " + std::to_string(mainView.getSize().y));
        window.draw(modeText);
        window.draw(debugText);

        // Display the updated contents of the window
        window.setView(mainView);
        window.display();
        // reset the release flags so events are not triggered multiple times
        rightPressed = false;   
        rightReleased = false;
        leftPressed = false;
        leftReleased = false;
        spaceReleased = false;
        A_Released = false;
        B_Released = false;
        C_Released = false;
        D_Released = false; 
    }

    return 0;
}

