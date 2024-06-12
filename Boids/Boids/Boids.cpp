#include "SFML/Graphics.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>
#include <cstdlib> 

static int randomInteger(int min, int max) {
	return (rand() % (max - min + 1)) + min;
}

int main() {

	sf::RenderWindow window(sf::VideoMode(800, 600), "Boids");
	sf::Event e;

	int time = 0;


	class Bird {
	public:
		sf::Vector2f position;
		float size;
		float angle;

		Bird() : position(0, 0), size(0), angle(0) {}

		Bird(float X, float Y, float Size, float Angle)
			: position(X, Y), size(Size), angle(Angle) {}

		void Move() {
			position.x += std::cos(angle * 3.14/180)/10;
			position.y += std::sin(angle * 3.14/180)/10;

			position.x = (position.x < -25) ? 825 : position.x;
			position.x = (position.x > 825) ? -25 : position.x;
			position.y = (position.y < -25) ? 625 : position.y;
			position.y = (position.y > 625) ? -25 : position.y;
		}

		void draw(sf::RenderWindow& window) const {
			sf::ConvexShape bird;
			bird.setPointCount(3);
			bird.setPoint(0, sf::Vector2f(position.x + std::cos((angle + 160) * 3.14/180) * size/2, position.y + std::sin((angle + 160) * 3.14/180) * size/2));
			bird.setPoint(1, sf::Vector2f(position.x + std::cos((angle - 160) * 3.14/180) * size/2, position.y + std::sin((angle - 160) * 3.14/180) * size/2));
			bird.setPoint(2, sf::Vector2f(position.x + std::cos((angle) * 3.14/180) * size/2, position.y + std::sin((angle) * 3.14/180) * size/2));

			bird.setFillColor(sf::Color(66, 135, 245));

			window.draw(bird);
		}
	};

	Bird* birds = new Bird[100];

	for(int i = 0; i < 100; i++) {
		birds[i] = Bird(randomInteger(25, 775), randomInteger(25, 575), 50, randomInteger(0, 360));
	}

	while(window.isOpen()) {
		while(window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window.close();
			}
		}

		window.clear(sf::Color::White);

		sf::Font font;
		if (!font.loadFromFile("Fonts/arial.ttf")) {
			return -1;
		}

		sf::Vector2i mouseP = sf::Mouse::getPosition(window);

		for (int i = 0; i < 100; i++) {
			birds[i].Move();

			birds[i].draw(window);
		}


		window.display();
		time++;
	}

	return 0;
}