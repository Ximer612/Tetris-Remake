#include <tetris.h>
#include <raylib_custom_functions.h>

extern int stage[];
extern int tetromino_types[];

int CheckCollision(const int current_tetromino_x, const int current_tetromino_y, const int *tetromino)
{
    for(int y = 0; y < TETROMINO_SIZE; y++)
    {
        for(int x = 0; x < TETROMINO_SIZE; x++)
        {
            const int offset = y * TETROMINO_SIZE + x;

            if(tetromino[offset] == 1)
            {
                const int offset_stage = (y + current_tetromino_y) * STAGE_WIDTH + (x +current_tetromino_x);

                if (stage[offset_stage] != 0)
                {
                    return 1;
                }
            }
        }
    }

    return 0;
}

const int filledTetromino[]=
{
    1,1,1,1,
    1,1,1,1,
    1,1,1,1,
    1,1,1,1,
};

int SpawnNewPlayerTetromino(const int tetromino_start_x, const int tetromino_start_y, int* current_tetromino_x, int* current_tetromino_y,int* current_tetromino_type,int* last_tetramino_type, int* current_rotation, int* current_player_color)
{
    *current_tetromino_x = tetromino_start_x;
    *current_tetromino_y = tetromino_start_y;

    do
    {
        *current_tetromino_type = GetRandomValue(0, 6);
    } while (*current_tetromino_type == *last_tetramino_type);
    *last_tetramino_type = *current_tetromino_type;
    
    *current_rotation = 0;
    *current_player_color = *current_tetromino_type+1;

    if(CheckCollision(*current_tetromino_x,*current_tetromino_y,filledTetromino))
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

