#include "vizgraph.hpp"
#include <cstdlib>
#include <ctime>
#include<string>
#include<cstdio>

int main(){
    sf::RenderWindow window(sf::VideoMode({800,600}),"Vizgraph");
    sf::Texture texture({800,600});
    sf::Sprite sprite(texture);
    sf::RenderTexture tx({800,600});

    // linegraph data
    int x[]={1,2,3,4,5,6,7,8,9};
    int y[]={1,4,9,16,25,36,49,64,81};
    // bargraph data
    int barValues[]        = {340, 210, 480, 155, 390, 270};
    std::string barLabels[] = {"Product A", "Product B", "Product C",
                               "Product D", "Product E", "Product F"};
    int barSize = sizeof(barValues) / sizeof(int);

    // piechart data
    int pieValues[]        = {65, 19, 4, 3, 9, 8};
    std::string pieLabels[] = {"Chrome", "Safari", "Edge", "Firefox","Brave", "Other"};
    int pieSize = sizeof(pieValues) / sizeof(int);

    // scatterplot data
      srand(time(NULL));
    int Sx[60], Sy[60];
    for (int i = 0; i < 60; i++) {
        Sx[i] = rand() % 100;
        Sy[i] = rand() % 100;
    }

    // donuntchart data
    int donutValues[]        = {32, 27, 18, 13, 10, 30};
    std::string donutLabels[] = {"Industry", "Transport", "Residential",
                                 "Commercial","Management" ,"Other"};
    int donutSize = sizeof(donutValues) / sizeof(int);

    // histogram data
    int counts[]            = {3, 12, 28, 42, 35, 19, 8, 3};
    std::string binLabels[] = {"0-10", "11-20", "21-30", "31-40",
                               "41-50", "51-60", "61-70", "71-80"};
    int histSize = sizeof(counts) / sizeof(int);

    int currentChart = 0;

    auto redraw = [&](){
        sf::Image canvas({800u, 600u}, sf::Color::White);
        tx.clear(sf::Color::Transparent);

        printf("drawing chart %d\n", currentChart);

        if      (currentChart == 0) linegraph  (canvas, tx, x, y, sizeof(x)/sizeof(int));
        else if (currentChart == 1) bargraph   (canvas, tx, barValues,   barLabels,   barSize);
        else if (currentChart == 2) piechart   (canvas, tx, pieValues,   pieLabels,   pieSize);
        else if (currentChart == 3) scatterplot(canvas, tx, Sx, Sy, sizeof(Sx)/sizeof(int));
        else if (currentChart == 4) donutchart (canvas, tx, donutValues, donutLabels, donutSize);
        else if (currentChart == 5) histogram  (canvas, tx, counts,      binLabels,   histSize);

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