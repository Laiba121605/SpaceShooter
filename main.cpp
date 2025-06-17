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

bool drawbutton(Rectangle bounds, const char* label, Color color, Color hovercolor, Color textcolor)
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

void savegame(const PlayerShip& player,
    const vector<EnemyShip>& enemies,
    const vector<Projectile>& projectiles,
    const vector<PowerUp*>& powerups,
    const vector<Asteroid>& asteroids,
    int speedboosttimer, bool speedactive, float originalspeed, int originalradius,
    int damageboosttimer, bool damageactive, int originaldamage,
    int powerupspawntimer, int asteroidspawntimer, int enemyspawntimer,
    bool gameover)
{
    ofstream file("savegame.dat", ios::binary);
    if (!file)
    {
        return;
    }

    int px = player.getx();
    int py = player.gety();
    int health = player.gethealth();
    int score = player.getscore();

    file.write((char*)&px, sizeof(px));
    file.write((char*)&py, sizeof(py));
    file.write((char*)&health, sizeof(health));
    file.write((char*)&score, sizeof(score));

    file.write((char*)&speedboosttimer, sizeof(speedboosttimer));
    file.write((char*)&speedactive, sizeof(speedactive));
    file.write((char*)&originalspeed, sizeof(originalspeed));
    file.write((char*)&originalradius, sizeof(originalradius));
    file.write((char*)&damageboosttimer, sizeof(damageboosttimer));
    file.write((char*)&damageactive, sizeof(damageactive));
    file.write((char*)&originaldamage, sizeof(originaldamage));
    file.write((char*)&powerupspawntimer, sizeof(powerupspawntimer));
    file.write((char*)&asteroidspawntimer, sizeof(asteroidspawntimer));
    file.write((char*)&enemyspawntimer, sizeof(enemyspawntimer));
    file.write((char*)&gameover, sizeof(gameover));

    int enemycount = (int)enemies.size();
    file.write((char*)&enemycount, sizeof(enemycount));
    for (const auto& enemy : enemies)
    {
        int ex = enemy.getx();
        int ey = enemy.gety();
        int ehp = enemy.gethealth();
        file.write((char*)&ex, sizeof(ex));
        file.write((char*)&ey, sizeof(ey));
        file.write((char*)&ehp, sizeof(ehp));
    }

    int asteroidcount = (int)asteroids.size();
    file.write((char*)&asteroidcount, sizeof(asteroidcount));
    for (const auto& asteroid : asteroids) 
    {
        float ax = asteroid.getx();
        float ay = asteroid.gety();
        int adamage = asteroid.getdamage();
        int aradius = asteroid.getradius();
        file.write((char*)&ax, sizeof(ax));
        file.write((char*)&ay, sizeof(ay));
        file.write((char*)&adamage, sizeof(adamage));
        file.write((char*)&aradius, sizeof(aradius));
    }

    file.close();
}

void loadgame(PlayerShip& player,
    vector<EnemyShip>& enemies,
    vector<Asteroid>& asteroids,
    int& speedboosttimer, bool& speedactive, float& originalspeed, int& originalradius,
    int& damageboosttimer, bool& damageactive, int& originaldamage,
    int& powerupspawntimer, int& asteroidspawntimer, int& enemyspawntimer,
    bool& gameover)
{
    ifstream file("savegame.dat", ios::binary);
    if (!file)
    {
        return;
    }

    int px, py, health, score;
    file.read((char*)&px, sizeof(px));
    file.read((char*)&py, sizeof(py));
    file.read((char*)&health, sizeof(health));
    file.read((char*)&score, sizeof(score));
    player = PlayerShip(px, py, "./playership.png");

    player.takedamage(player.getmaxhealth() - health);
    for (int i = 0; i < score; i++)
    {
        player.addscore(1);
    }

    file.read((char*)&speedboosttimer, sizeof(speedboosttimer));
    file.read((char*)&speedactive, sizeof(speedactive));
    file.read((char*)&originalspeed, sizeof(originalspeed));
    file.read((char*)&originalradius, sizeof(originalradius));
    file.read((char*)&damageboosttimer, sizeof(damageboosttimer));
    file.read((char*)&damageactive, sizeof(damageactive));
    file.read((char*)&originaldamage, sizeof(originaldamage));
    file.read((char*)&powerupspawntimer, sizeof(powerupspawntimer));
    file.read((char*)&asteroidspawntimer, sizeof(asteroidspawntimer));
    file.read((char*)&enemyspawntimer, sizeof(enemyspawntimer));
    file.read((char*)&gameover, sizeof(gameover));

    int enemycount = 0;
    file.read((char*)&enemycount, sizeof(enemycount));
    enemies.clear();
    for (int i = 0; i < enemycount; i++) 
    {
        int ex, ey, ehp;
        file.read((char*)&ex, sizeof(ex));
        file.read((char*)&ey, sizeof(ey));
        file.read((char*)&ehp, sizeof(ehp));
        EnemyShip enemy(ex, ey, "./enemyship.png");
        while (enemy.gethealth() > ehp) enemy.takedamage(1);
        enemies.push_back(enemy);
    }

    int asteroidcount = 0;
    file.read((char*)&asteroidcount, sizeof(asteroidcount));
    asteroids.clear();
    for (int i = 0; i < asteroidcount; i++) 
    {
        float ax, ay;
        int adamage, aradius;
        file.read((char*)&ax, sizeof(ax));
        file.read((char*)&ay, sizeof(ay));
        file.read((char*)&adamage, sizeof(adamage));
        file.read((char*)&aradius, sizeof(aradius));
        if(adamage == 5)
        { 
            Asteroid asteroid(ax, ay, adamage, "./asteroid1.PNG");
            asteroids.push_back(asteroid);
		}
		else if (adamage == 10)
		{
			Asteroid asteroid(ax, ay, adamage, "./asteroid2.PNG");
            asteroids.push_back(asteroid);
		}
        else
        {
            Asteroid asteroid(ax, ay, adamage, "./asteroid3.PNG");
            asteroids.push_back(asteroid);
        }
    }

    file.close();
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

    int gamestate = 0; // 0 = menu, 1 = game, 2 = pause, 3 = gameover/quit

    logger.writeerror("PlayerShip spawned.");

    while (!WindowShouldClose() && gamestate != 3)
    {
        BeginDrawing();

        //main menu
        if (gamestate == 0)
        {
            ClearBackground(BLACK);
            DrawTextureEx(mainmenu, pos, 0.0f, 1, WHITE);
			DrawText("Space Shooter", screenwidth / 2 - 375, screenheight / 2 - 200, 100, SKYBLUE);

            Rectangle newbutton = { screenwidth / 2.0f - 110, screenheight / 2.0f - 60, 200, 50 };
            Rectangle loadbutton = { screenwidth / 2.0f - 110, screenheight / 2.0f + 10, 200, 50 };

            if (drawbutton(newbutton, "New Game", GRAY, DARKBLUE, WHITE))
            {
                logger.writeerror("New Game started.");
                gamestate = 1;
            }
            if (drawbutton(loadbutton, "Load Game", GRAY, DARKBLUE, WHITE))
            {
                logger.writeerror("Load Game clicked.");

                loadgame(player, enemies, asteroids,
                    speedboosttimer, speedactive, originalspeed, originalradius,
                    damageboosttimer, damageactive, originaldamage,
                    powerupspawntimer, asteroidspawntimer, enemyspawntimer, gameover);

                projectiles.clear();
                for (auto* pu : powerups) delete pu;
                powerups.clear();

                gamestate = 1;
            }
            DrawText("Click to start", screenwidth / 2 - 80, screenheight / 2 + 90, 20, WHITE);

            EndDrawing();
            continue;
        }

        if (gamestate == 2)
        {
            ClearBackground(BLACK);
            DrawText("GAME PAUSED", screenwidth / 2 - 150, screenheight / 2 - 120, 40, WHITE);
            DrawText("Press P to resume", screenwidth / 2 - 100, screenheight / 2 - 60, 20, LIGHTGRAY);

            Rectangle savebutton = { screenwidth / 2.0f - 100, screenheight / 2.0f - 30, 200, 50 };
            Rectangle quitbutton = { screenwidth / 2.0f - 100, screenheight / 2.0f + 40, 200, 50 };

            if (drawbutton(savebutton, "Save Game", GRAY, DARKBLUE, WHITE))
            {
                savegame(player, enemies, projectiles, powerups, asteroids,
                    speedboosttimer, speedactive, originalspeed, originalradius,
                    damageboosttimer, damageactive, originaldamage,
                    powerupspawntimer, asteroidspawntimer, enemyspawntimer, gameover);

				gamestate = 0; // return to main menu after saving
                logger.writeerror("Game Saved.");
            }
            if (drawbutton(quitbutton, "Quit", GRAY, DARKBLUE, WHITE))
            {
                gamestate = 3;
                gameover = true;
                logger.writeerror("Quit Game clicked.");
            }
            if (IsKeyPressed(KEY_P)) //resume game
            {
                gamestate = 1;
            }
            EndDrawing();
            continue;
        }

		//gameplay background
        ClearBackground(BLACK);
        DrawTextureEx(gameplaybackground, pos, 0.0f, 1, WHITE);

        if (!gameover && gamestate == 1)
        {
            if (gamestate == gamestate == 1 && !gameover)
            {
                if (IsKeyPressed(KEY_P))
                {
                    gamestate = 2;
                    EndDrawing();
                    continue;
                }

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
        }
		if (gamestate == 3 || gameover)
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