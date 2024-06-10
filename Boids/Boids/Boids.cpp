#include "SFML/Graphics.hpp"
#include <SFML/Graphics/Font.hpp>
#include "iostream"

int main() {

	sf::RenderWindow window(sf::VideoMode(800, 600), "Boids");
	sf::Event e;

	int time = 0;

	while(window.isOpen()) {
		while(window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window.close();
			}
		}

		window.clear(sf::Color::White);

		sf::Font font;
		if (!font.loadFromFile("Fonts/arial.ttf")) {
			return -1;
		}

		sf::Text text;
		text.setFont(font);
		text.setString("Hello World!");
		text.setCharacterSize(24);
		text.setFillColor(sf::Color::Red);
		text.setPosition(10, 10);

		window.draw(text);


		sf::CircleShape shape(50.f);
		shape.setFillColor(sf::Color(150, 50, 250));
		shape.setPosition(50, 100);
		shape.setOutlineThickness(10.f);
		shape.setOutlineColor(sf::Color(250, 150, 100));

		window.draw(shape);


		sf::RectangleShape rectangle(sf::Vector2f(120, 50));
		rectangle.setFillColor(sf::Color::Green);
		rectangle.setPosition(200, 75);

		window.draw(rectangle);


		sf::VertexArray triangle(sf::Triangles, 3);

		triangle[0].position = sf::Vector2f(400, 100.f);
		triangle[1].position = sf::Vector2f(500.f, 100.f);
		triangle[2].position = sf::Vector2f(500.f, 200.f);

		triangle[0].color = sf::Color::Red;
		triangle[1].color = sf::Color::Blue;
		triangle[2].color = sf::Color::Green;

		window.draw(triangle);

		
		sf::Texture tex;
		if (!tex.loadFromFile("images/texture.jpg")) {
			return -1;
		}

		sf::RectangleShape player(sf::Vector2f(200, 200));
		player.setPosition(25, 300);
		player.setTexture(&tex);

		window.draw(player);


		sf::RectangleShape line(sf::Vector2f(5, 100));
		line.setPosition(400, 400);
		line.setFillColor(sf::Color::Black);
		line.rotate(time);

		window.draw(line);


		window.display();
		time++;
	}

	return 0;
}