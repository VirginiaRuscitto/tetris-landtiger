#include "LPC17xx.h"
#include "music.h"
#include "timer/timer.h"

const uint16_t SinTable[45] = 
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};

/* Background Music - Tetris Theme (Korobeiniki) Simplified */
const Note background_music[] = {
    {E5, QUARTER}, {B4, EIGHTH}, {C5, EIGHTH}, {D5, QUARTER}, {C5, EIGHTH}, {B4, EIGHTH},
    {A4, QUARTER}, {A4, EIGHTH}, {C5, EIGHTH}, {E5, QUARTER}, {D5, EIGHTH}, {C5, EIGHTH},
    {B4, QUARTER}, {B4, EIGHTH}, {C5, EIGHTH}, {D5, QUARTER}, {E5, QUARTER},
    {C5, QUARTER}, {A4, QUARTER}, {A4, HALF},
    
    {REST, EIGHTH}, {D5, QUARTER}, {D5, EIGHTH}, {F5, QUARTER}, {A5, QUARTER}, 
    {G5, EIGHTH}, {F5, EIGHTH}, {E5, QUARTER + EIGHTH}, {C5, EIGHTH}, {E5, QUARTER}, 
    {D5, EIGHTH}, {C5, EIGHTH}, {B4, QUARTER}, {B4, EIGHTH}, {C5, EIGHTH},
    {D5, QUARTER}, {E5, QUARTER}, {C5, QUARTER}, {A4, QUARTER}, {A4, HALF}
};

#define MUSIC_LENGTH (sizeof(background_music) / sizeof(Note))

/* Sound Effects */
const Note sfx_move[] = {{C5, 50}};
const Note sfx_rotate[] = {{E5, 60}};
const Note sfx_line_clear[] = {
    {E5, 80}, {G5, 80}, {B5, 80}, {C6, 120}
};
const Note sfx_tetris[] = {
    {C5, 100}, {E5, 100}, {G5, 100}, {C6, 100}, 
    {G5, 100}, {C6, 300}
};
const Note sfx_hard_drop[] = {{G4, 100}, {C4, 150}};
const Note sfx_game_over[] = {
    {E5, 200}, {D5, 200}, {C5, 200}, {B4, 200}, {A4, 400}
};
const Note sfx_powerup_spawn[] = {
    {C5, 80}, {G5, 80}, {C6, 120}
};
const Note sfx_powerup_activate[] = {
    {C6, 100}, {E6, 100}, {G6, 200}
};

//music
volatile uint8_t music_enabled = 0;
volatile uint8_t music_playing = 0;
volatile uint16_t current_note_index = 0;
volatile uint32_t note_timer = 0;
volatile uint32_t note_duration = 0;

//sfx
volatile uint8_t sfx_playing = 0;
volatile const Note* current_sfx = NULL;
volatile uint16_t sfx_length = 0;
volatile uint16_t sfx_index = 0;
volatile uint32_t sfx_timer = 0;
volatile uint32_t sfx_duration = 0;

volatile uint16_t current_frequency = 0;

#define DAC_FREQ 25000000

uint32_t calculate_reload_value(uint16_t freq){
	if (freq==0){
		return 0;
	}
  return (DAC_FREQ/(freq*45))-1; //45 samples per wave period
}

void music_init(){
	LPC_DAC->DACR=(512 << 6); //mid level
  music_enabled=0;
  music_playing=0;
  sfx_playing=0;
  current_frequency=0;
	init_timer(3, 0, 0, 3, 25000); //used for note duration timing
}

void play_note(uint16_t frequency){
	if (frequency == 0 || frequency==REST){ //silent
		disable_timer(2);
    LPC_DAC->DACR=(512 << 6);
    current_frequency=0;
    return;
	}
  
	current_frequency=frequency;
  
	uint32_t reload=calculate_reload_value(frequency);
	disable_timer(2);
	reset_timer(2);
	init_timer(2, 0, 0, 3, reload);
	enable_timer(2); //used for waveform generation
}

void stop_note(){
	disable_timer(2);
  LPC_DAC->DACR=(512 << 6);
  current_frequency=0;
}

void play_background_music(){
	if (!music_playing) {
		music_enabled=1;
    music_playing=1;
    current_note_index=0;
    note_timer=0;
		
    note_duration=background_music[0].duration;
    play_note(background_music[0].frequency);

    enable_timer(3);
	}
}

void stop_background_music(){
	disable_timer(3);
  stop_note();
	music_enabled=0;
  music_playing=0;

}

void play_sound_effect(SoundEffect sfx){
	const Note* notes;
  uint16_t len=0;
    
  switch(sfx) {
		case SFX_MOVE:
			notes=sfx_move;
      len=sizeof(sfx_move)/sizeof(Note);
      break;
		case SFX_ROTATE:
			notes=sfx_rotate;
			len=sizeof(sfx_rotate)/sizeof(Note);
			break;
		case SFX_LINE_CLEAR:
			notes=sfx_line_clear;
		  len=sizeof(sfx_line_clear)/sizeof(Note);
		  break;
		case SFX_TETRIS:
			notes=sfx_tetris;
		  len=sizeof(sfx_tetris)/sizeof(Note);
		  break;
		case SFX_HARD_DROP:
			notes=sfx_hard_drop;
		  len=sizeof(sfx_hard_drop)/sizeof(Note);
		  break;
		case SFX_GAME_OVER:
			notes=sfx_game_over;
		  len=sizeof(sfx_game_over)/sizeof(Note);
		  break;
		case SFX_POWERUP_SPAWN:
			notes=sfx_powerup_spawn;
		  len=sizeof(sfx_powerup_spawn)/sizeof(Note);
		  break;
		case SFX_POWERUP_ACTIVATE:
			notes=sfx_powerup_activate;
		  len=sizeof(sfx_powerup_activate)/sizeof(Note);
	    break;
	}
	if (notes && len > 0){
		sfx_playing=1;
		current_sfx=notes;
		sfx_length=len;
		sfx_index=0;
		sfx_timer=0;
    sfx_duration=notes[0].duration;
    play_note(notes[0].frequency);
    enable_timer(3);
	}
}

void music_timer2(){
	static uint8_t ticks=0;
	if (current_frequency>0){
		LPC_DAC->DACR=(SinTable[ticks] << 6);
    ticks++;
    if (ticks>=45){
			ticks=0;
		}
   }else{
		LPC_DAC->DACR = (512 << 6);
   }
}
void music_timer3(){
	if (sfx_playing){ //priority over music
		sfx_timer++;
    if (sfx_timer>=sfx_duration){
			sfx_index++; //next note
      sfx_timer=0;
      if (sfx_index>=sfx_length){
				sfx_playing=0; //sfx finished and resume music if necessary
        if (music_enabled && music_playing){
					play_note(background_music[current_note_index].frequency);
        } else{
					stop_note();
				}
			} else{ //next note
				sfx_duration=current_sfx[sfx_index].duration;
				play_note(current_sfx[sfx_index].frequency);
			}
		}
	}else if (music_enabled && music_playing){
		note_timer++;
		if (note_timer>=note_duration){
			current_note_index++;
			note_timer=0;
			if (current_note_index>=MUSIC_LENGTH) {
				current_note_index=0; //in order to restart the background song
			}
			note_duration=background_music[current_note_index].duration;
      play_note(background_music[current_note_index].frequency);
		}
	}
}