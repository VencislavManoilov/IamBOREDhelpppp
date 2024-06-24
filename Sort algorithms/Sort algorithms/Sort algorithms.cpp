#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <vector>

std::vector<std::vector<float>> AddData(std::vector<std::vector<float>> historyData, std::vector<float> newData) {
    historyData.push_back(newData);

    return historyData;
}

std::vector<std::vector<float>> historyData;

void merge(std::vector<float>& array, int const left, int const mid, int const right) {
    int const subArrayOne = mid - left + 1;
    int const subArrayTwo = right - mid;

    std::vector<float> leftArray;
    std::vector<float> rightArray;

    for (auto i = 0; i < subArrayOne; i++)
        leftArray.push_back(array[left + i]);
    for (auto j = 0; j < subArrayTwo; j++)
        rightArray.push_back(array[mid + 1 + j]);

    auto indexOfSubArrayOne = 0, indexOfSubArrayTwo = 0;
    int indexOfMergedArray = left;

    while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) {
        if (leftArray[indexOfSubArrayOne] <= rightArray[indexOfSubArrayTwo]) {
            array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
            indexOfSubArrayOne++;
        } else {
            array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
            indexOfSubArrayTwo++;
        }
        indexOfMergedArray++;
    }

    while (indexOfSubArrayOne < subArrayOne) {
        array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
        indexOfSubArrayOne++;
        indexOfMergedArray++;
    }

    while (indexOfSubArrayTwo < subArrayTwo) {
        array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
    }
}

void mergeSort(std::vector<float>& array, int const begin, int const end) {
    if (begin >= end)
        return;

    int mid = begin + (end - begin) / 2;
    mergeSort(array, begin, mid);
    mergeSort(array, mid + 1, end);
    merge(array, begin, mid, end);
    historyData = AddData(historyData, array);
}

float partition(std::vector<float>& arr, int low, int high) {
    float pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }

    std::swap(arr[i + 1], arr[high]);
    historyData = AddData(historyData, arr);
    return (i + 1);
}

void quickSort(std::vector<float>& arr, int low, int high){
    if (low < high) {
        float pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int main() {
    std::srand(std::time(0));

    sf::RenderWindow window(sf::VideoMode(800, 600), "Sort Algorithm");
    sf::Event e;

    const int DataSize = 1000;

    std::vector<float> suffledData;

    int time = 0;

    int option = 0;

    bool Start = false;

    int frame = 0;

    bool DoneSorting = false;

    for (int i = 0; i < DataSize; i++) {
        float num = i * (100.0f / DataSize);
        suffledData.push_back(num);
    }

    for (int i = DataSize - 1; i > 0; i--) {
        int j = std::rand() % (i + 1);
        std::swap(suffledData[i], suffledData[j]);
    }

    sf::RectangleShape shapes[DataSize];

    sf::Font font;
    if (!font.loadFromFile("Fonts/arial.ttf")) {
        return -1;
    }

    sf::Text controllsText("Start - Space; Bubble Sort - 1; Selection Sort - 2; Merge Sort - 3; Quick Sort - 4; Cocktail Sort - 5", font, 18);
    sf::Text selectedSortText("Bubble Sort", font, 25);
    selectedSortText.setPosition(0, 25);

    while (window.isOpen()) {
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color(50, 50, 50));

        window.draw(controllsText);
        window.draw(selectedSortText);

        if (Start && !DoneSorting) {
            switch (option) {
            case 0:
                // Bubble sort
                bool swapped;
                for (int i = 0; i < DataSize - 1; i++) {
                    swapped = false;
                    for (int j = 0; j < DataSize - i - 1; j++) {
                        if (suffledData[j] > suffledData[j + 1]) {
                            std::swap(suffledData[j], suffledData[j + 1]);
                            swapped = true;
                        }
                    }

                    historyData = AddData(historyData, suffledData);

                    if (swapped == false)
                        break;
                }

				DoneSorting = true;
            break;
            case 1:
                // Selection sort
                int min_idx;
                for (int i = 0; i < DataSize - 1; i++) {
                    min_idx = i;
                    for (int j = i + 1; j < DataSize; j++) {
                        if (suffledData[j] < suffledData[min_idx])
                            min_idx = j;
                    }

                    if (min_idx != i) {
                        std::swap(suffledData[min_idx], suffledData[i]);
                        historyData = AddData(historyData, suffledData);
                    }
                }
                DoneSorting = true;
            break;
            case 2:
                // Merge sort
                mergeSort(suffledData, 0, suffledData.size() - 1);
                DoneSorting = true;
            break;
            case 3:
                // Quick sort
                quickSort(suffledData, 0, suffledData.size() - 1);
                DoneSorting = true;
            break;
            case 4:
                // Cocktail sort
            default:
            break;
            }
        }

        float barWidth = 700.0f / DataSize;
        for (int i = 0; i < DataSize; i++) {
            float value = 0;
            if (!Start && !DoneSorting) {
                value = suffledData[i] / 100.0f;
            } else {
                value = historyData[frame][i] / 100.0f;
            }
            shapes[i].setPosition(50.0f + i * barWidth, 590.0f - value * 500.0f);
            shapes[i].setSize(sf::Vector2f(barWidth, value * 500.0f));
            shapes[i].setFillColor(sf::Color::Green);

            window.draw(shapes[i]);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
            option = 0;
            selectedSortText.setString("Bubble Sort");
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
            option = 1;
            selectedSortText.setString("Selection Sort");
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
            option = 2;
            selectedSortText.setString("Merge Sort");
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
            option = 3;
            selectedSortText.setString("Quick Sort");
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)) {
            option = 4;
            selectedSortText.setString("Cocktail Sort");
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            Start = true;
        }

        if (Start && DoneSorting) {
            frame++;

            if (frame >= historyData.size())
                frame = historyData.size() - 1;

            Sleep(1);
        }

        window.display();
        time++;
    }
}