#include "raylib.h"
#include "raymath.h"
#include <stdio.h>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800

#define BG_COLOR 0x1a1a1aFF

#define PLAYER_SIZE 10
#define FOV_LENGTH 300


// player
typedef struct Player {
    Vector2 position;
    float rotation;
    float fov;

    float moveSpeed;
    float rotSpeed;

    Color color;
} Player;


int main(void)
{
    // init player
    Player player    = {0};
    player.position  = (Vector2){500, 400};
    player.rotation  = 0.0f;
    player.moveSpeed = 2.0f;
    player.rotSpeed  = 1.0f;
    player.fov       = 60.0f;
    player.color     = BLUE;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    // init window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raycast");
    
    // get reefresh rate of monitor
    int refreshRate = GetMonitorRefreshRate(GetCurrentMonitor());
    printf("Refresh rate: %d\n", refreshRate);

    SetTargetFPS(refreshRate);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        {
            ClearBackground(GetColor(BG_COLOR));

            Rectangle box = {100, 100, 200, 200}; // your “cube”

            // ---- Raycast & draw rays ----
            const int   numRays   = 120;
            const float rayStep   = 2.5f;
            const float maxDist   = FOV_LENGTH;
            const float thickness = 1.2f;

            for (int i = 0; i < numRays; i++) {
                float t = (numRays == 1) ? 0.0f : (float)i/(numRays - 1);
                float rayAngle = player.rotation + player.fov*t;

                Vector2 dir = (Vector2){ cosf(DEG2RAD*rayAngle), sinf(DEG2RAD*rayAngle) };

                // march until hit or max
                Vector2 hit = player.position;
                float dist = 0.0f;
                for (; dist < maxDist; dist += rayStep) {
                    Vector2 p = Vector2Add(player.position, Vector2Scale(dir, dist));
                    if (CheckCollisionPointRec(p, box)) { 
                        hit = p;
                        DrawCircleV(hit, 1, RED);
                        break; 
                    }
                    hit = p; // last free point (or max)
                }

                // nice soft alpha by distance (closer = brighter)
                float distNorm = fminf(dist, maxDist) / maxDist;
                float alpha = 0.12f + (1.0f - distNorm) * 0.18f;
                DrawLineEx(player.position, hit, thickness, Fade(YELLOW, alpha));
            }
                
            // draw player
            DrawCircleV(player.position, PLAYER_SIZE, player.color);


            // handle input
            if (IsKeyDown(KEY_W)) {
                Vector2 forward = (Vector2){cosf(DEG2RAD*player.rotation), sinf(DEG2RAD*player.rotation)};
                player.position = Vector2Add(player.position, Vector2Scale(forward, player.moveSpeed));
            }
            if (IsKeyDown(KEY_S)) {
                Vector2 backward = (Vector2){-cosf(DEG2RAD*player.rotation), -sinf(DEG2RAD*player.rotation)};
                player.position = Vector2Add(player.position, Vector2Scale(backward, player.moveSpeed));
            }
            if (IsKeyDown(KEY_A)) {
                player.rotation -= player.rotSpeed;
            }
            if (IsKeyDown(KEY_D)) {
                player.rotation += player.rotSpeed;
            }
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
