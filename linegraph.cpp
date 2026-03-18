#include "linegraph.hpp"
#include<cmath>
//#include<stdio.h>
#include<cstdio>
void linegraph(sf::Image& canvas,sf::RenderTexture& rt, int* x, int* y,int sampleSize){
// Setting up the screen
draw_line(canvas,50,50,50,550,sf::Color::Black);//Yaxis line
draw_line(canvas,50,550,750,550,sf::Color::Black);//xaxis line

sortforlinegraph(x,y,sampleSize);

// various shenanigans to scale accordingly
float largestX = x[sampleSize-1];
float largestY=y[sampleSize-1];
for(int i =0;i<sampleSize;i++){
    if (y[i]>largestY){
        largestY=y[i];
    }
}

int tensX=1,tensY=1;
float tempX=largestX,tempY=largestY;
while(tempX>1){
    tempX/=10;
    tensX*=10;
}
while(tempY>1){
    tempY/=10;
    tensY*=10;
}

// plotting the points
float px,py;
for ( int i =0;i<sampleSize;i++){
    px = ((float)x[i]/tensX) *700+50;
    py = 600 - ((float)y[i]/tensY) *500-50;
    printf("%d,%d,%f,%f\n",tensX,tensY,px,600-py);
    draw_circle(canvas, px, py, 3, sf::Color::Blue, true);
}
// Connecting the dots
for (int i =0;i<sampleSize-1;i++){
    draw_line(canvas,(float)x[i]/tensX*700+50,600-(float)y[i]/tensY*500-50,(float)x[i+1]/tensX*700+50,600-(float)y[i+1]/tensY*500-50,sf::Color::Blue);
}
// making labels 
sf::Text labelText(font);
labelText.setFillColor(sf::Color::Black);
labelText.setCharacterSize(10);
for (int i =0;i<=10;i++){
    labelText.setString(std::to_string(tensY/10*i));
    labelText.setPosition({30,550-50*i});
    rt.draw(labelText);
}
for (int i =0;i<=10;i++){
    labelText.setString(std::to_string(tensX/10*i));
    labelText.setPosition({50+70*i,565});
    rt.draw(labelText);
}
rt.display();

}




void linegraph1(sf::Image& canvas, int* x, int* y,int sampleSize){
// Setting up the screen
draw_line(canvas,50,50,50,550,sf::Color::Black);//Yaxis line
draw_line(canvas,50,550,750,550,sf::Color::Black);//xaxis line

sortforlinegraph(x,y,sampleSize); // sort data ascendingly to plot


// find largest data to scale accordinglt
float largestX = x[sampleSize-1];
float largestY=y[sampleSize-1];

for(int i =0;i<sampleSize;i++){
    if (y[i]>largestY){
        largestY=y[i];
    }
}


// making dots
float px,py;
for ( int i =0;i<sampleSize;i++){
    px = (float)(x[i]/largestX) *700+50;
    py = 600 - (float)(y[i]/largestY) *500-50;
    printf("%f,%f,%f,%f\n",largestX,largestY,px,600-py);
    draw_circle(canvas, px, py, 3, sf::Color::Blue, true);
}

//connecting the dots
for (int i =0;i<sampleSize-1;i++){
    draw_line(canvas,x[i]/largestX*700+50,600-y[i]/largestY*500-50,x[i+1]/largestX*700+50,600-y[i+1]/largestY*500-50,sf::Color::Blue);
}


// making labels 



}
void sortforlinegraph(int* x,int* y,int sampleSize){
int tempx;
int tempy;
for(int i =0;i<sampleSize-1;i++){
    for (int j=0;j<sampleSize-1;j++){
        if (x[j]>x[j+1]){
            tempx = x[j];
            x[j]=x[j+1];
            x[j+1]=tempx;
            tempy = y[j];
            y[j] = y[j+1];
            y[j+1]=tempy;
        }
    }
}

}