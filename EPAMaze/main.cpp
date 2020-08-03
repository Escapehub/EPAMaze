#include <fstream>
#include <iostream>
#include <sstream>
#include "player.h"
#include "tilemap.h"
#include "sectors.hpp"

int main()
{
    int* map[10][10];
    std::cout << "Drag and drop config file: " << std::endl;
    std::string configPath;
    std::cin >> configPath;

    std::string currentLine;
    std::ifstream configFile(configPath);
    sf::Vector2i MapgenCurrent(0,0);
    while (std::getline(configFile, currentLine)) {
        std::stringstream stringstream(currentLine);
        while (stringstream.good()) {
            std::string substr;
            std::getline(stringstream, substr, ',');
            if (substr == "blank") {
                map[MapgenCurrent.x][MapgenCurrent.y] = Sector::Blank.getData().Sector;
            }
            if (substr == "topline") {
                map[MapgenCurrent.x][MapgenCurrent.y] = Sector::TopLine.getData().Sector;
            }
            if (substr == "horizontal") {
                map[MapgenCurrent.x][MapgenCurrent.y] = Sector::Horizontal.getData().Sector;
            }
            MapgenCurrent.y++;
        }
        MapgenCurrent.y = 0;
        MapgenCurrent.x++;
    }
    
    configFile.close();
    
    // Window
    float width = 1088, height = 512;
    sf::RenderWindow window(sf::VideoMode(width, height), "Maze");
    // Player
    sf::Texture playerTexture;
    playerTexture.loadFromFile("character.png");
    Player player(playerTexture);
    player.setEndFrame(192);
    player.setAnimationSpeed(.1f);
    player.setFrameSize(64);
    player.setSpeed(.5f); // .05f
    player.setPos(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));
    player.addAnimation(sf::IntRect(0, 192, 64, 64)); // up
    player.addAnimation(sf::IntRect(0, 0, 64, 64)); // down
    player.addAnimation(sf::IntRect(0, 64, 64, 64)); // left
    player.addAnimation(sf::IntRect(0, 128, 64, 64)); // right

    // TileMap
    TileMap tilemap;
    sf::Vector2i currentSector(0,0);
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
        }

        if (!tilemap.load("tileset.png", sf::Vector2u(32, 32), map[currentSector.x][currentSector.y], 34, 16))
            return -1;

        // Move Player
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            player.Move(Player::Direction::Up);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            player.Move(Player::Direction::Right);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            player.Move(Player::Direction::Down);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            player.Move(Player::Direction::Left);

        // Loop player around screen
        if (player.getSprite().getPosition().x > window.getSize().x) { // Horizontal Right
            currentSector.y++;
            player.setPos(sf::Vector2f(0, player.getSprite().getPosition().y));
        }
        if (player.getSprite().getPosition().x < 0) { // Horizontal left
            currentSector.y--;
            player.setPos(sf::Vector2f(window.getSize().x, player.getSprite().getPosition().y));
        }
        if (player.getSprite().getPosition().y > window.getSize().y) { // Vertical down
            currentSector.x++;
            player.setPos(sf::Vector2f(player.getSprite().getPosition().x, 0));
        }
        if (player.getSprite().getPosition().y < 0) { // Vertical up
            currentSector.x--;
            player.setPos(sf::Vector2f(player.getSprite().getPosition().x, window.getSize().y));
        }

        window.clear();
        window.draw(tilemap);
        window.draw(player.getSprite());
        window.display();
    }
    return 0;
}