#include "Asteroid.h"

Asteroid::Asteroid(float posx, float posy, int dmg, const char* path) :
    positionx(posx), positiony(posy), angle(0.0f), radius(30), isdead(false),
    damage(dmg), color(GRAY), rotationspeed(1.0f), speed(1.5f)
{
    sprite = LoadTexture(path);
}

void Asteroid::update(int px, int py)
{
    move();
    rotatetowards(px, py);
	if (offscreen(GetScreenWidth(), GetScreenHeight()))
	{
		isdead = true; // it destroys asteroid if it goes offscreen
	}
    rotate();
}

void Asteroid::rotatetowards(int tx, int ty)
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

void Asteroid::draw() const
{
    Vector2 pos{ positionx.get(), positiony.get()};
    DrawTextureEx(sprite, pos, angle, 0.200f, WHITE);
}

void Asteroid::move()
{
    positionx.set(positionx.get() + cos(angle * DEG2RAD) * speed);
    positiony.set(positiony.get() + sin(angle * DEG2RAD) * speed);
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
    return ((positionx.get() < -radius.get()) || (positionx.get() > screenw + radius.get()) ||
        (positiony.get() < -radius.get()) || (positiony.get() > screenh + radius.get()));
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
    return radius.get();
}

float Asteroid::getx() const
{
    return positionx.get();
}

float Asteroid::gety() const
{
    return positiony.get();
}

Asteroid::~Asteroid()
{
    
    UnloadTexture(sprite);
}