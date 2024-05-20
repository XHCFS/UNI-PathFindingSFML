// GUI.cpp
#include <SFML/Graphics.hpp>
#include <vector>

// Constants
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int PIXEL_SIZE = 10;

// Function to get color from value
sf::Color getColorFromValue(int value) {
    switch (value) {
        case 0: return sf::Color::White;
        case 1: return sf::Color::Black;
        case 2: return sf::Color::Green;
        case 3: return sf::Color::Blue;
        case 4: return sf::Color::Red;
        default: return sf::Color::White;
    }
}

// Function to display frames
void displayFrames(const std::vector<std::vector<std::vector<int>>>& frames) {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML Grid");

    sf::Clock clock;
    int frameIndex = 0;
    float frameDuration = 1.0f; // 1 second per frame

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (clock.getElapsedTime().asSeconds() > frameDuration) {
            clock.restart();
            frameIndex = (frameIndex + 1) % frames.size();
        }

        window.clear();

        const auto& currentFrame = frames[frameIndex];
        for (size_t y = 0; y < currentFrame.size(); ++y) {
            for (size_t x = 0; x < currentFrame[y].size(); ++x) {
                sf::RectangleShape pixel(sf::Vector2f(PIXEL_SIZE, PIXEL_SIZE));
                pixel.setFillColor(getColorFromValue(currentFrame[y][x]));
                pixel.setPosition(x * PIXEL_SIZE, y * PIXEL_SIZE);
                window.draw(pixel);
            }
        }

        window.display();
    }
}

