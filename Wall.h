#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

using namespace sf;


//
// wall is defined by orgin at po, with subseqent points rotating clockwise.
// p2 is the top right corner, p3 is the bottom right corner, p4 is the bottom left corner
//                           p1
//  origin/p0 .______________o_______________. p2
//            |                              |
//         p7 o                              o p3
//            |                              |
//         p6 ---------------o---------------- p4  
//                           p5

enum selectedPoint {
    None,
    P1,
    P2,
    TOP,
    BOTTOM
};

class Wall {
public:
    Wall(Vector2f& point, float thickness);
    RectangleShape shape;
    CircleShape p1shape;
    CircleShape p2shape;
    CircleShape topshape;
    CircleShape bottomshape;
    Vector2f p1;
    Vector2f p2;
    Vector2f top;
    Vector2f bottom;
    bool selected;
    selectedPoint selectedPoint;
    
    bool selectPoint(Vector2f& target);
    void edit(Vector2f& destination);
    void move(Vector2f& p);
    void select();  
    void unselect();
    void calculatePointsFromShape();
    void calculateShapeFromPoints();
    bool contains(Vector2f& point);
    float triArea(Vector2f& p1, Vector2f& p2, Vector2f& p3);
};
