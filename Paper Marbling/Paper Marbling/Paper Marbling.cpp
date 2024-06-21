#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <cmath>
#include <vector>

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
		float CX = other.position.x;
		float CY = other.position.y;

		for (int i = 0; i < points; i++) {
			float PX = shape.getPoint(i).x;
			float PY = shape.getPoint(i).y;

			// Calculate the difference between the point and the center of the other drop
			float dx = PX - CX;
			float dy = PY - CY;

			// Calculate the distance between the point and the center of the other drop
			float distanceSquared = dx * dx + dy * dy;

			// Calculate the factor
			float factor = std::sqrt(1 + (other.R * other.R) / distanceSquared);

			// Calculate the new positions
			float NewX = CX + dx * factor;
			float NewY = CY + dy * factor;

			// Set the new positions to the shape's point
			shape.setPoint(i, sf::Vector2f(NewX, NewY));
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
bool spacePressed = false;
bool rPressed = false;
bool hPressed = false;

bool ShowControlls = true;

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Paper Marbling");
	sf::Event e;

	srand(static_cast<unsigned int>(time(0)));

	sf::Font font;
	if (!font.loadFromFile("Fonts/arial.ttf")) {
		return -1;
	}

	sf::Text controllsText("Place Drop - Mouse; Spawn 10 Drops - Space; Reset - R; Hide Controlls - H", font, 20);
	controllsText.setFillColor(sf::Color::Black);

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

		if (ShowControlls)
			window.draw(controllsText);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			if (!clicked) {
				sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
				PlaceNewDrop(mousePosition.x, mousePosition.y);
			}
			clicked = true;
		} else {
			clicked = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			if (!spacePressed) {
				for (int i = 0; i < 10; i++) {
					PlaceNewDrop(randomInteger(100, 700), randomInteger(50, 550));
				}
			}
			spacePressed = true;
		} else {
			spacePressed = false;
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
			if (!rPressed)
				drops.clear();
			rPressed = true;
		} else {
			rPressed = false;
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
			if (!hPressed)
				ShowControlls = !ShowControlls;
			hPressed = true;
		} else {
			hPressed = false;
		}

		window.display();
	}
}
