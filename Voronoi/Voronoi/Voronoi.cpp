#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <iostream>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

static int randomInteger(int min, int max) {
	return (rand() % (max - min + 1)) + min;
}

static float distance(float X1, float Y1, float X2, float Y2) {
	return std::sqrt((X2 - X1) * (X2 - X1) + (Y2 - Y1) * (Y2 - Y1));
}

float manhattanDistance(int x1, int y1, int x2, int y2) {
	return abs(x1 - x2) + abs(y1 - y2);
}

// Creating the canvas to draw voronoi
sf::Image canvas;
sf::Sprite canvasSprite;
sf::Texture canvasTexture;

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
	float closestDistance;
	sf::Color color;

	Pixel() : closestDistance(99999), color(0, 0, 0) {}
};

const int PointsNum = 50;

Point points[PointsNum];
Pixel pixels[800][600];

enum SpawnPointsTypes {
	Random,
	Grid,
	Circular,
	Clustered,
	Border,
	Spiral,
	Suprise
};

SpawnPointsTypes types;

void SpawnPoints(SpawnPointsTypes type) {
	// This is the suprise
	if (type == 6) {
		type = SpawnPointsTypes(randomInteger(0, 5));
	}

	switch (type) {
	case 0: {
		for (int i = 0; i < PointsNum; i++) {
			points[i] = Point(sf::Vector2i(randomInteger(0, 799), randomInteger(0, 599)), sf::Color(randomInteger(0, 255), randomInteger(0, 255), randomInteger(0, 255)));
		}
	}
	break;
	case 1: {
		int rows = sqrt(PointsNum);
		int cols = (PointsNum + rows - 1) / rows; // ensures all points fit in the grid
		int xStep = 800 / cols;
		int yStep = 600 / rows;

		for (int i = 0; i < PointsNum; i++) {
			int row = i / cols;
			int col = i % cols;
			points[i] = Point(sf::Vector2i(col * xStep + xStep / 2, row * yStep + yStep / 2), sf::Color(randomInteger(0, 255), randomInteger(0, 255), randomInteger(0, 255)));
		}
	}
	break;
	case 2: {
		sf::Vector2f center(400, 300);
		float radius = 250.0f;

		for (int i = 0; i < PointsNum; i++) {
			float angle = 2 * 3.14159265f * i / PointsNum;
			sf::Vector2f pointPos = center + sf::Vector2f(radius * cos(angle), radius * sin(angle));
			points[i] = Point(sf::Vector2i(static_cast<int>(pointPos.x), static_cast<int>(pointPos.y)), sf::Color(randomInteger(0, 255), randomInteger(0, 255), randomInteger(0, 255)));
		}
	}
	break;
	case 3: {
		int clusters = 5; // Number of clusters
		std::vector<sf::Vector2i> clusterCenters;

		// Create random cluster centers
		for (int i = 0; i < clusters; i++) {
			clusterCenters.push_back(sf::Vector2i(randomInteger(0, 799), randomInteger(0, 599)));
		}

		for (int i = 0; i < PointsNum; i++) {
			sf::Vector2i center = clusterCenters[randomInteger(0, clusters - 1)];
			int xOffset = randomInteger(-50, 50);
			int yOffset = randomInteger(-50, 50);
			points[i] = Point(sf::Vector2i(center.x + xOffset, center.y + yOffset), sf::Color(randomInteger(0, 255), randomInteger(0, 255), randomInteger(0, 255)));
		}
	}
	break;
	case 4: {
		for (int i = 0; i < PointsNum; i++) {
			int side = randomInteger(0, 3);
			int x, y;
			switch (side) {
			case 0: // Top
				x = randomInteger(0, 799);
				y = 0;
				break;
			case 1: // Right
				x = 799;
				y = randomInteger(0, 599);
				break;
			case 2: // Bottom
				x = randomInteger(0, 799);
				y = 599;
				break;
			case 3: // Left
				x = 0;
				y = randomInteger(0, 599);
				break;
			}
			points[i] = Point(sf::Vector2i(x, y), sf::Color(randomInteger(0, 255), randomInteger(0, 255), randomInteger(0, 255)));
		}
	}
	break;
	case 5: {
		sf::Vector2f center(400, 300);
		float radius = 10.0f;
		float angleIncrement = 2 * 3.14159265f / PointsNum;

		for (int i = 0; i < PointsNum; i++) {
			float angle = i * angleIncrement;
			radius += 5.0f; // Increase radius to create spiral effect
			sf::Vector2f pointPos = center + sf::Vector2f(radius * cos(angle), radius * sin(angle));
			points[i] = Point(sf::Vector2i(static_cast<int>(pointPos.x), static_cast<int>(pointPos.y)), sf::Color(randomInteger(0, 255), randomInteger(0, 255), randomInteger(0, 255)));
		}
	}
	break;
	default:
	break;
	}
}

void Calculate(bool NormalDistance) {
	for (int x = 0; x < 800; x++) {
		for (int y = 0; y < 600; y++) {
			pixels[x][y].closestDistance = 99999;
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
			//pixels[x][y].shape.setPosition(x, y);
			//pixels[x][y].shape.setSize(sf::Vector2f(1, 1));
			//pixels[x][y].shape.setFillColor(pixels[x][y].color);

			canvas.setPixel(x, y, pixels[x][y].color);

		}
	}

	if (!canvasTexture.loadFromImage(canvas)) {
		return;
	}
	canvasSprite.setTexture(canvasTexture);
	window.draw(canvasSprite);

	window.display();
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Voronoi");
	sf::Event e;

	canvas.create(800, 600, sf::Color::White);
	if (!canvasTexture.loadFromImage(canvas)) {
		return -1;
	}
	canvasSprite.setTexture(canvasTexture);

	srand(time(NULL));

	SpawnPoints(SpawnPointsTypes(0));

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
