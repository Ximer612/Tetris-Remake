#include <raylib.h>
#include <tetris.h>

extern int stage[];

void DrawPlayerTetromino(const Texture2D tetrominoTexture, const Color current_color, const int current_tetromino_x, const int current_tetromino_y, const int start_offset_x, const int startOffsetY, const int *tetromino)
{
    for(int y = 0; y < TETROMINO_SIZE; y++)
    {
        for(int x = 0; x < TETROMINO_SIZE; x++)
        {
            const int offset = y * TETROMINO_SIZE + x;

            if(tetromino[offset] == 1)
            {
                DrawTexture(tetrominoTexture,(x + current_tetromino_x) * TILE_SIZE + start_offset_x, (y + current_tetromino_y) * TILE_SIZE + startOffsetY, current_color);
                //DrawRectangle((x + current_tetromino_x) * TILE_SIZE + start_offset_x, (y + current_tetromino_y) * TILE_SIZE + startOffsetY, TILE_SIZE, TILE_SIZE, current_color);
            }
        }
    }
}

extern Color tetromino_colors[];

void DrawStageTetrominos(const Texture2D tetrominoTexture, const int current_tetromino_x, const int current_tetromino_y, const int start_offset_x, const int startOffsetY)
{
    for(int y = 0; y < STAGE_HEIGHT; y++)
    {
        for(int x = 0; x < STAGE_WIDTH; x++)
        {
            const int offset = y * STAGE_WIDTH + x;
            const int color = stage[offset];

            if(stage[offset] != 0)
            {
                DrawTexture(tetrominoTexture,x * TILE_SIZE + start_offset_x, y * TILE_SIZE + startOffsetY, tetromino_colors[color-1]);
                //DrawRectangle(x * TILE_SIZE + start_offset_x, y * TILE_SIZE + startOffsetY, TILE_SIZE, TILE_SIZE, tetromino_colors[color-1]);
            }

            DrawRectangleLines(x * TILE_SIZE + start_offset_x, y * TILE_SIZE + startOffsetY, TILE_SIZE, TILE_SIZE, BLACK);
        }
    }
}