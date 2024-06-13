#include "SFML/Graphics.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>
#include <cmath>

static int randomInteger(int min, int max) {
	return (rand() % (max - min + 1)) + min;
}

static float distance(float X1, float Y1, float X2, float Y2) {
	return std::sqrt((X2 - X1) * (X2 - X1) + (Y2 - Y1) * (Y2 - Y1));
}

int main() {

	sf::RenderWindow window(sf::VideoMode(800, 600), "Boids");
	sf::Event e;

	int time = 0;


	const int BirdsNum = 100;
	const int BirdSize = 25;


	class Bird {
	public:
		sf::Vector2f position;
		float size;
		float angle;

		Bird() : position(0, 0), size(0), angle(0) {}

		Bird(float X, float Y, float Size, float Angle)
		: position(X, Y), size(Size), angle(Angle) {}

		void MoveForward(sf::RenderWindow& window) {
			position.x += std::cos(angle * 3.14 / 180) / 10;
			position.y += std::sin(angle * 3.14 / 180) / 10;

			sf::Vector2u windowSize = window.getSize();

			position.x = (position.x < -size / 2) ? (windowSize.x + size / 2) : position.x;
			position.x = (position.x > window.getSize().x + size / 2) ? -size / 2 : position.x;
			position.y = (position.y < -size / 2) ? (window.getSize().y + size / 2) : position.y;
			position.y = (position.y > window.getSize().y + size / 2) ? -size / 2 : position.y;

			/*float repulsionDistance = 50;

			if (position.x < repulsionDistance || position.x > windowSize.x - repulsionDistance || position.y < repulsionDistance || position.y > windowSize.y - repulsionDistance) {
				int rayNum = 20;
				for (int i = 0; i < rayNum; i++) {
					int rayAngle = (angle - 180) + (360 / rayNum) * i;

					float rayEndX = position.x + std::cos(rayAngle) * repulsionDistance;
					float rayEndY = position.y + std::sin(rayAngle) * repulsionDistance;

					if (rayEndX > repulsionDistance && rayEndY > repulsionDistance && rayEndX < windowSize.x - repulsionDistance && rayEndY < windowSize.y - repulsionDistance) {
						Alignment(rayAngle);
						i = rayNum;
					}
				}				
			}*/
		}

		void InRadius(Bird birds[], int length) {
			sf::Vector2f point(0, 0);
			float Angle = 0, moveX = 0, moveY = 0;

			for (int i = 0; i < length; i++) {

				// For Alignment
				point.x += birds[i].position.x;
				point.y += birds[i].position.y;

				// For Cohesion
				Angle += birds[i].angle;
			}

			moveX /= length;
			moveY /= length;
			point.x /= length;
			point.y /= length;
			Angle /= length;

			Separation(birds, length, BirdSize * 0.7);
			Alignment(Angle);
			Cohesion(point.x, point.y);
		}

		void Separation(const Bird* boids, int length, float separationDistance) {
			float moveX = 0;
			float moveY = 0;
			int count = 0;

			for(int i = 0; i < length; i++) {
				float dx = position.x - boids[i].position.x;
				float dy = position.y - boids[i].position.y;
				float distance = std::sqrt(dx * dx + dy * dy);

				if (distance < separationDistance && distance > 0) {
					moveX += dx / distance;  // Normalized direction vector
					moveY += dy / distance;  // Normalized direction vector
					count++;
				}
			}

			if (count > 0) {
				moveX /= count;
				moveY /= count;

				// Safeguard to ensure no NaN values are used
				if (!std::isnan(moveX) && !std::isnan(moveY)) {
					float moveAngle = atan2(moveY, moveX);
					position.x += std::cos(moveAngle) * 0.1;
					position.y += std::sin(moveAngle) * 0.1;
				}
			}
		}

		void Alignment(float Angle) {
			if (angle != Angle) {
				angle += (angle < Angle) ? 0.03 : -0.03;
			}
		}

		void Cohesion(float X, float Y) {
			if (!std::isnan(X) && !std::isnan(Y)) {
				float angle = atan2(Y - position.y, X - position.x);

				position.x += std::cos(angle)/1000;
				position.y += std::sin(angle)/1000;
			}
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

	Bird* birds = new Bird[BirdsNum];

	for(int i = 0; i < BirdsNum; i++) {
		birds[i] = Bird(randomInteger(25, 775), randomInteger(25, 575), BirdSize, randomInteger(0, 360));
	}

	while(window.isOpen()) {
		while(window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window.close();
			}
		}

		window.clear(sf::Color(50, 50, 50));

		sf::Font font;
		if (!font.loadFromFile("Fonts/arial.ttf")) {
			return -1;
		}

		sf::Vector2i mouseP = sf::Mouse::getPosition(window);

		for (int i = 0; i < BirdsNum; i++) {
			birds[i].MoveForward(window);

			Bird InRangeBirds[BirdsNum];
			int length = 0;

			for (int j = 0; j < BirdsNum; j++) {
				if (i != j) {
					if (distance(birds[i].position.x, birds[i].position.y, birds[j].position.x, birds[j].position.y) < 50) {
						float angleBetweenTheBirds = atan2(birds[i].position.y - birds[j].position.y, birds[i].position.x - birds[j].position.x) + (birds[i].angle * 3.14 / 180);

						if (abs(angleBetweenTheBirds) < 160) {
							InRangeBirds[length] = birds[j];
							length++;
						}
					}
				}
			}

			Bird* InRangeBirdsFinall = new Bird[length];
			for (int k = 0; k < length; k++) {
				InRangeBirdsFinall[k] = InRangeBirds[k];
			}

			birds[i].InRadius(InRangeBirdsFinall, length);

			delete[] InRangeBirdsFinall;

			birds[i].draw(window);
		}


		window.display();
		time++;
	}

	return 0;
}