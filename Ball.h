#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include "Balls.h"
using namespace sf;

struct Ball {
	//Variables
	CircleShape shape;
	int type = 0;

	//Functions
	Vector2f getPosition();
	void setPosition(Vector2f pos);
	void init(int t, int k, Ball* balls, CircleShape& Player);
};