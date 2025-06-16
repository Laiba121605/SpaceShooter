#include "Projectile.h"
#include <cmath>

Projectile::Projectile(int sx, int sy, float ang, float spd, int dmg, bool playerowned)
    : x((float)sx), y((float)sy), angle(ang), speed(spd), damage(dmg), fromplayer(playerowned), lifetime(90), radius(6)
{}

void Projectile::update()
{
    x.set(x.get() + cos(angle * DEG2RAD) * speed);
    y.set(y.get() + sin(angle * DEG2RAD) * speed);
    lifetime--;
}

void Projectile::draw() const
{
    if (fromplayer)
    {
		DrawCircle((int)x.get(), (int)y.get(), (float)radius.get(), GREEN);
	}
	else
	{
		DrawCircle((int)x.get(), (int)y.get(), (float)radius.get(), RED);
	}
}

bool Projectile::isoffscreen(int w, int h) const
{
	return ((x.get() < 0) || (x.get() > w) || (y.get() < 0) || (y.get() > h));
}

bool Projectile::isfromplayer() const
{
    return fromplayer;
}

int Projectile::getx() const
{
	return (int)x.get();
}

int Projectile::gety() const
{
	return (int)y.get();
}

int Projectile::getradius() const
{
	return radius.get();
}

int Projectile::getdamage() const
{
	return damage;
}

bool Projectile:: isalive() const
{
    return (lifetime > 0);
}