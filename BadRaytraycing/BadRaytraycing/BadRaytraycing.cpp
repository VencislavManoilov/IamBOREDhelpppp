#include <SFML/Graphics.hpp>
#include <iostream>

static int randomInteger(int min, int max) {
	return (rand() % (max - min + 1)) + min;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Boids");
    sf::Event e;

	int time = 0;
	int RenderOption = 0;
	
	sf::Vector2f position(400, 300);

	const int fieldOfView = 90;
	const int iterations = 100;
	const int width = 200;
	const int angles = 100;

	float startAngle = 0;
	bool SpacebarPressed = false;

	const int BoxNum = 20;

	class Box {
	public:
		sf::Vector2f position;
		sf::Vector2f size;
		sf::Color color;

		Box() : position(0, 0), size(0, 0), color(0, 0, 0) {}

		Box(sf::Vector2f Position, sf::Vector2f Size, sf::Color Color) :
		position(Position), size(Size), color(Color) {}

		void Draw(sf::RenderWindow& window) const {
			sf::RectangleShape shape;

			shape.setPosition(position);
			shape.setSize(size);
			shape.setFillColor(color);

			window.draw(shape);
		}
	};

	class Ray {
	public:
		sf::Vector2f startPosition;
		float angle;
		float width;
		int iterations;

		Ray() : startPosition(0, 0), angle(0), width(0), iterations(0) {}

		Ray(sf::Vector2f StartPosition, float Angle, float Width, int Iterations) :
		startPosition(StartPosition), angle(Angle), width(Width), iterations(Iterations) {}

		float GetDistance(Box boxes[], int length, bool Draw, sf::RenderWindow& window) {
			std::vector<sf::CircleShape> shape(iterations);

			for (int i = 0; i < iterations; i++) {
				float dist = i * (width/iterations);
				float X = startPosition.x + std::cos(angle * 3.14 / 180) * dist;
				float Y = startPosition.y + std::sin(angle * 3.14 / 180) * dist;

				if (Draw) {
					shape[i].setRadius(2);
					shape[i].setFillColor(sf::Color::White);

					shape[i].setPosition(X, Y);

					window.draw(shape[i]);
				}

				for (int j = 0; j < length; j++) {
					// Checks if inside
					if (X >= boxes[j].position.x && X <= boxes[j].position.x + boxes[j].size.x
					&&  Y >= boxes[j].position.y && Y <= boxes[j].position.y + boxes[j].size.y) {

						return dist;
					}
				}
			}

			return width;
		}
	};

	Box* boxes = new Box[BoxNum];
	Ray* rays = new Ray[angles];

	for (int i = 0; i < BoxNum; i++) {
		boxes[i] = Box(sf::Vector2f(randomInteger(0, 700), randomInteger(0, 500)), sf::Vector2f(randomInteger(0, 100), randomInteger(0, 100)), sf::Color::Red);
	}

	for (int i = 0; i < angles; i++) {
		float angle = ((float)fieldOfView / (float)angles) * i;
		rays[i] = Ray(sf::Vector2f(400, 300), angle, width, iterations);
	}

	while (window.isOpen()) {
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window.close();
			}
		}

		if (RenderOption == 0) {
			window.clear(sf::Color(50, 50, 50));
		} else {
			window.clear(sf::Color::Black);
		}

		if (RenderOption == 0) {
			for (int i = 0; i < BoxNum; i++) {
				boxes[i].Draw(window);
			}
		}

		sf::RectangleShape shape[angles];
		for (int i = 0; i < angles; i++) {
			rays[i].startPosition = position;
			rays[i].angle = startAngle + ((float)fieldOfView / (float)angles) * i;
			rays[i].width = width;

			if (RenderOption == 0) {
				rays[i].GetDistance(boxes, BoxNum, true, window);
			} else if (RenderOption == 1) {
				float value = rays[i].GetDistance(boxes, BoxNum, false, window);
				float wallHeigth = value * (600./width);

				/*shape[i].setPosition(i * (800/angles), 0);
				shape[i].setSize(sf::Vector2f(800 / angles, 600));
				shape[i].setFillColor(sf::Color(255 - value * (255. / width), 0, 0));*/
				
				if (value != width) {
					shape[i].setPosition(i * (800./angles), 300. - (300. - wallHeigth/2 + 100));
					shape[i].setSize(sf::Vector2f(800. / angles, 600. - wallHeigth + 200));
					shape[i].setFillColor(sf::Color(255. - value * (255. / width), 0, 0));
				}

				/* shape[i].setPosition(i* (800 / angles), 0);
				shape[i].setSize(sf::Vector2f(800 / angles, 600));
				shape[i].setFillColor(sf::Color(255 - value * (255. / width), 0, 0));*/

				window.draw(shape[i]);
			}
		}

		// Keyboard inputs
		float MoveX, MoveY;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			position.x += std::cos((startAngle + fieldOfView / 2) * 3.14 / 180.);
			position.y += std::sin((startAngle + fieldOfView / 2) * 3.14 / 180.);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			position.x += std::cos((startAngle + fieldOfView / 2 - 180) * 3.14 / 180.);
			position.y += std::sin((startAngle + fieldOfView / 2 - 180) * 3.14 / 180.);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			position.x += std::cos((startAngle + fieldOfView / 2 - 90) * 3.14 / 180.);
			position.y += std::sin((startAngle + fieldOfView / 2 - 90) * 3.14 / 180.);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			position.x += std::cos((startAngle + fieldOfView / 2 + 90) * 3.14 / 180.);
			position.y += std::sin((startAngle + fieldOfView / 2 + 90) * 3.14 / 180.);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
			startAngle--;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
			startAngle++;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			if (!SpacebarPressed) {
				RenderOption++;
			}
			SpacebarPressed = true;
		} else {
			SpacebarPressed = false;
		}

		if (RenderOption > 1) {
			RenderOption = 0;
		}

		window.display();
		time++;
	}
}
