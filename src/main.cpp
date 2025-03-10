#include "raylib.h"
#include "raymath.h"
#include "Planet.h"
#include "Slider.h"
#include "Direction.h"
#include <iostream>
#include <format>


int main(){
    InitWindow(1145, 745, "Orbit Explorer");
    SetTargetFPS(60);

    std::vector<int> codepoints;
    for (int i=32; i<128; i++) { codepoints.push_back(i); }
    Font font16 = LoadFontEx("resources/arial.ttf", 16, &codepoints[0], int(codepoints.size()));

    // window:
    int height = GetScreenHeight();
    int width = GetScreenWidth();
    Vector2 center = {width/2.f, height/2.f}; 

    Planet planet1{Vector2{320, 372}};
    planet1.startPoint = planet1.getPosition();
    Planet planet2{Vector2{255, 372}};
    planet2.startPoint = planet2.getPosition();
    planet2.circle.fillColor = Color{0xb2, 0x17, 0x0a, 255};
    
    Planet sun{center};
    sun.setFillColor(YELLOW);
    sun.circle.radius = 20;
    
    Slider forceSlider(10, 10, "Force");
    forceSlider.setRange(1000, 10000);
    forceSlider.setSliderValue(4000);
    Slider velSlider1(10, 70, "Velocity 1");
    velSlider1.setRange(0.0f, 10.0f);
    float initVel1 = 3.5f;
    velSlider1.setSliderValue(initVel1);
    Slider dirSlider1(10, 130, "Direction 1");
    dirSlider1.setRange(0.f, 360.f);
    float initAngle1 = 90.f;
    dirSlider1.setSliderValue(initAngle1);

    Slider velSlider2(10, 190, "Velocity 2");
    float initVel2 = 3.15f;
    float initAngle2 = 90.f;
    velSlider2.setRange(0.0f, 10.0f);
    velSlider2.setSliderValue(initVel2);
    Slider dirSlider2(10, 250, "Direction 2");
    dirSlider2.setRange(0.f, 360.f);
    dirSlider2.setSliderValue(initAngle2);

    Direction direction({10, 310}, 30);

    Slider massRatioSlider(10, 610, "Mass Ratio");
    massRatioSlider.setRange(1.0f, 2000.f);
    massRatioSlider.setSliderValue(200.f);
    Slider animSlider(10, 670, "Anim. Speed");
    animSlider.setRange(0.f, 5.f);
    animSlider.setSliderValue(1.5f);
    float animationSpeed{1.5f};
    
    planet1.velocity = {std::cosf(initAngle1 * DEG2RAD) * initVel1, 
        -std::sinf(initAngle1 * DEG2RAD) * initVel1};
    planet2.velocity = {std::cosf(initAngle2 * DEG2RAD) * initVel2, 
            -std::sinf(initAngle2 * DEG2RAD) * initVel2};
    

    float minVel1 = initVel1;
    float maxVel1 = initVel1;
    float minDist1 = Vector2Distance(planet1.getPosition(), sun.getPosition());
    float maxDist1 = minDist1;

    float minVel2 = initVel2;
    float maxVel2 = initVel2;
    float minDist2 = Vector2Distance(planet2.getPosition(), sun.getPosition());
    float maxDist2 = minDist2;


    const int TRACES_SIZE{1200};
    Vector2 traces[TRACES_SIZE];
    for (int i = 0; i < TRACES_SIZE; i++) {
        traces[i] = Vector2{-1, -1};
    }
    int traceIndex{0};
    int tillNextTrace{5};
    
    bool isTracing{true};
    bool isRunning{false};
    bool showHelp(true);

    while (!WindowShouldClose()){
        
        // poll input:
        if (IsKeyReleased(KEY_F)) {
            ToggleBorderlessWindowed();
        }
        if (IsKeyReleased(KEY_G)) { isRunning = !isRunning; } // 'G': go/stop
        if (IsKeyReleased(KEY_R)) { // 'R': reset planet1 to start position/velocity
            planet1.setPosition(planet1.startPoint);
            planet2.setPosition(planet2.startPoint);
            sun.setPosition(sun.startPoint);
            sun.velocity = Vector2{0, 0};
            float v = velSlider1.sliderValue;
            float a = dirSlider1.sliderValue;
            planet1.velocity = {std::cosf(a * DEG2RAD) * v, -std::sinf(a * DEG2RAD) * v};
            direction.velAngle = std::atan2(-planet1.velocity.y, planet1.velocity.x) * RAD2DEG;
            minVel1 = v; maxVel1 = v;
            minDist1 = Vector2Distance(planet1.getPosition(), sun.getPosition()); maxDist1 = minDist1;
            
            v = velSlider2.sliderValue;
            a = dirSlider2.sliderValue;
            planet2.velocity = {std::cosf(a * DEG2RAD) * v, -std::sinf(a * DEG2RAD) * v};
            //direction.velAngle = std::atan2(-planet1.velocity.y, planet1.velocity.x) * RAD2DEG;
            minVel2 = v; maxVel2 = v;
            minDist2 = Vector2Distance(planet2.getPosition(), sun.getPosition()); maxDist2 = minDist2;
        } 
        if (IsKeyReleased(KEY_T)) { isTracing = !isTracing; }  // 'T': trace planet1's path
        if (IsKeyReleased(KEY_C)) { // 'C': clear traces
            for (int i = 0; i < TRACES_SIZE; i++) {
                traces[i].x = -1.f;
            }
        }
        if (IsKeyReleased(KEY_SLASH)) {
            if (isRunning) { isRunning = false; }
            showHelp = !showHelp;
        }

        // check for resize
        int newheight = GetScreenHeight();
        int newwidth = GetScreenWidth();
        if (newheight != height || newwidth != width) {
            width = newwidth;
            height = newheight;
            Vector2 currentPlanet1VectorToCenter = center - planet1.getPosition();
            Vector2 currentPlanet2VectorToCenter = center - planet2.getPosition();
            Vector2 sunVectorToCenter = center - sun.circle.position;
            center = {width/2.f, height/2.f};
            sun.setPosition(center - sunVectorToCenter);
            planet1.setPosition(center - currentPlanet1VectorToCenter);
            planet2.setPosition(center - currentPlanet2VectorToCenter);
        }
        
        // read controls:
        float startDir = dirSlider1.sliderValue;
        direction.startAngle = startDir;
        
        // run sim:
        if (isRunning) {
            // inputs
            animationSpeed = animSlider.getSliderValue();
            float force = forceSlider.sliderValue; // 
            float distance1, distance2;
            const int divisor = 5; // number of time-fragments to simulate each frame
            for (int i = 0; i < divisor; i++) {
                // advance planets
                planet1.setPosition(planet1.getPosition() + planet1.velocity * animationSpeed / divisor);
                planet2.setPosition(planet2.getPosition() + planet2.velocity * animationSpeed / divisor);
                sun.setPosition(sun.getPosition() + sun.velocity * animationSpeed / divisor);
                // calculate and apply force
                Vector2 centripetalDir1 = (sun.circle.position - planet1.getPosition()); // whither the center
                Vector2 centripetalDir2 = (sun.circle.position - planet2.getPosition()); 
                distance1 = Vector2Length(centripetalDir1); // how far
                distance2 = Vector2Length(centripetalDir2);
                centripetalDir1 /= distance1; // normalize vectors
                centripetalDir2 /= distance2;
                float impulse1 = animationSpeed * force / (std::pow(distance1, 2) * divisor); // how strong a push?
                float impulse2 = animationSpeed * force / (std::pow(distance2, 2) * divisor); 
                float mass = massRatioSlider.getSliderValue();
                
                planet1.velocity += centripetalDir1 * impulse1; // apply push
                planet2.velocity += centripetalDir2 * impulse2;
                sun.velocity -= centripetalDir1 * impulse1 / mass; // same pushes to sun
                sun.velocity -= centripetalDir2 * impulse2 / mass; // up to mass ratio
            }
            // update readouts
            direction.velAngle = std::atan2(-planet1.velocity.y, planet1.velocity.x) * RAD2DEG;
            if (distance1 > maxDist1) { maxDist1 = distance1; }
            if (distance1 < minDist1) { minDist1 = distance1; }
            if (distance2 > maxDist2) { maxDist2 = distance2; }
            if (distance2 < minDist2) { minDist2 = distance2; }
            float velMag1 = Vector2Length(planet1.velocity);
            float velMag2 = Vector2Length(planet2.velocity);
            if (velMag1 > maxVel1) { maxVel1 = velMag1; }
            if (velMag1 < minVel1) { minVel1 = velMag1; }
            if (velMag2 > maxVel2) { maxVel2 = velMag2; }
            if (velMag2 < minVel2) { minVel2 = velMag2; }
        }
        
        // drawing:
        BeginDrawing();
        ClearBackground(BLACK);

        // controls:
        if (isTracing) {
            if (isRunning) {
                tillNextTrace -= 1;
                if (tillNextTrace == 0) {
                    tillNextTrace = 5;
                    traceIndex += 2;
                    if (traceIndex >= TRACES_SIZE - 1)
                        traceIndex = 0;
                    traces[traceIndex] = planet1.getPosition();
                    traces[traceIndex + 1] = planet2.getPosition();
                }
            }
            for (int i = 0; i < TRACES_SIZE; i++) {
                if (traces[i].x >= 0)
                    DrawPixelV(traces[i], WHITE);
            }
        }

        forceSlider.draw();
        velSlider1.draw();
        velSlider2.draw();
        dirSlider1.draw();
        dirSlider2.draw();
        direction.draw();
        animSlider.yCord = height - 85;
        animSlider.setSliderValue(animSlider.getSliderValue());
        animSlider.draw();
        massRatioSlider.yCord = height - 145;
        massRatioSlider.setSliderValue(massRatioSlider.getSliderValue());
        massRatioSlider.draw();

        // display measurements:
        float left = width - 240;
        float right = width - 150;
        DrawTextEx(font16, TextFormat("Vel1: %.2f", Vector2Length(planet1.velocity)), Vector2{left, 10}, 16, 0.8f, WHITE); // vel
        DrawTextEx(font16, TextFormat("Min / Max: %.2f / %.2f", minVel1, maxVel1), Vector2{right, 10}, 16, 0.8f, WHITE); // min/max vel
        DrawTextEx(font16, TextFormat("Vel2: %.2f", Vector2Length(planet2.velocity)), Vector2{left, 30}, 16, 0.8f, WHITE); // vel
        DrawTextEx(font16, TextFormat("Min / Max: %.2f / %.2f", minVel2, maxVel2), Vector2{right, 30}, 16, 0.8f, WHITE); // min/max vel

        DrawTextEx(font16, TextFormat("Dist1: %.2f", Vector2Length(planet1.getPosition() - center)), Vector2{left, 50}, 16, 0.8f, WHITE); // dist
        DrawTextEx(font16, TextFormat("Min / Max: %.2f / %.2f", minDist1, maxDist1), Vector2{right, 50}, 16, 0.8f, WHITE); // min/max dist
        DrawTextEx(font16, TextFormat("Dist2: %.2f", Vector2Length(planet2.getPosition() - center)), Vector2{left, 70}, 16, 0.8f, WHITE); // dist
        DrawTextEx(font16, TextFormat("Min / Max: %.2f / %.2f", minDist2, maxDist2), Vector2{right, 70}, 16, 0.8f, WHITE); // min/max dist

        DrawTextEx(font16, TextFormat("Start Pos1: (%03d, %03d)",  int(planet1.startPoint.x), int(planet1.startPoint.y)), Vector2{left, 90}, 16, 0.8f, WHITE);
        DrawTextEx(font16, TextFormat("Start Pos2: (%03d, %03d)",  int(planet2.startPoint.x), int(planet2.startPoint.y)), Vector2{left, 110}, 16, 0.8f, WHITE);
        DrawTextEx(font16, TextFormat("Current Pos1: (%03d, %03d)",  int(planet1.getPosition().x), int(planet1.getPosition().y)), Vector2{left, 130}, 16, 0.8f, WHITE);
        DrawTextEx(font16, TextFormat("Current Pos2: (%03d, %03d)",  int(planet2.getPosition().x), int(planet2.getPosition().y)), Vector2{left, 150}, 16, 0.8f, WHITE);


        // draw axes:
        DrawLineEx(Vector2{0, height/2.f}, Vector2{float(width), height/2.f}, 1.f, WHITE);
        DrawLineEx(Vector2{width/2.f, 0}, Vector2{width/2.f, float(height)}, 1.f, WHITE);

        //DrawTextEx(font16, TextFormat("W/H: %d/%d", newwidth, newheight), Vector2{500, 10}, 16, 0.8f, WHITE); // vel

        // draw bodies:
        sun.draw();
        planet1.draw(); // planet1 on top in case of overlap
        planet2.draw();
        
        
        // draw help screen:
        if (showHelp) {
            DrawRectangle(center.x - 88, center.y - 80, 176, 160, BLACK);
            DrawRectangleLines(center.x - 88, center.y - 80, 176, 160, WHITE);
            //'F': toggle fullscreen
            DrawTextEx(font16, "'F': toggle fullscreen", Vector2{center.x - 75, center.y - 70}, 16, 0.8f, WHITE);
            DrawTextEx(font16, "'G': go/stop", Vector2{center.x - 75, center.y - 50}, 16, 0.8f, WHITE);
            DrawTextEx(font16, "'R': reset", Vector2{center.x - 75, center.y - 30}, 16, 0.8f, WHITE);
            DrawTextEx(font16, "'T': trace orbit", Vector2{center.x - 75, center.y - 10}, 16, 0.8f, WHITE);
            DrawTextEx(font16, "'C': clear traces", Vector2{center.x - 75, center.y + 10}, 16, 0.8f, WHITE);
            DrawTextEx(font16, "'?': this help screen", Vector2{center.x - 75, center.y + 30}, 16, 0.8f, WHITE);
            DrawTextEx(font16, "drag planet1 for new start", Vector2{center.x - 75, center.y + 50}, 16, 0.8f, WHITE);        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

// web compilation:

// em++ -std=c++20 -o game.html src/main.cpp src/Slider.cpp -Os -Wall \
-I ~/dev/emsdk/upstream/emscripten/cache/sysroot/include \
-L ~/dev/emsdk/upstream/emscripten/cache/sysroot/lib/libraylib.a -s USE_GLFW=3 -s ASYNCIFY --preload-file resources \
--shell-file minshell.html -DPLATFORM_WEB ~/dev/emsdk/upstream/emscripten/cache/sysroot/lib/libraylib.a
//
