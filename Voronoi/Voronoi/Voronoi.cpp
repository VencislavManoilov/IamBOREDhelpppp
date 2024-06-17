#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int randomInteger(int min, int max) {
	return (rand() % (max - min + 1)) + min;
}

static float distance(float X1, float Y1, float X2, float Y2) {
	return std::sqrt((X2 - X1) * (X2 - X1) + (Y2 - Y1) * (Y2 - Y1));
}

float manhattanDistance(int x1, int y1, int x2, int y2) {
	return abs(x1 - x2) + abs(y1 - y2);
}

class Point {
public:
	sf::Vector2i position;
	sf::Color color;

	Point() : position(0, 0), color(0, 0, 0) {}

	Point(sf::Vector2i Position, sf::Color Color) :
	position(Position), color(Color) {}
};

class Pixel {
public:
	sf::RectangleShape shape;
	float closestDistance;
	sf::Color color;

	Pixel() : shape(), closestDistance(99999), color(0, 0, 0) {}
};

const int PointsNum = 50;

Point points[PointsNum];
Pixel pixels[800][600];

void Calculate(bool NormalDistance) {
	for (int x = 0; x < 800; x++) {
		for (int y = 0; y < 600; y++) {
			for (int i = 0; i < PointsNum; i++) {
				float distancePixelPoint;

				if (NormalDistance)
					distancePixelPoint = distance(x, y, points[i].position.x - 2, points[i].position.y - 2);
				else
					distancePixelPoint = manhattanDistance(x, y, points[i].position.x - 2, points[i].position.y - 2);

				if (distancePixelPoint < pixels[x][y].closestDistance) {
					pixels[x][y].closestDistance = distancePixelPoint;
					pixels[x][y].color = points[i].color;
				}
			}
		}
	}
}

void Draw(sf::RenderWindow& window) {
	window.clear();

	for (int x = 0; x < 800; x++) {
		for (int y = 0; y < 600; y++) {
			pixels[x][y].shape.setPosition(x, y);
			pixels[x][y].shape.setSize(sf::Vector2f(1, 1));
			pixels[x][y].shape.setFillColor(pixels[x][y].color);

			window.draw(pixels[x][y].shape);
		}
	}

	window.display();
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Voronoi");
	sf::Event e;

	srand(time(NULL));

	for (int i = 0; i < PointsNum; i++) {
		points[i] = Point(sf::Vector2i(randomInteger(0, 799), randomInteger(0, 599)), sf::Color(randomInteger(0, 255), randomInteger(0, 255), randomInteger(0, 255)));
	}

	Calculate(true);

	Draw(window);

	while (window.isOpen()) {
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window.close();
			}
		}

		
	}
}
