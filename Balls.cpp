#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <time.h>
using namespace sf;

static const int width = 1800;
static const int height = 1000;
static const float a = float(0.15);
static const int n = 20;
static const int FullPoints = 50;

struct Balls {
	CircleShape shape;
	int type = 0;

	Vector2f getPosition() {
		return shape.getPosition() + Vector2f(shape.getRadius(), shape.getRadius());
	}

	void setPosition(Vector2f pos) {
		shape.setPosition(pos - Vector2f(shape.getRadius(), shape.getRadius()));
	}

	void init(int t, int k, Balls* balls, CircleShape& Player) {
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
			float dx, dy;

			if (i == 0) { // Setting to Check with Player Position
				dx = shape.getPosition().x - Player.getPosition().x;
				dy = shape.getPosition().y - Player.getPosition().y;
				r = shape.getRadius() + Player.getRadius();
			}
			else { // Setting to Check with Other Ball Position
				dx = getPosition().x - balls[i - 1].getPosition().x;
				dy = getPosition().y - balls[i - 1].getPosition().y;
				r = balls[i - 1].shape.getRadius() + shape.getRadius();
			}

			if (r * r >= dx * dx + dy * dy) {
				shape.setPosition(float(rand() % ((int)(width - 2 * r))), float(rand() % ((int)(height - 2 * r))));
				i = 0;
			}
		}
	}
};

struct PlayerBall {
	CircleShape shape;
	Vector2f v = { 0,0 };
	int points = 0;

	void borderBounce(float t) {
		const Vector2f pos = getPosition();
		const float r = shape.getRadius();

		if (pos.x + v.x * t >= width - r || pos.x + v.x * t <= r) {
			v.x = (a - 1) * v.x;
			points--;
		}
		if (pos.y + v.y * t >= height - r || pos.y + v.y * t <= r) {
			v.y = (a - 1) * v.y;
			points--;
		}
	}

	Vector2f getPosition() {
		return shape.getPosition() + Vector2f(shape.getRadius(), shape.getRadius());
	}

	void setPosition(Vector2f pos) {
		shape.setPosition(pos - Vector2f(shape.getRadius(), shape.getRadius()));
	}

	void init(float r, Color color, Vector2f pos) {
		shape.setRadius(r);
		shape.setFillColor(color);
		setPosition(pos);
	}

	bool isMoving() {
		return v.x != 0 && v.y != 0;
	}

	void ballBounce(Balls& Ball, float t) {
		const float dx = getPosition().x - Ball.getPosition().x + v.x * t, dy = getPosition().y - Ball.getPosition().y + v.y * t;
		const float r = Ball.shape.getRadius() + shape.getRadius();

		if (r * r >= dx * dx + dy * dy) {
			float fr = 1;
			switch (Ball.type) {
			case 0: // Red
				points -= 5;
				fr = 1.5;
				break;
			case 1: // Yellow
				points += 5;
				fr = 2;
				break;
			case 2: // Blue
				points++;
				fr = 0;
				break;
			case 3: // White
				fr = 1;
				break;
			}
			const Vector2f vp = ((v.x * dx + v.y * dy) / (dx * dx + dy * dy)) * Vector2f(dx, dy); // Vector projection on the axis of the radius
			v -= 2.f * vp; // v after bounce
		}
	}

	void move(float t) {
		borderBounce(t);

		v = Vector2f(v.x * (1 - 8 * a * t), v.y * (1 - 8 * a * t)); // Friction
		if (v.x <= 16 && v.x >= -16 && v.y <= 16 && v.y >= -16) v = Vector2f(0, 0);

		shape.setPosition(shape.getPosition() + Vector2f(v.x * t, v.y * t)); // Setting Player Position
	}

};

bool MainMenu(RenderWindow& window, Font& font) {
	char s[50];

	Event event;
	while (window.pollEvent(event)) { // Events
		if (event.type == Event::KeyPressed) {
			if (event.key.code == Keyboard::Escape)
				window.close();
			if (event.key.code == Keyboard::Enter)
				return true;
		}
	}

	Text mainText;
	mainText.setFont(font);
	mainText.setCharacterSize(48);
	mainText.setFillColor(Color::White);
	mainText.setPosition({ 850,0 });
	mainText.setString("Balls");

	Text text1;
	text1.setFont(font);
	text1.setCharacterSize(24);
	text1.setFillColor(Color::White);
	text1.setPosition({ 775,500 });
	text1.setString("Press Enter to Continue");

	Text text2;
	text2.setFont(font);
	text2.setCharacterSize(24);
	text2.setFillColor(Color::White);
	text2.setPosition({ 790,450 });
	sprintf_s(s, "Maximum Points: %d", FullPoints);
	text2.setString(s);

	Text text3;
	text3.setFont(font);
	text3.setCharacterSize(24);
	text3.setFillColor(Color::Yellow);
	text3.setPosition({ 100,200 });
	text3.setString("5 Points");

	Text text4;
	text4.setFont(font);
	text4.setCharacterSize(24);
	text4.setFillColor(Color::Red);
	text4.setPosition({ 100,250 });
	text4.setString("-5 Points");

	Text text5;
	text5.setFont(font);
	text5.setCharacterSize(24);
	text5.setFillColor(Color::Blue);
	text5.setPosition({ 100,300 });
	text5.setString("1 Point");

	Text text6;
	text6.setFont(font);
	text6.setCharacterSize(24);
	text6.setFillColor(Color::White);
	text6.setPosition({ 100,350 });
	text6.setString("0 Points");

	Text text7;
	text7.setFont(font);
	text7.setCharacterSize(24);
	text7.setFillColor(Color::White);
	text7.setPosition({ 100,400 });
	text7.setString("Border: -1 Point");

	window.clear(Color::Black);
	window.draw(text1);
	window.draw(text2);
	window.draw(text3);
	window.draw(text4);
	window.draw(text5);
	window.draw(text6);
	window.draw(text7);
	window.draw(mainText);
	window.display();

	return false;
}

int main() {
	// Variables
	int k = 3;
	int pointsGain = 0;
	char s[50];
	bool lMouse = false;

	// Objects
	Clock clock;
	PlayerBall Player;
	Balls balls[n];

	// Initialization
	srand(unsigned int(time(NULL)));
	RenderWindow window(VideoMode(width, height), "Kule");
	window.setFramerateLimit(60);

	Player.init(50.f, Color::White, Vector2f(width / 2, height / 2)); // Player Initialization

	for (int i = 0; i < k; i++) // Balls First Initialization
		balls[i].init(rand() % 4, i, balls, Player.shape);

	Font font;
	if (!font.loadFromFile("Font.ttf")) // Loading Font
		return 0;
	Text text;
	text.setFont(font);
	text.setCharacterSize(24);
	text.setFillColor(Color::White);

	while (window.isOpen()) { // Main Menu Loop
		if (MainMenu(window, font)) break;
	}

	while (window.isOpen()) { // Main Loop
		float t = clock.restart().asSeconds();
		Event event;
		while (window.pollEvent(event)) { // Events
			if (event.type == Event::MouseButtonPressed && Player.points < FullPoints)
				if (event.mouseButton.button == Mouse::Left)
					lMouse = true;

			if (event.type == Event::MouseButtonReleased && Player.points < FullPoints)
				if (event.mouseButton.button == Mouse::Left && !Player.isMoving()) {
					lMouse = false;
					Vector2f vt = Player.getPosition() - ((Vector2f)Mouse::getPosition(window));
					Player.v = 8.f * Vector2f(vt.x, vt.y); // Setting Player Speed Using Drawed Line

					pointsGain = Player.points; // Setting Starting Amount of Points
				}

			if (event.type == Event::KeyPressed)
				if (event.key.code == Keyboard::Escape)
					window.close();

			if (event.type == Event::Closed || Player.points < 0)
				window.close();
		}

		if (Player.points < FullPoints) {
			if (Player.isMoving()) { // When Player is in Motion
				for (int i = 0; i < k; i++) //Balls Collision
					Player.ballBounce(balls[i], t);

				Player.move(t); // Motion, Friction and Border Collision

				if (!Player.isMoving()) { // When Player Stops Moving
					pointsGain = Player.points - pointsGain; // Setting Points gain

					if (k < n && k >= 3) k += pointsGain / 5; // Setting Amount of Balls to Add
					else if (k > n) k = n;
					else k = 3;

					for (int i = 0; i < k; i++) // Balls Initialization
						balls[i].init(rand() % 4, i, balls, Player.shape);
				}
			}

			sprintf_s(s, "Points: %d", Player.points);
			text.setString(s);
		}
		else if (Player.points >= FullPoints) { // Winning Condition
			text.setString("You Won! - Congratulations!!");
			text.setPosition({ 600,450 });
			text.setFillColor(Color::Green);
			text.setCharacterSize(48);
		}

		// Drawing
		window.clear(Color::Black);
		for (int i = 0; i < k; i++)
			window.draw(balls[i].shape);
		if (lMouse) { // Line Between Player and Mouse
			Vertex line[] = { Vertex((Vector2f)Mouse::getPosition(window)),Vertex(Player.getPosition()) };
			window.draw(line, 2, Lines);
		}
		window.draw(Player.shape);
		window.draw(text);
		window.display();
	}

	return 0;
}