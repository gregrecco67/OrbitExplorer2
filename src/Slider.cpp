#include "Slider.h"

Slider::Slider(int x, int y)
{
    for (int i=32; i<128; i++) { codepoints.push_back(i); }
    font16 = LoadFontEx("resources/arial.ttf", 16, &codepoints[0], int(codepoints.size()));
    font12 = LoadFontEx("resources/arial.ttf", 12, &codepoints[0], int(codepoints.size()));
	xCord = x;
	yCord = y;
	axisHeight = 10;
	axisWidth = 150;
	sliderWidth = 20;
	sliderHeight = 25;

	axis.x = xCord;
    axis.y = yCord + sliderHeight/2.f - axisHeight/2.f;
    axis.height = axisHeight;
    axis.width = axisWidth;

    slider.x = xCord;
    slider.y = yCord;
    slider.height = sliderHeight;
    slider.width = sliderWidth;
}

Slider::Slider(int x, int y, const char* _name) : name(_name) {
    for (int i=32; i<128; i++) { codepoints.push_back(i); }
    font16 = LoadFontEx("resources/arial.ttf", 16, &codepoints[0], int(codepoints.size()));
    font12 = LoadFontEx("resources/arial.ttf", 12, &codepoints[0], int(codepoints.size()));
	xCord = x;
	yCord = y;
	axisHeight = 10;
	axisWidth = 150;
	sliderWidth = 20;
	sliderHeight = 30;

	axis.x = xCord;
    axis.y = yCord + sliderHeight/2.f - axisHeight/2.f;
    axis.height = axisHeight;
    axis.width = axisWidth;

    slider.x = xCord;
    slider.y = yCord;
    slider.height = sliderHeight;
    slider.width = sliderWidth;
}

void Slider::setRange(float min, float max)
{
	sliderValue = min;
	minValue = min;
	maxValue = max;
}

void Slider::logic()
{
	bool pressed = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
	Vector2 mousePos = GetMousePosition();
	if (!isDragging && slider.contains(mousePos) && pressed)
	{
		isDragging = true;
	}
	if (isDragging && !pressed)
	{
		isDragging = false;
	}
	if (isDragging)
	{
		if (mousePos.x >= xCord && mousePos.x <= xCord + axisWidth && 
            mousePos.y >= slider.y && mousePos.y <= slider.y + sliderHeight)
		{
			float newX = int(std::max(float(xCord), mousePos.x - sliderWidth/2.f));
			float newestX = int(std::min(xCord + axisWidth - sliderWidth, int(newX)));
			slider.x = newestX;
			sliderValue = minValue + ((maxValue - minValue) * (slider.x - xCord) / (axisWidth - sliderWidth));
		}
	}
}

float Slider::getSliderValue()
{
	return sliderValue;
}

void Slider::setSliderValue(float newValue)
{
	if (newValue >= minValue && newValue <= maxValue)
	{
		sliderValue = newValue;
		float diff = maxValue - minValue;
		float diff2 = newValue - minValue;
		float zzz = axisWidth / diff;
		float posX = zzz * diff2;
		posX += xCord;
		slider.x = posX;
        slider.y = yCord;
		axis.y = yCord + sliderHeight/2.f - axisHeight/2.f;
	}
}


void Slider::setName(char * _name)
{
	name = _name;
}

void Slider::draw()
{
	logic();
    DrawTextEx(font16, TextFormat("%.2f", minValue), Vector2{float(xCord - 10), float(yCord + sliderHeight)}, 16, .8f, WHITE); // min
    axis.draw();
    DrawTextEx(font16, TextFormat("%.2f", maxValue), Vector2{float(xCord + axisWidth - 10), float(yCord + sliderHeight)}, 16, .8f, WHITE); // max
    slider.draw(WHITE);
    DrawTextEx(font12, TextFormat("%.2f", sliderValue), Vector2{slider.x, float(yCord - 10)}, 12, .6f, WHITE); // value
	DrawTextEx(font16, TextFormat("%s", name), Vector2{float(xCord + axisWidth + 5), float(yCord)}, 16, .8f, WHITE); // name
	
}
