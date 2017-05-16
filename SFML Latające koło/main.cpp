#include <SFML/Graphics.hpp>
#include <cmath>
#include <windows.h>


int main()
{
    int i=0;
    sf::RenderWindow window(sf::VideoMode(400, 400), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(100,100);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        shape.setPosition(50*cos(i/10)+100,50*sin(i/10)+100);
        Sleep(10);
        window.clear();
        window.draw(shape);
        window.display();
        i++;


    }

    return 0;
}
