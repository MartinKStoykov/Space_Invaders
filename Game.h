#ifndef GAME_H
#define GAME_H
#pragma once
#include <vector>
#include "Player.h"
#include "GameObject.h"
#include <chrono>
#include <map>

class Game {
private:
    Player player;
    std::vector<GameObject*> enemies;
    std::vector<GameObject*> bullets;
    int score, level;
    bool running;
    std::chrono::system_clock::time_point start;
    std::chrono::milliseconds shotCooldown;
public:
    Game();

    Game(Player player, const std::vector<GameObject *> &enemies, const std::vector<GameObject*> &bullets, int score,
         int level, bool running);


    void initializeEnemies();
    void input();
    void update();
    void checkCollisions(map<GameObject*, map<int, int>> loc);
    void render();
    void run();
    std::vector<GameObject*>& getBullets();
};



#endif //GAME_H
