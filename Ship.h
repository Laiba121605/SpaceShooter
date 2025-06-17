#ifndef SHIP_H
#define SHIP_H

#include <iostream>
#include "raylib.h"
#include "SafeValue.h"  
#include "MyStr.h"
using namespace std;

class Ship
{
protected:
    SafeValue <int> positionx;
    SafeValue <int> positiony;
    SafeValue <float> velocityx;
    SafeValue <float> velocityy;
    SafeValue <int> radius;         // radius for collision detection
    SafeValue <int> size;          // size of the ship, used for drawing 
    float angle;        // rotation angle in degrees
    float acceleration;
    float mass;
    int health;
    int maxhealth;
    Texture2D sprite;
    Color color;   

public:
    Ship(int posx, int posy, Color col, const char* path);
    virtual ~Ship();

    virtual void rotate();
    virtual void move();
    virtual void draw() const = 0;
    void warp();
    int getx() const;
    int gety() const;
    int getradius() const;
    SafeValue<int> getsize() const;
    int gethealth() const;
    void takedamage(int dmg);
    bool isdead() const;
};

#endif