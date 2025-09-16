#include "raymath.h"
#include "bullets.h"

int launch_bullet(uint32_t nbullets,bullet_t *bulletarr,Vector2 start_pos,Vector2 start_dir,float speed)
{
    uint32_t next_id = nbullets + 1;
    if(next_id > MAX_BULLETS)
    {
        return -1; // we've got too many bullets, but return and fail 
    }
    bullet_t new_bullet;
    new_bullet.bullet_pos = start_pos;
    new_bullet.bullet_ds = start_dir;
    new_bullet.id = next_id;
    new_bullet.org_pos = start_pos;
    new_bullet.bullet_ds = Vector2Scale(start_dir,speed); // scale the speed of the bullet
    bulletarr[next_id-1] = new_bullet;
    return 0;
}

void update_bullets(bullet_t *bullarr,uint32_t nbullets)
{
    for(uint32_t i=0;i<nbullets;i++)
    {
        bullarr[i].bullet_pos = Vector2Add(bullarr[i].bullet_pos,bullarr[i].bullet_ds);
    }
}


void draw_bullets(bullet_t *bullarr,uint32_t nbullets)
{
    float bullet_rad = 2.0f;
    for(uint32_t i=0;i<nbullets;i++)
    {
        DrawCircleV(bullarr[i].bullet_pos,bullet_rad,GREEN);
    }
}