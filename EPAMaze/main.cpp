#include <fstream>
#include <iostream>
#include <sstream>
#include "player.h"
#include "tilemap.h"
#include "sectors.hpp"
#include "menu.h"

bool shouldShowTreasure() {
    int random = rand() % 2;
    if (random == 2)
        return true;
    else
        return false;
}
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
    player.setMoveEndFrame(192);
    player.setAnimationSpeed(.1f);
    player.setFrameSize(64);
    player.setSpeed(.1f); // .05f
    player.setPos(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));
    player.addMoveAnimation(sf::IntRect(0, 192, 64, 64)); // up
    player.addMoveAnimation(sf::IntRect(0, 0, 64, 64)); // down
    player.addMoveAnimation(sf::IntRect(0, 64, 64, 64)); // left
    player.addMoveAnimation(sf::IntRect(0, 128, 64, 64)); // right

    sf::Texture knightTexture;
    knightTexture.loadFromFile("knight.png");
    Player knight(knightTexture);
    knight.setMoveEndFrame(896);
    knight.setAnimationSpeed(0.1f);
    knight.setFrameSize(128);
    knight.setSpeed(.1f);
    knight.setPos(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));
    knight.addMoveAnimation(sf::IntRect(0, 256, 128, 128)); // up
    knight.addMoveAnimation(sf::IntRect(0, 768, 128, 128)); // down
    knight.addMoveAnimation(sf::IntRect(0, 0, 128, 128)); // left
    knight.addMoveAnimation(sf::IntRect(0, 512, 128, 128)); // Right

    knight.setAttackStartFrame(1024);
    knight.setAttackEndFrame(1408);
    knight.addAttackAnimation(sf::IntRect(0, 896, 128, 128)); // attack left
    knight.addAttackAnimation(sf::IntRect(256, 896, 128, 128)); // attack up
    knight.addAttackAnimation(sf::IntRect(512, 896, 128, 128)); // attack right
    knight.addAttackAnimation(sf::IntRect(768, 896, 128, 128)); // attack down
    

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
    
    // Treasure
    sf::Texture treasureTexture;
    treasureTexture.loadFromFile("treasure.png");
    sf::Sprite treasure(treasureTexture);
    treasure.setTextureRect(sf::IntRect(0, 0, 32, 32));
    treasure.setPosition(sf::Vector2f(rand() % window.getSize().x, rand() % window.getSize().y));
    bool showTreasure = false;
    // Game running loop
    while (window.isOpen())
    {
        window.clear();
        bool isAttacking = false;
        playerwealth.setString("Wealth: " + std::to_string(wealth));
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // key released events
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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            knight.Attack();
            isAttacking = true;
        }
        // Move Player
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            knight.Move(Player::Direction::Up);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            knight.Move(Player::Direction::Right);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            knight.Move(Player::Direction::Down);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            knight.Move(Player::Direction::Left);

        // Loop player around screen
        if (knight.getSprite().getPosition().x > window.getSize().x) { // Horizontal Right
            currentSector.y++;
            showTreasure = shouldShowTreasure();
            knight.setPos(sf::Vector2f(0, knight.getSprite().getPosition().y));
        }
        if (knight.getSprite().getPosition().x < 0) { // Horizontal left
            currentSector.y--;
            showTreasure = shouldShowTreasure();
            knight.setPos(sf::Vector2f(window.getSize().x, knight.getSprite().getPosition().y));
        }
        if (knight.getSprite().getPosition().y > window.getSize().y) { // Vertical down
            currentSector.x++;
            showTreasure = shouldShowTreasure();
            knight.setPos(sf::Vector2f(knight.getSprite().getPosition().x, 0));
        }
        if (knight.getSprite().getPosition().y < 0) { // Vertical up
            currentSector.x--;
            showTreasure = shouldShowTreasure();
            knight.setPos(sf::Vector2f(knight.getSprite().getPosition().x, window.getSize().y));
        }

        // Attack treasure to pick up
        if (knight.getSprite().getGlobalBounds().intersects(treasure.getGlobalBounds())) {
            if (isAttacking)
                wealth += 1;

        }

        // Show window with assests
        
        if (isPlaying) {
            window.draw(tilemap);
            window.draw(knight.getSprite());
            window.draw(playerwealth);
            if (showTreasure)
                window.draw(treasure);
        }
        else {
            menu.Draw(window);
        }
        
        window.display();
    }
    return 0;
}