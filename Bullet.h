#ifndef BULLET_H
#define BULLET_H
#pragma once
#include "GameObject.h"

class Bullet : public GameObject {
private:
int direction;
public:
    Bullet();
    Bullet(int x, int y, int direction, char symbol, COLORS color);
    Bullet(int direction);
    Bullet(const Bullet& other);
    Bullet& operator=(const Bullet& other);
    Bullet(Bullet&& other) noexcept;
    Bullet& operator=(Bullet&& other) noexcept;

    void update();
    void render();
    int getDirection();


};



#endif //BULLET_H
