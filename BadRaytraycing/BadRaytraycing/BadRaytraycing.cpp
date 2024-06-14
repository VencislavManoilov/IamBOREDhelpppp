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

	Box* boxes = new Box[BoxNum];

	for (int i = 0; i < BoxNum; i++) {
		boxes[i] = Box(sf::Vector2f(randomInteger(0, 700), randomInteger(0, 500)), sf::Vector2f(randomInteger(0, 100), randomInteger(0, 100)), sf::Color::Green);
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

		window.display();
	}
}
