#include "piechart.hpp"
#include <cmath>
#include <cstdio>
#include <vector>

#define M_PI 3.14159265358979323846f
static const int ARC_STEPS = 120;

// Converts HSL to sf::Color (h: 0-360, s: 0-1, l: 0-1)
static sf::Color hslToColor(float h, float s, float l) {
    float c = (1.0f - std::fabs(2.0f * l - 1.0f)) * s;
    float x = c * (1.0f - std::fabs(std::fmod(h / 60.0f, 2.0f) - 1.0f));
    float m = l - c / 2.0f;

    float r, g, b;
    if      (h < 60)  { r = c; g = x; b = 0; }
    else if (h < 120) { r = x; g = c; b = 0; }
    else if (h < 180) { r = 0; g = c; b = x; }
    else if (h < 240) { r = 0; g = x; b = c; }
    else if (h < 300) { r = x; g = 0; b = c; }
    else              { r = c; g = 0; b = x; }

    return sf::Color(
        (uint8_t)((r + m) * 255),
        (uint8_t)((g + m) * 255),
        (uint8_t)((b + m) * 255)
    );
}

// Generates a distinct color for slice i out of total slices
static sf::Color colorForSlice(int i, int total) {
    float hue        = (360.0f * i) / total;   
    float saturation = 0.65f;                   
    float lightness  = 0.55f;                   
    return hslToColor(hue, saturation, lightness);
}

void draw_sector(sf::Image& canvas, int cx, int cy, int r,
                         float startAngle, float endAngle, sf::Color color) {
    std::vector<sf::Vector2f> poly;
    poly.push_back({(float)cx, (float)cy});
    for (int s = 0; s <= ARC_STEPS; s++) {
        float angle = startAngle + ((float)s / ARC_STEPS) * (endAngle - startAngle);
        poly.push_back({cx + std::cos(angle) * r, cy + std::sin(angle) * r});
    }
    scanline_fill_polygon(canvas, poly, color);

    int sx0 = (int)(cx + std::cos(startAngle) * r), sy0 = (int)(cy + std::sin(startAngle) * r);
    int sx1 = (int)(cx + std::cos(endAngle)   * r), sy1 = (int)(cy + std::sin(endAngle)   * r);
    draw_line(canvas, cx, cy, sx0, sy0, sf::Color::White);
    draw_line(canvas, cx, cy, sx1, sy1, sf::Color::White);

    float mid  = startAngle + (endAngle - startAngle) * 0.5f;
    int seedX  = cx + (int)(std::cos(mid) * r * 0.65f);
    int seedY  = cy + (int)(std::sin(mid) * r * 0.65f);
    if (canvas.getPixel({(unsigned)seedX, (unsigned)seedY}) == sf::Color::White)
        flood_fill(canvas, seedX, seedY, color, sf::Color::White);
}

void piechart(sf::Image& canvas, sf::RenderTexture& rt,
              int* values, std::string* labels, int sampleSize) {
    int cx = 400, cy = 300, r = 220;
    float total = 0;
    for (int i = 0; i < sampleSize; i++) total += values[i];

    float currentAngle = 0.0f;
    for (int i = 0; i < sampleSize; i++) {
        float sweep = (values[i] / total) * 2.0f * M_PI;
        sf::Color color = colorForSlice(i, sampleSize);  
        draw_sector(canvas, cx, cy, r, currentAngle, currentAngle + sweep, color);
        currentAngle += sweep;
    }

    draw_circle(canvas, cx, cy, r, sf::Color::White, false);

    sf::Text labelText(font);
    labelText.setCharacterSize(18);
    int legendX = 30, legendY = 30;
    currentAngle = 0.0f;

    for (int i = 0; i < sampleSize; i++) {
        sf::Color color = colorForSlice(i, sampleSize);  
        float sweep    = (values[i] / total) * 2.0f * M_PI;
        float midAngle = currentAngle + sweep * 0.5f;
        int lx = cx + (int)(std::cos(midAngle) * r * 0.65f);
        int ly = cy + (int)(std::sin(midAngle) * r * 0.65f);

        char buf[32];
        snprintf(buf, sizeof(buf), "%.1f%%", (values[i] / total) * 100.0f);
        labelText.setFillColor(sf::Color::Black);
        labelText.setString(buf);
        labelText.setPosition({(float)lx - 12, (float)ly - 6});
        rt.draw(labelText);

        draw_rect(canvas, legendX, legendY + i * 22, 14, 14, color, true);
        labelText.setFillColor(sf::Color::Black);
        labelText.setString(labels[i] + " (" + std::to_string(values[i]) + ")");
        labelText.setPosition({(float)legendX + 18, (float)legendY + i * 22});
        rt.draw(labelText);

        currentAngle += sweep;
    }
    rt.display();
}