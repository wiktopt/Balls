#include "Ball.h"
using namespace sf;

Vector2f Ball::getPosition(){
	return shape.getPosition() + Vector2f(shape.getRadius(), shape.getRadius());
}

void Ball::setPosition(Vector2f pos){
	shape.setPosition(pos - Vector2f(shape.getRadius(), shape.getRadius()));
}

void Ball::init(int t, int k, Ball* balls, CircleShape& Player){
	type = t;
	float r = 50.f;
	switch (t) {
	case 0: // Red
		shape.setFillColor(Color::Red);
		r = 50.f + rand() % 50;
		break;
	case 1: // Yellow
		shape.setFillColor(Color::Yellow);
		r = 25.f + rand() % 25;
		break;
	case 2: // Blue
		shape.setFillColor(Color::Blue);
		r = 50.f + rand() % 25;
		break;
	case 3: // White
		shape.setFillColor(Color::White);
		r = 50.f;
		break;
	}
	shape.setRadius(r);
	shape.setPosition(float(rand() % ((int)(width - 2 * r))), float(rand() % ((int)(height - 2 * r))));

	for (int i = 0; i <= k; i++) {
		float dx, dy, d;

		if (i == 0) { // Setting to Check with Player Position
			dx = getPosition().x - Player.getPosition().x - Player.getRadius();
			dy = getPosition().y - Player.getPosition().y - Player.getRadius();
			d = shape.getRadius() + Player.getRadius();
		}
		else { // Setting to Check with Other Ball Position
			dx = getPosition().x - balls[i - 1].getPosition().x;
			dy = getPosition().y - balls[i - 1].getPosition().y;
			d = balls[i - 1].shape.getRadius() + shape.getRadius();
		}

		if (d * d >= dx * dx + dy * dy) {
			shape.setPosition(float(rand() % ((int)(width - 2 * r))), float(rand() % ((int)(height - 2 * r))));
			i = -1;
		}
	}
}