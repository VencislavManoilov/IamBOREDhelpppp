#include <SFML/Graphics.hpp>
#include <iostream>

static int randomInteger(int min, int max) {
	return (rand() % (max - min + 1)) + min;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Boids");
    sf::Event e;

	const int BoxNum = 5;

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

		float GetDistance(Box boxes[], int length) {


			return 0;
		}

		void Draw(sf::RenderWindow& window) {
			std::vector<sf::CircleShape> shape(iterations);

			for (int i = 0; i < iterations; i++) {
				shape[i].setRadius(2);
				shape[i].setFillColor(sf::Color::White);

				float dist = i * (width/iterations);
				shape[i].setPosition(startPosition.x + std::cos(angle * 3.14 / 180) * dist, startPosition.y + std::sin(angle * 3.14 / 180) * dist);

				window.draw(shape[i]);
			}
		}
	};

	Box* boxes = new Box[BoxNum];
	Ray* rays = new Ray[100];

	for (int i = 0; i < BoxNum; i++) {
		boxes[i] = Box(sf::Vector2f(randomInteger(0, 700), randomInteger(0, 500)), sf::Vector2f(randomInteger(0, 100), randomInteger(0, 100)), sf::Color::Green);
	}

	for (int i = 0; i < 100; i++) {
		float angle = (130. / 99.) * i;
		rays[i] = Ray(sf::Vector2f(400, 300), angle, 200, 20);
	}

	while (window.isOpen()) {
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window.close();
			}
		}

		window.clear(sf::Color(50, 50, 50));

		for (int i = 0; i < BoxNum; i++) {
			boxes[i].Draw(window);
		}

		for (int i = 0; i < 100; i++) {
			rays[i].Draw(window);
		}

		window.display();
	}
}
