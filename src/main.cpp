#include "Character.h"
#include "Curve.h"
#include "ImprovedPerlinNoise.h"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>

int main()
{
    sf::ContextSettings contextSettings;
    contextSettings.antialiasingLevel = 8;
    sf::RenderWindow window{
        {600, 300}, "Infinite Curve", sf::Style::Default, contextSettings};
    window.setFramerateLimit(60);

    ImprovedPerlinNoise noiseGen;
    using namespace std::chrono;
    auto seed = duration_cast<duration<unsigned>>(
                    system_clock::now().time_since_epoch())
                    .count();
    noiseGen.shuffle(std::mt19937{seed});
    auto yGen = [&](float x) {
        float xScale = 2e-3f;
        float yMagnitude1 = 10;
        float yMagnitude2 = 120;
        float yMagnitude3 = 60;
        float yMagnitude4 = 20;
        return float(yMagnitude1 * noiseGen.noise(x * xScale * 0.5) +
                     yMagnitude2 * noiseGen.noise(x * xScale * 1) +
                     yMagnitude3 * noiseGen.noise(x * xScale * 2) +
                     yMagnitude4 * noiseGen.noise(x * xScale * 4));
    };

    Curve curve{6.f, window.getSize().x, yGen};
    sf::RectangleShape shape{{20, 40}};
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(10, 40);
    Character character{shape, curve};

    sf::View view = window.getView();
    view.setCenter(0, 0);
    curve.syncWithView(view);

    sf::Font font;
    if (!font.loadFromFile("HelvetiPixel.ttf"))
    {
        std::cerr << "Cannot open HelvetiPixel.ttf\n";
        return 1;
    }
    sf::Text seedText{"Seed: " + std::to_string(seed), font, 16};
    seedText.setPosition(0, 0);
    seedText.setFillColor(sf::Color::Black);
    sf::Text moveSpeedText{
        "Move speed: " + std::to_string(character.getMoveSpeed()), font, 16};
    moveSpeedText.setPosition(0, 16);
    moveSpeedText.setFillColor(sf::Color::Black);
    sf::Text curvePointsText{
        "Curve points: " + std::to_string(curve.getPointsCount()), font, 16};
    curvePointsText.setPosition(0, 32);
    curvePointsText.setFillColor(sf::Color::Black);
    sf::Text positionText{
        "Position: " + std::to_string(character.getPosition().x) + ", " +
            std::to_string(character.getPosition().y),
        font, 16};
    positionText.setPosition(0, 48);
    positionText.setFillColor(sf::Color::Black);

    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Up)
                {
                    character.addMoveSpeed(10.f);
                    moveSpeedText.setString(
                        "Move speed: " +
                        std::to_string(character.getMoveSpeed()));
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    character.addMoveSpeed(-10.f);
                    moveSpeedText.setString(
                        "Move speed: " +
                        std::to_string(character.getMoveSpeed()));
                }
            }
        }

        sf::Time elapsed = clock.restart();
        float moveAmount = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            moveAmount += character.getMoveSpeed() * elapsed.asSeconds();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            moveAmount -= character.getMoveSpeed() * elapsed.asSeconds();
        if (moveAmount != 0)
        {
            view.move(character.move(moveAmount));
            curve.syncWithView(view);
            curvePointsText.setString("Curve points: " +
                                      std::to_string(curve.getPointsCount()));
            positionText.setString(
                "Position: " + std::to_string(character.getPosition().x) +
                ", " + std::to_string(character.getPosition().y));
        }

        window.setView(view);
        window.clear(sf::Color::White);
        window.draw(curve);
        window.draw(character);
        window.setView(window.getDefaultView());
        window.draw(seedText);
        window.draw(moveSpeedText);
        window.draw(curvePointsText);
        window.draw(positionText);
        window.display();
    }
}
