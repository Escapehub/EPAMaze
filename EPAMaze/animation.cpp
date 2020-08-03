#include "animation.h"

sf::Sprite Animation::getSprite() {
	return sprite;
}

void Animation::addAnimation(sf::IntRect newani) {
	animations.push_back(newani);
}

void Animation::setAnimationSpeed(float speed) {
	animationSpeed = speed;
}

void Animation::setEndFrame(int endframe) {
	endFrame = endframe;
}

void Animation::setFrameSize(int size) {
	frameSize = size;
}

void Animation::update(int ani) {
	if (clock.getElapsedTime().asSeconds() > animationSpeed) {
		if (animations[ani].left == endFrame)
			animations[ani].left = 0;
		else
			animations[ani].left += frameSize;

		sprite.setTextureRect(animations[ani]);
		clock.restart();
	}
}