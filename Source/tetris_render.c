//RENDER

#include "tetris.h"
#include "GLCD/GLCD.h"
#include <stdio.h>

uint16_t get_tetromino_color(uint8_t type) //based on the image in the pdf
{
	switch(type){
		case TETROMINO_I:
			return Cyan;
		case TETROMINO_O:
			return Yellow;    
    case TETROMINO_T: 
			return Magenta;  
    case TETROMINO_J: 
			return Blue;     
    case TETROMINO_L: 
			return RGB565CONVERT(255, 165, 0); //orange
    case TETROMINO_S: 
			return Green; 
    case TETROMINO_Z: 
			return Red;
    default: 
			return White;
	}
}

void draw_field_border(void)
{
	uint16_t field_width=FIELD_WIDTH*SQUARE_SIZE, field_height = FIELD_HEIGHT * SQUARE_SIZE, border_color = White;
	
	LCD_DrawLine(FIELD_OFFSET_X-1, FIELD_OFFSET_Y-1, 
	             FIELD_OFFSET_X-1, FIELD_OFFSET_Y+field_height, border_color);

	LCD_DrawLine(FIELD_OFFSET_X-1, FIELD_OFFSET_Y-1, 
	             FIELD_OFFSET_X+field_width, FIELD_OFFSET_Y-1, border_color);
	
	LCD_DrawLine(FIELD_OFFSET_X+field_width, FIELD_OFFSET_Y-1, 
	             FIELD_OFFSET_X+field_width, FIELD_OFFSET_Y+field_height, border_color);
	
	LCD_DrawLine(FIELD_OFFSET_X-1, FIELD_OFFSET_Y+field_height, 
	             FIELD_OFFSET_X+field_width, FIELD_OFFSET_Y+field_height, border_color);
}

void draw_square(uint16_t x, uint16_t y, uint16_t size, uint16_t color)
{
	uint16_t i, j;
	for (j=0; j<size; j++){
		for (i=0; i<size; i++){
			LCD_SetPoint(x+i, y+j, color);
     }
   }
}

void draw_field(void)
{
	int i, j;
	uint16_t pixel_x, pixel_y, color;
	for(i=0; i<FIELD_WIDTH; i++){
		for(j=0; j<FIELD_HEIGHT; j++){
			pixel_x=FIELD_OFFSET_X + i*SQUARE_SIZE;
			pixel_y=FIELD_OFFSET_Y + j*SQUARE_SIZE;
			
			if(game.field[j][i]!=0){ //presence of tetromino
				color=get_tetromino_color(game.field[j][i]-1);
			}
			else{
				color=Black;
			}
			
			draw_square(pixel_x, pixel_y, SQUARE_SIZE, color);
		}
	}
}

void draw_falling_tetromino(void)
{
	int i, j, global_x, global_y;
	uint16_t pixel_x, pixel_y, color;
	for(i=0; i<4; i++){
		for(j=0; j<4; j++){
			if(shapes[game.current_piece.type][game.current_piece.rotation][i][j]){
				global_x=game.current_piece.x+j;
				global_y=game.current_piece.y+i;
				if(global_x>=0 && global_x<FIELD_WIDTH && global_y>=0 && global_y<FIELD_HEIGHT){
					pixel_x=FIELD_OFFSET_X + global_x*SQUARE_SIZE;
					pixel_y=FIELD_OFFSET_Y + global_y*SQUARE_SIZE;
					color = get_tetromino_color(game.current_piece.type);
					
					draw_square(pixel_x, pixel_y, SQUARE_SIZE, color);
				}
			}
		}
	}
}

void draw_tetromino_move(int old_x, int old_y, int new_x, int new_y)
{
	int i, j, global_x, global_y;
  uint16_t pixel_x, pixel_y;
	//delete old position by drawing black squares
  for(i=0; i<4; i++){
		for(j=0; j<4; j++){
			if(shapes[game.current_piece.type][game.current_piece.rotation][i][j]){
				global_x = old_x + j;
        global_y = old_y + i;
        if(global_x>=0 && global_x<FIELD_WIDTH && global_y>=0 && global_y<FIELD_HEIGHT){
					pixel_x = FIELD_OFFSET_X + global_x*SQUARE_SIZE;
          pixel_y = FIELD_OFFSET_Y + global_y*SQUARE_SIZE;
          draw_square(pixel_x, pixel_y, SQUARE_SIZE, Black);
        }
			}
		}
	}
    
	//draw tetramino in the new position
  for(i=0; i<4; i++){
		for(j=0; j<4; j++){
			if(shapes[game.current_piece.type][game.current_piece.rotation][i][j]){
				global_x = new_x + j;
        global_y = new_y + i;
        if(global_x>=0 && global_x<FIELD_WIDTH && global_y>=0 && global_y<FIELD_HEIGHT){
					pixel_x = FIELD_OFFSET_X + global_x*SQUARE_SIZE;
          pixel_y = FIELD_OFFSET_Y + global_y*SQUARE_SIZE;
          uint16_t color = get_tetromino_color(game.current_piece.type);
          draw_square(pixel_x, pixel_y, SQUARE_SIZE, color);
        }
			}
		}
	}
}

void draw_data(void)
{
	char buffer[32];
	
	//score
	GUI_Text(DATA_OFFSET_X, 20, (uint8_t *)"SCORE:", White, Black);
	sprintf(buffer, "%d     ", game.score); //spaces erase additional numbers from previous score
	GUI_Text(DATA_OFFSET_X, 40, (uint8_t *)buffer, White, Black);
	
	//highest score
	GUI_Text(DATA_OFFSET_X, 80, (uint8_t *)"HIGHEST SCORE:", White, Black);
	sprintf(buffer, "%d     ", game.high_score); 
	GUI_Text(DATA_OFFSET_X, 100, (uint8_t *)buffer, White, Black);
	
	//lines cleared
	GUI_Text(DATA_OFFSET_X, 140, (uint8_t *)"LINES CLEARED:", White, Black);
	sprintf(buffer, "%d     ", game.lines_cleared); 
	GUI_Text(DATA_OFFSET_X, 160, (uint8_t *)buffer, White, Black);
}

void draw_game(void)
{
	draw_field_border();
	draw_field();
	
	if(game.state==PLAY){
		draw_falling_tetromino();
	}
	
	draw_data();
}