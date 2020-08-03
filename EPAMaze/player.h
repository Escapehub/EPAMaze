#ifndef PLAYER_H
#define PLAYER_H
#include "animation.h"

class Player : public Animation 
{
public:
	Player(sf::Texture&);
	void setSpeed(float);
	enum Direction {Up, Down, Left, Right};
	void Move(Direction);
	void setPos(sf::Vector2f);
private:
	float speed;
};

#endif