#include <raylib.h>
#include <tetris.h>

extern int stage[];

void DrawPlayerTetromino(const Texture2D tetrominoTexture, const Color current_color, const int currentTetrominoX, const int currentTetrominoY, const int startOffsetX, const int startOffsetY, const int *tetromino)
{
    for(int y = 0; y < TETROMINO_SIZE; y++)
    {
        for(int x = 0; x < TETROMINO_SIZE; x++)
        {
            const int offset = y * TETROMINO_SIZE + x;

            if(tetromino[offset] == 1)
            {
                DrawTexture(tetrominoTexture,(x + currentTetrominoX) * TILE_SIZE + startOffsetX, (y + currentTetrominoY) * TILE_SIZE + startOffsetY, current_color);
                //DrawRectangle((x + currentTetrominoX) * TILE_SIZE + startOffsetX, (y + currentTetrominoY) * TILE_SIZE + startOffsetY, TILE_SIZE, TILE_SIZE, current_color);
            }
        }
    }
}

extern Color tetrominoColors[];

void DrawStageTetrominos(const Texture2D tetrominoTexture, const int currentTetrominoX, const int currentTetrominoY, const int startOffsetX, const int startOffsetY)
{
    for(int y = 0; y < STAGE_HEIGHT; y++)
    {
        for(int x = 0; x < STAGE_WIDTH; x++)
        {
            const int offset = y * STAGE_WIDTH + x;
            const int color = stage[offset];

            if(stage[offset] != 0)
            {
                DrawTexture(tetrominoTexture,x * TILE_SIZE + startOffsetX, y * TILE_SIZE + startOffsetY, tetrominoColors[color-1]);
                //DrawRectangle(x * TILE_SIZE + startOffsetX, y * TILE_SIZE + startOffsetY, TILE_SIZE, TILE_SIZE, tetrominoColors[color-1]);
            }

            DrawRectangleLines(x * TILE_SIZE + startOffsetX, y * TILE_SIZE + startOffsetY, TILE_SIZE, TILE_SIZE, BLACK);
        }
    }
}