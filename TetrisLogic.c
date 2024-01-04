#include <Tetris.h>

extern int stage[];

int CheckCollision(const int currentTetrominoX, const int currentTetrominoY, const int *tetromino)
{
    for(int y = 0; y < TETROMINO_SIZE; y++)
    {
        for(int x = 0; x < TETROMINO_SIZE; x++)
        {
            const int offset = y * TETROMINO_SIZE + x;

            if(tetromino[offset] == 1)
            {
                const int offset_stage = (y + currentTetrominoY) * STAGE_WIDTH + (x +currentTetrominoX);

                if (stage[offset_stage] != 0)
                {
                    return 1;
                }
            }
        }
    }

    return 0;
}

int SpawnNewPlayerTetromino(const int tetrominoStartX, const int tetrominoStartY, int* currentTetrominoX, int* currentTetrominoY,int* currentTetrominoType,int* last_tetramino_type, int* currentRotation, int* currentColor)
{
    *currentTetrominoX = tetrominoStartX;
    *currentTetrominoY = tetrominoStartY;

    do
    {
        *currentTetrominoType = GetRandomValue(0, 6);
    } while (*currentTetrominoType == *last_tetramino_type);
    *last_tetramino_type = *currentTetrominoType;
    
    *currentRotation = 0;
    *currentColor = *currentTetrominoType+1;

    if(CheckCollision(*currentTetrominoX,*currentTetrominoY,currentTetrominoType))
    {
        return -1;
    }
    return 0;
}

extern Sound sfx_line_completed;
extern int is_waiting_for_ending_effect;

int DeleteCompletedLines(int* to_remove_lines)
{
    int deleted_lines = 0;
 
    for (int y = 0; y < STAGE_HEIGHT - 1; y++)
    {
        int checkLine = 1;

        for (int x = 1; x < STAGE_WIDTH - 1; x++)
        {
            const int offset = y * STAGE_WIDTH + x;

            //if there is at least one 0 = this line isn't completed
            if (stage[offset] == 0)
            {
                checkLine = 0;
                break;
            }
        }

        if(checkLine)
        {
            is_waiting_for_ending_effect = true;
            PlaySound(sfx_line_completed);
            const int offset = y * STAGE_WIDTH + 1; //offset of the first tetramino of the line
            to_remove_lines[y] = offset;
            deleted_lines++;
        }
    }   

    return deleted_lines;
}

void PushDownTetrominos(int start_line_y)
{
    for (int y = start_line_y; y > 0; y--)
    {
        for (int x = 1; x < STAGE_WIDTH - 1; x++)
        {
            const int offset = y * STAGE_WIDTH + x;
            const int offset_below = (y+1) * STAGE_WIDTH + x;

            if (stage[offset_below] == 0 && stage[offset] > 0)
            {
                stage[offset_below] = stage[offset];
                stage[offset] = 0;
            }
        }
    }   
}

