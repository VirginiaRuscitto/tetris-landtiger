#ifndef __MUSIC_H
#define __MUSIC_H

#include <stdint.h>
#include <stdlib.h>

/*Note Frequencies (in Hz)*/
#define C3  131
#define D3  147
#define E3  165
#define F3  175
#define G3  196
#define A3  220
#define B3  247

#define C4  262
#define D4  294
#define E4  330
#define F4  349
#define G4  392
#define A4  440
#define B4  494

#define C5  523
#define D5  587
#define E5  659
#define F5  698
#define G5  784
#define A5  880
#define B5  988

#define C6  1047
#define D6  1175
#define E6  1319
#define F6  1397
#define G6  1568
#define A6  1760
#define B6  1976

#define C7  2093
#define REST 0

/*Note Durations (in ms)*/
#define WHOLE       2000
#define HALF        1000
#define QUARTER     500
#define EIGHTH      250
#define SIXTEENTH   125

typedef struct {
    uint16_t frequency;
    uint16_t duration;
}Note;

typedef enum {
    SFX_MOVE,
    SFX_ROTATE,
    SFX_LINE_CLEAR,
    SFX_TETRIS,
    SFX_HARD_DROP,
    SFX_GAME_OVER,
    SFX_POWERUP_SPAWN,
    SFX_POWERUP_ACTIVATE
}SoundEffect;

uint32_t calculate_reload_value(uint16_t freq);
void music_init(void);
void play_note(uint16_t frequency);
void stop_note(void);
void play_background_music(void);
void stop_background_music(void);
void play_sound_effect(SoundEffect sfx);
void music_timer2();
void music_timer3();

#endif