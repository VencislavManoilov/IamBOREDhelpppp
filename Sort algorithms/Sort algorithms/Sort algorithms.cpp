#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Sort Algorithm");
    sf::Event e;

    const int DataSize = 1000;

    float data[DataSize]{};

    int time = 0;

    int option = 0;

    bool Start = false;

    for (int i = 0; i < DataSize; i++) {
        float num = i * (100.0f / DataSize);
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

    sf::Text controllsText("Start - Space; Bubble Sort - 1; Selection Sort - 2", font, 25);
    sf::Text selectedSortText("Bubble Sort", font, 25);
    selectedSortText.setPosition(0, 30);

    int selectionSortI = 0;

    while (window.isOpen()) {
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color(50, 50, 50));

        window.draw(controllsText);
        window.draw(selectedSortText);

        float barWidth = 700.0f / DataSize;
        for (int i = 0; i < DataSize; i++) {
            float value = static_cast<float>(data[i]) / 100.0f;
            shapes[i].setPosition(50.0f + i * barWidth, 590.0f - value * 500.0f);
            shapes[i].setSize(sf::Vector2f(barWidth, value * 500.0f));
            shapes[i].setFillColor(sf::Color::Green);

            window.draw(shapes[i]);
        }

        if (Start) {
            switch (option) {
            case 0: // Bubble Sort
                for (int j = 0; j < DataSize - 1; j++) {
                    if (data[j] > data[j + 1]) {
                        std::swap(data[j], data[j + 1]);
                    }
                }
                break;
            case 1: // Selection Sort
                if (selectionSortI < DataSize - 1) {
                    int min_idx = selectionSortI;
                    for (int j = selectionSortI + 1; j < DataSize; j++) {
                        if (data[j] < data[min_idx]) {
                            min_idx = j;
                        }
                    }
                    if (min_idx != selectionSortI) {
                        std::swap(data[min_idx], data[selectionSortI]);
                    }
                    selectionSortI++;
                }
                break;
            default:
            break;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
            option = 0;
            selectedSortText.setString("Bubble Sort");
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
            option = 1;
            selectedSortText.setString("Selection Sort");
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