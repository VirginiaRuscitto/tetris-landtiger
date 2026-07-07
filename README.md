# Tetris on LandTiger LPC1768

Bare-metal implementation of Tetris for the LandTiger development board (NXP LPC1768, ARM Cortex-M3). The game runs entirely on the microcontroller, directly interfacing with the onboard GLCD, joystick, buttons, and DAC using memory-mapped peripherals and interrupt handlers.

## Overview

The game features a classic 10×20 playing field rendered on the onboard GLCD. Players control falling tetrominoes using the LandTiger joystick and buttons to move, rotate, and drop pieces. The implementation follows the standard Tetris ruleset, including all seven tetrominoes, collision detection, line clearing, score multipliers, and a bonus for clearing four lines simultaneously ("Tetris"). 

A background music track and event-triggered sound effects are synthesized in real time through the onboard DAC.

## Main features

- Full Tetris ruleset: 7 tetromino shapes (I, O, T, J, L, S, Z), 4 rotation states each, collision detection
- Line clearing with row shifting and score computation (single/multi-line/tetris bonus)
- Soft drop (2 squares/sec while held) and hard drop (instant fall)
- Pause/resume state machine, high score persisted across games within a session
- Real-time rendering on GLCD: field, falling piece, live score/high score/lines cleared counters
- Background music (Tetris theme, simplified) and 8 distinct sound effects (move, rotate, line clear, tetris, hard drop, game over, power-up spawn/activate), generated via DAC direct digital synthesis (sine lookup table)
- Fully interrupt-driven architecture: no polling in the main loop

## Tech stack

| Layer | Technology |
|---|---|
| Language | C |
| Target | LPC1768 (ARM Cortex-M3), LandTiger board |
| IDE / Toolchain | Keil µVision (MDK-ARM) |
| Peripherals | GPIO, External Interrupts (EINT), Repetitive Interrupt Timer (RIT), Timer0/Timer2/Timer3, DAC, GLCD |
| Emulation | Keil µVision LandTiger simulator (SW_Debug target) |

## Controls

| Input | Action |
|---|---|
| KEY1 | Start game / toggle pause |
| KEY2 | Hard drop |
| Joystick left/right | Move piece |
| Joystick up | Rotate piece 90° clockwise |
| Joystick down (hold) | Soft drop |

## Screenshots

![start-game](screenshots/start-game.jpeg)

![gameplay](screenshots/gameplay.jpeg)

## Run in Keil µVision

1. Open `sample.uvprojx` in Keil µVision (MDK-ARM).
2. Select the "SW_Debug" compilation target to run on the LandTiger emulator (or the release target if using a physical board).
3. Build and start a debug session.