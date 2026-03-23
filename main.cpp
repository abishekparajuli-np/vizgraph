#include<SFML/Graphics.hpp>
#include"linegraph.hpp"
#include"piechart.hpp"
#include"donutchart.hpp"
#include "bargraph.hpp"
#include "histogram.hpp"
#include "scatterplot.hpp"
#include <cstdlib>
#include <ctime>
#include<string>
#include<cstdio>

int main(){
    sf::RenderWindow window(sf::VideoMode({800,600}),"Vizgraph");
    sf::Texture texture({800,600});
    sf::Sprite sprite(texture);
    sf::RenderTexture tx({800,600});

    // --- linegraph data ---
    int x[]={1,2,3,4,5,6,7,8,9};
    int y[]={1,4,9,16,25,36,49,64,81};

    // --- scatterplot data ---
    srand(time(NULL));
    int Sx[60], Sy[60];
    for (int i = 0; i < 60; i++) {
        Sx[i] = rand() % 100;
        Sy[i] = rand() % 100;
    }

    // --- pie / donut data ---
    // Wide spread: one dominant slice, two mid, two tiny
    int values[] = {42, 28, 15, 10, 5};
    std::string labels[] = {"A", "B", "C", "D", "E"};
    int sampleSize = sizeof(values) / sizeof(int);

    // --- histogram data ---
    // Perfect symmetric bell curve
int counts[] = {210, 980, 154, 105, 580, 310, 150};
std::string binLabels[] = {
    "13-17",
    "18-24",
    "25-34",
    "35-44",
    "45-54",
    "55-64",
    "65+"
};
int histSize = sizeof(counts) / sizeof(int);

    int currentChart = 0;

    // helper: full clear and redraw
    auto redraw = [&](){
        sf::Image canvas({800u, 600u}, sf::Color::White);
        tx.clear(sf::Color::Transparent);

        printf("drawing chart %d\n", currentChart);

        if      (currentChart == 0) linegraph  (canvas, tx, x, y, sizeof(x)/sizeof(int));
        else if (currentChart == 1) bargraph   (canvas, tx, values, labels, sampleSize);
        else if (currentChart == 2) piechart   (canvas, tx, values, labels, sampleSize);
        else if (currentChart == 3) scatterplot(canvas, tx, Sx, Sy, sizeof(Sx)/sizeof(int));
        else if (currentChart == 4) donutchart (canvas, tx, values, labels, sampleSize);
        else if (currentChart == 5) histogram  (canvas, tx, counts, binLabels, histSize);

        texture.update(canvas);
    };

    redraw();

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* key = event->getIf<sf::Event::KeyPressed>()){
                printf("key pressed: %d\n", (int)key->code);
                if      (key->code == sf::Keyboard::Key::Num1){ currentChart = 0; redraw(); }
                else if (key->code == sf::Keyboard::Key::Num2){ currentChart = 1; redraw(); }
                else if (key->code == sf::Keyboard::Key::Num3){ currentChart = 2; redraw(); }
                else if (key->code == sf::Keyboard::Key::Num4){ currentChart = 3; redraw(); }
                else if (key->code == sf::Keyboard::Key::Num5){ currentChart = 4; redraw(); }
                else if (key->code == sf::Keyboard::Key::Num6){ currentChart = 5; redraw(); }
            }
        }

        window.clear(sf::Color::White);
        window.draw(sprite);
        window.draw(sf::Sprite(tx.getTexture()));
        window.display();
    }
}