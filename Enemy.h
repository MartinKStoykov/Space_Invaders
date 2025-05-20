#ifndef ENEMY_H
#define ENEMY_H
#pragma once
#include <vector>

#include "GameObject.h"

class Enemy : public GameObject {
private:
    int direction;
    int points;

public:

    Enemy();
    Enemy(int direction, int points, int x, int y, char symbol, COLORS color);
    Enemy(const Enemy &other);
    Enemy &operator=(const Enemy &other);
    Enemy(Enemy &&other) noexcept;
    Enemy &operator=(Enemy &&other) noexcept;

    void update();
    void render();
    int getPoints() override;
    void shoot(vector<GameObject*>& bullets) override;
};




#endif //ENEMY_H
