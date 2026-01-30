#ifndef RADAR_H
#define RADAR_H

#include "Target.h"
#include <fstream>
#include <string>

class RadarSystem{
public:
    RadarSystem(Vector2 center);
    ~RadarSystem();

    void Update(float dt);
    void Draw();

private:
    Vector2 radarCenter{};
    float angle{};
    float rotationSpeed{};
    std::vector<Target> targets{};
    std::ofstream logFile{};

    void InitializeTargets();
    void HandleLogging(Target& t, float distance, float angleDeg);
    void ApplyDynamicMovement(Target& t, float dt);
    int selectedTargetId = -1; // -1: Hiçbir hedef seçili değil

};

#endif // RADAR_H
