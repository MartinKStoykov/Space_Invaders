#include "Bullet.h"
#include <iostream>


Bullet::Bullet() : direction(0){}
Bullet::Bullet(int x, int y, int direction, char symbol, COLORS color) : GameObject(x, y, symbol, color), direction(direction) {}
Bullet::Bullet(int direction) : direction(direction) {}
Bullet::Bullet(const Bullet &other) : direction(other.direction) {}
Bullet &Bullet::operator=(const Bullet &other) {
    direction = other.direction;
    return *this;
}
Bullet::Bullet(Bullet &&other) noexcept {
    direction = other.direction;
    other.direction = 0;
}
Bullet &Bullet::operator=(Bullet &&other) noexcept {
    direction = other.direction;
    other.direction = 0;
    return *this;
}


void Bullet::update(){
    if (this->getY() > 25) {
        delete this;
    }
    GameObject::update(this->direction);
}
void Bullet::render() {
    GameObject::render();
}

int Bullet::getDirection() {
    return this->direction;
}





