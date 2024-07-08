#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

const int Persision = 255;

double Zoom = 200;

double offsetX = -0.7;
double offsetY = 0;

static double distanceFromCenter(double X, double Y) {
	return std::sqrt(X * X + Y * Y);
}

static double Mandelbrot(double X, double Y) {
	double NewX = X;
	double NewY = Y;
	double num = 0;

	for (int i = 0; i < Persision; i++) {
		if (distanceFromCenter(NewX, NewY) > 2) {
			return num;
		}

		double OldX = NewX;
		double OldY = NewY;

		NewX = OldX * OldX - OldY * OldY + X;
		NewY = 2 * OldX * OldY + Y;
		
		num++;
	}

	return Persision;
}

double pixels[800][600];

// Making canvas to draw mandelbrot set faster
sf::Image canvas;
sf::Texture canvasTexture;
sf::Sprite canvasSprite;

sf::Font font;

// Convert HSV to RGB
sf::Color hsvToRgb(double h, double s, double v) {
	double r, g, b;

	int i = static_cast<int>(h * 6);
	double f = h * 6 - i;
	double p = v * (1 - s);
	double q = v * (1 - f * s);
	double t = v * (1 - (1 - f) * s);

	switch (i % 6) {
	case 0: r = v, g = t, b = p; break;
	case 1: r = q, g = v, b = p; break;
	case 2: r = p, g = v, b = t; break;
	case 3: r = p, g = q, b = v; break;
	case 4: r = t, g = p, b = v; break;
	case 5: r = v, g = p, b = q; break;
	}

	return sf::Color(static_cast<sf::Uint8>(r * 255), static_cast<sf::Uint8>(g * 255), static_cast<sf::Uint8>(b * 255));
}

static void Calculate() {
	for (int x = 0; x < 800; x++) {
		for (int y = 0; y < 600; y++) {
			double coordX = (x - 400) / Zoom + offsetX;
			double coordY = (y - 300) / Zoom + offsetY;
			pixels[x][y] = Mandelbrot(coordX, coordY);

			// Normalize the iteration count to [0, 1]
			double norm = pixels[x][y] / Persision;

			// Map the normalized value to a color
			sf::Color color = hsvToRgb(norm, 1.0f, norm < 1 ? 1.0f : 0.0f);

			canvas.setPixel(x, y, color);
		}
	}
}

static void Draw(sf::RenderWindow& window, sf::Text controllsText, sf::Text controllsSecondText) {
	window.clear();

	if (!canvasTexture.loadFromImage(canvas))
		return;

	canvasTexture.setSmooth(true);
	canvasSprite.setTexture(canvasTexture);
	
	window.draw(canvasSprite);
	
	window.draw(controllsText);
	window.draw(controllsSecondText);

	window.display();
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Boids");
	sf::Event e;

	canvas.create(800, 600, sf::Color::White);

	if (!font.loadFromFile("Fonts/arial.ttf")) {
		return -1;
	}

	sf::Text controllsText("Zoom - Space; Zoom Out - Left Shift; Cool Point - E; Reset - R", font, 25);
	sf::Text controllsSecondText("Move - WASD", font, 25);
	controllsSecondText.setPosition(0, 30);

	Calculate();

	Draw(window, controllsText, controllsSecondText);

	while (window.isOpen()) {
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window.close();
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			Zoom *= 1.2f;

			Calculate();
			Draw(window, controllsText, controllsSecondText);
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
			Zoom /= 1.2f;
			Calculate();
			Draw(window, controllsText, controllsSecondText);
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
			Zoom = 1000000;
			offsetX = -0.743643887037151f;
			offsetY = 0.13182590420533f;

			Calculate();
			Draw(window, controllsText, controllsSecondText);
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
			Zoom = 200;

			offsetX = -0.7;
			offsetY = 0.;

			Calculate();
			Draw(window, controllsText, controllsSecondText);
		}

		double MoveSpeed = 20. / Zoom;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			offsetY -= MoveSpeed;
			Calculate();
			Draw(window, controllsText, controllsSecondText);
		} if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			offsetX -= MoveSpeed;
			Calculate();
			Draw(window, controllsText, controllsSecondText);
		} if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			offsetY += MoveSpeed;
			Calculate();
			Draw(window, controllsText, controllsSecondText);
		} if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			offsetX += MoveSpeed;
			Calculate();
			Draw(window, controllsText, controllsSecondText);
		}
	}
}