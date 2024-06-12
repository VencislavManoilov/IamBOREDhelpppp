#include "SFML/Graphics.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>
#include <string>

int main() {

	sf::RenderWindow window(sf::VideoMode(800, 600), "Boids");
	sf::Event e;

	int time = 0;

	class Bird {
	public:
		sf::Vector2f position;
		float size;
		float angle;

		Bird(float X, float Y, float Size, float Angle) {
			position.x = X;
			position.y = Y;
			size = Size;
			angle = Angle;

		}

		void Move() {
			position.x += std::cos(angle * 3.14/180)/10;
			position.y += std::sin(angle * 3.14/180)/10;
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

	Bird bird(100, 100, 50, 0);

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
		 //mouseP -= window.getPosition();


		bird.angle+=0.1;
		bird.Move();
		bird.draw(window);


		window.display();
		time++;
	}

	return 0;
}