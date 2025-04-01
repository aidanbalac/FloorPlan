#pragma once
#include <SFML/Window.hpp>

class Inputs {
public:
    Inputs();
    // Mouse position
    sf::Vector2i windowPos;
    sf::Vector2i windowPosOld;
    sf::Vector2f worldPos;
    sf::Vector2f worldPosOld;

    // Mouse buttons
    bool leftPressed;
    bool leftReleased;
    bool leftHeld;  
    bool rightPressed;
    bool rightReleased;
    bool rightHeld;

    // Keyboard keys
    bool spaceReleased;
    bool A_Released;
    bool B_Released;
    bool C_Released;
    bool D_Released;
    bool Shift_held;
};

