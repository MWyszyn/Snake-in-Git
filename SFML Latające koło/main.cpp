#include <SFML/Graphics.hpp>
#include <cmath>
#include <windows.h>


int main()
{
    int i=0,zx,zy,mx,my;
    sf::RenderWindow window(sf::VideoMode(700, 700), "WOLOLOLO");
    sf::CircleShape shape(30.f);
    shape.setFillColor(sf::Color::Green);
    sf::CircleShape slonce (80.f);
    slonce.setFillColor (sf::Color::Yellow);
    slonce.setPosition(120+150,120+150);
    sf::CircleShape moon(10.f);
    moon.setFillColor(sf::Color::White);



    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        zx=250*cos(i*3.1415/1000)+170+150;
        zy=250*sin(i*3.1415/1000)+170+150;
        mx=(65)*cos(i*3.1415*13.53/1000)+zx+20;
        my=(65)*sin(i*3.1415*13.53/1000)+zy+20;
        shape.setPosition(zx,zy);
        moon.setPosition(mx,my);
        Sleep(2);
        window.clear();
        window.draw(slonce);
        window.draw(shape);
        window.draw(moon);
        window.display();
        i++;

    }

    return 0;
}
