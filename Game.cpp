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
    // Изчиства вектора с врагове, така че при ново ниво да бъдат премахнати, ако има останали, врагове
    enemies.clear();

    // създава временен вектор числа
    std::vector<int> temp;
    for (int i = 0; i < 40; ++i) {
        temp.push_back(i);
    }

    // Създаване на сийд, който веднага след това се използва, за да бъдат разбъркани числата в горния вектор
    std::random_device rd;
    std::mt19937 g(rd());
    shuffle(temp.begin(), temp.end(), g);

    // Разбърканият вектор бива използван, като числата от него определят хоризонталните позиции на враговете
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

    // Проверява се дали бутон на клавиатурата бива натиснат
    if (_kbhit()) {
        // Проверява се дали натиснатият бутон е "А", и ако да, тогава хоризонталната позиция на играчът намалява
        if (GetAsyncKeyState(*"A")) {
            player.moveLeft();
        }
        // Същото, но проверява за "D", като следователно увеличава хоризонталната позиция на играча
        else if (GetAsyncKeyState(*"D")) {
            player.moveRight();
        }
        // Проверява за натиснат "SPACE" бутон
        if (GetAsyncKeyState(VK_SPACE)) {
            // Взима сегашното време от часа на компютъра
            auto now = chrono::system_clock::now();
            // Съхранява разликата между now и playerStart
            // (който се инициализира в Game.h при започването на играта) в секунди
            auto timePassed = std::chrono::duration_cast<std::chrono::seconds>(now - playerStart);
            // Проверява дали shotCooldown(инициализиран при старта на играта) е по-малък от
            // timePassed + 500 милисекунди(произволно число, така че играчът да не стреля твърде често)
            // тоест гарантира, че играчът не стреля твърдо често
            if (shotCooldown <= timePassed + chrono::milliseconds(500)) {
                player.shoot(getBullets());
                // playerStart приема времето на now, така че при следващата проверка
                // timePassed използва времето при предишния изстрел(сегашния)
                playerStart = now;
            }



        }
    }

}
void Game::update() {
    // Взима сегашното време, което след това се използва за да се
    // изчисли изминалото време от последното движение на враговете
    auto now2 = chrono::system_clock::now();
    auto timePassedSinceEnemyMovement = std::chrono::duration_cast<std::chrono::seconds>(now2 - enemyMovementStart);

    // Проверява дали играчът няма останали животи, при което играта приключва
    if (player.getLives() == 0) {
        running = false;
        return;
    }
    // Проверява дали играчът е събрал достатъчно точки и дали вече не е получил допълнителен живот,
    // при което получава още един живот
    if (player.getScore() >= 300 && !player.getBonusLife()) {
        player.setLives(player.getLives() + 1);
        player.setBonusLife();
    }
    // Проверява дали изминалото време от последното движение на враговете * сегашното ниво >= 4 секунди,
    // и ако да, се променят вертикалните позиции на враговете с едно напред
    if (timePassedSinceEnemyMovement * level >= (chrono::seconds(4))) {
        for (GameObject* enemy : enemies) {
            enemy->setY(enemy->getY() + 1);
            enemyMovementStart = now2;
        }
    }
    // Проверява дали векторът с изстрели не е празен, при което минава през всеки изстрел
    // и променя вертикалната им позиция в зависимост от тяхната инициализирана посока
    if (!bullets.empty()) {
        for (GameObject* bullet : this->getBullets()) {
            bullet->update(bullet->getDirection());
        }
    }

    // Проверява нивото, и при положителен резултат увеличава enemyLevelBoost,
    // който се използва за увеличаването на трудността в отделните нива
    if (level == 2) {
        enemyLevelBoost = std::chrono::milliseconds(170);
    }
    else if (level == 3) {
        enemyLevelBoost = std::chrono::milliseconds(350);
    }

    // Съхранява сегашното време и изважда от него времето от последното стреляне на враговете
        auto now1 = chrono::system_clock::now();
        auto timePassedSinceEnemyShot = std::chrono::duration_cast<std::chrono::milliseconds>(now1 - enemyShootingStart);

    // Проверява дали достатъчно време е изминало от последният изстрел на враговете
    if (timePassedSinceEnemyShot >= (shotCooldown - enemyLevelBoost)) {
        // Създава рандъм сийд
        std::random_device rd;
        std::mt19937 g(rd());
        // Сийдът се използва, за да се избере случаен враг, който след това стреля
        std::uniform_int_distribution<int> dist(0, enemies.size() - 1);
        enemies.at(dist(g))->shoot(getBullets());
        enemyShootingStart = now1;
    }



}


void Game::checkCollisions(map<GameObject*, map<int, int>> loc) {
    // Проверява дали векторът с изстрели и/или loc(map с обектите на изстрелите и враговете, и техните позиции)
    if (bullets.empty() || loc.empty()) return;
    vector<GameObject*> enemyToRemove;
    vector<GameObject*> bulletToRemove;

    bool playerHitThisFrame = false;

    // Проверява дали враговете са достигнали вертикалното ниво на играча, което води до game over
    if (enemies.front()->getY() == player.getY()) {
        player.setLives(0);
        update();
    }

    // Минава през всички изстрели
    for (GameObject* bullet : this->getBullets()) {
        if (bullet == nullptr) continue;

        bool collision = false;

        int xPosition = bullet->getX();
        int yPosition = bullet->getY();

        // Изстрива изстрели, ако вертикалната им позиция е по-голяма от тази на играча
        if (bullet->getY() > 25) {
            bulletToRemove.push_back(bullet);
            continue;
        }
        // Минава през всички двойки в loc; двойката v = обектът(изстрел/враг) : друга двойка,
        // която съдържа позициите на обектите, тоест двойката е = хоризонтална позиция : вертикална позиция
        for (auto& v : loc) {
            for (auto& e : v.second) {
                // Проверява дали позициите на обекта(вражески изстрел в този случай) са еднакви на тези на играча,
                // и дали същият вече не е загубил живот в същия frame
                if (e.first == player.getX() && e.second == player.getY() && !playerHitThisFrame) {
                    player.setLives(player.getLives() - 1);
                    bulletToRemove.push_back(bullet);
                    collision = true;
                    playerHitThisFrame = true;
                    break;
                }
                // Проверява дали изстрел заема същите позиции на друг обект(вражески изстрел/враг)
                if (xPosition == e.first && yPosition == e.second) {
                    // Ако обектът(в loc) е враг, то той и оригиналния изстрел влизат във вектори за изтриване
                    if (typeid(*v.first) == typeid(Enemy)){
                        bulletToRemove.push_back(bullet);
                        enemyToRemove.push_back(v.first);
                        collision = true;
                        break;
                    }
                    // А ако обектът(в loc) е изстрел(вражески) и неговата посока е различна от тази на
                    // оригиналния изстрел, то тогаава и двата биват добавени във вектор за изтриване
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
    // Минава през вектора с врагове за изтриване и добавя точки в зависимост от нивото на врага
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
    // Съшото, но минава през вектора в изстрели за изтриване
    for (auto* bullet: bulletToRemove) {
        if (bullet != nullptr) {
            auto it = std::find(bullets.begin(), bullets.end(), bullet);
            if (it != bullets.end()) {
                delete bullet;
                bullets.erase(it);
            }
        }


    }
    // Проверява дали играчът е събрал достатъчно точки за преминаване на следващото ниво
    if (player.getScore() >= pointsToPass && level < 3) {
            level++;
            pointsToPass += 300;
            run();
        }
    // При липса на врагове  enemies играта приключва
    if (enemies.empty()) {
        bullets.clear();
        running = false;
        system("cls");
    }
}

void Game::render(map<GameObject*, map<int, int>> locations, map<GameObject*, map<int, int>> enemyBullets) {
    // Изчиства конзолата, локациите на обектите и изстрелите при нов рендър
    system("cls");
    locations.clear();
    enemyBullets.clear();

    // Изобразява всеки враг и добавя него и позициите му в locations map
    for (GameObject* enemy : enemies) {
        enemy->render();
        locations[enemy] = {{enemy->getX(), enemy->getY()}};
    }

    // Същото но за изстрели
    for (GameObject* bullet : this->getBullets()) {
        if (bullet != nullptr) {
            locations[bullet] = {{bullet->getX(), bullet->getY()}};
            bullet->render();
        }
    }
    // Проверява колизиите между различните обекти
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




