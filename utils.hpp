#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

void draw_line(sf::Image& canvas, int x0, int y0, int x1, int y1, sf::Color color);
void draw_circle(sf::Image& canvas, int cx, int cy, int r, sf::Color color, bool fill = false);
void draw_rect(sf::Image& canvas, int x, int y, int w, int h, sf::Color color, bool fill = false);

//  scan-line polygon fill
void scanline_fill_polygon(sf::Image& canvas, const std::vector<sf::Vector2f>& verts, sf::Color color);

// flood-fill: replaces target_color region connected to (x,y)
void flood_fill(sf::Image& canvas, int x, int y, sf::Color fill_color, sf::Color target_color);

// boundary-fill: fills region reachable from (x,y) not touching boundary_color
void boundary_fill(sf::Image& canvas, int x, int y, sf::Color fill_color, sf::Color boundary_color);

extern sf::Font font;
