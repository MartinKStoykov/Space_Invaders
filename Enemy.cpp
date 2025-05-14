#include "Enemy.h"
#include "Visualization.h"
Enemy::Enemy() : direction(0), points(0), speed(0) {}

Enemy::Enemy(int direction, int points, float speed, int x, int y, char symbol, COLORS color) : GameObject(x, y, symbol, color), direction(direction), points(points), speed(speed) {}

Enemy::Enemy(const Enemy &other) : direction(other.direction), points(other.points), speed(other.speed) {}

Enemy&Enemy::operator=(const Enemy &other) {
    direction = other.direction;
    speed = other.speed;
    points = other.points;
    return *this;
}

Enemy::Enemy(Enemy &&other) noexcept : direction(other.direction), points(other.points), speed(other.speed) {}

Enemy &Enemy::operator=(Enemy &&other) noexcept {
    direction = other.direction;
    points = other.points;
    speed = other.speed;
    other.direction = 0;
    other.points = 0;
    speed = 1.0;
    return *this;
    }

void Enemy::update() {

}

void Enemy::render() {
    GameObject::render();
}