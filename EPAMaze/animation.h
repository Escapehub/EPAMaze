#ifndef ANIMATION_H
#define ANIMATION_H
#include <SFML/Graphics.hpp>

class Animation
{
public:
	sf::Sprite getSprite();
	void addAnimation(sf::IntRect);
	void setAnimationSpeed(float);
	void setEndFrame(int);
	void setFrameSize(int);
private:
	sf::Clock clock;
	int endFrame;
	float animationSpeed;
	int frameSize;
protected:
	void update(int);
	std::vector<sf::IntRect> animations;
	sf::Sprite sprite;
};

#endif