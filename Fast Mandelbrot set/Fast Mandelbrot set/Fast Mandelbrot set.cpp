#include <SFML/Graphics.hpp>
#include <iostream>

const int Persision = 255;

float Zoom = 200;

static float distanceFromCenter(float X, float Y) {
	return std::sqrt(X * X + Y * Y);
}

static float Mandelbrot(float X, float Y) {
	float NewX = X;
	float NewY = Y;
	float num = 0;

	for (int i = 0; i < Persision; i++) {
		if (distanceFromCenter(NewX, NewY) > 2) {
			return num;
		}

		float OldX = NewX;
		float OldY = NewY;

		NewX = OldX * OldX - OldY * OldY + X;
		NewY = 2 * OldX * OldY + Y;
		
		num++;
	}

	return Persision;
}

float pixels[800][600];

// Making canvas to draw mandelbrot set faster
sf::Image canvas;
sf::Texture canvasTexture;
sf::Sprite canvasSprite;

static void Calculate() {
	for (int x = 0; x < 800; x++) {
		for (int y = 0; y < 600; y++) {
			pixels[x][y] = Mandelbrot((x - 500) / Zoom, (y - 300) / Zoom);

			float GrayScaleColor = 255 - pixels[x][y];
			canvas.setPixel(x, y, sf::Color(GrayScaleColor, GrayScaleColor, GrayScaleColor));
		}
	}
}

static void Draw(sf::RenderWindow& window) {
	window.clear();

	if (!canvasTexture.loadFromImage(canvas))
		return;

	canvasSprite.setTexture(canvasTexture);
	
	window.draw(canvasSprite);

	window.display();
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Boids");
	sf::Event e;

	canvas.create(800, 600, sf::Color::White);

	Calculate();

	Draw(window);

	while (window.isOpen()) {
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window.close();
			}
		}
	}
}