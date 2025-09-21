#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <MQTTClient.h> // paho mqtt
#include <stdint.h>
#include "raylib.h"
#include "raymath.h"
#include "bullets.h"
#include "map.h"
#include "toml.h"

typedef struct 
{
    Vector2 spritePos;
    float angle; // angle in radians
    uint16_t bullets; // how many bullets the tank has
    // TODO: add a type for ammo type
} player_t;

// update player
void update_player(player_t *player,map_t *worldmap);

int main(int argc,char **argv)
{
    // define the map that we well be playing on
    int gamepad = 0;
    map_t gamemap;
    random_walk(&gamemap,MAP_WIDTH,MAP_HEIGHT,0.74,-1);

    bool key_state = false;
    // number of bullets and bullets on the screen, MAX bullets defined in bullets.h
    uint32_t bullet_count = 0; 
    bullet_t bullets[MAX_BULLETS];

    int windowX = 1600;
    int windowY = 900;


    // if(argc > 1 && strcmp(argv[1],"-fullscreen") == 0)
    // {
    //     SetConfigFlags(FLAG_FULLSCREEN_MODE);
    //     InitWindow(GetScreenWidth(), GetScreenHeight(), "My Fullscreen Game");
    //     windowX = GetScreenWidth();
    //     windowY = GetScreenHeight();
    // }
    // else
    // {
    //     InitWindow(windowX, windowY, "MQTT TANK GAME"); 
    // }

    // better way to handle command line arguments
    // options
    bool fullscreen = false;
    int cfgfile_index = 0;
    char cfgfileBuffer[80];

    if(argc > 1)
    {
        for(int i=0;i<argc;i++)
        {
            if(strcmp(argv[i],"-fullscreen") == 0)
            {
                fullscreen = true;
            }
            else if(strcmp(argv[i],"-configfile") == 0)
            {
                cfgfile_index = i;
            }
        }
    }

    if(cfgfile_index > 0 && (cfgfile_index+1) < argc && strlen(argv[cfgfile_index+1]) > 0)
    {
        strcpy(cfgfileBuffer,argv[cfgfile_index+1]);
        printf("toml file: %s\n",cfgfileBuffer);
    }

    // after the argus are parsed run the options
    if(fullscreen)
    {
        SetConfigFlags(FLAG_FULLSCREEN_MODE);
        InitWindow(GetScreenWidth(), GetScreenHeight(), "My Fullscreen Game");
        windowX = GetScreenWidth();
        windowY = GetScreenHeight();
    }
    else
    {
        InitWindow(windowX, windowY, "MQTT TANK GAME"); 
    }

    // basicly instance variables
    player_t player;

    Vector2 playerpos = {(float)windowX/2,(float)windowY/2};
    float playerAngle = PI/2; 
    float da = 10.0f;
    // delta X and delta Y
    float dy = 0.0f;
    float dx = 0.0f;
    float ds = 2.0f;

    // init some values
    player.spritePos = playerpos;
    player.angle = playerAngle;
    player.bullets = 99; // starting number of bullets

    //init the game camera to view the player
    Camera2D camera = {0};
    camera.target = playerpos;
    camera.offset = (Vector2){ windowX/2.0f, windowY/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

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
        // BeginDrawing();
        // ClearBackground(BLACK);

        // grab keyborad input
        if(IsKeyDown(KEY_W) || IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_UP)) 
        {
            printf("%f\n",player.angle);
            // do a check to figure out if we're less than half than half the window width from the side of the wall
            if(player.spritePos.x > 400)
            {

                player.spritePos.x += dx;
                player.spritePos.y += dy;
            }
        }
        if(IsKeyDown(KEY_S)) 
        {
            player.spritePos.x -= dx;
            player.spritePos.y -= dy;
        }
        if(IsKeyDown(KEY_A)) 
        {

            //printf("angle: %f\n",(playerAngle));
            player.angle = player.angle - da; 
            playerAngle = playerAngle - da;
            // if(playerAngle >= 2*PI)
            // {
            //     playerAngle -= 2*PI;
            // }
            //float p_angle_rad = playerAngle * (PI/180) - PI/2;
            float p_angle_rad = player.angle * (PI/180) - PI/2;
            //float p_angle_rad = player.angle * (PI/180) - PI/2;
            dx = cosf(p_angle_rad)*ds;
            dy = sinf(p_angle_rad)*ds;
        }
        if(IsKeyDown(KEY_D)) 
        {
            //printf("angle: %f\n",(playerAngle));
            player.angle = player.angle + da; 
            playerAngle = playerAngle + da; 
            // if(playerAngle < 0.0f)
            // {
            //     playerAngle += 2*PI;
            // } 

            //float p_angle_rad = playerAngle * (PI/180) - PI/2;
            float p_angle_rad = player.angle * (PI/180) - PI/2;
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
                if(launch_bullet(bullet_count,bullets,player.spritePos,init_dir,init_speed) == 0)
                {
                    bullet_count ++;
                    player.bullets --;
                }
                key_state = true;
            }
        }
        else
        {
            key_state = false;
        }
        
        // update the camera
        camera.target = (Vector2){player.spritePos.x,player.spritePos.y};

        BeginDrawing();
            BeginMode2D(camera);
                ClearBackground(BLACK);

                draw_map(&gamemap,0,0,MAP_WIDTH,MAP_HEIGHT);

                destRec.x = player.spritePos.x;
                destRec.y = player.spritePos.y; 
                //float player_angle_deg = playerAngle * (180/PI);
                DrawTexturePro(tank_sp, sourceRec, destRec, origin,playerAngle, WHITE);
                // draw all the bullets
                draw_bullets(bullets,bullet_count);
                //DrawTexture(tank_sp,playerpos.x,playerpos.y, WHITE);
                //DrawCircleV(playerpos,20,RED);
            EndMode2D();
        EndDrawing();

        // cycle through the bullets and update them
        update_bullets(bullets,bullet_count);
    }

    return 0;
}


void update_player(player_t *player,map_t *worldmap)
{

}