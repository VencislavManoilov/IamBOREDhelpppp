#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Boids");
	sf::Event e;

	const int DataSize = 100;

	int data[DataSize]{};

	for (int i = 0; i < DataSize; i++) {
		int num = i * (100. / DataSize);
		data[i] = num;
	}

	while (window.isOpen()) {
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window.close();
			}
		}

		window.clear(sf::Color(50, 50, 50));

		sf::RectangleShape shapes[DataSize];
		for (int i = 0; i < DataSize; i++) {
			float value = (float)data[i] / 100.;

			shapes[i].setPosition(50. + value * 700., 590. - value * 500.);
			shapes[i].setSize(sf::Vector2f(500. / DataSize, value * 500));
			shapes[i].setFillColor(sf::Color::Green);

			window.draw(shapes[i]);
		}

		window.display();
	}
}
