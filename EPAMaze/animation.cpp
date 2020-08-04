#include "animation.h"

sf::Sprite Animation::getSprite() {
	return sprite;
}

void Animation::addMoveAnimation(sf::IntRect newani) {
	moveAnimations.push_back(newani);
}

void Animation::addAttackAnimation(sf::IntRect newani) {
	attackAnimations.push_back(newani);
}

void Animation::setAnimationSpeed(float speed) {
	animationSpeed = speed;
}

void Animation::setMoveEndFrame(int endframe) {
	moveEndFrame = endframe;
}

void Animation::setAttackEndFrame(int endframe) {
	attackEndFrame = endframe;
}

void Animation::setAttackStartFrame(int frame) {
	attackStartFrame = frame;
}

void Animation::setFrameSize(int size) {
	frameSize = size;
}

void Animation::updateMove(int ani) {
	if (clock.getElapsedTime().asSeconds() > animationSpeed) {
		if (moveAnimations[ani].left == moveEndFrame)
			moveAnimations[ani].left = 0;
		else
			moveAnimations[ani].left += frameSize;

		sprite.setTextureRect(moveAnimations[ani]);
		clock.restart();
	}
}

void Animation::updateAttack(int ani) {
	while (attackAnimations[ani].left != attackEndFrame) {
		if (clock.getElapsedTime().asSeconds() > animationSpeed) {
			if (attackAnimations[ani].left == attackEndFrame)
				attackAnimations[ani].left = attackStartFrame;
			else
				attackAnimations[ani].left += frameSize;

			sprite.setTextureRect(attackAnimations[ani]);
			clock.restart();
		}
	}
	attackAnimations[ani].left = attackStartFrame;
}