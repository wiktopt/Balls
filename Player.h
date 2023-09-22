#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include "Ball.h"
using namespace sf;

struct Player {
	// Variables
	CircleShape shape;
	Vector2f v = { 0,0 };
	int points = 0;

	// Functions
	void borderBounce(float t);
	Vector2f getPosition();
	void setPosition(Vector2f pos);
	void init(float r, Color color, Vector2f pos);
	bool isMoving();
	void move(float t);
	void ballBounce(Ball& ball, float& t);
};
