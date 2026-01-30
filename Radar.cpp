#include "Radar.h"
#include "raymath.h"
#include <cmath>
#include <ctime>

RadarSystem::RadarSystem(Vector2 center)
    :radarCenter(center), angle(0.0f), rotationSpeed(1.0f)
{
    logFile.open("radar_log.csv");
    logFile << "time,target_id,x,y,distance,angle\n";
    InitializeTargets();
}

RadarSystem::~RadarSystem()
{
    if(logFile.is_open()) logFile.close();
}

void RadarSystem::InitializeTargets()
{
    for(int i=0; i < TARGET_COUNT; i++)
    {
        float r = GetRandomValue(0, (int)RADAR_RADIUS);
        float a = GetRandomValue(0, 360) * DEG2RAD;

        Vector2 pos =
            {
                radarCenter.x + cosf(a) * r, radarCenter.y + sinf(a) * r
            };
        Vector2 vel =
            {
                GetRandomValue(-30, 30) / 10.0f, GetRandomValue(-30, 30) / 10.0f
            };

        targets.push_back({i, pos, vel, 0.0f });
    }
}

void RadarSystem::ApplyDynamicMovement(Target& t, float dt)
{
    // Rastgele ivme: %2 ihtimalle rotada küçük bir sapma yarat
    if (GetRandomValue(0, 100) < 2)
    {
        Vector2 acceleration =
        {
            (float)GetRandomValue(-20, 20), (float)GetRandomValue(-20, 20)
    };
    t.velocity = Vector2Add(t.velocity, Vector2Scale(acceleration, dt * 10.0f));

    // Hız limitleme (Hızın çok artmasını veya durmasını engelle)
    float speed = Vector2Length(t.velocity);
    if (speed > MAX_SPEED) t.velocity = Vector2Scale(Vector2Normalize(t.velocity), MAX_SPEED);
    if (speed < MIN_SPEED) t.velocity = Vector2Scale(Vector2Normalize(t.velocity), MIN_SPEED);
}

// Pozisyon güncelleme
t.position = Vector2Add(t.position, Vector2Scale(t.velocity, dt));

// Sınır kontrolü (Radar dışına çıkarsa merkeze yönelt)
if (Vector2Distance(t.position, radarCenter) > RADAR_RADIUS)
{
    Vector2 toCenter = Vector2Normalize(Vector2Subtract(radarCenter, t.position));
    t.velocity = Vector2Scale(toCenter, Vector2Length(t.velocity));
}
}

void RadarSystem::Update(float dt)
{
    float radarAngle = angle - PI / 2;
    angle += rotationSpeed * dt;
    if(angle > 2 * PI)
    {
        angle -= 2 * PI;
    }

    Vector2 radarDir
    {
        cosf(radarAngle), sinf(radarAngle)
    };

    // Mouse ile hedef seçme kontrolü
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 mousePos = GetMousePosition();
        bool found = false;

        for (const auto& t : targets) {
            // Sadece görünür (tespit edilmiş) hedeflere tıklanabilir
            if (t.visibleTime > 0.0f && CheckCollisionPointCircle(mousePos, t.position, 15.0f))
            {
                selectedTargetId = t.id;
                found = true;
                break;
            }
        }
        if (!found) selectedTargetId = -1; // Boşluğa tıklanırsa seçimi iptal et
    }

    for(auto&t : targets)
    {
        ApplyDynamicMovement(t, dt);

        // Tarama hattı kontrolü
        Vector2 toTarget = Vector2Normalize(Vector2Subtract(t.position, radarCenter));
        float dot = Vector2DotProduct(radarDir, toTarget);

        if(dot > 0.9999f)
        {
            float distance = Vector2Distance(t.position,radarCenter);
            Vector2 diff = Vector2Subtract(t.position, radarCenter);
            float targetAngle = atan2f(diff.y, diff.x);
            float compassAngle = targetAngle + PI / 2;

            if(compassAngle < 0)
                compassAngle += 2 * PI;

            float angleDeg = compassAngle * RAD2DEG;

            if(t.visibleTime <= 0.0f)
            {
                HandleLogging(t, distance, angleDeg);
            }
            t.visibleTime = MAX_VISIBLE_TIME;
        }
        if (t.visibleTime > 0.0f) t.visibleTime -= dt;
    }
}

void RadarSystem::HandleLogging(Target& t, float distance, float angleDeg)
{
    std::time_t tnow = std::time(nullptr);
    std::tm* lt = std::localtime(&tnow);
    char timeStr[20];
    std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", lt);

    logFile << timeStr << "," << t.id << "," << t.position.x << "," << t.position.y
            << "," << distance << "," << angleDeg << "\n";
}

void RadarSystem::Draw()
{
    // Trail Effect
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.15f));

    // Background Rings & Labels
    const int ringCount = 5;
    float ringStep = RADAR_RADIUS / ringCount;
    for(int i = 1; i <= ringCount; i++)
    {
        DrawCircleLines((int)radarCenter.x, (int)radarCenter.y, ringStep * i, Fade(GREEN, 0.3f));
        DrawText(TextFormat("%.0f", ringStep * i), radarCenter.x + 5, radarCenter.y - ringStep * i + 5, 10, Fade(GREEN, 0.5f));
    }

    // Grid Lines (bearingStep)
    const int bearingStep = 30;
    for(int deg = 0; deg < 360; deg += bearingStep)
    {
        float rad = deg * DEG2RAD - PI / 2;
        Vector2 end =
            {
                radarCenter.x + cosf(rad) * RADAR_RADIUS, radarCenter.y + sinf(rad) * RADAR_RADIUS
            };
        DrawLineV(radarCenter, end, Fade(GREEN, 0.2f));
    }

    // Outer Ticks & Angle Degrees
    for(int deg = 0; deg < 360; deg += 30)
    {
        float rad = deg * DEG2RAD - PI / 2;
        Vector2 outer =
            {
                radarCenter.x + cosf(rad) * RADAR_RADIUS, radarCenter.y + sinf(rad) * RADAR_RADIUS
            };
        Vector2 inner =
        {
            radarCenter.x + cosf(rad) * (RADAR_RADIUS - 12), radarCenter.y + sinf(rad) * (RADAR_RADIUS - 12)
    };
    DrawLineV(inner, outer, GREEN);
    Vector2 textPos =
    {
        radarCenter.x + cosf(rad) * (RADAR_RADIUS + 14), radarCenter.y + sinf(rad) * (RADAR_RADIUS + 14)
};
DrawText(TextFormat("%d", deg), (int)textPos.x - 10, (int)textPos.y - 5, 10, GREEN);
}

for (const auto& t : targets)
{
    if (t.visibleTime > 0.0f)
    {
        float alpha = t.visibleTime / MAX_VISIBLE_TIME;
        DrawCircleV(t.position, 4.0f, Fade(RED, alpha));

        // Lock-on square
        if (t.id == selectedTargetId)
        {
            DrawRectangleLines(t.position.x - 10, t.position.y - 10, 20, 20, LIME);
            DrawLine(t.position.x - 12, t.position.y, t.position.x + 12, t.position.y, LIME);
            DrawLine(t.position.x, t.position.y - 12, t.position.x, t.position.y + 12, LIME);
        }
    }
}

// Data Panel
DrawRectangle(580, 20, 200, 140, Fade(DARKGRAY, 0.1f));
DrawRectangleLines(580, 20, 200, 140, GREEN);
DrawText("TARGET INFO", 600, 30, 15, GREEN);

if (selectedTargetId != -1)
{
    const Target& st = targets[selectedTargetId];
    float dist = Vector2Distance(st.position, radarCenter);
    float speed = Vector2Length(st.velocity);

    DrawText(TextFormat("ID: %02d", st.id), 600, 60, 12, WHITE);
    DrawText(TextFormat("RANGE: %.1f m", dist), 600, 80, 12, WHITE);
    DrawText(TextFormat("SPEED: %.1f km/h", speed * 10.0f), 600, 100, 12, WHITE);
    DrawText(TextFormat("STATUS: TRACKING", selectedTargetId), 600, 120, 12, LIME);
} else{
    DrawText("NO TARGET SELECTED", 595, 80, 12, GRAY);
}

// Rotating Radar Line
float radarAngle = angle - PI / 2;
Vector2 lineEnd = { radarCenter.x + cosf(radarAngle) * RADAR_RADIUS, radarCenter.y + sinf(radarAngle) * RADAR_RADIUS };
DrawLineEx(radarCenter, lineEnd, 2.0f, GREEN);
}
