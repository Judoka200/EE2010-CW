#include <SFML/Graphics.hpp>
int main()
{
    //creates window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML Window");
    //keeps running loop while window open
    while (window.isOpen())
    {
        //Check for all window events triggered since the last iteration
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        //clears screen
        window.clear(sf::Color::Black);
        //draw game objects

        //displays drawn frame
        window.display();
    }
    return 0;
}
