#pragma once

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 720

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 25
#define BOARD_X_OFFSET 5
#define BOARD_SKYLINE 20

#define NUMBER_OF_BRICK_TYPES 7
#define NUMBER_OF_MINOS_IN_BRICK 4
#define BRICK_PIXEL_SIZE 36

#define GA_POPSIZE 25
#define GA_ITERATIONS 10000
#define GA_ELITISM 0.10f
#define GA_MUTATION 0.1f
#define GA_PLAYS_PER_CHROMOSME 1
#define GA_NUM_OF_ALLELES 4

#define AI_LOOK_AHEAD 0 //Number of moves to look ahead; 0 = examine only current move, 1 = examine current and next move, etc.