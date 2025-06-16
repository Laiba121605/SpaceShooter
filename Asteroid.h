#ifndef ASTEROID_H
#define ASTEROID_H

#include "raylib.h"
#include "Ship.h"
#include <cmath>

class Asteroid
{
    SafeValue <int> positionx;
    SafeValue <int> positiony;
    SafeValue <int> radius;
    float angle;
    float rotation;       // rotation angle
    float speed;
    int damage;           // damage dealt on collision
    int rotationspeed;    // how fast the asteroid spins
    bool isdead; 
    Texture2D sprite;  
    Color color;

public:
    Asteroid(float posx, float posy, int dmg, const char* path);
    void rotatetowards(int tx, int ty);
    void update(int px, int py);
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