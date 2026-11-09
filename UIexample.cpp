//hello
//this is my first attempt at sfml ui
#include <SFML/Graphics.hpp>

int main()
{
    // 1. Create the window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML Window");

    // 2. Run the main loop as long as the window is open
    while (window.isOpen())
    {
        // 3. Check for all window events triggered since the last iteration
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // 4. Clear the screen
        window.clear(sf::Color::Black);

        // 5. Draw your game objects here

        // 6. Display the drawn frame on screen
        window.display();
    }

    return 0;
}
