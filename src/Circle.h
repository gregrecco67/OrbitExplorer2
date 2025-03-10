#pragma once
#include "raylib.h"

class Circle {
    public:
        Vector2 position{0, 0};
        Color fillColor{DARKBLUE};
        float radius;
        Circle() {}

        void draw() {
            DrawCircle(position.x, position.y, radius, fillColor);
        }
};