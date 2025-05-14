#ifndef PLAYER_H
#define PLAYER_H
#pragma once
#include <vector>

#include "GameObject.h"

class Player : public GameObject{
private:
    int lives;
    int score;

public:

    Player();
    Player(int lives, int score, int x, int y, char symbol, COLORS colors);
    Player(const Player &other);
    Player &operator=(const Player &other);

    Player(Player &&other) noexcept;
    Player &operator=(Player &&other) noexcept;

    void moveLeft();
    void moveRight();
    void shoot(vector<GameObject*>& bullets);

    int getLives();
    int getScore();

    void setLives(int newLives);
    void setScore(int newScore);

    Player &operator+(int points);
    Player &operator-(int points);
};
#endif //PLAYER_H
