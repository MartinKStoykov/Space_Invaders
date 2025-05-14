#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "Visualization.h"

class GameObject {
private:
    int x, y;
    char symbol;
    COLORS color;

public:
    GameObject();
    GameObject(int x, int y, char symbol, COLORS color);
    GameObject(const GameObject &other);
    GameObject& operator=(const GameObject &other);
    GameObject(const GameObject &&other) noexcept;
    GameObject&operator=(GameObject &&other) noexcept;

    int getX() const;
    int getY() const;

    char getSymbol() const;
    COLORS getColor() const;

    void setX(int newX);
    void setY(int newY);

    void setSymbol(char newSymbol);
    void setColor(COLORS newColor);

    virtual void update(int direction);

    virtual void render();

    virtual ~GameObject();
};

#endif // GAMEOBJECT_HPP
