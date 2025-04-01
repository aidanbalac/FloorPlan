#include <SFML/Window.hpp>
#include "../include/Inputs.hpp"

Inputs::Inputs() {
    // Mouse position
    windowPos = sf::Vector2i(0, 0);
    windowPosOld = sf::Vector2i(0, 0);
    worldPos = sf::Vector2f(0, 0);
    worldPosOld = sf::Vector2f(0, 0);

    // Mouse buttons
    leftPressed = false;
    leftReleased = false;
    leftHeld = false;
    rightPressed = false;
    rightReleased = false;
    rightHeld = false;

    // Keyboard keys
    spaceReleased = false;
    A_Released = false; 
    B_Released = false;
    C_Released = false;
    D_Released = false;
    Shift_held = false;
}


