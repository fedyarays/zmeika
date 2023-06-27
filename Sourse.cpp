#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <comdef.h>
#include "string"
#include "Source.h"

/**
 * функция генерации рандомного числа(была позаимствована)
 * @param snake
 * @return
 */
int randomcell(snake &snake) {
    int cell_count = 0;
    //пересчитываем свободные клетки, в которых ничего нет, то есть FIELD_CELL_TYPE_NONE
    for (int j = 0; j < field_size_y; j++) {
        for (int i = 0; i < field_size_x; i++) {
            if (snake.field[j][i] == FIELD_CELL_TYPE_NONE) {
                cell_count++;
            }
        }
    }
    //выбираем номер пустой клетки, считая от левого верхнего угла
    //путем нахождения остатка от деления рандомного числа на выбранное
    int empty_number = rand() % cell_count;
    int cell_index = 0;
    for (int j = 0; j < field_size_y; j++) {
        for (int i = 0; i < field_size_x; i++) {
            if (snake.field[j][i] == FIELD_CELL_TYPE_NONE) {
                if (cell_index == empty_number) {
                    return j * field_size_x + i;
                }
                cell_index++;
            }
        }
    }

    snake.gameover_status = true; //Нет пустых клеток
    return -1;
}
/**
 * Функция спавнит в выбранную клетку яблоко
 * @param snake
 */
void apple_spawn(snake &snake){
    int apple_position = randomcell(snake);
    if (apple_position != -1)  //Проверка на ошибку randomcell
        snake.field[apple_position / field_size_x][apple_position % field_size_x] = FIELD_CELL_TYPE_APPLE;
}
/**
 * функция спавнит звездочку,которая уменьшает длину змейки
 * @param snake
 */
void help_spawn(snake &snake) {
    int help_position = randomcell(snake);
    if (help_position != -1)
        snake.field[help_position / field_size_x][help_position % field_size_x] = FIELD_CELL_TYPE_HELP;
}
/**
 * функция перерисовывает поле с заданным временем sleep, за которое отвечает функция level
 * @param snake
 */
void clear_field(snake &snake)
{
    for (int j = 0; j < field_size_y; j++) {
        for (int i = 0; i < field_size_x; i++) {
            snake.field[j][i] = FIELD_CELL_TYPE_NONE;
        }
    }
    for (int i = 0; i < snake.snake_len; i ++){
        snake.field[snake.snake_pos_y][snake.snake_pos_x - i] = snake.snake_len - i;
    }
    apple_spawn(snake);
    help_spawn(snake);
}

snake::snake(){
    body_texture.loadFromFile("image/snake.png");
    wall_texture.loadFromFile("image/wall.png");
    help_texture.loadFromFile("image/cursor.png");
    apple_texture.loadFromFile("image/apple.png");
    none_texture.loadFromFile("image/none.png");
}

/**
 * функция определяет все методы перерисовки поля
 * @param window
 * @param snake
 */
void draw_field(sf::RenderWindow &window, snake &snake) {

    sf::Sprite body;
    body.setTexture(snake.body_texture);

    sf::Sprite wall;
    wall.setTexture(snake.wall_texture);

    sf::Sprite help;
    help.setTexture(snake.help_texture);

    sf::Sprite apple;
    apple.setTexture(snake.apple_texture);

    sf::Sprite none;
    none.setTexture(snake.none_texture);

    for (int x = 12; x < field_size_x - 12; x++) {
        snake.field[0][x] = FIELD_CELL_TYPE_WALL;
        snake.field[field_size_y - 1][x] = FIELD_CELL_TYPE_WALL;
    }

    for (int j = 0; j < field_size_y; j++) {
        for (int i = 0; i < field_size_x; i++) {
            switch (snake.field[j][i]) {
                case FIELD_CELL_TYPE_NONE:
                    none.setPosition(i * cell_size, j * cell_size);
                    window.draw(none);
                    break;
                case FIELD_CELL_TYPE_WALL:
                    wall.setPosition(i * cell_size, j * cell_size);
                    window.draw(wall);
                    break;
                case FIELD_CELL_TYPE_APPLE:
                    apple.setPosition(i * cell_size, j * cell_size);
                    window.draw(apple);
                    break;
                case FIELD_CELL_TYPE_HELP:
                    help.setPosition(i * cell_size, j * cell_size);
                    window.draw(help);
                    break;
                default:
                    body.setPosition(i * cell_size, j * cell_size);
                    window.draw(body);
                    break;
            }
        }
    }
    sf::Font font;
    font.loadFromFile("fonts/font.otf");
    sf::Text score;
    score.setFont(font);
    score.setCharacterSize(30);
    score.setString("Score: " + std::to_string(snake.score));
    score.setPosition(field_widht / 15, 1);
    window.draw(score);
}
/**
 * Функция movement() описывает движение змейки и изменение ее длины
 * snake_pos_y и snake_pos_x -- координаты головы змейки
 * input_direction -- напрвление змейки
 */
void movement(snake &snake)
{
    switch (snake.input_direction) {
        case SNAKE_MOVE_UP:
            snake.snake_pos_y--;
            if (snake.snake_pos_y < FIELD_CELL_TYPE_NONE) {
                snake.snake_pos_y = field_size_y - 1;
            }
            break;
        case SNAKE_MOVE_RIGHT:
            snake.snake_pos_x++;
            if (snake.snake_pos_x > field_size_x - 1) {
                snake.snake_pos_x = FIELD_CELL_TYPE_NONE;
            }

            break;
        case SNAKE_MOVE_DOWN:
            snake.snake_pos_y++;
            if (snake.snake_pos_y > field_size_y - 1) {
                snake.snake_pos_y = FIELD_CELL_TYPE_NONE;
            }
            break;
        case SNAKE_MOVE_LEFT:
            snake.snake_pos_x--;
            if (snake.snake_pos_x < FIELD_CELL_TYPE_NONE) {
                snake.snake_pos_x = field_size_x - 1;
            }
            break;
    }
    snake.current_direction = snake.input_direction;

    //змейка попадает в одну из ячеек поля и получает определенные изменения в длине или скорости движения
    if (snake.field[snake.snake_pos_y][snake.snake_pos_x] != FIELD_CELL_TYPE_NONE) {
        switch (snake.field[snake.snake_pos_y][snake.snake_pos_x]) {
            case FIELD_CELL_TYPE_APPLE:
                snake.snake_len++;
                snake.score++;
                apple_spawn(snake);
                snake.level_check = true;
                break;
            case FIELD_CELL_TYPE_HELP:
                if(snake.snake_len > 1)
                    snake.snake_len--;
                snake.hard_level += 7;
                help_spawn(snake);
                snake.level_check = true;
                break;
            default:
                snake.gameover_status = true;
        }
    }

    snake.field[snake.snake_pos_y][snake.snake_pos_x] = snake.snake_len + 1;
    //удаление хвоста змейки при ее движении на одну ячейку в выбранном направлении
    for (int j = 0; j < field_size_y; j++) {
        for (int i = 0; i < field_size_x; i++) {
            if(snake.field[j][i] > FIELD_CELL_TYPE_NONE){
                snake.field[j][i]--;
            }

        }
    }
}
/**
 * Функция keyboard() считавыет нажатия клавиш(стрелочек) и меняет направление змейки
 *  input_direction - переменная, определяющая начальное направление змейки(в моем случае вправо)
 *  gameover_status - переменная, которая определяет закончилась ли игра или нет
 */
void keyboard(snake &snake)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
        if (snake.current_direction != SNAKE_MOVE_DOWN) { //Замена input на current для устранения бага двойного разворота на одной клетке
            snake.input_direction = SNAKE_MOVE_UP;
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
        if (snake.current_direction != SNAKE_MOVE_UP) {
            snake.input_direction = SNAKE_MOVE_DOWN;
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
        if (snake.current_direction != SNAKE_MOVE_RIGHT) {
            snake.input_direction = SNAKE_MOVE_LEFT;
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
        if (snake.current_direction != SNAKE_MOVE_LEFT) {
            snake.input_direction = SNAKE_MOVE_RIGHT;
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        snake.gameover_status = true;
    }
}
/**
 * Функция level() определяет, с какой скоростью будет передвигаться змейка
 * snake_len --длина змейки
 * level_check -- переменная, которая проверяет, что змейка прибавила в длине, переменная требуется для того
 * чтобы скорость змейки не меняласть при неизменной длине
 * hard_level -- переменная, определяющаяя задержку в отрисовывании поля
 * Вся суть функции в том, что с ростом длины змейки уменьшается время сна отрисовки поля
 */
void level(snake &snake)
{
    if (snake.level_check) {
        switch (snake.snake_len % 4) {
            case 0://если длина 4, 8, 12, 16, ....
                snake.hard_level -= 3;
                snake.level_check = false;
            case 2://если длина 6, 10, 14, 18, ....
                snake.hard_level -= 1;
                snake.level_check = false;
        }
    }
}

