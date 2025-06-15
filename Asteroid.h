#ifndef ASTEROID_H
#define ASTEROID_H

#include "raylib.h"
#include "Ship.h"
#include <cmath>

class Asteroid
{
    float positionx;
    float positiony;
    float angle;
    int radius;
    float rotation;       // rotation angle, for drawing
    float speed;
    int lifetime;
    int damage;           // damage dealt on collision
    int rotationspeed;    // how fast the asteroid spins
    bool isdead;          // whether the asteroid is destroyed
    Texture2D sprite;     // texture for the asteroid
    Color color;

public:
    Asteroid(float posx, float posy, int dmg, const char* path);
    void rotatetowards(int tx, int ty);
    void update(int tx, int ty);
    void draw() const;
    void move();
    void rotate();
    void destroy();
    bool offscreen(int screenw, int screenh) const;
    bool dead() const;
    int getdamage() const;
    int getradius() const;
    float getx() const;
    float gety() const;
    ~Asteroid();
};

#endif