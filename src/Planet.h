#pragma once
#include "raylib.h"
#include "Circle.h"
#include <cmath>

class Planet
{
public:
    Circle circle;
    bool isDragging = false;
    Vector2 velocity;
    Vector2 startPoint;

    Planet(Vector2 _position)
    {
        circle.position = _position;
        circle.radius = 10;
    }

    void draw()
    {
        logic();
        circle.draw();
    };

    void logic()
    {
        bool pressed = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
        Vector2 mousePos = GetMousePosition();

        if (!isDragging && std::abs(mousePos.x - circle.position.x) < circle.radius 
            && std::abs(mousePos.y - circle.position.y) < circle.radius && pressed)
        {
            isDragging = true;
        }
        if (isDragging && !pressed)
        {
            isDragging = false;
        }
        if (isDragging)
        {
            circle.position = Vector2{mousePos.x, mousePos.y};
            startPoint = Vector2{mousePos.x, mousePos.y};
        }
    };

    void setFillColor(Color newColor) {
        circle.fillColor = newColor;
    }

    Vector2 getPosition() {
        return Vector2{circle.position.x, circle.position.y};
    }

    void setPosition(Vector2 newpos) {
        circle.position = newpos;
    }
};
