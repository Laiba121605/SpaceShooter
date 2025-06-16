#include "EnemyShip.h"
#include <cmath>
#include "raylib.h"

EnemyShip::EnemyShip(int posx, int posy, const char* path)
    : Ship(posx, posy, RED, path), firecooldown(0), speed(2.5f)
{
}

void EnemyShip::rotatetowards(int tx, int ty)
{
    // linear algebra 🔥🔥🔥
    float dx = tx - positionx.get();
    float dy = ty - positiony.get();
    float length = sqrtf((dx * dx) + (dy * dy));

    if (length > 0) 
    {
        dx /= length;
        dy /= length;
    }
}

void EnemyShip::movetowards(int tx, int ty)
{
    float dx = tx - positionx.get();
    float dy = ty - positiony.get();
    float dist = sqrt((dx * dx) + (dy * dy));

    if (dist > 3)
    {
        velocityx = (dx / dist) * speed;
        velocityy = (dy / dist) * speed;
    }
    else // close enough to PlayerShip
    {
        velocityx = 0;
        velocityy = 0;
    }

    positionx.set(positionx.get() + (int)velocityx.get());
    positiony.set(positiony.get() + (int)velocityy.get());

    warp();
}

void EnemyShip::fireprojectile(vector<Projectile>& projectiles, int tx, int ty)
{
    if (firecooldown == 0)
    {
        float dx = tx - positionx.get();
        float dy = ty - positiony.get();
        float ang;

        if (dx == 0)   // to avoid division by zero
        {  
			if (dy == 0) // if the target is at the same position
			{
				ang = 0.0f; // no angle needed
			}
			else if (dy > 0)
			{
				ang = 90.0f; // straight up
			}
			else
			{
				ang = -90.0f; // straight down
			}
		}
		else if (dy == 0) // if the target is directly to the left or right
		{
            if (dx > 0)
            {
				ang = 0.0f; // straight right
            }
            else
            {
                ang = 180.0f; // straight left
            }
        }
        else 
        {
            float slope = dy / dx;
            ang = slope * (180.0f / PI);    
            if (dx < 0)
            {
                ang += 180.0f;
            }
        }

        projectiles.emplace_back((int)positionx.get(), (int)positiony.get(), ang, 8.0f, 15, false);
        firecooldown = 40;
    }
	else if (firecooldown > 0)
	{
		firecooldown--;
	}
}

void EnemyShip::update(vector<Projectile>& projectiles, int targetx, int targety)
{
    rotatetowards(targetx, targety);
    movetowards(targetx, targety);
    fireprojectile(projectiles, targetx, targety);
}

Texture EnemyShip:: getsprite() const
{
    return sprite; 
}
