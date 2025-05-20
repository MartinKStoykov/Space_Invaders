#include "Enemy.h"

#include <vector>

#include "Bullet.h"
#include "Visualization.h"
class Bullet;
Enemy::Enemy() : direction(1), points(0) {}

Enemy::Enemy(
    int direction,
    int points,
    int x, int y,
    char symbol,
    COLORS color)
    : GameObject(x, y, symbol, color), direction(direction), points(points){}

Enemy::Enemy(const Enemy &other) : direction(other.direction), points(other.points) {}

Enemy&Enemy::operator=(const Enemy &other) {
    direction = other.direction;
    points = other.points;
    return *this;
}

Enemy::Enemy(Enemy &&other) noexcept : direction(other.direction), points(other.points) {}

Enemy &Enemy::operator=(Enemy &&other) noexcept {
    direction = other.direction;
    points = other.points;
    other.direction = 1;
    other.points = 0;
    return *this;
    }

void Enemy::update() {

}

void Enemy::render() {
    GameObject::render();
}

int Enemy::getPoints() {
    return this->points;
}

void Enemy::shoot(vector<GameObject*>& bullets) {
    Bullet* bullet = new Bullet(this->getX(), this->getY() +1, 1,  '|', LIGHT_RED);
    bullets.push_back(bullet);
}