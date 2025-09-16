#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include "raylib.h"
#include "raymath.h"
#include "bullets.h"

typedef struct 
{
    Vector2 spritePos;
    float angle; // angle in radians
} playerPos_t;


int main()
{


    bool key_state = false;
    // number of bullets and bullets on the screen, MAX bullets defined in bullets.h
    uint32_t bullet_count = 0; 
    bullet_t bullets[MAX_BULLETS];

    const int windowX = 800;
    const int windowY = 600;

    Vector2 playerpos = {(float)windowX/2,(float)windowY/2};
    float playerAngle = PI/2; 
    float da = 10.0f;
    // delta X and delta Y
    float dy = 0.0f;
    float dx = 0.0f;
    float ds = 2.0f;

    InitWindow(windowX, windowY, "raylib [core] example - basic window"); 
    Texture2D tank_sp = LoadTexture("tanksprite.png");

    int frameWidth = tank_sp.width;
    int frameHeight = tank_sp.height;

    // Source rectangle (part of the texture to use for drawing)
    Rectangle sourceRec = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight };

    // Destination rectangle (screen rectangle where drawing part of texture)
    Rectangle destRec = { windowX/2.0f, windowY/2.0f, frameWidth*2.0f, frameHeight*2.0f };

    // Origin of the texture (rotation/scale point), it's relative to destination rectangle size
    Vector2 origin = { (float)frameWidth, (float)frameHeight };

    float deltaTime;
    SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        deltaTime = GetFrameTime();
        BeginDrawing();
        ClearBackground(BLACK);

        // grab keyborad input
        if(IsKeyDown(KEY_W)) 
        {
            playerpos.x += dx;
            playerpos.y += dy;
        }
        if(IsKeyDown(KEY_S)) 
        {
            playerpos.x -= dx;
            playerpos.y -= dy;
        }
        if(IsKeyDown(KEY_A)) 
        {

            printf("angle: %f\n",(playerAngle));
            playerAngle = playerAngle - da;
            // if(playerAngle >= 2*PI)
            // {
            //     playerAngle -= 2*PI;
            // }
            float p_angle_rad = playerAngle * (PI/180) - PI/2;
            dx = cosf(p_angle_rad)*ds;
            dy = sinf(p_angle_rad)*ds;
        }
        if(IsKeyDown(KEY_D)) 
        {
            printf("angle: %f\n",(playerAngle));
            playerAngle = playerAngle + da;
            // if(playerAngle < 0.0f)
            // {
            //     playerAngle += 2*PI;
            // } 

            float p_angle_rad = playerAngle * (PI/180) - PI/2;
            dx = cosf(p_angle_rad)*ds;
            dy = sinf(p_angle_rad)*ds;
        }
        if(IsKeyDown(KEY_SPACE))
        {
            if(key_state == false)
            {
                float init_speed = 10.0f;
                float bv = 10.0f;
                Vector2 pos = {dx,dy};
                Vector2 init_dir = Vector2Normalize(pos);
                printf("%s %d\n","launch a bullet!",bullet_count); 
                if(launch_bullet(bullet_count,bullets,playerpos,init_dir,init_speed) == 0)
                {
                    bullet_count ++;
                }
                key_state = true;
            }
        }
        else
        {
            key_state = false;
        }
        // grab gamepad input


        //destRec = { playerpos.x, playerpos.y, frameWidth*2.0f, frameHeight*2.0f };
        destRec.x = playerpos.x;
        destRec.y = playerpos.y;
        //float player_angle_deg = playerAngle * (180/PI);
        DrawTexturePro(tank_sp, sourceRec, destRec, origin,playerAngle, WHITE);
        // draw all the bullets
        draw_bullets(bullets,bullet_count);
        //DrawTexture(tank_sp,playerpos.x,playerpos.y, WHITE);
        //DrawCircleV(playerpos,20,RED);

        EndDrawing();

        // cycle through the bullets and update them
        update_bullets(bullets,bullet_count);
    }

    return 0;
}