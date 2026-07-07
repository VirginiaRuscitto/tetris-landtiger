#ifndef __TETRIS_H
#define __TETRIS_H

#include <stdint.h>

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20
#define SQUARE_SIZE 12
#define FIELD_OFFSET_X 5
#define FIELD_OFFSET_Y 10
#define DATA_OFFSET_X 130 //12*10 = 120; 120+10=130

#define TETROMINO_I 0
#define TETROMINO_O 1
#define TETROMINO_T 2
#define TETROMINO_J 3
#define TETROMINO_L 4
#define TETROMINO_S 5
#define TETROMINO_Z 6
#define NUM_TETROMINOES 7

typedef enum {
	PAUSE,
  PLAY,
  GAME_OVER,
	START
} State;

typedef struct {
  int8_t x;
  int8_t y;          
  uint8_t type;    
  uint8_t rotation;
} Tetromino;

typedef struct {
	State state;
  uint16_t score;
  uint16_t high_score;
  uint16_t lines_cleared;
  Tetromino current_piece;
  uint8_t field[FIELD_HEIGHT][FIELD_WIDTH];
  uint8_t soft_drop_active; 
} Game;

extern Game game;
extern const uint8_t shapes[7][4][4][4]; //all possible configurations - saved in ROM

void init_game(void);
void start_game(void);
void pause_game(void);
void game_over(void);
int can_place_tetromino(int x, int y, uint8_t type, uint8_t rotation);
void generate_tetromino(void);
void place_tetromino(void);
int clear_lines(void);
void move_down(void);
void move_left(void);
void move_right(void);
void rotate(void);
void hard_drop(void);
void soft_drop_on(void);
void soft_drop_off(void);

uint16_t get_tetromino_color(uint8_t type);
void draw_field_border(void);
void draw_square(uint16_t x, uint16_t y, uint16_t size, uint16_t color);
void draw_field(void);
void draw_falling_tetromino(void);
void draw_tetromino_move(int old_x, int old_y, int new_x, int new_y);
void draw_data(void);
void draw_game(void);

#endif 