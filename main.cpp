#include "raylib.h"
#include "PlayerShip.h"
#include "EnemyShip.h"
#include "Projectile.h"
#include "Powerup.h"
#include "Logger.h"
#include "MyStr.h"
#include <string>
#include "Asteroid.h"

#include <vector>
#include <ctime>
#include <cstdlib>
#include <fstream>

using namespace std;

bool collisiondetection(int x1, int y1, int r1, int x2, int y2, int r2)
{
    int dx = x1 - x2;
    int dy = y1 - y2;
    int dist2 = (dx * dx) + (dy * dy);
    int rad = r1 + r2;
    return (dist2 <= (rad * rad));
}

bool DrawButton(Rectangle bounds, const char* label, Color color, Color hovercolor, Color textcolor)
{
    Vector2 mouse = GetMousePosition();
    bool hovered = CheckCollisionPointRec(mouse, bounds);

    if(hovered)
    { 
		DrawRectangleRec(bounds, hovercolor);
	}
	else
	{
		DrawRectangleRec(bounds, color);
    }

    DrawRectangleLines(bounds.x, bounds.y, bounds.width, bounds.height, DARKGRAY);

    int fontsize = 24;
    int textwidth = MeasureText(label, fontsize);
    DrawText(label, bounds.x + (bounds.width - textwidth) / 2, bounds.y + (bounds.height - fontsize) / 2, fontsize, textcolor);
    return (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON));
}


int main()
{
    Logger& logger = Logger::getinstance();
    logger.initialize("gamelog.txt");
    logger.writeerror("Game started.");

    int screenwidth = 900;
    int  screenheight = 700;
    InitWindow(screenwidth, screenheight, "Space Shooter");
    SetTargetFPS(60);
    Vector2 pos = { 0.0f, 0.0f };

    Texture2D gameplaybackground = LoadTexture("./background.png");
    gameplaybackground.width = screenwidth;
    gameplaybackground.height = screenheight;

    Texture2D mainmenu = LoadTexture("./mainmenu.png");
    mainmenu.width = screenwidth;
    mainmenu.height = screenheight;

    srand((unsigned)time(0));
    PlayerShip player(screenwidth / 2, screenheight / 2, "./playership.png"); 

    vector<EnemyShip> enemies;
    vector<Projectile> projectiles;
    vector<PowerUp*> powerups;
    vector<Asteroid> asteroids;

    int speedboosttimer = 0;
    bool speedactive = false;
    float originalspeed = player.getspeed();
    int originalradius = player.getradius();
    int damageboosttimer = 0;
    bool damageactive = false;
    int originaldamage = player.getdamage();

    int powerupspawntimer = 0;
    int asteroidspawntimer = 0;
    int enemyspawntimer = 0;
    bool gameover = false;

	int buttonlength = 200;
	int buttonheight = 50;

    int gamestate = 0; // 0 = main menu, 1 = game, 2 = exit

    logger.writeerror("PlayerShip spawned.");

    while (!WindowShouldClose() && gamestate != 2)
    {
        BeginDrawing();

        //main menu
        if (gamestate == 0)
        {
            ClearBackground(BLACK);
            DrawTextureEx(mainmenu, pos, 0.0f, 1, WHITE);

            Rectangle newButton = { screenwidth / 2.0f - 100, screenheight / 2.0f - 60, 200, 50 };
            Rectangle loadButton = { screenwidth / 2.0f - 100, screenheight / 2.0f + 10, 200, 50 };

            if (DrawButton(newButton, "New Game", GRAY, DARKBLUE, WHITE))
            {
                logger.writeerror("New Game started.");
                gamestate = 1;
            }
            if (DrawButton(loadButton, "Load Game", GRAY, DARKBLUE, WHITE))
            {
                logger.writeerror("Load Game clicked.");
            }
            DrawText("Click to start", screenwidth / 2 - 70, screenheight / 2 + 90, 20, WHITE);

            EndDrawing();
            continue;
        }

		//gameplay background
        ClearBackground(BLACK);
        DrawTextureEx(gameplaybackground, pos, 0.0f, 1, WHITE);

        if (!gameover && gamestate == 1)
        {
            // this updates player
            player.update(projectiles);

            // spawn enemies randomly
            if (enemyspawntimer == 0)
            {
                int side = rand() % 4;
                int ex;
                int ey;
                if (side == 0) 
                { 
                    ex = rand() % screenwidth; ey = 0;
                }
                else if (side == 1)
                {
                    ex = rand() % screenwidth; ey = screenheight;
                }
                else if (side == 2) 
                {
                    ex = 0; ey = rand() % screenheight;
                }
                else
                {
                    ex = screenwidth; ey = rand() % screenheight;
                }
                enemies.emplace_back(ex, ey, "./enemyship.png");
                logger.writeerror("EnemyShip spawned.");
                enemyspawntimer = 300 + rand() % 100;
            }
            else
            {
                enemyspawntimer--;
            }

            //spawn asteroids randomly
            if (asteroidspawntimer == 0)
            {
                int side = rand() % 4;
                int ax;
                int ay;
                int type = rand() % 3; // 0=little damage, 1=medium damage, 2=significant damage

                if (side == 0)
                {
                    ax = rand() % screenwidth; ay = 0;
                }
                else if (side == 1)
                {
                    ax = rand() % screenwidth; ay = screenheight;
                }
                else if (side == 2)
                {
                    ax = 0; ay = rand() % screenheight;
                }
                else
                {
                    ax = screenwidth; ay = rand() % screenheight;
                }

                if (type == 0)
                {
                    asteroids.emplace_back(ax, ay, 5, "./asteroid1.PNG");
                }
				else if (type == 1)
				{
                    asteroids.emplace_back(ax, ay, 10, "./asteroid2.PNG");
				}
				else
				{
                    asteroids.emplace_back(ax, ay, 15, "./asteroid3.PNG");
				}
                logger.writeerror("Asteroid spawned.");
                asteroidspawntimer = 200 + rand() % 100;
            }
            else
            {
                asteroidspawntimer--;
            }

            if (powerupspawntimer <= 0)
            {
                int x = 40 + rand() % (screenwidth - 80);
                int y = 40 + rand() % (screenheight - 80);
                int type = rand() % 3; // 0=MaxHealth, 1=TwiceSpeed, 2=TwiceDamage
                if (type == 0)
                {
                    powerups.push_back(new MaxHealthPowerUp(x, y));
                    logger.writeerror("MaxHealthPowerUp spawned.");
                }
                else if (type == 1)
                {
                    powerups.push_back(new TwiceSpeedPowerUp(x, y));
                    logger.writeerror("TwiceSpeedPowerUp spawned.");
                }
                else
                {
                    powerups.push_back(new TwiceDamagePowerUp(x, y));
                    logger.writeerror("TwiceDamagePowerUp spawned.");
                }

                powerupspawntimer = 600 + rand() % 300; // next powerup in 10-15 seconds 
            }
            else 
            {
                powerupspawntimer--;
            }

            // this updates enemies
            for (auto& enemy : enemies)
            {
                enemy.update(projectiles, player.getx(), player.gety());
            }

            //this updates asteriods
            for (auto& asteroid : asteroids)
            {
                asteroid.update(player.getx(), player.gety());
            }

            // this updates projectiles
            for (auto& proj : projectiles)
            {
                proj.update();
            }

			// this updates powerups    
            for (size_t i = 0; i < powerups.size(); ++i)
            {
                PowerUp* pu = powerups[i];
                if (pu->isactive() && collisiondetection(player.getx(), player.gety(), player.getradius(), pu->getx(), pu->gety(), pu->getradius()))
                {
                    // Apply effect
                    pu->apply(player);

                    // if effect is timed, activate timer and flags
                    if (dynamic_cast<TwiceSpeedPowerUp*>(pu))
                    {
                        if (!speedactive)
                        {
                            originalspeed = player.getspeed() / 2.0f; // since already doubled
                            speedboosttimer = pu->getduration();
                            speedactive = true;
                        }
                        else 
                        {
                            speedboosttimer = pu->getduration(); // refresh
                        }
                        logger.writeerror("Player collected a TwiceSpeedPowerUp.");
                    }
                    if (dynamic_cast<TwiceDamagePowerUp*>(pu))
                    {
						if (!damageactive)
						{
							originaldamage = player.getdamage() / 2; // Since already doubled
							damageboosttimer = pu->getduration();
							damageactive = true;
						}
						else
						{
							damageboosttimer = pu->getduration(); // refresh
						}
                        logger.writeerror("Player collected a TwiceDamagePowerUp.");
                    }
                    if (dynamic_cast<MaxHealthPowerUp*>(pu))
                    {
                        logger.writeerror("Player collected a MaxHealthPowerUp.");
                    }
                    pu->deactivate();
                }
            }

            // update timers and remove timed effects if expired
            if (speedactive)
            {
                speedboosttimer--;
                if (speedboosttimer <= 0)
                {
                    player.setspeed(originalspeed);
                    speedactive = false;
                    logger.writeerror("TwiceSpeedPowerUp effect expired.");
                }
            }
            if (damageactive)
            {
                damageboosttimer--;
                if (damageboosttimer <= 0)
                {
					player.setdamage(originaldamage);
                    player.setradius(originalradius);
                    damageactive = false;
                    logger.writeerror("TwiceDamagePowerUp effect expired.");
                }
            }

            // Remove inactive powerups
            for (int i = (int)powerups.size() - 1; i >= 0; i--)
            {
                if (!(powerups[i])->isactive()) 
                {
                    delete powerups[i];
                    powerups.erase(powerups.begin() + i);
                }
            }

            // checks for collisions
            // Projectiles vs Ships
            for (auto& proj : projectiles)
            {
                if (!proj.isalive())
                {
                    continue;
                }
                if (proj.isfromplayer())
                {
                    for (auto& enemy : enemies)
                    {
                        if (!enemy.isdead() && collisiondetection(proj.getx(), proj.gety(), proj.getradius(),
                            enemy.getx(), enemy.gety(), enemy.getradius()))
                        {
                            enemy.takedamage(proj.getdamage());
                            if (enemy.isdead())
                            {
                                player.addscore(1);
                                logger.writeerror("Enemy Destroyed.");
                            }
                            proj = Projectile((int)0, (int)0, 0, 0, 0, true); // kill projectile
                        }
                    }
                }
                else // Enemy projectile vs Player
                {
                    if (!player.isdead() && collisiondetection(proj.getx(), proj.gety(), proj.getradius(),
                        player.getx(), player.gety(), player.getradius()))
                    {
                        player.takedamage(proj.getdamage());
                        proj = Projectile((int)0, (int)0, 0, 0, 0, true); // kill projectile
                    }
                }
            }
            // Player vs Enemy collision
            for (auto& enemy : enemies)
            {
                if (!enemy.isdead() && !player.isdead() && collisiondetection(player.getx(), player.gety(), player.getradius(),
                    enemy.getx(), enemy.gety(), enemy.getradius()))
                {
                    player.takedamage(15);
                    enemy.takedamage(15);
                }
            }

			// Player vs Asteroid collision
			for (auto& asteroid : asteroids)
			{
                if (!player.isdead() && collisiondetection(player.getx(), player.gety(), player.getradius(),
                    asteroid.getx(), asteroid.gety(), asteroid.getradius()))
                {
                    player.takedamage(asteroid.getdamage());
                    asteroid.destroy();
                    logger.writeerror("Asteroid Destroyed.");
                }
			}

			// Player Projectile vs Asteroid collision
            for (auto& proj : projectiles)
            {
                if (!(proj.isalive()) || !(proj.isfromplayer()))
                {
                    continue;
                }
                for (auto& asteroid : asteroids)
                {
                    if (collisiondetection(proj.getx(), proj.gety(), proj.getradius(),
                        asteroid.getx(), asteroid.gety(), asteroid.getradius()))
                    {
                        asteroid.destroy();
                        logger.writeerror("Asteroid Destroyed.");
                        player.addscore(1);
                        proj = Projectile((int)0, (int)0, 0, 0, 0, true); // kill projectile
                        break;
                    }
                }
            }
            // remove dead projectiles
            projectiles.erase(
                remove_if(projectiles.begin(), projectiles.end(), [&](Projectile& p) {
                    return (!(p.isalive()) || (p.isoffscreen(screenwidth, screenheight)));
                    }), projectiles.end());

            // remove dead enemies
            enemies.erase(
                remove_if(enemies.begin(), enemies.end(), [](EnemyShip& e) { return e.isdead(); }),
                enemies.end());

			// remove dead asteroids
			asteroids.erase(
                remove_if(asteroids.begin(), asteroids.end(), [](Asteroid& a) { return a.dead(); }),
				asteroids.end());

            // draws
            player.draw();
            for (const auto& enemy : enemies)
            {
                enemy.draw();
            }
            for (const auto& proj : projectiles)
            {
                proj.draw();
            }
            for (const auto* pu : powerups)
            {
                pu->draw();
            }
            for (const auto& asteroid : asteroids) 
            {
                asteroid.draw();
            }

            DrawText(TextFormat("Score: %d", player.getscore()), 15, 10, 24, YELLOW);

            if (player.isdead())
            {
                gameover = true;
				logger.writeerror("PlayerShip destroyed. GAME OVER.");
            }
        }
        else
        {
            UnloadTexture(gameplaybackground);
            ClearBackground(BLACK);
            DrawText("GAME OVER", screenwidth / 2 - 140, screenheight / 2 - 70, 40, RED);
            DrawText(TextFormat("Final Score: %d", player.getscore()), screenwidth / 2 - 100, screenheight / 2 - 10, 24, LIGHTGRAY);

            enemies.clear();
            projectiles.clear();
        }

        EndDrawing();
    }
    for (auto* pu : powerups)
    {
        delete pu;
    }

    logger.cleanup("gamelog.txt");

    CloseWindow();
    return 0;
}