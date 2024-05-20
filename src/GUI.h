// GUI.h
#ifndef GUI_H
#define GUI_H

#include <SFML/Graphics.hpp>
#include <vector>

sf::Color getColorFromValue(int value);
void displayFrames(const std::vector<std::vector<std::vector<int>>>& frames);

#endif // GUI_H
