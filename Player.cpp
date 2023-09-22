#include "Player.h"
#include <math.h>
using namespace sf;

void Player::borderBounce(float& t) {
	const Vector2f pos = getPosition();
	const float r = shape.getRadius();
	float c_time=0;

	if (pos.x + v.x * t >= width - r) {
		c_time = (-r - pos.x + width) / v.x;
		shape.setPosition(shape.getPosition() + v * c_time);
		t -= c_time;
		v.x *= -1;
		points--;
	}
	else if (pos.x + v.x * t <= r) {
		c_time = (r - pos.x) / v.x;
		shape.setPosition(shape.getPosition() + v * c_time);
		t -= c_time;
		v.x *= -1;
		points--;
	}
	if (pos.y + v.y * t >= height - r) {
		c_time = (-r - pos.y + height) / v.y;
		shape.setPosition(shape.getPosition() + v * c_time);
		t -= c_time;
		v.y *= -1;
		points--;
	}
	else if (pos.y + v.y * t <= r) {
		c_time = (r - pos.y) / v.y;
		shape.setPosition(shape.getPosition() + v * c_time);
		t -= c_time;
		v.y *= -1;
		points--;
	}
}

Vector2f Player::getPosition() {
	return shape.getPosition() + Vector2f(shape.getRadius(), shape.getRadius());
}

void Player::setPosition(Vector2f pos) {
	shape.setPosition(pos - Vector2f(shape.getRadius(), shape.getRadius()));
}

void Player::init(float r, Color color, Vector2f pos) {
	shape.setRadius(r);
	shape.setFillColor(color);
	setPosition(pos);
}

bool Player::isMoving() {
	if (v.x <= speedError && v.x >= -speedError && v.y <= speedError && v.y >= -speedError) {
		v = Vector2f(0, 0);
		return false;
	}
	return true;
}

void Player::move(float t){
	borderBounce(t);

	v *= (1 - 8 * a * t); // Friction

	shape.setPosition(shape.getPosition() + v*t); // Setting Player Position
}

void Player::ballBounce(Ball& ball, float& t){
	float dx = getPosition().x - ball.getPosition().x + v.x * t, dy = getPosition().y - ball.getPosition().y + v.y * t;
	const float r = ball.shape.getRadius() + shape.getRadius();

	if (r * r >= dx * dx + dy * dy) {
		switch (ball.type) {
		case 0: // Red
			points -= 5;
			break;
		case 1: // Yellow
			points += 5;
			break;
		case 2: // Blue
			points++;
			break;
		case 3: // White
			break;
		}

		dx = getPosition().x - ball.getPosition().x, dy = getPosition().y - ball.getPosition().y;
		const float c_time = -(sqrtf(r * r * (v.x * v.x + v.y * v.y) - (v.x * dy - v.y * dx) * (v.x * dy - v.y * dx)) + v.x * dx + v.y * dy) / (v.x * v.x + v.y * v.y);
		shape.setPosition(shape.getPosition() + v * c_time);
		dx = getPosition().x - ball.getPosition().x, dy = getPosition().y - ball.getPosition().y;
		t -= c_time;

		const Vector2f vp = ((v.x * dx + v.y * dy) / (dx * dx + dy * dy)) * Vector2f(dx, dy); // Vector projection on the axis of the radius
		v -= 2.f * vp; // v after bounce
	}
}