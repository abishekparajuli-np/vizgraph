#include<SFML/Graphics.hpp>
#include"linegraph.hpp"


int main(){
    sf::RenderWindow window(sf::VideoMode({800,600}),"Vizgraph");
    sf::Image canvas({800,600},sf::Color::White);
    sf::Texture texture({800,600});
    sf::Sprite sprite(texture);
    sf::RenderTexture tx({800,600});
    
    tx.clear(sf::Color::Transparent);
    
    int x[]={0,1,2,3,4,5,6,7,8,9,10};
    int y[]={0,1,4,9,16,25,36,49,64,81,100};


    
    


    linegraph(canvas,tx,x,y,sizeof(x)/sizeof(int));
    texture.update(canvas);  // update() instead of loadFromImage in SFML3

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        window.clear(sf::Color::White);
        window.draw(sprite);
        window.draw(sf::Sprite(tx.getTexture()));
        window.display();
    }

}