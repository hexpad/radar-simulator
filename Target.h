#ifndef TARGET_H
#define TARGET_H

#include "raylib.h"
#include <vector>

struct Target {
    int id{};
    Vector2 position{};
    Vector2 velocity{};
    float visibleTime{};
};

// Sabitler
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGTH = 600;
const float MAX_VISIBLE_TIME = 2.0f;
const float RADAR_RADIUS = 250.0f;
const int TARGET_COUNT = 10;
const float MAX_SPEED = 60.0f; // Saniyedeki piksel cinsinden
const float MIN_SPEED = 10.0f;

#endif // TARGET_H
