#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <comdef.h>
#include "string"
#include "Source.h"
#include <chrono>

#define clock std::chrono::high_resolution_clock

int main()
{

    snake snake;
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(field_widht, field_height), "Snake", sf::Style::Close);

    clear_field(snake);

    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        movement(snake);
        if (snake.gameover_status) {
            sf::sleep(sf::seconds(1));
            window.close();
        }
        window.clear(sf::Color(120, 165 , 210));

        //отрисовываем поле
        draw_field(window, snake);
        window.display();
        level(snake);

        //Считываем кнопки во время ожидания
        clock::time_point start_timer = clock::now();
        std::chrono::duration<double, std::milli> time_span;
        do {
            time_span = clock::now() - start_timer;
            keyboard(snake);
        } while (time_span.count() < snake.hard_level);
    }
    return 0;
}