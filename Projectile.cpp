#include "Projectile.h"
#include <cmath>

Projectile::Projectile(int sx, int sy, float ang, float spd, int dmg, bool playerowned)
    : x((float)sx), y((float)sy), angle(ang), speed(spd), damage(dmg), fromplayer(playerowned), lifetime(90), radius(6)
{}

void Projectile::update()
{
    x += cos(angle * DEG2RAD) * speed;
    y += sin(angle * DEG2RAD) * speed;
    lifetime--;
}

void Projectile::draw() const
{
    if (fromplayer)
    {
		DrawCircle((int)x, (int)y, (float)radius, SKYBLUE);
	}
	else
	{
		DrawCircle((int)x, (int)y, (float)radius, RED);
	}
}

bool Projectile::isoffscreen(int w, int h) const
{
    return (x < 0 || x > w || y < 0 || y > h);
}

bool Projectile::isfromplayer() const
{
    return fromplayer;
}

int Projectile::getx() const
{
	return (int)x;
}

int Projectile::gety() const
{
	return (int)y;
}

int Projectile::getradius() const
{
	return radius;
}

int Projectile::getdamage() const
{
	return damage;
}

bool Projectile:: isalive() const
{
    return (lifetime > 0);
}