#ifndef ENEMY_SHIP_H
#define ENEMY_SHIP_H

#include "Ship.h"
#include <vector>
#include "Projectile.h"
#include "MyStr.h"
using namespace std;

class EnemyShip : public Ship
{
    int firecooldown;
    float speed;

public:
    EnemyShip(int posx, int posy, const char* path);
    void update(vector<Projectile>& projectiles, int targetx, int targety);
    void rotatetowards(int tx, int ty);
    void movetowards(int tx, int ty);
    void fireprojectile(vector<Projectile>& projectiles, int tx, int ty);
    Texture getsprite() const;
};

#endif
