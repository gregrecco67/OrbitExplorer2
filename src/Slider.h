#pragma once
#include "raylib.h"
#include <algorithm>
#include <vector>

class RectEx : public Rectangle {
    public:
        Vector2 origin{0, 0}; // subtract this offset when drawing or when repositioning
        bool contains(Vector2 pt) { 
            if(
                pt.x >= this->x && pt.x <= this->x + this->width &&
                pt.y >= this->y && pt.y <= this->y + this->height
            )
            { return true; }
            else { return false; }
        }
        void setOrigin(Vector2 newOrigin) {
            this->origin =  newOrigin;
        }
        void draw() {
            DrawRectangle(this->x - origin.x, this->y - origin.y, this->width, this->height, DARKGRAY);
        }
        void draw(Color color) {
            DrawRectangle(this->x - origin.x, this->y - origin.y, this->width, this->height, color);
        }
        void setPosition(Vector2 newPos) {
            this->x = newPos.x - origin.x;
            this->y = newPos.y - origin.y; 
        }
};

class Slider
{
public:
    float minValue;
    float maxValue;
    int xCord;
    int yCord;
    RectEx axis; // x, y, width, height
    RectEx slider;
    int axisWidth;
    int axisHeight;
    int sliderWidth;
    int sliderHeight;
    float sliderValue;
    bool isDragging = false;
    const char* name;
    Font font16;
    Font font12;
    std::vector<int> codepoints;

    Slider(int x, int y);
    Slider(int x, int y, const char* _name);
    void setRange(float min, float max);
    void logic();
    float getSliderValue();
    void setSliderValue(float newValue);
    void draw();
    void setName(char* _name);
};
