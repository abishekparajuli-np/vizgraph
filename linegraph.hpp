#pragma once
#include<SFML/Graphics.hpp>
#include "utils.hpp"
void linegraph(sf::Image& canvas,sf::RenderTexture& tx, int* x, int* y,int sampleSize);
void linegraph1(sf::Image& canvas, int* x, int* y,int sampleSize);
void sortforlinegraph(int* x,int* y,int sampleSize);