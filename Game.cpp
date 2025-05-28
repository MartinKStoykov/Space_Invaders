#include "Game.h"
#include "Enemy.h"
#include "conio.h"
#include <chrono>
#include <map>
#include <algorithm>
#include <random>

#include "Bullet.h"


Game::Game() : player(3, 0, 30, 25, '^', WHITE, false),
               pointsToPass(200),
               enemies(0),
               bullets(0),
               score(0),
               level(1),
               running(true),
               shotCooldown(std::chrono::milliseconds(600)),
               enemyLevelBoost(std::chrono::milliseconds(0)){}

Game::Game(Player player, const std::vector<GameObject*>& enemies, const std::vector<GameObject*>& bullets, int score, int level, bool running, int pointsToPass)
    : player(player), enemies(enemies), bullets(bullets), score(score), level(level), running(running), shotCooldown(), pointsToPass(pointsToPass) {
}

void Game::initializeEnemies() {
    enemies.clear();

    std::vector<int> temp;
    for (int i = 0; i < 40; ++i) {
        temp.push_back(i);
    }
    std::random_device rd;
    std::mt19937 g(rd());
    shuffle(temp.begin(), temp.end(), g);


    for (int e1 = 0; e1 < 18; ++e1) {
        enemies.push_back(new Enemy(1 , 10, temp[e1], 1, 'S', LIGHT_BLUE));}
    for (int e2 = 18; e2 < 30; ++e2) {
        enemies.push_back( new Enemy(1 , 20, temp[e2], 1, 'D', BLUE));}
    for (int e3 = 30; e3 < 37; ++e3) {
        enemies.push_back(new Enemy(1 , 30, temp[e3], 1, 'H', LIGHT_RED));}
    for (int e4 = 37; e4 < 40; ++e4) {
        enemies.push_back(new Enemy(1 , 40, temp[e4], 1, 'X', RED));}
}
void Game::input() {


    if (_kbhit()) {
        if (GetAsyncKeyState(*"A")) {
            player.moveLeft();
        }
        else if (GetAsyncKeyState(*"D")) {
            player.moveRight();
        }
        if (GetAsyncKeyState(VK_SPACE)) {
            auto now = chrono::system_clock::now();
            auto timePassed = std::chrono::duration_cast<std::chrono::seconds>(now - playerStart);
            if (shotCooldown <= timePassed + chrono::milliseconds(500)) {
                player.shoot(getBullets());
                playerStart = now;
            }



        }
    }

}
void Game::update() {
    auto now2 = chrono::system_clock::now();
    auto timePassedSinceEnemyMovement = std::chrono::duration_cast<std::chrono::seconds>(now2 - enemyMovementStart);

    if (player.getLives() == 0) {
        running = false;
        return;
    }
    if (player.getScore() >= 300 && !player.getBonusLife()) {
        player.setLives(player.getLives() + 1);
        player.setBonusLife();
    }
    if (timePassedSinceEnemyMovement * level >= (chrono::seconds(4))) {
        for (GameObject* enemy : enemies) {
            enemy->setY(enemy->getY() + 1);
            enemyMovementStart = now2;
        }
    }
    if (!bullets.empty()) {
        for (GameObject* bullet : this->getBullets()) {
            bullet->update(bullet->getDirection());
        }
    }

    auto now1 = chrono::system_clock::now();
    auto timePassedSinceEnemyShot = std::chrono::duration_cast<std::chrono::milliseconds>(now1 - enemyShootingStart);
    if (level == 2) {
        enemyLevelBoost = std::chrono::milliseconds(170);
    }
    else if (level == 3) {
        enemyLevelBoost = std::chrono::milliseconds(350);
    }



    if (timePassedSinceEnemyShot >= (shotCooldown - enemyLevelBoost)) {
        std::random_device rd;
        std::mt19937 g(rd());
        std::uniform_int_distribution<int> dist(0, enemies.size() - 1);
        enemies.at(dist(g))->shoot(getBullets());
        enemyShootingStart = now1;
    }



}


void Game::checkCollisions(map<GameObject*, map<int, int>> loc) {
    if (bullets.empty() || loc.empty()) return;
    vector<GameObject*> enemyToRemove;
    vector<GameObject*> bulletToRemove;

    bool playerHitThisFrame = false;

    if (enemies.front()->getY() == player.getY()) {
        player.setLives(0);
        update();
    }

    for (GameObject* bullet : this->getBullets()) {
        if (bullet == nullptr) continue;

        bool collision = false;

        int xPosition = bullet->getX();
        int yPosition = bullet->getY();


        if (bullet->getY() > 25) {
            bulletToRemove.push_back(bullet);
            continue;
        }
        for (auto& v : loc) {
            for (auto& e : v.second) {
                if (e.first == player.getX() && e.second == player.getY() && !playerHitThisFrame) {
                    player.setLives(player.getLives() - 1);
                    bulletToRemove.push_back(bullet);
                    collision = true;
                    playerHitThisFrame = true;
                    break;
                }
                if (xPosition == e.first && yPosition == e.second) {
                    if (typeid(*v.first) == typeid(Enemy)){
                        bulletToRemove.push_back(bullet);
                        enemyToRemove.push_back(v.first);
                        collision = true;
                        break;
                    }
                    if (typeid(*v.first) == typeid(Bullet) && bullet->getDirection() != v.first->getDirection()) {
                        bulletToRemove.push_back(bullet);
                        bulletToRemove.push_back(v.first);
                        collision = true;
                        break;
                    }
                }
            }
            if (collision) break;
        }
    }
    for (auto* enemy: enemyToRemove) {
        if (enemy != nullptr) {
            player.setScore(enemy->getPoints() + player.getScore());
            auto it = std::find(enemies.begin(), enemies.end(), enemy);
            if (it != enemies.end()) {
                delete enemy;
                enemies.erase(it);
            }
        }
    }
    for (auto* bullet: bulletToRemove) {
        if (bullet != nullptr) {
            auto it = std::find(bullets.begin(), bullets.end(), bullet);
            if (it != bullets.end()) {
                delete bullet;
                bullets.erase(it);
            }
        }


    }
    if (player.getScore() >= pointsToPass && level < 3) {
            level++;
            pointsToPass += 300;
            run();
        }

    if (enemies.empty()) {
        bullets.clear();
        running = false;
        system("cls");
    }
}

void Game::render(map<GameObject*, map<int, int>> locations, map<GameObject*, map<int, int>> enemyBullets) {
    system("cls");
    locations.clear();
    enemyBullets.clear();

    for (GameObject* enemy : enemies) {
        enemy->render();
        locations[enemy] = {{enemy->getX(), enemy->getY()}};
    }


    for (GameObject* bullet : this->getBullets()) {
        if (bullet != nullptr) {
            locations[bullet] = {{bullet->getX(), bullet->getY()}};
            bullet->render();
        }
    }

    checkCollisions(locations);


    player.render();
    std::cout << "LEVEL<" << this->level << "> SCORE<" << player.getScore()  << "> LIVES<" << player.getLives() << ">" << std::endl;

    Sleep(20);
}

void Game::run() {
    enemyShootingStart = std::chrono::system_clock::now();
    initializeEnemies();
    map<GameObject*, map<int, int>> enemyLocations;
    map<GameObject*, map<int, int>> enemyBullets;
    while (running) {

        input();
        update();
        if (!running) break;
        render(enemyLocations, enemyBullets);
    }

    if (player.getLives() == 0) {
        cout << "GAME OVER";
    } else {
        cout<< "CONGRATULATIONS YOU WON THE GAME!!!";
    }

    std::cin.get();
};

std::vector<GameObject*>& Game::getBullets() {
    return this->bullets;
};




