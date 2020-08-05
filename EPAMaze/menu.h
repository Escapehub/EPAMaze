#ifndef MENU_H
#define MENU_H
#define MAX_NUMBER_OF_ITEMS 2
#include <SFML/graphics.hpp>
#include <iostream>

class Menu
{
private:
	int selectedItemIndex;
	sf::Font font;
	sf::Text menu[MAX_NUMBER_OF_ITEMS];
	sf::Text endMenu[MAX_NUMBER_OF_ITEMS];
	sf::Text title;
	sf::Text score;
public:
	Menu(float, float);
	void DrawMainMenu(sf::RenderWindow&);
	void DrawEndMenu(sf::RenderWindow&, int);
	void moveUp();
	void moveDown();
	int GetPressedItem() { return selectedItemIndex; }
};

#endif 
