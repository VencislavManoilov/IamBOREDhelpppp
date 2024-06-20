#include <SFML/Graphics.hpp>
#include <iostream>

class Drop {
public:
	sf::Vector2f position;
	float R;
	int points;
	sf::Color color;
	sf::ConvexShape shape;

	Drop() : position(0, 0), R(0), points(0), color(0, 0, 0) {}

	Drop(sf::Vector2f Position, float Radius, int Points, sf::Color Color) :
	position(Position), R(Radius), points(Points), color(Color) {
		shape.setPointCount(points);

		float angleFromPointToPoint = 360 / points * 3.14 / 180;
		for (int i = 0; i < points; i++) {
			shape.setPoint(i, sf::Vector2f(position.x + std::cos(angleFromPointToPoint * i) * R, position.y + std::sin(angleFromPointToPoint * i) * R));
		}

		shape.setFillColor(color);
	}
};

Drop drop = Drop(sf::Vector2f(400, 300), 50, 30, sf::Color::Black);

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Paper Marbling");
	sf::Event e;

	while (window.isOpen()) {
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window.close();
			}
		}

		window.clear(sf::Color::White);

		window.draw(drop.shape);

		window.display();
	}
}
