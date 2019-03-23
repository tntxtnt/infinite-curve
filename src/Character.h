#pragma once

#include "Curve.h"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Shape.hpp>

class Character : public sf::Drawable
{
public:
    Character(sf::Shape& shape, const Curve& curve, float moveSpeed = 300.f);
    sf::Vector2f move(float amount);
    sf::Vector2f moveLeft(float moveAmt);
    sf::Vector2f moveRight(float moveAmt);
    void fixRotation();
    void addMoveSpeed(float amount);
    float getMoveSpeed() const;
    sf::Vector2f getPosition() const;

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    sf::Shape& shape;
    float moveSpeed;
    const Curve& curve;
    int segmentIndex;
    float segmentPercentage;
};
