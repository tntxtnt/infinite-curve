#pragma once

#include "VertexDeque.h"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/View.hpp>
#include <functional>

class Curve : public sf::Drawable
{
public:
    template <class YGen>
    Curve(float xStep, unsigned screenWidth, YGen&& yGen)
    : xStep{xStep},
      lines{sf::LinesStrip, (int)std::ceil(screenWidth / xStep)},
      indexOffset{0},
      yGen{yGen}
    {
        for (int i = 0; i < lines.getVertexCount(); ++i)
        {
            lines[i].position = sf::Vector2f{i * xStep, yGen(i * xStep)};
            lines[i].color = sf::Color::Black;
        }
    }
    sf::Vector2f& operator[](int index);
    const sf::Vector2f& operator[](int index) const;
    void syncWithView(const sf::View& view);
    size_t getPointsCount() const;

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    float xStep;
    VertexContiguousDeque lines;
    int indexOffset;
    std::function<float(float)> yGen;
};
