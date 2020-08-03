#ifndef PLAYER_H
#define PLAYER_H
#include "animation.h"
#include <iostream>

class Player : public Animation 
{
public:
	Player(sf::Texture&);
	void setSpeed(float);
	enum Direction {Up, Down, Left, Right};
	void Move(Direction);
	void setPos(sf::Vector2f);
	void Attack();
private:
	float speed;
	Direction currentDirection;
};

#endif