        
#include "../include/Wall.hpp"
#include "../include/Structure.hpp"
#include "../include/Inputs.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <iostream>
#include <string>   



int main(){

    Structure structure = Structure();
    Inputs inputs = Inputs();
    structure.loadFloorPlan("res/2-house-plan.png");

    // Create a window with a resolution of 800x600 and a title
    sf::RenderWindow window(sf::VideoMode(800, 600), "FloorPlan");
    window.setFramerateLimit(60);
    sf::Clock clock;
    sf::Font font;
    sf::View mainView= window.getDefaultView();
    sf::View textView = window.getDefaultView();

    
    if(!font.loadFromFile("res/MarkerFelt.ttc")) // load font
        return -1; // Handle font loading error

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

    sf::Vector2f oldWorldPos; // old position of the mouse in world coordinates
    sf::Vector2f worldPos; // mouse position in world coordinates
    sf::Vector2i oldWindowPos; // old position of the mouse in window coordinates
    sf::Vector2i windowPos; // mouse position in window coordinates

    std::cout << "Window created" << std::endl;
    // Main loop to keep the window open and handle events
    while (window.isOpen()){
        // Event handling
        sf::Event event;
        inputs.windowPos = sf::Mouse::getPosition(window);
        inputs.worldPos = window.mapPixelToCoords(inputs.windowPos);
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed)
                window.close();  // Close the window when the close event is triggered
            if (event.type == sf::Event::Resized){ // catch the resize events
                // update the view to the new size of the window
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                mainView.setSize(visibleArea.width, visibleArea.height);
                textView.setSize(visibleArea.width, visibleArea.height);
                window.setView(mainView);
            }
            if (event.type == sf::Event::MouseWheelMoved){ // catch the mouse wheel events
                if (event.mouseWheel.delta > 0)
                    mainView.zoom(1.1f);
                else if (event.mouseWheel.delta < 0)
                    mainView.zoom(0.9f);
            }
            if (event.type == sf::Event::MouseButtonPressed){ // catch the mouse button pressed events
                if (event.mouseButton.button == sf::Mouse::Left){
                    inputs.leftPressed = true;
                    inputs.leftHeld = true;}
                else if (event.mouseButton.button == sf::Mouse::Right){
                    inputs.rightPressed = true;
                    inputs.rightHeld = true;}
            }
            if (event.type == sf::Event::MouseButtonReleased){ // catch the mouse button released events
                if (event.mouseButton.button == sf::Mouse::Left){
                    inputs.leftPressed = false; 
                    inputs.leftHeld = false;
                    inputs.leftReleased = true;} 
                else if (event.mouseButton.button == sf::Mouse::Right){
                    inputs.rightPressed = false; 
                    inputs.rightHeld = false;
                    inputs.rightReleased = true;}
            }
            if (event.type == sf::Event::KeyPressed){ // catch the key pressed events
                if (event.key.code == sf::Keyboard::LShift) 
                    inputs.Shift_held = true;
            }
            if (event.type == sf::Event::KeyReleased){ // catch the key released events
                if (event.key.code == sf::Keyboard::Space)  inputs.spaceReleased = true;
                if (event.key.code == sf::Keyboard::A)      inputs.A_Released = true;
                if (event.key.code == sf::Keyboard::B)      inputs.B_Released = true;
                if (event.key.code == sf::Keyboard::C)      inputs.C_Released = true;
                if (event.key.code == sf::Keyboard::D)      inputs.D_Released = true; 
                if (event.key.code == sf::Keyboard::LShift) inputs.Shift_held = false;
            }
        }
        structure.update(window, mainView, inputs); // update the structure using new inputs
    
        window.clear(sf::Color::White); // Clear the window to White screen
        //Draw objects
        window.setView(mainView);
        window.draw(structure.floorPlanSprite);
        for (Wall& wall : structure.walls) // draw all walls
            window.draw(wall.shape);
        // draw selectable edit points if 1 wall is selected
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
        std::string modeString = structure.getModeString(structure.currentMode);
        modeText.setString("Mode: " + modeString);
        debugText.setString("view: " + std::to_string(mainView.getCenter().x) + " " + std::to_string(mainView.getCenter().y) + "\n" + "size: " + std::to_string(mainView.getSize().x) + " " + std::to_string(mainView.getSize().y));
        window.draw(modeText);
        window.draw(debugText);

        // Display the updated contents of the window
        window.setView(mainView);
        window.display();
        inputs.windowPosOld = inputs.windowPos;
        inputs.worldPosOld = inputs.worldPos;
    }

    return 0;
}

