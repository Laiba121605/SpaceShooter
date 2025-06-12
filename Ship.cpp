#include "Ship.h"
#include "raylib.h"
#include <cmath>

Ship::Ship(int posx, int posy, Color col, const char* path) :
    positionx(posx), positiony(posy), velocityx(0), velocityy(0), angle(0.0f),
    acceleration(0.1f), mass(1.0f), radius(20), health(100), maxhealth(100), color(col)
{
    sprite = LoadTexture(path);
}

void Ship::rotate() {}
void Ship::move() {}

void Ship::draw() const
{
    Vector2 pos{ (float)positionx, (float)positiony };
    DrawTextureEx(sprite, pos, angle, 0.075f, WHITE);

    // healthbar display

    float hpbarw = 40, hpbarh = 5;
    float pct = (float)health / maxhealth;

	if (health <= 0) 
    {
		pct = 0; // make sure health bar doesn't show negative health
		return; // so health is not drawn when dead
	}

    DrawRectangle(positionx - 20, positiony + radius + 10, hpbarw, hpbarh, GRAY);
    DrawRectangle(positionx - 20, positiony + radius + 10, (int)(hpbarw * pct), hpbarh, GREEN);
}

void Ship::warp()
{
    int w = GetScreenWidth();
    int h = GetScreenHeight();

    if (positionx < 0)
    {
        positionx = w;
    }
    if (positionx > w)
    {
        positionx = 0;
    }
    if (positiony < 0)
    {
        positiony = h;
    }
    if (positiony > h)
    {
        positiony = 0;
    }
}

void Ship::takedamage(int dmg)
{
    health -= dmg;
}

int Ship::getx() const
{
    return positionx;
}

int Ship::gety() const
{
    return positiony;
}

int Ship::getradius() const
{
    return radius;
}

int Ship::gethealth() const
{
    return health;
}

bool Ship::isdead() const
{
    return (health <= 0);
}

Ship::~Ship()
{
    UnloadTexture(sprite);
}