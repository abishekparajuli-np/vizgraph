#include "utils.hpp"
#include <algorithm>
#include <stack>
#include <cmath>

sf::Font font("font1.ttf");

// Bresenham line algorithm
void draw_line(sf::Image& canvas, int x0, int y0, int x1, int y1, sf::Color color) {
    int dx = std::abs(x1 - x0), dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1, sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;
    while (true) {
        if (x0 >= 0 && y0 >= 0 && x0 < (int)canvas.getSize().x && y0 < (int)canvas.getSize().y)
            canvas.setPixel({(unsigned)x0, (unsigned)y0}, color);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 <  dx) { err += dx; y0 += sy; }
    }
}

// Mid-point circle algorithm
void draw_circle(sf::Image& canvas, int cx, int cy, int r, sf::Color color, bool fill) {
    int x = 0, y = r, d = 1 - r;
    while (x <= y) {
        if (fill) {
            draw_line(canvas, cx-x, cy+y, cx+x, cy+y, color);
            draw_line(canvas, cx-x, cy-y, cx+x, cy-y, color);
            draw_line(canvas, cx-y, cy+x, cx+y, cy+x, color);
            draw_line(canvas, cx-y, cy-x, cx+y, cy-x, color);
        } else {
            auto sp = [&](int px, int py) {
                if (px >= 0 && py >= 0 && px < (int)canvas.getSize().x && py < (int)canvas.getSize().y)
                    canvas.setPixel({(unsigned)px, (unsigned)py}, color);
            };
            sp(cx+x,cy+y); sp(cx-x,cy+y); sp(cx+x,cy-y); sp(cx-x,cy-y);
            sp(cx+y,cy+x); sp(cx-y,cy+x); sp(cx+y,cy-x); sp(cx-y,cy-x);
        }
        if (d < 0) d += 2*x + 3;
        else     { d += 2*(x-y) + 5; y--; }
        x++;
    }
}

//Scan-line rectangle fill
void draw_rect(sf::Image& canvas, int x, int y, int w, int h, sf::Color color, bool fill) {
    if (fill) {
        for (int row = y; row <= y + h; row++) draw_line(canvas, x, row, x+w, row, color);
    } else {
        draw_line(canvas, x,   y,   x+w, y,   color);
        draw_line(canvas, x+w, y,   x+w, y+h, color);
        draw_line(canvas, x,   y+h, x+w, y+h, color);
        draw_line(canvas, x,   y,   x,   y+h, color);
    }
}

// Scan-line polygon fill: intersect each scanline with edges, fill between pairs
void scanline_fill_polygon(sf::Image& canvas, const std::vector<sf::Vector2f>& verts, sf::Color color) {
    if (verts.size() < 3) return;
    int imgW = (int)canvas.getSize().x, imgH = (int)canvas.getSize().y, n = (int)verts.size();
    float yMinF = verts[0].y, yMaxF = verts[0].y;
    for (auto& v : verts) { yMinF = std::min(yMinF, v.y); yMaxF = std::max(yMaxF, v.y); }
    int yMin = std::max(0, (int)std::ceil(yMinF));
    int yMax = std::min(imgH - 1, (int)std::floor(yMaxF));

    for (int y = yMin; y <= yMax; y++) {
        float fy = (float)y + 0.5f;
        std::vector<float> xs;
        for (int i = 0; i < n; i++) {
            const sf::Vector2f& a = verts[i], &b = verts[(i+1) % n];
            if (a.y == b.y) continue;
            float loY = std::min(a.y, b.y), hiY = std::max(a.y, b.y);
            if (fy < loY || fy >= hiY) continue;
            float t = (fy - a.y) / (b.y - a.y);
            xs.push_back(a.x + t * (b.x - a.x));
        }
        std::sort(xs.begin(), xs.end());
        for (size_t k = 0; k + 1 < xs.size(); k += 2) {
            int x0 = std::max(0, (int)std::ceil(xs[k]));
            int x1 = std::min(imgW - 1, (int)std::floor(xs[k+1]));
            if (x0 <= x1) draw_line(canvas, x0, y, x1, y, color);
        }
    }
}

// Flood-fill: span-based iterative 4-connected fill matching target_color
void flood_fill(sf::Image& canvas, int x, int y, sf::Color fill_color, sf::Color target_color) {
    if (fill_color == target_color) return;
    int W = (int)canvas.getSize().x, H = (int)canvas.getSize().y;
    if (x < 0 || y < 0 || x >= W || y >= H) return;
    if (canvas.getPixel({(unsigned)x, (unsigned)y}) != target_color) return;

    std::stack<sf::Vector2i> stk;
    stk.push({x, y});
    while (!stk.empty()) {
        auto [px, py] = stk.top(); stk.pop();
        if (px < 0 || py < 0 || px >= W || py >= H) continue;
        if (canvas.getPixel({(unsigned)px, (unsigned)py}) != target_color) continue;
        int left = px, right = px;
        while (left  > 0     && canvas.getPixel({(unsigned)(left-1),  (unsigned)py}) == target_color) left--;
        while (right < W - 1 && canvas.getPixel({(unsigned)(right+1), (unsigned)py}) == target_color) right++;
        draw_line(canvas, left, py, right, py, fill_color);
        for (int ix = left; ix <= right; ix++) {
            if (py > 0   && canvas.getPixel({(unsigned)ix, (unsigned)(py-1)}) == target_color) stk.push({ix, py-1});
            if (py < H-1 && canvas.getPixel({(unsigned)ix, (unsigned)(py+1)}) == target_color) stk.push({ix, py+1});
        }
    }
}

//  Boundary-fill: span-based iterative fill stopping at boundary_color
void boundary_fill(sf::Image& canvas, int x, int y, sf::Color fill_color, sf::Color boundary_color) {
    int W = (int)canvas.getSize().x, H = (int)canvas.getSize().y;
    if (x < 0 || y < 0 || x >= W || y >= H) return;
    sf::Color cur = canvas.getPixel({(unsigned)x, (unsigned)y});
    if (cur == boundary_color || cur == fill_color) return;

    std::stack<sf::Vector2i> stk;
    stk.push({x, y});
    while (!stk.empty()) {
        auto [px, py] = stk.top(); stk.pop();
        if (px < 0 || py < 0 || px >= W || py >= H) continue;
        sf::Color c = canvas.getPixel({(unsigned)px, (unsigned)py});
        if (c == boundary_color || c == fill_color) continue;
        int left = px, right = px;
        while (left > 0) {
            sf::Color cl = canvas.getPixel({(unsigned)(left-1), (unsigned)py});
            if (cl == boundary_color || cl == fill_color) break;
            left--;
        }
        while (right < W - 1) {
            sf::Color cr = canvas.getPixel({(unsigned)(right+1), (unsigned)py});
            if (cr == boundary_color || cr == fill_color) break;
            right++;
        }
        draw_line(canvas, left, py, right, py, fill_color);
        for (int ix = left; ix <= right; ix++) {
            if (py > 0) {
                sf::Color cu = canvas.getPixel({(unsigned)ix, (unsigned)(py-1)});
                if (cu != boundary_color && cu != fill_color) stk.push({ix, py-1});
            }
            if (py < H - 1) {
                sf::Color cd = canvas.getPixel({(unsigned)ix, (unsigned)(py+1)});
                if (cd != boundary_color && cd != fill_color) stk.push({ix, py+1});
            }
        }
    }
}
