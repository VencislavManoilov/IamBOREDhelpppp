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

	void Marble(Drop other) {
		for (int i = 0; i < points; i++) {
			float X = shape.getPoint(i).x;
			float Y = shape.getPoint(i).y;

			float NewX = other.position.x + (X - other.position.x) * std::sqrt(1 + other.R * other.R / std::abs((X - other.position.x) * (X - other.position.x)));
			float NewY = other.position.y + (Y - other.position.y) * std::sqrt(1 + other.R * other.R / std::abs((Y - other.position.y) * (Y - other.position.y)));

			shape.setPoint(i, sf::Vector2f(NewX, NewY));
		}
	}
};

std::vector<Drop> drops;

void PlaceNewDrop(float X, float Y) {
	drops.push_back(Drop(sf::Vector2f(X, Y), 50, 30, sf::Color::Black));

	for (int i = 0; i < drops.size() - 1; i++) {
		drops[i].Marble(drops[drops.size() - 1]);
	}
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
				std::cout << "X: " << mousePosition.x << " Y:" << mousePosition.y << std::endl;
			}
			clicked = true;
		}
		else {
			clicked = false;
		}

		window.display();
	}
}
