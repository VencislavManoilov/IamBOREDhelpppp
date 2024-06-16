#include <SFML/Graphics.hpp>
#include <iostream>

const int SquareSize = 10;

const int SandWidth = 800 / SquareSize;
const int SandHeight = 600 / SquareSize;

int sand[SandWidth][SandHeight]{};
sf::RectangleShape shapes[SandWidth][SandHeight];

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Sand Simulation");
    sf::Event e;

	for (int x = 0; x < SandWidth; x++) {
		for (int y = 0; y < SandHeight; y++) {
			sand[x][y] = 0;

			shapes[x][y].setPosition(x * SquareSize, y * SquareSize);
			shapes[x][y].setSize(sf::Vector2f(SquareSize, SquareSize));
			shapes[x][y].setFillColor(sf::Color::Yellow);
		}
	}

	while (window.isOpen()) {
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window.close();
			}
		}

		window.clear(sf::Color::Black);

		sand[10][10] = 1;

		for (int x = 0; x < SandWidth; x++) {
			for (int y = 0; y < SandHeight; y++) {
				if (sand[x][y] == 1) {
					window.draw(shapes[x][y]);
				}
			}
		}

		window.display();
    }
}
