#include "Balls.h"
#include "Ball.h"
#include "Player.h"
using namespace sf;

bool MainMenu(RenderWindow& window, Font& font) {
	char s[50];

	Event event;
	while (window.pollEvent(event)) { // Events
		if(event.type == Event::Closed || (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape))
			window.close();
		if (event.type == Event::KeyPressed) {
			if (event.key.code == Keyboard::Escape)
				window.close();
			if (event.key.code == Keyboard::Enter)
				return true;
		}
	}

	Text mainText = Text("Balls", font, 48);
	mainText.setPosition(Vector2f(850, 0));

	Text text1 = Text("Press Enter to Continue", font, 24);
	text1.setPosition(Vector2f(775, 500));

	sprintf_s(s, "Maximum Points: %d", FullPoints);
	Text text2 = Text(s, font, 24);
	text2.setPosition(Vector2f(790, 450));

	Text text3 = Text("5 Points", font, 24);
	text3.setFillColor(Color::Yellow);
	text3.setPosition(Vector2f(100, 200));

	Text text4 = Text("-5 Points", font, 24);
	text4.setFillColor(Color::Red);
	text4.setPosition(Vector2f(100, 250));

	Text text5 = Text("1 Point", font, 24);
	text5.setFillColor(Color::Blue);
	text5.setPosition(Vector2f(100, 300));

	Text text6 = Text("0 Points", font, 24);
	text6.setPosition(Vector2f(100, 350));

	Text text7 = Text("Border: -1 Point", font, 24);
	text7.setPosition(Vector2f(100, 400));

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
	Player player;
	Ball balls[n];

	// Initialization
	srand(unsigned int(time(NULL)));
	RenderWindow window(VideoMode(width, height), "Balls");
	window.setFramerateLimit(60);

	player.init(50.f, Color::White, Vector2f(width / 2, height / 2)); // Player Initialization

	for (int i = 0; i < k; i++) // Balls First Initialization
		balls[i].init(rand() % 4, i, balls, player.shape);

	Font font;
	if (!font.loadFromFile("Font.ttf")) // Loading Font
		return 0;
	Text text = Text("", font, 24);

	while (window.isOpen()) { // Main Menu Loop
		if (MainMenu(window, font)) break;
	}

	while (window.isOpen()) { // Main Loop
		float t = clock.restart().asSeconds();
		Event event;
		while (window.pollEvent(event)) { // Events
			if (event.type == Event::MouseButtonPressed && player.points < FullPoints && player.points >= 0)
				lMouse = event.mouseButton.button == Mouse::Left;

			if (event.type == Event::MouseButtonReleased && player.points < FullPoints && player.points >= 0)
				if (event.mouseButton.button == Mouse::Left ) {
					lMouse = false;
					if (!player.isMoving()) {
						player.v = 8.f * (player.getPosition() - ((Vector2f)Mouse::getPosition(window))); // Setting Player Speed Using Drawed Line

						pointsGain = player.points; // Setting Starting Amount of Points
					}
				}

			if (event.type == Event::Closed || (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape))
				window.close();
		}
		if (player.points < FullPoints && player.points >= 0) {
			if (player.isMoving()) { // When Player is in Motion
				for (int i = 0; i < k; i++) //Balls Collision
					player.ballBounce(balls[i], t);

					player.move(t); // Motion, Friction and Border Collision

					if (!player.isMoving()) { // When Player Stops Moving
						pointsGain = player.points - pointsGain; // Setting pointsGain as dummy variable

						if (k < n && k >= 3) k += pointsGain / 5; // Setting Amount of Balls to Add
						else if (k > n) k = n;
						else k = 3;

						for (int i = 0; i < k; i++) // Balls Initialization
							balls[i].init(rand() % 4, i, balls, player.shape);
					}
				}

			sprintf_s(s, "Points: %d", player.points);
			text.setString(s);
		}
		else if (player.points >= FullPoints) { // Winning Condition
			text = Text("You Won! - Congratulations!!", font, 48);
			text.setPosition({ 600,450 });
			text.setFillColor(Color::Green);
		}
		else if (player.points < 0) { // Losing Condition
			text = Text("You Lose! - :(", font, 48);
			text.setPosition({ 750,450 });
			text.setFillColor(Color::Red);
		}

		// Drawing
		window.clear(Color::Black);
		for (int i = 0; i < k; i++)
			window.draw(balls[i].shape);
		if (lMouse && !player.isMoving()) { // Line Between Player and Mouse
			Vertex line[] = { Vertex((Vector2f)Mouse::getPosition(window)),Vertex(player.getPosition()) };
			window.draw(line, 2, Lines);
		}
		window.draw(player.shape);
		window.draw(text);
		window.display();
	}

	return 0;
}