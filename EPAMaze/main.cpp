#include <fstream>
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
    knight.setSpeed(.1f); // 0.1f
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
    sf::Clock goblinMoveClock;

    // Menu
    Menu menu(window.getSize().x, window.getSize().y);

    // TileMap
    TileMap tilemap;

    // Setting random start and end point
    sf::Vector2i currentSector(rand() % 9 , rand() % 9);
    sf::Vector2i randomEnd(rand() % 9, rand() % 9);
    map[randomEnd.x][randomEnd.y].MetaData["EndSector"] = true;

    // Player wealth
    int wealth = 0;
    sf::Font arcadeFont;
    arcadeFont.loadFromFile("arcade.ttf");
    sf::Text playerwealth;
    playerwealth.setFont(arcadeFont);
    playerwealth.setFillColor(sf::Color::Black);
    playerwealth.setPosition(sf::Vector2f(window.getSize().x - 150, 20));

    // Clear room text
    sf::Text clearRoom;
    clearRoom.setFont(arcadeFont);
    clearRoom.setFillColor(sf::Color::Black);
    clearRoom.setString("You must clear the room!");
    clearRoom.setPosition(sf::Vector2f(window.getSize().y / 2, 0));
    
    // Treasure
    sf::Texture treasureTexture;
    treasureTexture.loadFromFile("treasure.png");
    sf::Sprite treasure(treasureTexture);
    treasure.setTextureRect(sf::IntRect(64, 0, 32, 32));
    treasure.setPosition(sf::Vector2f(window.getSize().x / 3, window.getSize().y / 4));

    // Bomb
    sf::Texture bombTexture;
    bombTexture.loadFromFile("bomb.png");
    sf::Sprite bomb(bombTexture);
    bomb.setPosition(sf::Vector2f(window.getSize().x - window.getSize().x / 4, window.getSize().y / 3));

    // Game running loop
    while (window.isOpen())
    {
        window.clear();

        // Player controlled variables
        bool isAttacking = false;
        bool showClearRoom = false;
        bool endGame = false;

        // restting bomb texture
        bomb.setTexture(bombTexture);
        bomb.setTextureRect(sf::IntRect(0, 0, 64, 64));
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
                    if (knight.getSprite().getGlobalBounds().intersects(treasure.getGlobalBounds()) && isAttacking && map[currentSector.x][currentSector.y].MetaData["Treasure"]) {
                        wealth += 5;
                        map[currentSector.x][currentSector.y].MetaData["Treasure"] = false;
                    }
                    if (knight.getSprite().getGlobalBounds().intersects(goblin.getSprite().getGlobalBounds()) && isAttacking && map[currentSector.x][currentSector.y].MetaData["Goblin"]) {
                        wealth += 10;
                        map[currentSector.x][currentSector.y].MetaData["Goblin"] = false;
                    }
                    if (knight.getSprite().getGlobalBounds().intersects(bomb.getGlobalBounds()) && isAttacking && map[currentSector.x][currentSector.y].MetaData["Bomb"]) {
                        wealth -= 20;
                        sf::Texture bombExplode;
                        bombExplode.loadFromFile("bombExplode.png");
                        sf::IntRect bombExplodeRect(0, 0, 192, 192);
                        bomb.setTextureRect(bombExplodeRect);
                        bomb.setTexture(bombExplode);
                        sf::Clock bombTime;
                        if (bombTime.getElapsedTime().asSeconds() > 1.0f) {
                            if (bombExplodeRect.left == 1152)
                                bombExplodeRect.left = 0;
                            else
                                bombExplodeRect.left += 192;

                            bomb.setTextureRect(bombExplodeRect);
                            bombTime.restart();
                        }
                        map[currentSector.x][currentSector.y].MetaData["Bomb"] = false;
                    }
                }
                // Drop treasure
                if (event.key.code == sf::Keyboard::E && !map[currentSector.x][currentSector.y].MetaData["HasDroppedTreasure"]) {
                    map[currentSector.x][currentSector.y].MetaData["HasDroppedTreasure"] = true;
                    map[currentSector.x][currentSector.y].droppedTreasure.setPosition(sf::Vector2f(knight.getSprite().getPosition().x, knight.getSprite().getPosition().y));
                    wealth -= 1;
                }
                // Defusing bomb
                if (event.key.code == sf::Keyboard::F) {
                    if (knight.getSprite().getGlobalBounds().intersects(bomb.getGlobalBounds()) && map[currentSector.x][currentSector.y].MetaData["Bomb"]) {
                        wealth += 12;
                        map[currentSector.x][currentSector.y].MetaData["Bomb"] = false;
                    }
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
                        main();
                        break;
                    case 2:
                        window.close();
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
            if (!map[currentSector.x][currentSector.y].MetaData["Goblin"] && !map[currentSector.x][currentSector.y].MetaData["Bomb"]) {
                if (currentSector.y == 9)
                    currentSector.y = 0;
                else
                    currentSector.y++;
                knight.setPos(sf::Vector2f(0, knight.getSprite().getPosition().y));
            }
            else {
                showClearRoom = true;
            }
        }
        if (knight.getSprite().getPosition().x < 0) { // Horizontal left
            if (!map[currentSector.x][currentSector.y].MetaData["Goblin"] && !map[currentSector.x][currentSector.y].MetaData["Bomb"]) {
                if (currentSector.y == 0)
                    currentSector.y = 9;
                else
                    currentSector.y--;
                knight.setPos(sf::Vector2f(window.getSize().x, knight.getSprite().getPosition().y));
            }
            else {
                showClearRoom = true;
            }
        }
        if (knight.getSprite().getPosition().y > window.getSize().y) { // Vertical down
            if (!map[currentSector.x][currentSector.y].MetaData["Goblin"] && !map[currentSector.x][currentSector.y].MetaData["Bomb"]) {
                if (currentSector.x == 9)
                    currentSector.x = 0;
                else
                    currentSector.x++;
                knight.setPos(sf::Vector2f(knight.getSprite().getPosition().x, 0));
            }
            else {
                showClearRoom = true;
            }
        }
        if (knight.getSprite().getPosition().y < 0) { // Vertical up
            if (!map[currentSector.x][currentSector.y].MetaData["Goblin"] && !map[currentSector.x][currentSector.y].MetaData["Bomb"]) {
                if (currentSector.x == 0)
                    currentSector.y = 9;
                else
                    currentSector.x--;
                knight.setPos(sf::Vector2f(knight.getSprite().getPosition().x, window.getSize().y));
            }
            else {
                showClearRoom = true;
            }
        }

        // Goblin movement
        if (goblinMoveClock.getElapsedTime().asSeconds() > 1) {
            int random = rand() % 3;
            switch (random) {
            case 0:
                goblin.Move(Player::Direction::Up);
                break;
            case 1:
                goblin.Move(Player::Direction::Right);
                break;
            case 2:
                goblin.Move(Player::Direction::Down);
            case 3:
                goblin.Move(Player::Direction::Left);
                break;
            }
            goblinMoveClock.restart();
        }

        // Game over
        if (map[currentSector.x][currentSector.y].MetaData["EndSector"]) {
            endGame = true;
            isPlaying = false;
        }

        // Show window with assests
        if (isPlaying) {
            window.draw(tilemap);
            window.draw(knight.getSprite());
            window.draw(playerwealth);
            if (showClearRoom)
                window.draw(clearRoom);
            if (map[currentSector.x][currentSector.y].MetaData["Treasure"])
                window.draw(treasure);
            if (map[currentSector.x][currentSector.y].MetaData["Goblin"])
                window.draw(goblin.getSprite());
            if (map[currentSector.x][currentSector.y].MetaData["Bomb"])
                window.draw(bomb);
            if (map[currentSector.x][currentSector.y].MetaData["HasDroppedTreasure"])
                window.draw(map[currentSector.x][currentSector.y].droppedTreasure);
        }
        else if (!isPlaying && endGame) {
            menu.DrawEndMenu(window, wealth);
        }
        else {
            menu.DrawMainMenu(window);
        }           
        window.display();
    }
    return 0;
}