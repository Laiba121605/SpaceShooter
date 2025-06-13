#include "PlayerShip.h"
#include "raylib.h"
#include <cmath>

PlayerShip::PlayerShip(int posx, int posy, const char* path) :
    Ship(posx, posy, GREEN, path), score(0), firecooldown(0),
    rotationspeed(3.0f), thrustpower(0.2f)
{}

void PlayerShip::rotate()
{
    if (IsKeyDown(KEY_LEFT))
    {
        angle -= rotationspeed;
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        angle += rotationspeed;
    }
}

void PlayerShip::move()
{
	const float maxspeed = 10.0f;

    if (IsKeyDown(KEY_UP))
    {
        float rad = (angle - 90.0f) * DEG2RAD;

        velocityx += cos(rad) * thrustpower;
        velocityy += sin(rad) * thrustpower;

        if (velocityx > maxspeed)
        {
            velocityx = maxspeed;
        }
		if (velocityy > maxspeed)
		{
			velocityy = maxspeed;
		}
    }
    else if (IsKeyDown(KEY_DOWN))
    {
        velocityx *= 0.98f;
        velocityy *= 0.98f;

        if (fabs(velocityx) < 0.01f)
        {
            velocityx = 0;
        }
		if (fabs(velocityy) < 0.01f)
		{
			velocityy = 0;
		}
	}
	else
	{
		velocityx *= 0.98f;
		velocityy *= 0.98f;

		if (fabs(velocityx) < 0.01f)
		{
			velocityx = 0;
		}
        if (fabs(velocityy) < 0.01f)
        {
            velocityy = 0;
        }
    }

    positionx += (int)velocityx;
    positiony += (int)velocityy;

    warp();
}

void PlayerShip::fireprojectile(vector<Projectile>& projectiles)
{
    if (IsKeyDown(KEY_SPACE) && firecooldown == 0)
    {
        projectiles.emplace_back(positionx, positiony, angle-85, 12.0f, 50, true);
        firecooldown = 12;
	}

	else if (IsKeyDown(KEY_SPACE) && firecooldown > 0)
	{
		return; // so it doesnt fire while cooldown is active
	}
}

void PlayerShip::update(vector<Projectile>& projectiles)
{
    if (firecooldown > 0)
    {
        firecooldown--;
    }
    rotate();
    move();
    fireprojectile(projectiles);
}

int PlayerShip:: getscore() const 
{
    return score; 
}

void PlayerShip:: addscore(int points) 
{
    score += points; 
}

Texture PlayerShip::getsprite() const
{
    return sprite;
}
