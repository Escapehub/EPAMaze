#include <fstream>
#include <iostream>
#include <sstream>
#include "player.h"
#include "tilemap.h"
#include "sectors.hpp"
#include "menu.h"

int main()
{

    // Loading config file
    std::cout << "Drag and drop config file: " << std::endl;
    std::string configPath;
    std::cin >> configPath;

    std::string currentLine;
    std::ifstream configFile(configPath);
    sf::Vector2i MapgenCurrent(0,0);

    // Generating map from config file
    int* map[10][10];
    while (std::getline(configFile, currentLine)) {
        std::stringstream stringstream(currentLine);
        while (stringstream.good()) {
            std::string substr;
            std::getline(stringstream, substr, ',');
            if (substr == "horizontal")
                map[MapgenCurrent.x][MapgenCurrent.y] = Sector::Horizontal.getData().Sector;
            else if (substr == "cross")
                map[MapgenCurrent.x][MapgenCurrent.y] = Sector::Cross.getData().Sector;
            else if (substr == "vertical")
                map[MapgenCurrent.x][MapgenCurrent.y] = Sector::Vertical.getData().Sector;
            else if (substr == "bottomright")
                map[MapgenCurrent.x][MapgenCurrent.y] = Sector::BottomRight.getData().Sector;
            else if (substr == "bottomleft")
                map[MapgenCurrent.x][MapgenCurrent.y] = Sector::BottomLeft.getData().Sector;
            else if (substr == "topright")
                map[MapgenCurrent.x][MapgenCurrent.y] = Sector::TopRight.getData().Sector;
            else if (substr == "topleft")
                map[MapgenCurrent.x][MapgenCurrent.y] = Sector::TopLeft.getData().Sector;
            else if (substr == "verticalright")
                map[MapgenCurrent.x][MapgenCurrent.y] = Sector::VerticalRight.getData().Sector;
            else if (substr == "verticalleft")
                map[MapgenCurrent.x][MapgenCurrent.y] = Sector::VerticalLeft.getData().Sector;
            else if (substr == "horizontaltop")
                map[MapgenCurrent.x][MapgenCurrent.y] = Sector::HorizontalTop.getData().Sector;
            else if (substr == "horizontalbottom")
                map[MapgenCurrent.x][MapgenCurrent.y] = Sector::HorizontalBottom.getData().Sector;
            else {
                std::cout << "There is an error in your config file: " << substr << std::endl;
                main();
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
    bool isPlaying = false;

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

    // Menu
    Menu menu(window.getSize().x, window.getSize().y);

    // TileMap
    TileMap tilemap;
    sf::Vector2i currentSector(0,0);

    // Player wealth
    int wealth = 0;
    sf::Font arcadefont;
    arcadefont.loadFromFile("arcade.ttf");
    sf::Text playerwealth;
    playerwealth.setFont(arcadefont);
    playerwealth.setFillColor(sf::Color::Black);
    playerwealth.setPosition(sf::Vector2f(window.getSize().x - 150, 20));
    
    // Game running loop
    while (window.isOpen())
    {
        playerwealth.setString("Wealth: " + std::to_string(wealth));
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // Menu pressed events
            switch (event.type)
            {
            case sf::Event::KeyReleased:
                switch (event.key.code) {
                case sf::Keyboard::Up:
                    menu.moveUp();
                    break;
                case sf::Keyboard::Down:
                    menu.moveDown();
                    break;
                case sf::Keyboard::Return:
                    switch (menu.GetPressedItem())
                    {
                    case 0:
                        isPlaying = true;
                        break;
                    case 1:
                        window.close();
                        break;
                    }
                    break;
                }
                break;
            }
        }

        // Load TileMap
        if (!tilemap.load("tileset.png", sf::Vector2u(32, 32), map[currentSector.x][currentSector.y], 34, 16))
            return -1;

        // Show pause menu
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            isPlaying = false;

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

        // Show window with assests
        window.clear();
        if (isPlaying) {
            window.draw(tilemap);
            window.draw(player.getSprite());
            window.draw(playerwealth);
        }
        else {
            menu.Draw(window);
        }
        
        window.display();
    }
    return 0;
}