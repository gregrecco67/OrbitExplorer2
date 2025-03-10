#pragma once
#include "raylib.h"
#include "raymath.h"

class Direction
{
public:
    //sf::CircleShape circle;
    Color circleFill{0, 0, 0, 0};
    int circleThickness{3};
    int size{40};
    Vector2 position{0, 0};
    Rectangle xAxis;
    Rectangle yAxis;
    Rectangle startPointer;
    float startAngle = 90.f;
    Rectangle velPointer;
    float velAngle = 90.f;

    Direction(Vector2 pos, int _size)
    {
        position = pos;
        size = _size;
        xAxis = {position.x, position.y + size, size*2.f, 1};
        yAxis = {position.x + size, position.y, 1, size*2.f};
        startPointer = {position.x+size, position.y+size, float(size+5), 1};
        velPointer = {position.x+size, position.y+size, float(size+5), 1};
    }

    void draw()
    {
        DrawCircleLinesV(position + Vector2{float(size), float(size)}, size, LIGHTGRAY);
        DrawRectangleRec(xAxis, LIGHTGRAY);
        DrawRectangleRec(yAxis, LIGHTGRAY);
        DrawRectanglePro(startPointer, {0, 0}, -startAngle, YELLOW);
        DrawRectanglePro(velPointer, {0, 0}, -velAngle, PURPLE);
    };

};
