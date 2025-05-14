#include "Game.h"
#include "Enemy.h"
#include "conio.h"
#include <chrono>
#include <map>


Game::Game() : player(3, 0, 30, 14, '^', WHITE),enemies(0),bullets(0),score(0),level(1),running(false), shotCooldown(350){}

Game::Game(Player player, const std::vector<GameObject*>& enemies, const std::vector<GameObject*>& bullets, int score, int level, bool running)
    : player(player), enemies(0), bullets(bullets), score(score), level(level), running(running), shotCooldown() {
}

void Game::initializeEnemies() {
    float alienSpeed = 1.0;

    if (level == 2) {
        alienSpeed = 1.3;
    }
    else if (level == 3) {
        alienSpeed = 1.6;
    }

    for (int i = 0; i <= 40; ++i) {
        for ( int j = 0; j <= 3; ++j) {
            GameObject* enemy = new Enemy(1 , 10, alienSpeed, i, j, 'X', RED);
            enemies.push_back(enemy);
        }
    }
}
void Game::input() {


    if (_kbhit()) {
        if (GetAsyncKeyState(VK_LEFT)) {
            player.moveLeft();
        }
        else if (GetAsyncKeyState(VK_RIGHT)) {
            player.moveRight();
        }
        if (GetAsyncKeyState(VK_RETURN)) {
            auto now = chrono::system_clock::now();
            auto timePassed = std::chrono::duration_cast<std::chrono::seconds>(now - start);
            if (shotCooldown <= timePassed) {
                player.shoot(getBullets());
                start = now;
            }



        }
    }

}

void Game::update() {
    for (GameObject* bullet : this->getBullets()) {
        bullet->update(-1);
    }
}


void Game::checkCollisions(map<GameObject*, map<int, int>> loc) {
    if (bullets.empty() || loc.empty()) return;
    vector<GameObject*> enemyToRemove;
    vector<GameObject*> bulletToRemove;
    for (GameObject* bullet : this->getBullets()) {
        int xPosition = bullet->getX();
        int yPosition = bullet->getY();
        bool collision = false;

        for (auto& v : loc) {
            for (auto& e : v.second) {
                if (xPosition == e.first && yPosition == e.second) {
                    bulletToRemove.push_back(bullet);
                    enemyToRemove.push_back(v.first);
                    bullet = nullptr;
                    collision = true;
                    break;
                }
            }
            if (collision) {
                break;
                };
            }
        }

    for (auto* enemy: enemyToRemove) {
        auto it = std::find(enemies.begin(), enemies.end(), enemy);
        enemies.erase(it);
    }

    for (auto* bullet: bulletToRemove) {
        auto it = std::find(bullets.begin(), bullets.end(), bullet);
        bullets.erase(it);
    }


}


map<GameObject*, map<int, int>> enemyLocations;
void Game::render() {
    system("cls");
    enemyLocations.clear();
    for (GameObject* enemy : enemies) {
        enemy->render();
        enemyLocations[enemy] = {{enemy->getX(), enemy->getY()}};
    }

    for (GameObject* bullet : this->getBullets()) {
        checkCollisions(enemyLocations);

        bullet->render();
    }

    player.render();
    //std::cout << "LEVEL<" << this->level << "> SCORE<" << player.getScore()  << "> LIVES<" << player.getLives() << ">" << std::endl;

    Sleep(20);
}

void Game::run() {
    running = true;
    initializeEnemies();
    while (running) {

        input();
        update();
        render();
    }
};

std::vector<GameObject*>& Game::getBullets() {
    return this->bullets;
};




