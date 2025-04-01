#include "../include/Wall.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include <vector>
#include <iostream>
#include <list>

// walls are defined by orgin at p1. p2 is the top right corner, 
// "top" and "bottom" are midpoints as shown below used for editing wall thickness
// p3 and p4 are the bottom right and bottom left corners respectively, used for corner snapping
//
//                           top
//  origin/p1 *---------------*---------------* p2
//            |                               |
//            |                               |
//            |                               |
//        p4  +---------------*---------------+  p3
//                          bottom
//
using namespace sf;



Wall::Wall(Vector2f& point, float thickness) : 
shape(RectangleShape(Vector2f(1, 1))), p1shape(CircleShape(.3)), p2shape(CircleShape(.3)), 
topshape(CircleShape(.3)), bottomshape(CircleShape(.3)) {
    p1 = Vector2f(point.x, point.y);
    p2 = p1 + Vector2f(1, 0);
    p3 = p1 + Vector2f(1, thickness);
    p4 = p1 + Vector2f(0, thickness);
    top = p1 + Vector2f(.5, 0);
    bottom = p1 + Vector2f(.5, thickness);
    calculateShapeFromPoints();
    selected = false;
    selectedPoint = P2; // initializes selected point to p2 for immediate editing
    shape.setFillColor(Color(200, 200, 200)); // Light grey RGB values
    shape.setOutlineThickness(-.1);
    shape.setOutlineColor(Color::Black);
    shape.setPosition(p1);
    p1shape.setFillColor(Color::Blue);
    p1shape.setOrigin(p1shape.getRadius(), p1shape.getRadius());
    p1shape.setPosition(p1);
    p2shape.setFillColor(Color::Blue);
    p2shape.setOrigin(p2shape.getRadius(), p2shape.getRadius());
    p2shape.setPosition(p2);
    topshape.setFillColor(Color::Blue);
    topshape.setOrigin(topshape.getRadius(), topshape.getRadius());
    topshape.setPosition(top);
    bottomshape.setFillColor(Color::Blue);
    bottomshape.setOrigin(bottomshape.getRadius(), bottomshape.getRadius());
    bottomshape.setPosition(bottom);
}


bool Wall::selectPoint(Vector2f& target) {

    // this is not the cleanest way to do this, but sf::Vector2f length() and lengthSquared() is not working and i have no idea why so for now its this.
    //could be a version problem. 
    // Also, std::min({a, b, c,...}) is not working either which is even weirder. thats a c++11 feature. and i'm definitely using post-c++11.
    std::vector<float> dists;
    float d1 = sqrt(pow(target.x - p1.x, 2) + pow(target.y - p1.y, 2));
    dists.push_back(d1);
    float d2 = sqrt(pow(target.x - p2.x, 2) + pow(target.y - p2.y, 2));
    dists.push_back(d2);
    float dtop = sqrt(pow(target.x - top.x, 2) + pow(target.y - top.y, 2));
    dists.push_back(dtop);
    float dbottom = sqrt(pow(target.x - bottom.x, 2) + pow(target.y - bottom.y, 2));
    dists.push_back(dbottom); 

    // Find minimum distance
    float minDist = *std::min_element(dists.begin(), dists.end());
    if (minDist > 1) {
        return false;
    }

    // Set selectedPoint to closest point
    if (minDist == d1) selectedPoint = P1;
    else if (minDist == d2) selectedPoint = P2;
    else if (minDist == dtop) selectedPoint = TOP;
    else if (minDist == dbottom) selectedPoint = BOTTOM;
    return true;
}

// tells the wall it is selected
void Wall::select() {
    shape.setOutlineColor(Color::Red);
    shape.setOutlineThickness(-.3);
    selected = true;
}

// tells the wall it is not selected
void Wall::unselect() {
    shape.setOutlineColor(Color::Black);
    shape.setOutlineThickness(-.1);
    selectedPoint = None;
    selected = false;
}   

void Wall::edit(Vector2f& destination) {
    
    if (selectedPoint == P1) {
        Vector2f p1p2 = p2 - p1;
        if (p1p2.x == 0 && p1p2.y == 0){//handles the case where p1 and p2 are the same point
            p1 = destination;
            p2 = p1 + Vector2f(1, 0);
            top = p1 + Vector2f(0.5, 0);
            bottom = p1 + Vector2f(0.5, 1);
            return;
        }
        p1 = destination;
        top = p1 + (p2 - p1)*.5f;
        Vector2f perpendicular = Vector2f(-p1p2.y, p1p2.x);
        float perp_length = sqrt(pow(perpendicular.x, 2) + pow(perpendicular.y, 2));
        if (perp_length == 0) 
            perp_length = 1;
        bottom = top + (perpendicular/perp_length*shape.getSize().y);
        calculateShapeFromPoints();
        return;

    // if selected point is p2, lock p1 and recalculate shape
    } else if (selectedPoint == P2) {
        Vector2f p1p2 = p2 - p1;
        if (p1p2.x == 0 && p1p2.y == 0){//handles the case where p1 and p2 are the same point
            p2 = destination;
            top = p1 + Vector2f(0.5, 0);
            bottom = p1 + Vector2f(0.5, 1); 
            return;
        }
        p2 = destination;
        top = p1 + (p2 - p1)*.5f;
        Vector2f perpendicular = Vector2f(-p1p2.y, p1p2.x);
        float perp_length = sqrt(pow(perpendicular.x, 2) + pow(perpendicular.y, 2));
        if (perp_length == 0) 
            perp_length = 1;
        bottom = top + (perpendicular/perp_length*shape.getSize().y);
        calculateShapeFromPoints();
        return;

    // if selected point is top, lock bottom and stretch only in shapes y direction
    } else if (selectedPoint == TOP) {
        Vector2f delta = destination - top;
        Vector2f direction = bottom - top;
        float dir_length = sqrt(pow(direction.x, 2) + pow(direction.y, 2));
        Vector2f normal = direction/dir_length;
        float vertical = delta.x*normal.x + delta.y*normal.y;
        top += normal * vertical;
        p1 += normal * vertical;
        p2 += normal * vertical;
        calculateShapeFromPoints();
        return;

    // if selected point is bottom, lock top and stretch only in shapes y direction
    } else if (selectedPoint == BOTTOM) {
        Vector2f delta = destination - bottom;
        Vector2f direction = bottom - top;
        float dir_length = sqrt(pow(direction.x, 2) + pow(direction.y, 2));
        Vector2f normal = direction/dir_length;
        float vertical = delta.x*normal.x + delta.y*normal.y;
        bottom += normal * vertical;
        calculateShapeFromPoints();
        return;
    }
    else
        // if we get here, selectedPoint is not valid, something has gone horribly wrong 
        throw std::runtime_error("Selected point is invalid - something has gone horribly wrong and you should feel bad.");
}   

void Wall::move(Vector2f& p) {
    p1 += p;
    p2 += p;
    top += p;
    bottom += p;
    calculateShapeFromPoints();
}     

// Rotates wall 180 degrees about "top" point , flips selected point {P1, P2}
// ONLY USED DURING BUILD_WALL MODE!! Nothing will break but why would this feature be used otherwise?
void Wall::flip() {
    Vector2f temp = p1;
    p1 = p2;
    p2 = temp;
    bottom = top + top - bottom;
    selectedPoint = selectedPoint == P1 ? P2 : P1;
    calculateShapeFromPoints();
}

std::vector<Vector2f> Wall::getPoints() {
    std::vector<Vector2f> points;
    Vector2f p3 = p2 + bottom - top;
    Vector2f p4 = p1 + bottom - top;
    points.push_back(p1);
    points.push_back(p2);
    points.push_back(p3);
    points.push_back(p4);
    return points;
}

void Wall::calculateShapeFromPoints() {
    float angle = atan2(p2.y - p1.y, p2.x - p1.x) * 180.0f / M_PI; // angle in degrees
    shape.setPosition(p1);
    shape.setRotation(angle);
    float x = sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
    float y = sqrt(pow(bottom.x - top.x, 2) + pow(bottom.y - top.y, 2));
    shape.setSize(Vector2f(x, y));
    p1shape.setPosition(p1);
    p2shape.setPosition(p2);
    topshape.setPosition(top);
    bottomshape.setPosition(bottom);
}

bool Wall::contains(Vector2f& point) {
    sf::Vector2f p3 = p2 + bottom - top;
    sf::Vector2f p4 = p1 + bottom - top;
    float triAreas = triArea(point,p1,p2) + triArea(point,p2,p3) + triArea(point,p3,p4) + triArea(point,p4,p1);
    float shapeArea = shape.getSize().x * shape.getSize().y;
    //if triAreas is greater than shapeArea, the point is outside the shape, return false.
    if (triAreas > shapeArea+1) { // +1 to account for floating point precision, may need to be adjusted
        return false;
    }
    return true;
}   

float Wall::triArea(Vector2f& A, Vector2f& B, Vector2f& C) {
    return abs( (B.x * A.y - A.x * B.y) + (C.x * B.y - B.x * C.y) + (A.x * C.y - C.x * A.y) ) / 2;
}

