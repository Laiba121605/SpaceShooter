#ifndef PLAYER_SHIP_H
#define PLAYER_SHIP_H

#include "Ship.h"
#include <vector>
#include "Projectile.h"
#include "MyStr.h"
using namespace std;

class PlayerShip : public Ship
{
    int score;
    int firecooldown;
    float rotationspeed;
    float thrustpower;
    int projectiledamage;

public:
    PlayerShip(int posx, int posy, const char* path);
    void update(vector<Projectile>& projectiles);
    void fireprojectile(vector<Projectile>& projectiles);
    void rotate() override;
    void move() override;
    int getscore() const;
    void addscore(int points);
    Texture getsprite() const;
    int getmaxhealth() const;
    void heal(int amt);
    void setspeed(float s);
    float getspeed() const;
    void setradius(int r);
    void setdamage(int d);
    int getdamage() const;
};

#endif