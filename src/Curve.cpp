#include "Curve.h"
#include <SFML/Graphics/RenderTarget.hpp>

sf::Vector2f& Curve::operator[](int index)
{
    return lines[index - indexOffset].position;
}

const sf::Vector2f& Curve::operator[](int index) const
{
    return lines[index - indexOffset].position;
}

void Curve::syncWithView(const sf::View& view)
{
    float left = view.getCenter().x - view.getSize().x / 2;
    float right = view.getCenter().x + view.getSize().x / 2;
    // Append more points
    while (lines[lines.getVertexCount() - 1].position.x < right)
    {
        float x = lines[lines.getVertexCount() - 1].position.x + xStep;
        float y = yGen(x);
        lines.append(sf::Vertex{sf::Vector2f{x, y}, sf::Color::Black});
        // doesn't affect indexOffset
    }
    // Prepend more points
    while (lines[0].position.x > left)
    {
        float x = lines[0].position.x - xStep;
        float y = yGen(x);
        lines.prepend(sf::Vertex{sf::Vector2f{x, y}, sf::Color::Black});
        indexOffset--;
    }
    // Trim right points
    while (lines[lines.getVertexCount() - 1].position.x - xStep > right)
    {
        lines.removeLast();
        // doesn't affect indexOffset
    }
    // Trim left points
    while (lines[0].position.x + xStep < left)
    {
        lines.removeFirst();
        indexOffset++;
    }
}

size_t Curve::getPointsCount() const
{
    return lines.getVertexCount();
}

void Curve::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(lines, states);
}
