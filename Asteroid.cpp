#include "Asteroid.h"

Asteroid::Asteroid(float posx, float posy, int dmg, const char* path) :
    positionx(posx), positiony(posy), angle(0.0f), radius(70), isdead(false), lifetime(500),
    damage(dmg), color(GRAY), rotationspeed(1.0f), speed(1.5f)
{
    sprite = LoadTexture(path);
}

void Asteroid::update(int tx,int ty)
{
    move();
    rotatetowards(tx, ty);
	if (offscreen(GetScreenWidth(), GetScreenHeight()))
	{
		isdead = true; // destroy asteroid if it goes offscreen
	}
    rotate();
    lifetime--;
    if (lifetime <= 0)
    {
        isdead = true;
    }
}

void Asteroid::rotatetowards(int tx, int ty)
{
    // linear algebra 🔥🔥🔥
    float dx = tx - positionx;
    float dy = ty - positiony;
    float length = sqrtf((dx * dx) + (dy * dy));

    if (length > 0)
    {
        dx /= length;
        dy /= length;
    }
}

void Asteroid::draw() const
{
    Vector2 pos{ positionx, positiony };
    DrawTextureEx(sprite, pos, angle, 0.150f, WHITE);
}

void Asteroid::move()
{
    positionx += cos(angle * DEG2RAD) * speed;
    positiony += sin(angle * DEG2RAD) * speed;
}

void Asteroid::rotate()
{
    angle += rotationspeed;
    if (angle >= 360.0f)
    {
        angle -= 360.0f;
    }
}

void Asteroid::destroy()
{
    isdead = true;
}

bool Asteroid::offscreen(int screenw, int screenh) const
{
    return ((positionx < -radius) || (positionx > screenw + radius) ||
        (positiony < -radius) || (positiony > screenh + radius));
}

bool Asteroid::dead() const
{
    return isdead;
}

int Asteroid::getdamage() const
{
    return damage;
}

int Asteroid::getradius() const
{
    return radius;
}

float Asteroid::getx() const
{
    return positionx;
}

float Asteroid::gety() const
{
    return positiony;
}

Asteroid::~Asteroid()
{
    
    UnloadTexture(sprite);
}