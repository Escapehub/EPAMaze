#ifndef ANIMATION_H
#define ANIMATION_H
#include <SFML/Graphics.hpp>

class Animation
{
public:
	sf::Sprite getSprite();
	void addMoveAnimation(sf::IntRect);
	void addAttackAnimation(sf::IntRect);
	void setAnimationSpeed(float);
	void setMoveEndFrame(int);
	void setAttackEndFrame(int);
	void setAttackStartFrame(int);
	void setFrameSize(int);
private:
	sf::Clock clock;
	int moveEndFrame;
	int attackEndFrame;
	int attackStartFrame;
	float animationSpeed;
	int frameSize;
protected:
	void updateMove(int);
	void updateAttack(int);
	std::vector<sf::IntRect> moveAnimations;
	std::vector<sf::IntRect> attackAnimations;
	sf::Sprite sprite;
};

#endif