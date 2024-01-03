#ifndef TETRIS_DEFINITIONS
#define TETRIS_DEFINITIONS

#define STAGE_WIDTH     12
#define STAGE_HEIGHT    22
#define TILE_SIZE       24
#define TETROMINO_SIZE  4
#define MIN_FALL_SPEED  0.05
#define MAX_FALL_SPEED  1
#define DOWN_POINTS     10
#define LINE_POINTS     200

#define WINDOW_WIDTH    600
#define WINDOW_HEIGHT   700





#endif //TETRIS_DEFINITIONS


int CheckCollision(const int tetrominoStartX, const int tetrominoStartY, const int *tetromino);