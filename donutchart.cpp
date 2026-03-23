#include "donutchart.hpp"
#include <cmath>
#include <cstdio>
#include <vector>

#define M_PI 3.14159265358979323846f
static const int ARC_STEPS = 120;

static sf::Color sliceColors[] = {
    sf::Color(220, 80,  80), sf::Color(80,  150, 220), sf::Color(80,  200, 100),
    sf::Color(240, 180, 50), sf::Color(180, 80,  200), sf::Color(240, 130, 50),
    sf::Color(80,  210, 210), sf::Color(200, 100, 140),
};
static const int NUM_COLORS = sizeof(sliceColors) / sizeof(sf::Color);

// fills outer wedge via §2.4 scanline, punches hole via §2.6 boundary-fill, restores ring via §2.7 flood-fill
void draw_donut_sector(sf::Image& canvas, int cx, int cy, int outerR, int innerR,
                       float startAngle, float endAngle, sf::Color color) {
    std::vector<sf::Vector2f> outerPoly;
    outerPoly.push_back({(float)cx, (float)cy});
    for (int s = 0; s <= ARC_STEPS; s++) {
        float angle = startAngle + ((float)s / ARC_STEPS) * (endAngle - startAngle);
        outerPoly.push_back({cx + std::cos(angle) * outerR, cy + std::sin(angle) * outerR});
    }
    scanline_fill_polygon(canvas, outerPoly, color); 

    draw_circle(canvas, cx, cy, innerR, sf::Color::White, false); 
    boundary_fill(canvas, cx, cy, sf::Color::White, sf::Color::White); 

    float mid = startAngle + (endAngle - startAngle) * 0.5f;
    float ringR = (outerR + innerR) * 0.5f;
    int seedX = cx + (int)(std::cos(mid) * ringR);
    int seedY = cy + (int)(std::sin(mid) * ringR);
    if (canvas.getPixel({(unsigned)seedX, (unsigned)seedY}) == sf::Color::White)
        flood_fill(canvas, seedX, seedY, color, sf::Color::White); 

    float sx1 = cx + std::cos(startAngle) * innerR, sy1 = cy + std::sin(startAngle) * innerR;
    float sx2 = cx + std::cos(startAngle) * outerR, sy2 = cy + std::sin(startAngle) * outerR;
    draw_line(canvas, (int)sx1, (int)sy1, (int)sx2, (int)sy2, sf::Color::White); 
}

void donutchart(sf::Image& canvas, sf::RenderTexture& rt, int* values, std::string* labels, int sampleSize) {
    int cx = 400, cy = 300, outerR = 220, innerR = 110;
    float total = 0;
    for (int i = 0; i < sampleSize; i++) total += values[i];

    float currentAngle = 0.0f;
    for (int i = 0; i < sampleSize; i++) {
        float sweep = (values[i] / total) * 2.0f * M_PI;
        draw_donut_sector(canvas, cx, cy, outerR, innerR, currentAngle, currentAngle + sweep, sliceColors[i % NUM_COLORS]);
        currentAngle += sweep;
    }

    draw_circle(canvas, cx, cy, outerR, sf::Color::White, false); 
    draw_circle(canvas, cx, cy, innerR, sf::Color::White, false); 
    draw_circle(canvas, cx, cy, innerR - 1, sf::Color::White, true);

    sf::Text centerText(font);
    centerText.setFillColor(sf::Color::Black);
    centerText.setCharacterSize(16);
    centerText.setString("Total\n" + std::to_string((int)total));
    centerText.setPosition({(float)cx - 26, (float)cy - 16});
    rt.draw(centerText);

    sf::Text labelText(font);
    labelText.setCharacterSize(18);
    int legendX = 30, legendY = 30;
    currentAngle = 0.0f;

    for (int i = 0; i < sampleSize; i++) {
        float sweep    = (values[i] / total) * 2.0f * M_PI;
        float midAngle = currentAngle + sweep * 0.5f;
        float labelR   = (outerR + innerR) * 0.5f;
        int lx = cx + (int)(std::cos(midAngle) * labelR);
        int ly = cy + (int)(std::sin(midAngle) * labelR);

        char buf[32];
        snprintf(buf, sizeof(buf), "%.1f%%", (values[i] / total) * 100.0f);
        labelText.setFillColor(sf::Color::Black);
        labelText.setString(buf);
        labelText.setPosition({(float)lx - 12, (float)ly - 6});
        rt.draw(labelText);

        draw_rect(canvas, legendX, legendY + i * 22, 14, 14, sliceColors[i % NUM_COLORS], true);
        labelText.setFillColor(sf::Color::Black);
        labelText.setString(labels[i] + " (" + std::to_string(values[i]) + ")");
        labelText.setPosition({(float)legendX + 18, (float)legendY + i * 22});
        rt.draw(labelText);

        currentAngle += sweep;
    }
    rt.display();
}
