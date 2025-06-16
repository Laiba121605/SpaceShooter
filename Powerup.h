#ifndef POWERUP_H
#define POWERUP_H

#include "raylib.h"
#include "PlayerShip.h"
#include <iostream>
using namespace std;

class PowerUp 
{
protected:
    SafeValue <int> positionx;
    SafeValue <int> positiony;
    SafeValue <int> radius;
    Color color;
    bool active;
    int duration;  // baiscally the number of frames the effect lasts
public:
    PowerUp(int x, int y, Color c, int dur = 300);
    virtual ~PowerUp() {}
    virtual void apply(PlayerShip& player) = 0;
    virtual void remove(PlayerShip& player) = 0;
    void draw() const;
    int getx() const;
    int gety() const;
    int getradius() const;
    bool isactive() const;
    void deactivate();
    int getduration() const;
};

class MaxHealthPowerUp : public PowerUp
{
public:
    MaxHealthPowerUp(int x, int y);
    void apply(PlayerShip& player) override;
    void remove(PlayerShip& player) override {}
};

class TwiceSpeedPowerUp : public PowerUp 
{
public:
    TwiceSpeedPowerUp(int x, int y);
    void apply(PlayerShip& player) override;
    void remove(PlayerShip& player) override;
};

class TwiceDamagePowerUp : public PowerUp
{
public:
    TwiceDamagePowerUp(int x, int y);
    void apply(PlayerShip& player) override;
    void remove(PlayerShip& player) override;
};
#endif