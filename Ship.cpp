#include "Ship.h"
#include "raylib.h"
#include <cmath>

Ship::Ship(int posx, int posy, Color col, const char* path) :
    positionx(posx), positiony(posy), velocityx(0), velocityy(0), angle(0.0f),
    acceleration(0.1f), mass(1.0f), radius(20), health(100), maxhealth(100), color(col), size(0.100)
{}

void Ship::rotate() {}
void Ship::move() {}

void Ship::draw() const
{
    Vector2 pos{ (float)positionx.get(), (float)positiony.get()};
    DrawTextureEx(sprite, pos, angle, 0.100f, RAYWHITE);

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

void Ship::warp()
{
    int w = GetScreenWidth();
    int h = GetScreenHeight();

    if (positionx.get() < 0)
    {
        positionx.set(w);
    }
    if (positionx.get() > w)
    {
        positionx.set(0);
    }
    if (positiony.get() < 0)
    {
        positiony.set(h);
    }
    if (positiony.get() > h)
    {
        positiony.set(0);
    }
}

void Ship::takedamage(int dmg)
{
    health -= dmg;
}

int Ship::getx() const
{
    return positionx.get();
}

int Ship::gety() const
{
    return positiony.get();
}

int Ship::getradius() const
{
    return radius.get();
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
{}

SafeValue<int> Ship::getsize() const
{
	return size.get();
}