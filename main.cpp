#include "Radar.h"

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGTH, "Radar Simulator");

    SetExitKey(KEY_NULL);

    SetTargetFPS(60);

    {
        RadarSystem radar({ SCREEN_WIDTH / 2.0f, SCREEN_HEIGTH / 2.0f });

        while (!WindowShouldClose()) {

            if (IsKeyPressed(KEY_ESCAPE)) break;

            radar.Update(GetFrameTime());

            BeginDrawing();
            radar.Draw();
            EndDrawing();
        }


    }

    CloseWindow();
    return 0;
}
