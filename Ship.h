#ifndef SHIP_H
#define SHIP_H

#include <iostream>
#include "raylib.h"
#include "MyStr.h"
using namespace std;

class Ship
{
protected:
    int positionx;
    int positiony;
    float velocityx;
    float velocityy;
    float angle;        // rotation angle in degrees
    float acceleration;
    float mass;
    int radius;         // radius for collision detection
    int health;
    int maxhealth;
    Texture2D sprite;
    Color color;   

public:
    Ship(int posx, int posy, Color col, const char* path);
    virtual ~Ship();

    virtual void rotate();
    virtual void move();
    void draw() const;
    void warp();
    int getx() const;
    int gety() const;
    int getradius() const;
    int gethealth() const;
    void takedamage(int dmg);
    bool isdead() const;
};

#endif