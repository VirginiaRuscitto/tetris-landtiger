//LOGIC

#include "tetris.h"
#include "timer/timer.h"
#include "music.h"
#include "GLCD/GLCD.h"
#include <stdlib.h>
#include <stdio.h>

Game game;

void init_game(void)
{
	int i, j;
	game.state=START;
	game.score=0;
	game.high_score=0;
	game.lines_cleared=0;
	game.soft_drop_active=0;
	
	for(i=0; i<FIELD_HEIGHT; i++){
		for(j=0; j<FIELD_WIDTH; j++){
			game.field[i][j]=0;
		}
	}
	LCD_Clear(Black);
	draw_game();
}

void start_game(void){
	if(game.state == PLAY){
		return;
	}
	
	if(game.state==START || game.state==GAME_OVER){
		int i, j;
		game.score=0;
		game.lines_cleared=0;
		disable_timer(0);
		if(game.state==START){
			srand(get_timer_value(0));
		}
		for(i=0; i<FIELD_HEIGHT; i++){
			for(j=0; j<FIELD_WIDTH; j++){
				game.field[i][j]=0;
			}
		}
	}

	reset_timer(0);
	init_timer(0, 0, 0, 3, 0x017D7840); //1sec=0x017D7840 1/4=0x002625A0
	enable_timer(0);
	
	if(game.state==START){
		generate_tetromino();
	}
	game.state=PLAY;
	play_background_music();
	draw_game();	
}

void pause_game(void)
{
	if(game.state != PLAY){
		return;
	}
	game.state=PAUSE;
	disable_timer(0);
	stop_background_music();

	GUI_Text(FIELD_OFFSET_X+40, FIELD_HEIGHT*SQUARE_SIZE/2 + FIELD_OFFSET_Y, (uint8_t *)"PAUSE", Yellow, Black);
}

void game_over(void)
{
	game.state=GAME_OVER;
	disable_timer(0);
	if(game.score>game.high_score){
		game.high_score = game.score;
	}
	stop_background_music();   
    play_sound_effect(SFX_GAME_OVER);

	GUI_Text(FIELD_OFFSET_X+20, FIELD_HEIGHT*SQUARE_SIZE/2 + FIELD_OFFSET_Y, (uint8_t *)"GAME OVER", Yellow, Black);
}

int can_place_tetromino(int x, int y, uint8_t type, uint8_t rotation)
{
	int i, j, global_x, global_y;
	for(i=0; i<4; i++){
		for(j=0; j<4; j++){
			if(shapes[type][rotation][i][j]){ //full square
				global_x = x + j; //global=same coordinate system as the game field
				global_y = y + i;
				
				//check field boundaries (negative y are allowed because they represent the situation where the tetromino is above the field)
				if(global_x<0 || global_x>=FIELD_WIDTH || global_y>=FIELD_HEIGHT){
					return 0;
				}
				
				//check collision
				if(global_y >= 0 && game.field[global_y][global_x]!=0){
					return 0;
				}
			}
		}
	}
	return 1;
}

void generate_tetromino(void)
{
	game.current_piece.type=rand()%NUM_TETROMINOES;
	game.current_piece.rotation=0;
	game.current_piece.x=FIELD_WIDTH/2 - 2; //horizontally centered
	game.current_piece.y=-1;
	
	if(!can_place_tetromino(game.current_piece.x, game.current_piece.y, game.current_piece.type, game.current_piece.rotation)){
		game_over();
	}
}

void place_tetromino(void)
{
	int i, j;
	if(game.soft_drop_active==1){
		game.soft_drop_active=0; //if the tetromino is placed in soft drop mode
	}
	disable_timer(0);
	
	for(i=0; i<4; i++){
		for(j=0; j<4; j++){
			if(shapes[game.current_piece.type][game.current_piece.rotation][i][j]){
				int global_x = game.current_piece.x + j;
				int global_y = game.current_piece.y + i;
				
				if(global_y<0){
					game_over();
					return;
				}
				
				game.field[global_y][global_x] = game.current_piece.type+1; //types start from 0, but 0 is an indicator for an empty cell; therefore tetraminoes type is incremented by 1
			}
		}
	}
	
	
	game.score+=10; 
	draw_game();
	
	int clear = clear_lines();
	if(clear!=0){
		draw_game();
	}
	
	
	reset_timer(0);
	init_timer(0, 0, 0, 3, 0x017D7840); //1sec=0x017D7840 1/4=0x002625A0
	enable_timer(0);
	
	generate_tetromino();
	if(game.state==GAME_OVER){
    	return;
	}
	draw_game();
}

int clear_lines(void){
	int cleared=0, j, i, j_shift;
	
	for(j=FIELD_HEIGHT-1; j>=0; j--){ //starting from bottom line
		int full=1;
		for(i=0; i<FIELD_WIDTH; i++){
			if(game.field[j][i]==0){
				full=0;
				break; //line cannot be cleared
			}
		}
		
		if(full){
			cleared++;
			//shift 1 down
			for(j_shift=j; j_shift>0; j_shift--){
				for(i=0; i<FIELD_WIDTH; i++){
					game.field[j_shift][i]=game.field[j_shift-1][i];
				}
			}
			for(i=0; i<FIELD_WIDTH; i++){
				game.field[0][i]=0; //clear top row
			}
			
			j++; //since lines have been shifted down, the current line has to be checked again
		}
	}
	
	//score computation
	if(cleared>0){
		game.lines_cleared += cleared;
		if(cleared!=4){
			game.score += 100*cleared;
			play_sound_effect(SFX_LINE_CLEAR);
		} else{
			game.score += 600;
			play_sound_effect(SFX_TETRIS);
		}
	}
	return cleared;
}

void move_down(void){
	int old_y;
	
	if(game.state!=PLAY){
		return;
	}
	
	if(can_place_tetromino(game.current_piece.x, game.current_piece.y + 1, game.current_piece.type, game.current_piece.rotation)){
		old_y = game.current_piece.y;
    	game.current_piece.y++;
    	draw_tetromino_move(game.current_piece.x, old_y, game.current_piece.x, game.current_piece.y);
	} else{
		place_tetromino();
	}
}

void move_left(void){
	int old_x;
	if(game.state!=PLAY){
		return;
	}
	
	if(can_place_tetromino(game.current_piece.x-1, game.current_piece.y, game.current_piece.type, game.current_piece.rotation)){
		old_x = game.current_piece.x;
		game.current_piece.x--;
		play_sound_effect(SFX_MOVE);
		draw_tetromino_move(old_x, game.current_piece.y, game.current_piece.x, game.current_piece.y);
	}
}

void move_right(void){
	int old_x;
	if(game.state!=PLAY){
		return;
	}
	
	if(can_place_tetromino(game.current_piece.x+1, game.current_piece.y, game.current_piece.type, game.current_piece.rotation)){
		old_x = game.current_piece.x;
		game.current_piece.x++;
		play_sound_effect(SFX_MOVE);
		draw_tetromino_move(old_x, game.current_piece.y, game.current_piece.x, game.current_piece.y);
	}
}

void rotate(void){
	if(game.state!=PLAY){
		return;
	}
	
	uint8_t rotation = (game.current_piece.rotation +1)%4;
	if(can_place_tetromino(game.current_piece.x, game.current_piece.y, game.current_piece.type, rotation)){
		game.current_piece.rotation=rotation;
		play_sound_effect(SFX_ROTATE);
		draw_game();
	}
}

void hard_drop(void){
	if(game.state!=PLAY){
		return;
	}
	
	while(can_place_tetromino(game.current_piece.x, game.current_piece.y+1, game.current_piece.type, game.current_piece.rotation)){
		game.current_piece.y++;
	}
	play_sound_effect(SFX_HARD_DROP);
	place_tetromino();
	disable_timer(0);
	reset_timer(0);
	enable_timer(0);
}

void soft_drop_on(void){
	if(game.state != PLAY || game.soft_drop_active){
		return;
	}
	
	game.soft_drop_active=1;
	
	disable_timer(0);
	reset_timer(0);
	init_timer(0, 0, 0, 3, 0x00BEBC20); //0.5 sec= 0x00BEBC20 1/4=0x001312D0
	enable_timer(0);
}

void soft_drop_off(void){
	if(game.state != PLAY || game.soft_drop_active==0){
		return;
	}
	
	game.soft_drop_active=0;
	
	disable_timer(0);
	reset_timer(0);
	init_timer(0, 0, 0, 3, 0x017D7840); //1sec=0x017D7840 1/4=0x002625A0
	enable_timer(0);
}
