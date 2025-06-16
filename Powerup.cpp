#include"Powerup.h"

PowerUp::PowerUp(int x, int y, Color c, int dur)
	: positionx(x), positiony(y), radius(20),
	color(c), active(true), duration(dur)
{}

void PowerUp::draw() const
{
	if (active)
	{
		DrawCircle(positionx.get(), positiony.get(), (float)radius.get(), color);
	}
}

int PowerUp::getx() const
{
	return positionx.get();
}

int PowerUp::gety() const
{
	return positiony.get();
}

int PowerUp::getradius() const
{
	return radius.get();
}

bool PowerUp::isactive() const
{
	return active;
}

void PowerUp::deactivate()
{
	active = false;
}

int PowerUp::getduration() const
{
	return duration;
}

MaxHealthPowerUp::MaxHealthPowerUp(int x, int y) : PowerUp(x, y, PINK, 1) 
{}

void MaxHealthPowerUp::apply(PlayerShip& player)
{
	player.heal(player.getmaxhealth());
	deactivate();
}

TwiceSpeedPowerUp::TwiceSpeedPowerUp(int x, int y) : PowerUp(x, y, PURPLE, 420)
{}

void TwiceSpeedPowerUp::apply(PlayerShip& player)
{
	player.setspeed(player.getspeed() * 2.0f);
	deactivate();
}

void TwiceSpeedPowerUp::remove(PlayerShip& player)
{
	player.setspeed(player.getspeed() / 2.0f);
}

TwiceDamagePowerUp::TwiceDamagePowerUp(int x, int y) : PowerUp(x, y, ORANGE, 420)
{}

void TwiceDamagePowerUp::apply(PlayerShip& player)
{
	player.setdamage(player.getdamage() * 2.0f);
	deactivate();
}

void TwiceDamagePowerUp::remove(PlayerShip& player)
{
	player.setdamage(player.getdamage() / 2.0f);
}
