#include "Player.h"
#include "Bullet.h"
#include "Game.h"


Player::Player() : GameObject(0, 0, 'X' , WHITE), lives(0), score(0) {}

Player::Player(int lives, int score, int x, int y, char symbol, COLORS color) : GameObject(x, y, symbol, color),
lives(lives), score(score) {}

Player::Player(const Player &other) : lives(other.lives), score(other.score) {}

Player &Player::operator=(const Player &other) {
        return *this;
}

Player::Player(Player &&other) noexcept : lives(other.lives), score(other.score) {
    other.lives = 0;
    other.score = 0;
}

Player &Player::operator=(Player &&other) noexcept {
    lives = other.lives;
    score = other.score;
    other.lives = 0;
    other.score = 0;
    return *this;
}
void Player::moveLeft() {
    if (this->getX() != 0) {
        this->setX(this->getX() - 1);
    }

}

void Player::moveRight() {
    if (this->getX() != 50) {
        this->setX(this->getX() + 1);
    }

}

void Player::shoot(std::vector<GameObject*>& bullets) {
    Bullet* bullet = new Bullet(this->getX(), this->getY() - 1, 0,  '|', LIGHT_GREEN);
    bullets.push_back(bullet);
}


int Player::getLives() {
    return this->lives;
}

int Player::getScore() {
    return this->score;
}


void Player::setLives(int newLives) {
    lives = newLives;
}

void Player::setScore(int newScore) {
    score = newScore;
}


Player &Player::operator+(int points) {
    score += points;
    return *this;
}

Player &Player::operator-(int points) {
    score -= points;
    return *this;
}
