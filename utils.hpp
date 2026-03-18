#pragma once
#include <SFML/Graphics.hpp>

void draw_line(sf::Image& canvas, int x0, int y0, int x1, int y1, sf::Color color);
void draw_circle(sf::Image& canvas, int cx, int cy, int r, sf::Color color, bool fill = false);
void draw_rect(sf::Image& canvas, int x, int y, int w, int h, sf::Color color, bool fill = false);
void flood_fill(sf::Image& canvas, int x, int y, sf::Color fill_color, sf::Color target_color);
extern sf::Font font;