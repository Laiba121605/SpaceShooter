#include "PlayerShip.h"
#include "raylib.h"
#include <cmath>

PlayerShip::PlayerShip(int posx, int posy, const char* path) :
    Ship(posx, posy, GREEN, path), score(0), firecooldown(0),
	rotationspeed(3.0f), thrustpower(0.3f), projectiledamage(50)
{
	sprite = LoadTexture(path);
}

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

        velocityx.set(velocityx.get() + cos(rad) * thrustpower);
        velocityy.set(velocityy.get() + sin(rad) * thrustpower);

        if (velocityx.get() > maxspeed)
        {
            velocityx.set(maxspeed);
        }
        if (velocityy.get() > maxspeed)
        {
            velocityy.set(maxspeed);
        }
    }
    else if (IsKeyDown(KEY_DOWN))
    {
        float rad = (angle + 90.0f) * DEG2RAD;

        velocityx.set(velocityx.get() + cos(rad) * thrustpower);
        velocityy.set(velocityy.get() + sin(rad) * thrustpower);

        if (velocityx.get() > maxspeed)
        {
            velocityx.set(maxspeed);
        }
        if (velocityy.get() > maxspeed)
        {
            velocityy.set(maxspeed);
        }
	}
	else
	{
		velocityx.set(velocityx.get() * 0.98f);
        velocityy.set(velocityy.get() * 0.98f);

        // so it stops when no key is pressed
		if (fabs(velocityx.get()) < 0.01f)
		{
			velocityx = 0;
		}
        if (fabs(velocityy.get()) < 0.01f)
        {
            velocityy = 0;
        }
    }

    positionx.set(positionx.get() + (int)velocityx.get());
    positiony.set(positiony.get() + (int)velocityy.get());

    warp();
}

void PlayerShip::fireprojectile(vector<Projectile>& projectiles)
{
    if (IsKeyDown(KEY_SPACE) && firecooldown == 0)
    {
        projectiles.emplace_back((int)positionx.get(), (int)positiony.get(), angle - 85, 12.0f, projectiledamage, true);
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

void PlayerShip::draw() const
{
    Vector2 pos{ (float)positionx.get(), (float)positiony.get() };
    DrawTextureEx(sprite, pos, angle, 0.1f, WHITE);

    // healthbar display

    float hpbarw = 40, hpbarh = 5;
    float pct = (float)health / maxhealth;

    if (health <= 0)
    {
        pct = 0; // make sure health bar doesn't show negative health
        return; // so health is not drawn when dead
    }

    DrawRectangle(positionx.get() - 20, positiony.get() + radius.get() + 10, hpbarw, hpbarh, GRAY);
    DrawRectangle(positionx.get() - 20, positiony.get() + radius.get() + 10, (int)(hpbarw * pct), hpbarh, GREEN);
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

int PlayerShip::getmaxhealth() const
{
	return maxhealth;
}

void PlayerShip::heal(int m)
{
    maxhealth = m;
    health = m; // reset health to max when setting max health
}

void PlayerShip::setspeed(float s)
{
	thrustpower = s; // for powerup (treating thrustpower as speed)
}

float PlayerShip::getspeed() const
{
	return thrustpower;
}

void PlayerShip::setradius(int r)
{
	radius.set(r);
}

void PlayerShip::setdamage(int d)
{
	projectiledamage = d;
}

int PlayerShip::getdamage() const
{
    return projectiledamage;
}

PlayerShip::~PlayerShip()
{
	UnloadTexture(sprite); 
}