#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "raylib.h"
#include "SafeValue.h"
using namespace std;

class Projectile
{
    SafeValue <float> x;
    SafeValue <float> y;
    SafeValue <int> radius;
    float speed;
    int damage;
    bool fromplayer;
    int lifetime;
    float angle;

public:
    Projectile(int sx, int sy, float ang, float spd, int dmg, bool playerowned);
    void update();
    void draw() const;
    bool isoffscreen(int w, int h) const;
    bool isfromplayer() const;
    int getx() const;
    int gety() const;
    int getradius() const;
    int getdamage() const;
    bool isalive() const;
};

#endif