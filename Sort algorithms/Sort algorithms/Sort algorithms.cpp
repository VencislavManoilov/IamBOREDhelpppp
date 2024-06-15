#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Sort Algorithm");
	sf::Event e;

	const int DataSize = 1000;

	float data[DataSize]{};

	int time = 0;

	bool Start = false;

	for (int i = 0; i < DataSize; i++) {
		float num = i * (100. / DataSize);
		data[i] = num;
	}

	for (int i = DataSize - 1; i > 0; i--) {
		int j = std::rand() % (i + 1);
		std::swap(data[i], data[j]);
	}

	sf::RectangleShape shapes[DataSize];

	sf::Font font;
	if (!font.loadFromFile("Fonts/arial.ttf")) {
		return -1;
	}

	sf::Text controllsText("Start - Space", font, 25);

	while (window.isOpen()) {
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window.close();
			}
		}

		window.clear(sf::Color(50, 50, 50));

		window.draw(controllsText);
		
		float barWidth = 700.0f / DataSize;

		for (int i = 0; i < DataSize; i++) {
			float value = static_cast<float>(data[i]) / 100.0f;

			shapes[i].setPosition(50. + i * barWidth, 590. - value * 500.);
			shapes[i].setSize(sf::Vector2f(barWidth, value * 500.0f));
			shapes[i].setFillColor(sf::Color::Green);

			window.draw(shapes[i]);
		}

		if (Start) {
			for (int j = 0; j < DataSize - 1; j++) {
				if (data[j] > data[j + 1]) {
					std::swap(data[j], data[j + 1]);
				}
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			Start = true;
		}

		window.display();
		time++;

		if (Start) {
			Sleep(1);
		}
	}
}