#include "GameObject.h"

#include <iostream>
#include "Visualization.h"

GameObject::GameObject() : x(0), y(0), symbol('X'), color(WHITE) {}

GameObject::GameObject(int x, int y, char symbol, COLORS color) : x(x), y(y), symbol(symbol), color(color) {}

GameObject::GameObject(const GameObject &other) : x(other.x), y(other.y), symbol(other.symbol), color(other.color){}

GameObject&GameObject::operator=(const GameObject &other) {
    x = other.x;
    y = other.y;
    symbol = other.symbol;
    color = other.color;
    return *this;
    }

GameObject::GameObject(const GameObject &&other) noexcept : x(other.x), y(other.y), symbol(other.symbol), color(other.color) {}

GameObject&GameObject::operator=(GameObject &&other) noexcept {
    x = other.x;
    y = other.y;
    symbol = other.symbol;
    color = other.color;

    other.x = 0;
    other.y = 0;
    other.symbol = 'X';
    other.color = COLORS::WHITE;

    return *this;
}

int GameObject::getX() const {
    return this->x;
}

int GameObject::getY() const {
    return this->y;
}

char GameObject::getSymbol() const {
    return this->symbol;
}

COLORS GameObject::getColor() const {
    return this->color;
}

void GameObject::setX(int newX) {
    this->x = newX;
}

void GameObject::setY(int newY) {

    this->y = newY;
}


void GameObject::setSymbol(char newSymbol) {this->symbol = newSymbol;}

void GameObject::setColor(COLORS newColor) {
    this->color = newColor;
}

void GameObject::update(int direction) {
    this->setY(this->getY() + direction);
}

void GameObject::render() {
    draw_char(this->getSymbol(), this->getY(), this->getX(), this->getColor(), BLACK);
}

GameObject::~GameObject() {}

