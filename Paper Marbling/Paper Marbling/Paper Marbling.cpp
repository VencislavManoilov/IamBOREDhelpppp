#include <SFML/Graphics.hpp>
#include <iostream>

static int randomInteger(int min, int max) {
	return (rand() % (max - min + 1)) + min;
}

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

	void Marble(Drop other) {
		for (int i = 0; i < points; i++) {
			sf::Vector2f point = shape.getPoint(i);
			float dx = point.x - other.position.x;
			float dy = point.y - other.position.y;
			float distance = std::sqrt(dx * dx + dy * dy);

			float factor = 1 + (other.R * other.R) / (distance * distance + 1);

			float newX = other.position.x + dx * factor;
			float newY = other.position.y + dy * factor;

			shape.setPoint(i, sf::Vector2f(newX, newY));
		}
	}
};

std::vector<Drop> drops;

void PlaceNewDrop(float X, float Y) {
    int ColorR = randomInteger(0, 255);
    int ColorG = randomInteger(0, 255);
    int ColorB = randomInteger(0, 255);

    Drop newDrop(sf::Vector2f(X, Y), 50, 30, sf::Color(ColorR, ColorG, ColorB));

    for (Drop& drop : drops) {
        drop.Marble(newDrop);
    }

    drops.push_back(newDrop);
}

bool clicked = false;

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

		for (int i = 0; i < drops.size(); i++) {
			window.draw(drops[i].shape);
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			if (!clicked) {
				sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
				PlaceNewDrop(mousePosition.x, mousePosition.y);
			}
			clicked = true;
		}
		else {
			clicked = false;
		}

		window.display();
	}
}
