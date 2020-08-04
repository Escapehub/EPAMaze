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
    SectorMetaData map[10][10];
    while (std::getline(configFile, currentLine)) {
        std::stringstream stringstream(currentLine);
        while (stringstream.good()) {
            std::string substr;
            std::getline(stringstream, substr, ',');
            if (substr == "horizontal")
                map[MapgenCurrent.x][MapgenCurrent.y] = Sector::Horizontal.getData();
            else if (substr == "cross")
                map[MapgenCurrent.x][MapgenCurrent.y] = Sector::Cross.getData();
            else if (substr == "vertical")
                map[MapgenCurrent.x][MapgenCurrent.y] = Sector::Vertical.getData();
            else if (substr == "bottomright")
                map[MapgenCurrent.x][MapgenCurrent.y] = Sector::BottomRight.getData();
            else if (substr == "bottomleft")
                map[MapgenCurrent.x][MapgenCurrent.y] = Sector::BottomLeft.getData();
            else if (substr == "topright")
                map[MapgenCurrent.x][MapgenCurrent.y] = Sector::TopRight.getData();
            else if (substr == "topleft")
                map[MapgenCurrent.x][MapgenCurrent.y] = Sector::TopLeft.getData();
            else if (substr == "verticalright")
                map[MapgenCurrent.x][MapgenCurrent.y] = Sector::VerticalRight.getData();
            else if (substr == "verticalleft")
                map[MapgenCurrent.x][MapgenCurrent.y] = Sector::VerticalLeft.getData();
            else if (substr == "horizontaltop")
                map[MapgenCurrent.x][MapgenCurrent.y] = Sector::HorizontalTop.getData();
            else if (substr == "horizontalbottom")
                map[MapgenCurrent.x][MapgenCurrent.y] = Sector::HorizontalBottom.getData();
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
    sf::Texture knightTexture;
    knightTexture.loadFromFile("knight.png");
    Player knight(knightTexture, sf::IntRect(0, 768, 128, 128));
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
    
    // Goblin 704 x 320 // 11 x 5
    sf::Texture goblinTexture;
    goblinTexture.loadFromFile("goblin.png");
    Player goblin(goblinTexture, sf::IntRect(0, 0, 64, 64));
    goblin.setFrameSize(64);
    goblin.setSpeed(.2f);
    goblin.setAnimationSpeed(0.1f);
    goblin.setMoveEndFrame(448);
    goblin.setPos(sf::Vector2f(window.getSize().x / 4, window.getSize().y / 2));
    goblin.addMoveAnimation(sf::IntRect(0, 128, 64, 64)); // up
    goblin.addMoveAnimation(sf::IntRect(0, 0, 64, 64)); // down
    goblin.addMoveAnimation(sf::IntRect(0, 192, 64, 64)); // left
    goblin.addMoveAnimation(sf::IntRect(0, 64, 64, 64)); // right
    
    // Menu
    Menu menu(window.getSize().x, window.getSize().y);

    // TileMap
    TileMap tilemap;
    // Setting random start and end point
    sf::Vector2i currentSector(rand() % 9 ,rand() % 9);
    sf::Vector2i randomEnd(rand() % 9, rand() % 9);
    map[randomEnd.x][randomEnd.y].MetaData["EndSector"] = true;

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
    treasure.setPosition(sf::Vector2f(window.getSize().x / 3, window.getSize().y / 4));

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
            
            if (event.type == sf::Event::KeyPressed) {
                // Item pickup and goblin attack
                if (event.key.code == sf::Keyboard::Space) {
                    knight.Attack();
                    isAttacking = true;
                    if (knight.getSprite().getGlobalBounds().intersects(treasure.getGlobalBounds()) && isAttacking) {
                        wealth += 5;
                        map[currentSector.x][currentSector.y].MetaData["Treasure"] = false;
                    }
                    if (knight.getSprite().getGlobalBounds().intersects(goblin.getSprite().getGlobalBounds()) && isAttacking) {
                        wealth += 10;
                        map[currentSector.x][currentSector.y].MetaData["Goblin"] = false;
                    }
                }
                // Drop treasure
                if (event.key.code == sf::Keyboard::E) {
                    map[currentSector.x][currentSector.y].MetaData["HasDroppedTreasure"] = true;
                    map[currentSector.x][currentSector.y].droppedTreasure.setPosition(sf::Vector2f(knight.getSprite().getPosition().x, knight.getSprite().getPosition().y));
                    wealth -= 5;
                }
            }

            // Main menu control
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
        if (!tilemap.load("tileset.png", sf::Vector2u(32, 32), map[currentSector.x][currentSector.y].Sector, 34, 16))
            return -1;

        // Show pause menu
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            isPlaying = false;

        // Move Player
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            if (!map[currentSector.x][currentSector.y].MetaData["Up"]) {
                if (knight.getSprite().getPosition().y > 20)
                    knight.Move(Player::Direction::Up);
            }
            else {
                knight.Move(Player::Direction::Up);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            if (!map[currentSector.x][currentSector.y].MetaData["Right"]) {
                if (knight.getSprite().getPosition().x < window.getSize().x - 128)
                    knight.Move(Player::Direction::Right);
            }
            else {
                knight.Move(Player::Direction::Right);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            if (!map[currentSector.x][currentSector.y].MetaData["Down"]) {
                if (knight.getSprite().getPosition().y < window.getSize().y - 128)
                    knight.Move(Player::Direction::Down);
            }
            else {
                knight.Move(Player::Direction::Down);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            if (!map[currentSector.x][currentSector.y].MetaData["Left"]) {
                if (knight.getSprite().getPosition().x > 20)
                    knight.Move(Player::Direction::Left);
            }
            else {
                knight.Move(Player::Direction::Left);
            }
        }
        // Loop player around screen
        if (knight.getSprite().getPosition().x > window.getSize().x) { // Horizontal Right
            if (!map[currentSector.x][currentSector.y].MetaData["Goblin"]) {
                currentSector.y++;
                knight.setPos(sf::Vector2f(0, knight.getSprite().getPosition().y));
            }
            else {
                std::printf("You must clear the room before moving on! \n");
            }
        }
        if (knight.getSprite().getPosition().x < 0) { // Horizontal left
            if (!map[currentSector.x][currentSector.y].MetaData["Goblin"]) {
                currentSector.y--;
                knight.setPos(sf::Vector2f(window.getSize().x, knight.getSprite().getPosition().y));
            }
            else {
                std::printf("You must clear the room before moving on! \n");
            }
        }
        if (knight.getSprite().getPosition().y > window.getSize().y) { // Vertical down
            if (!map[currentSector.x][currentSector.y].MetaData["Goblin"]) {
                currentSector.x++;
                knight.setPos(sf::Vector2f(knight.getSprite().getPosition().x, 0));
            }
            else {
                std::printf("You must clear the room before moving on! \n");
            }
        }
        if (knight.getSprite().getPosition().y < 0) { // Vertical up
            if (!map[currentSector.x][currentSector.y].MetaData["Goblin"]) {
                currentSector.x--;
                knight.setPos(sf::Vector2f(knight.getSprite().getPosition().x, window.getSize().y));
            }
            else {
                std::printf("You must clear the room before moving on! \n");
            }
        }

        // Goblin temp movement
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6))
            goblin.Move(Player::Direction::Right);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4))
            goblin.Move(Player::Direction::Left);
        
        // Game over
        if (map[currentSector.x][currentSector.y].MetaData["EndSector"]) {
            std::cout << "You made it to the end \n Your score was: " << std::to_string(wealth) << "\nLoad a new config file to play again!" << std::endl;
            isPlaying = false;
            main();
        }
        // Show window with assests
        if (isPlaying) {
            window.draw(tilemap);
            window.draw(knight.getSprite());
            window.draw(playerwealth);
            if (map[currentSector.x][currentSector.y].MetaData["Treasure"])
                window.draw(treasure);
            if (map[currentSector.x][currentSector.y].MetaData["Goblin"])
                window.draw(goblin.getSprite());
            if (map[currentSector.x][currentSector.y].MetaData["HasDroppedTreasure"])
                window.draw(map[currentSector.x][currentSector.y].droppedTreasure);
        }
        else {
            menu.Draw(window);
        }
        
        window.display();
    }
    return 0;
}