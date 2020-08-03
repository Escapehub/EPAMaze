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
		update(0);
		break;
	case Down:
		sprite.move(0, speed);
		update(1);
		break;
	case Left:
		sprite.move(-speed, 0);
		update(2);
		break;
	case Right:
		sprite.move(speed, 0);
		update(3);
		break;
	};
}

void Player::setPos(sf::Vector2f pos) {
	sprite.setPosition(pos.x, pos.y);
}