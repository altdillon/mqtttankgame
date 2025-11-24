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
    new_bullet.ticks = 0;
    new_bullet.bullet_pos = start_pos;
    new_bullet.bullet_ds = start_dir;
    new_bullet.id = next_id;
    new_bullet.org_pos = start_pos;
    new_bullet.bullet_ds = Vector2Scale(start_dir,speed); // scale the speed of the bullet
    new_bullet.damage = 20; // initional amount of hit points or damage a bullet can do
    bulletarr[next_id-1] = new_bullet;
    return 0;
}

uint32_t enforce_bullet_range(bullet_t *bullarr,uint32_t nbullets)
{
    uint32_t updated_nbullets = nbullets;
    for(uint32_t i=0;i<updated_nbullets;i++)
    {
        if(bullarr[i].ticks > MAX_TICKS)
        {
            // null out that element and shift the array down
            for(uint32_t j=i;j<updated_nbullets;j++)
            {
                bullarr[j] = bullarr[j+1];
            }
            updated_nbullets--;
        }
    }

    return updated_nbullets;
}

void update_bullets(bullet_t *bullarr,uint32_t nbullets)
{
    
    // update all the bullets that havn't been removed
    for(uint32_t i=0;i<nbullets;i++)
    {
        bullarr[i].bullet_pos = Vector2Add(bullarr[i].bullet_pos,bullarr[i].bullet_ds);
        bullarr[i].ticks++;
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