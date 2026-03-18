#include "utils.hpp"
sf::Font font("font1.ttf");
void draw_line(sf::Image& canvas, int x0, int y0, int x1, int y1, sf::Color color) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;  // step direction x
    int sy = (y0 < y1) ? 1 : -1;  // step direction y
    int err = dx - dy;

    while (true) {
        if (x0 >= 0 && y0 >= 0 && x0 < (int)canvas.getSize().x && y0 < (int)canvas.getSize().y)
            canvas.setPixel({x0, y0}, color);

        if (x0 == x1 && y0 == y1) break;

        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 <  dx) { err += dx; y0 += sy; }
    }
}
void draw_circle(sf::Image& canvas, int cx, int cy, int r, sf::Color color, bool fill) {
    int x = 0, y = r;
    int d = 1 - r;

    while (x <= y) {
        if (fill) {
            // draw horizontal spans instead of just outline points
            draw_line(canvas, cx - x, cy + y, cx + x, cy + y, color);
            draw_line(canvas, cx - x, cy - y, cx + x, cy - y, color);
            draw_line(canvas, cx - y, cy + x, cx + y, cy + x, color);
            draw_line(canvas, cx - y, cy - x, cx + y, cy - x, color);
        } else {
            canvas.setPixel({cx + x, cy + y}, color);
            canvas.setPixel({cx - x, cy + y}, color);
            canvas.setPixel({cx + x, cy - y}, color);
            canvas.setPixel({cx - x, cy - y}, color);
            canvas.setPixel({cx + y, cy + x}, color);
            canvas.setPixel({cx - y, cy + x}, color);
            canvas.setPixel({cx + y, cy - x}, color);
            canvas.setPixel({cx - y, cy - x}, color);
        }

        if (d < 0) {
            d += 2 * x + 3;
        } else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}

void draw_rect(sf::Image& canvas, int x, int y, int w, int h, sf::Color color, bool fill) {
    if (fill) {
        for (int row = y; row <= y + h; row++)
            draw_line(canvas, x, row, x + w, row, color);
    } else {
        draw_line(canvas, x,   y,   x+w, y,   color);  // top
        draw_line(canvas, x+w, y,   x+w, y+h, color);  // right
        draw_line(canvas, x,   y+h, x+w, y+h, color);  // bottom
        draw_line(canvas, x,   y,   x,   y+h, color);  // left
    }
}