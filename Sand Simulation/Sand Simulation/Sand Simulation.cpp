#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <windows.h>

const int SquareSize = 10;

const int SandWidth = 800 / SquareSize;
const int SandHeight = 600 / SquareSize;

std::vector<std::vector<int>> sandStage(SandWidth, std::vector<int>(SandHeight));
std::vector<std::vector<int>> sandNextStage(SandWidth, std::vector<int>(SandHeight));
sf::RectangleShape shapes[SandWidth][SandHeight];

static int randomInteger(int min, int max) {
	return (rand() % (max - min + 1)) + min;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Sand Simulation");
    sf::Event e;

	for (int x = 0; x < SandWidth; x++) {
		for (int y = 0; y < SandHeight; y++) {
			int stage = randomInteger(0, 1);
			sandStage[x][y] = stage;
			sandNextStage[x][y] = stage;

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

		for (int x = 0; x < SandWidth; x++) {
			for (int y = 0; y < SandHeight - 1; y++) {
				if (sandStage[x][y] == 1) {
					if (sandStage[x][y + 1] != 1) {
						sandNextStage[x][y] = 0;
						sandNextStage[x][y + 1] = 1;
					} else {
						sandNextStage[x][y] = 1;
					}
				}
			}
		}

		for (int x = 0; x < SandWidth; x++) {
			for (int y = 0; y < SandHeight; y++) {
				if (sandStage[x][y] == 1) {
					window.draw(shapes[x][y]);
				}
			}
		}

		for (int x = 0; x < SandWidth; x++) {
			for (int y = 0; y < SandHeight; y++) {
				sandStage[x][y] = sandNextStage[x][y];
			}
		}

		window.display();

		Sleep(100);
    }
}
