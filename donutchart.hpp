#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "utils.hpp"

void draw_donut_sector(sf::Image& canvas, int cx, int cy, int outerR, int innerR, float startAngle, float endAngle, sf::Color color);
void donutchart(sf::Image& canvas, sf::RenderTexture& rt, int* values, std::string* labels, int sampleSize);
