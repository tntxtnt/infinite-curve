#pragma once

#include <SFML/System/Vector2.hpp>

namespace math
{
const double pi = 3.14159265358979323846;
const double deg_per_rad = 180 / pi;
const double rad_per_dec = pi / 180;
} // namespace math

float distance(const sf::Vector2f& a, const sf::Vector2f& b);
sf::Vector2f lerp(const sf::Vector2f& a, const sf::Vector2f& b,
                  float percentage);
float angleSlope(const sf::Vector2f& a, const sf::Vector2f& b);
