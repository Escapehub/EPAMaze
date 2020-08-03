#include "player.h"

int main()
{
    // Window
    float width = 1088, height = 512;
    sf::RenderWindow window(sf::VideoMode(width, height), "Maze");

    // Player
    sf::Texture playerTexture;
    playerTexture.loadFromFile("character.png");
    Player player(playerTexture);
    player.setEndFrame(192);
    player.setAnimationSpeed(.1f);
    player.setSpeed(.05f);
    player.setPos(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));
    player.addAnimation(sf::IntRect(0, 192, 64, 64)); // up
    player.addAnimation(sf::IntRect(0, 0, 64, 64)); // down
    player.addAnimation(sf::IntRect(0, 64, 64, 64)); // left
    player.addAnimation(sf::IntRect(0, 128, 64, 64)); // right

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            player.Move(Player::Direction::Up);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            player.Move(Player::Direction::Right);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            player.Move(Player::Direction::Down);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            player.Move(Player::Direction::Left);

        window.clear();
        window.draw(player.getSprite());
        window.display();
    }
    return 0;
}