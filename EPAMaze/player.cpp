#include "player.h"

Player::Player(sf::Texture& texture) {
	sprite.setTexture(texture);
}

void Player::setSpeed(float _speed) {
	speed = _speed;
}

void Player::Move(Direction direction) {
	switch (direction) {
	case Up:
		sprite.move(0, -speed);
		currentDirection = Up;
		updateMove(0);
		break;
	case Down:
		sprite.move(0, speed);
		currentDirection = Down;
		updateMove(1);
		break;
	case Left:
		sprite.move(-speed, 0);
		currentDirection = Left;
		updateMove(2);
		break;
	case Right:
		sprite.move(speed, 0);
		currentDirection = Right;
		updateMove(3);
		break;
	};
}

void Player::setPos(sf::Vector2f pos) {
	sprite.setPosition(pos.x, pos.y);
}

void Player::Attack() {
	switch (currentDirection) {
	case Up:
		updateAttack(1);
		break;
	case Down:
		updateAttack(3);
		break;
	case Left:
		updateAttack(0);
		break;
	case Right:
		updateAttack(2);
		break;
	}
	std::cout << "CurrentDirect: " << currentDirection << std::endl;
}