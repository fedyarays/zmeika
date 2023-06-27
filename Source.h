#ifndef PROEKT2_SOURCE_H
#define PROEKT2_SOURCE_H
#endif //PROEKT2_SOURCE_H
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <comdef.h>
#include "string"
const int FIELD_CELL_TYPE_NONE = 0;
const int FIELD_CELL_TYPE_APPLE = -1;
const int FIELD_CELL_TYPE_WALL =  -2;
const int FIELD_CELL_TYPE_HELP =  -3;
const int SNAKE_MOVE_UP = 0;
const int SNAKE_MOVE_RIGHT = 1;
const int SNAKE_MOVE_DOWN = 2;
const int SNAKE_MOVE_LEFT = 3;
///объявляем константы, такие как размер поля(в клетках), размер клетки(в пикселях), размер окна, длина змеи вначале
///кол-во клеток по x
const int field_size_x = 55;
///кол-во клеток по y
const int field_size_y = 30;
///размер клеток в пикселях
const int cell_size = 32;

///определяем размеры окна
const int field_widht = field_size_x * cell_size;
const int field_height = field_size_y * cell_size;
struct snake {

///длина змеи
    int snake_len = 4;
///двумерный массив, в котором хранятся координаты
    int field[field_size_y][field_size_x];
///стартовая позиция змейки
    int snake_pos_x = field_size_x / 2;
    int snake_pos_y = field_size_y / 2;
///указание движения змейки от пользователя
    int input_direction = SNAKE_MOVE_RIGHT;
///текущее направление змейки
    int current_direction = SNAKE_MOVE_RIGHT;
///переменная определяющая, закончилась ли игра
    bool gameover_status = false;
///переменная типа bool, используется в level()
    bool level_check = false;
///начальное значение сложности(time sleep)
    int hard_level = 100;
///счетчик очков
    int score = 0;
    sf::Texture body_texture;
    sf::Texture wall_texture;
    sf::Texture help_texture;
    sf::Texture apple_texture;
    sf::Texture none_texture;
    snake();
};

extern void level(snake &snake);
extern int randomcell(snake &snake);
extern void apple_spawn(snake &snake);
extern void help_spawn(snake &snake);
extern void clear_field(snake &snake);
extern void draw_field(sf::RenderWindow &window, snake &snake);
extern void movement(snake &snake);
extern void keyboard(snake &snake);