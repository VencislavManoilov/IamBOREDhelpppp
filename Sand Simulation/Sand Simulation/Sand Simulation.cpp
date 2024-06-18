#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <random>

const int SquareSize = 1;

const int SandWidth = 800 / SquareSize;
const int SandHeight = 600 / SquareSize;

std::vector<std::vector<int>> sandStage(SandWidth, std::vector<int>(SandHeight));
std::vector<std::vector<int>> sandNextStage(SandWidth, std::vector<int>(SandHeight));

// Making a canvas to draw stuff Fasteeer
sf::Image canvas;
sf::Sprite canvasSprite;
sf::Texture canvasTexture;

void seedRandomGenerator() {
	std::srand(static_cast<unsigned int>(std::time(nullptr)));  // Using current time as seed
}

// Function to generate a random integer between min and max (inclusive)
int randomInteger(int min, int max) {
	// Ensure srand() is called to seed rand() before generating random numbers
	static bool seeded = false;
	if (!seeded) {
		seedRandomGenerator();
		seeded = true;
	}

	// Generate and return a random integer in the range [min, max]
	return (std::rand() % (max - min + 1)) + min;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Sand Simulation");
    sf::Event e;

	canvas.create(800, 600, sf::Color::Black);
	if (!canvasTexture.loadFromImage(canvas)) {
		return -1;
	}
	canvasSprite.setTexture(canvasTexture);

	sf::Font font;
	if (!font.loadFromFile("Fonts/arial.ttf")) {
		return -1;
	}

	sf::Text controllsText("Place Sand - Mouse; Erase All - Del", font, 25);

	for (int x = 0; x < SandWidth; x++) {
		for (int y = 0; y < SandHeight; y++) {
			sandStage[x][y] = 0;
			sandNextStage[x][y] = 0;
		}
	}

	while (window.isOpen()) {
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window.close();
			}
		}

		window.clear(sf::Color::Black);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
			int GridPositionX = mousePosition.x / SquareSize;
			int GridPositionY = mousePosition.y / SquareSize;

			if (GridPositionX >= 0 && GridPositionX <= SandWidth - 1 && GridPositionY >= 0 && GridPositionY <= SandHeight - 1) {
				sandStage[GridPositionX][GridPositionY] = 1;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Delete)) {
			for (int x = 0; x < SandWidth; x++) {
				for (int y = 0; y < SandHeight; y++) {
					sandStage[x][y] = 0;
					sandNextStage[x][y] = 0;
				}
			}
		}

		for (int x = 0; x < SandWidth; x++) {
			for (int y = 0; y < SandHeight - 1; y++) {
				if (sandStage[x][y] == 1) {
					if (sandStage[x][y + 1] != 1) {
						sandNextStage[x][y] = 0;
						sandNextStage[x][y + 1] = 1;
					} else {
						bool done = false;
						if (x > 0 && x < SandWidth - 1) {
							if (sandStage[x - 1][y + 1] != 1 && sandStage[x + 1][y + 1] != 1) {
								sandNextStage[x + (randomInteger(0, 1) * 2) - 1][y + 1] = 1;
								sandNextStage[x][y] = 0;
								done = true;
							}
						}

						if (x > 0) {
							if (sandStage[x - 1][y + 1] != 1 && x > 0) {
								sandNextStage[x - 1][y + 1] = 1;
								sandNextStage[x][y] = 0;
								done = true;
							}
						}

						if (x < SandWidth - 1) {
							if (sandStage[x + 1][y + 1] != 1) {
								sandNextStage[x + 1][y + 1] = 1;
								sandNextStage[x][y] = 0;
								done = true;
							}
						}

						if (!done) {
							sandNextStage[x][y] = 1;
						}
					}
				}
			}
		}

		for (int x = 0; x < SandWidth; x++) {
			for (int y = 0; y < SandHeight; y++) {
				for (int pixelX = 0; pixelX < SquareSize; pixelX++) {
					for (int pixelY = 0; pixelY < SquareSize; pixelY++) {
						if (sandStage[x][y] == 1) {
							canvas.setPixel(x * SquareSize + pixelX, y * SquareSize + pixelY, sf::Color::Yellow);
						} else {
							canvas.setPixel(x * SquareSize + pixelX, y * SquareSize + pixelY, sf::Color::Black);
						}
					}
				}
				sandStage[x][y] = sandNextStage[x][y];
			}
		}

		if (!canvasTexture.loadFromImage(canvas)) {
			return -1;
		}
		canvasSprite.setTexture(canvasTexture);

		window.draw(canvasSprite);

		window.draw(controllsText);

		window.display();

		//Sleep(5);
    }
}
