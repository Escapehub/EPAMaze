#include "menu.h"

Menu::Menu(float width, float height) {
	if (!font.loadFromFile("arcade.ttf"))
		std::cout << "Failed to load font" << std::endl;

	title.setFont(font);
	title.setCharacterSize(50);
	title.setFillColor(sf::Color::White);
	title.setString("Maze Game");
	title.setPosition(sf::Vector2f(width / 3, 0));

	menu[0].setFont(font);
	menu[0].setCharacterSize(30);
	menu[0].setFillColor(sf::Color::Red);
	menu[0].setString("Play");
	menu[0].setPosition(sf::Vector2f(width / 3, height / (MAX_NUMBER_OF_ITEMS + 1) * 1));

	menu[1].setFont(font);
	menu[1].setCharacterSize(30);
	menu[1].setFillColor(sf::Color::White);
	menu[1].setString("Exit");
	menu[1].setPosition(sf::Vector2f(width / 3, height / (MAX_NUMBER_OF_ITEMS + 1) * 2));

	selectedItemIndex = 0;
}

void Menu::Draw(sf::RenderWindow& window) {
	window.draw(title);
	for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++) {
		window.draw(menu[i]);
	}
}

void Menu::moveUp() {
	if (selectedItemIndex - 1 >= 0) {
		menu[selectedItemIndex].setFillColor(sf::Color::White);
		selectedItemIndex--;
		menu[selectedItemIndex].setFillColor(sf::Color::Red);
	}
}

void Menu::moveDown() {
	if (selectedItemIndex + 1 < MAX_NUMBER_OF_ITEMS) {
		menu[selectedItemIndex].setFillColor(sf::Color::White);
		selectedItemIndex++;
		menu[selectedItemIndex].setFillColor(sf::Color::Red);
	}
}