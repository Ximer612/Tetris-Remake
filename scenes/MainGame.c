#include <raylib.h>
#include <scenes.h>
#define TETRIS_DEFINITIONS
#include <tetris.h>
#include <extra_colors.h>
#include <string.h>
#include <math.h>
#include <game_settings.h>

extern Music* current_music;
extern Music music_ingame2;
extern Sound sfx_hit_piece;
extern Texture2D tetromino_texture;
extern int highscore;
extern int actual_score;
extern GameScene gameover_scene;
extern GameScene* actual_game_scene;


//CONST VARIABLES
const int startOffsetX = (WINDOW_WIDTH / 2) - ((STAGE_WIDTH * TILE_SIZE) / 2);
const int startOffsetY = (WINDOW_HEIGHT / 2) - ((STAGE_HEIGHT * TILE_SIZE) / 2);
const int tetrominoStartX = STAGE_WIDTH / 2 - 2;
const int tetrominoStartY = 0;

int is_game_paused = 0;
int is_waiting_for_ending_effect = 0;

float actual_falling_speed = MAX_FALL_SPEED;

int currentColor;
int currentTetrominoType;
int currentRotation;
int currentTetrominoX;
int currentTetrominoY;
int last_tetramino_type = -1;

//TIMERS
float counterToMoveTetrominoDown;

const float timerToShowCompletedLineEffect = 0.5f;
float counterToShowCompletedLineEffect;

//EFFECT VARIABLES
int to_remove_lines[STAGE_HEIGHT];
int index_color_effect = 0;


void MainGameOnExit()
{
    if(actual_score > highscore)
    {
        highscore = actual_score;
        SaveFileText(SAVE_PATH,(char*)TextFormat("%d",highscore));
    } 
}

void MainGameOnEnter()
{
    memcpy(stage,stage_empty,STAGE_HEIGHT*STAGE_WIDTH*sizeof(int));

    actual_score = 0;

    float timeToMoveTetrominoDown = actual_falling_speed;
    counterToShowCompletedLineEffect = timerToShowCompletedLineEffect;

    current_music = &music_ingame2;
    PlayMusicStream(*current_music);

    SpawnNewPlayerTetromino(tetrominoStartX, tetrominoStartY, &currentTetrominoX, &currentTetrominoY, &currentTetrominoType, &last_tetramino_type, &currentRotation, &currentColor);
}

void RecalculateFallingSpeed(){

    actual_falling_speed = MAX_FALL_SPEED - (actual_score * 0.0001f); 
    actual_falling_speed = MIN(actual_falling_speed,MAX_FALL_SPEED);
    actual_falling_speed = MAX(actual_falling_speed,MIN_FALL_SPEED);
}

void AddScore(const int score_to_add)
{
    actual_score +=score_to_add;
    RecalculateFallingSpeed();
}

void PlayerInputManager()
{
    if (IsKeyPressed(KEY_SPACE))
    {
        const int lastRotation = currentRotation;
        const int lastCounterToMoveTetrominoDown = counterToMoveTetrominoDown;    

        currentRotation++;
        counterToMoveTetrominoDown = counterToMoveTetrominoDown + GetFrameTime();

        if (currentRotation > 3)
        {
            currentRotation = 0;
        }

        if (CheckCollision(currentTetrominoX,currentTetrominoY,tetrominoTypes[currentTetrominoType][currentRotation]))
        {
            currentRotation = lastRotation;
            counterToMoveTetrominoDown = lastCounterToMoveTetrominoDown;
        }
    }

    else if (IsKeyPressed(KEY_RIGHT))
    {
        // No need to check overflow, wall is your protector
        if (!CheckCollision(currentTetrominoX+1,currentTetrominoY,tetrominoTypes[currentTetrominoType][currentRotation]))
        {
            currentTetrominoX++;
        }
    }
    else if (IsKeyPressed(KEY_LEFT))
    {
        // No need to check overflow, wall is your protector
        if (!CheckCollision(currentTetrominoX-1,currentTetrominoY,tetrominoTypes[currentTetrominoType][currentRotation]))
        {
            currentTetrominoX--;
        }
    }

    if(counterToMoveTetrominoDown <= 0 || IsKeyPressed(KEY_DOWN))
    {            
        AddScore(DOWN_POINTS);

        if(!CheckCollision(currentTetrominoX,currentTetrominoY+1,tetrominoTypes[currentTetrominoType][currentRotation]))
        {
            currentTetrominoY++;
            counterToMoveTetrominoDown = actual_falling_speed;
        }

        else
        {
            for(int y = 0; y < TETROMINO_SIZE; y++)
            {
                for(int x = 0; x < TETROMINO_SIZE; x++)
                {
                    const int offset = y * TETROMINO_SIZE + x;

                    const int *tetromino = tetrominoTypes[currentTetrominoType][currentRotation];

                    if(tetromino[offset] == 1)
                    {
                        const int offset_stage = (y + currentTetrominoY) * STAGE_WIDTH + (x + currentTetrominoX);
                        stage[offset_stage] = currentColor+1;
                    }
                }
            }

            SetSoundPitch(sfx_hit_piece,GetRandomValue(5,150)*0.01f); // from 0.05f to 1.50f
            PlaySound(sfx_hit_piece);

            const int deleted_lines = DeleteCompletedLines(to_remove_lines);
            AddScore((deleted_lines * 1.5f) *LINE_POINTS);
            
            if(SpawnNewPlayerTetromino(tetrominoStartX, tetrominoStartY, &currentTetrominoX, &currentTetrominoY, &currentTetrominoType, &last_tetramino_type, &currentRotation, &currentColor))
            {
                SwitchScene(&gameover_scene,&actual_game_scene);
            }
        }
    }
}

void MainGameLoop()
{
    UpdateMusicStream(*current_music);

    if(IsKeyPressed(KEY_P))
    {
        is_game_paused = !is_game_paused;
    }

    if(is_game_paused) goto game_loop_drawing;

    if(is_waiting_for_ending_effect) //the line completed effect
    {
        for (int i = 0; i < STAGE_HEIGHT; i++)
        {
            if(to_remove_lines[i] == 0) continue;

            for (int j = 0; j < STAGE_WIDTH-2; j++)
            {
                stage[to_remove_lines[i]+j] = index_color_effect;
            }
        }

        counterToShowCompletedLineEffect -= GetFrameTime();
        index_color_effect = (index_color_effect + 1) % 8;
        if(counterToShowCompletedLineEffect < 0)
        {
            is_waiting_for_ending_effect = false;
            counterToShowCompletedLineEffect = timerToShowCompletedLineEffect;

            for (int i = 0; i < STAGE_HEIGHT; i++)
            {
                if(to_remove_lines[i] == 0) continue;               
                
                memset(stage+to_remove_lines[i],0,(STAGE_WIDTH-2)* sizeof(int)); //set the whole line to 0

                PushDownTetrominos(to_remove_lines[i]/STAGE_WIDTH -1); //y * STAGE_WIDTH + 1
            }

            memset(to_remove_lines,0,sizeof(to_remove_lines));

        }
        goto game_loop_drawing;
    }


    //ACTUAL GAME LOOP

    counterToMoveTetrominoDown -= GetFrameTime();

    if(IsKeyPressed(KEY_E))
    {
        //debug key for doubling points
        AddScore(actual_score);
    }

    PlayerInputManager();

    game_loop_drawing:

    BeginDrawing();
    ClearBackground(TETRIS_GRAY);

    DrawText(TextFormat("Score: %08i", actual_score), 20, 10, 20, TETRIS_BLUE_PURPLE);
    DrawText(TextFormat("Highscore: %08i", highscore), 20, 30, 20, TETRIS_LIGHT_GREEN);
    DrawText(TextFormat("Vertical Speed: %.3f", (1 - actual_falling_speed) ), 20, 50, 20, TETRIS_YELLOW);

    DrawStageTetrominos(tetromino_texture,currentTetrominoX,currentTetrominoY,startOffsetX,startOffsetY);

    DrawPlayerTetromino(tetromino_texture,tetrominoColors[currentColor],currentTetrominoX,currentTetrominoY,startOffsetX,startOffsetY, tetrominoTypes[currentTetrominoType][currentRotation]);

    if(is_game_paused) 
    {
        DrawRectangle(0,0,WINDOW_WIDTH,WINDOW_HEIGHT,TETRIS_BLACK_OPAQUE);
        DrawText("GAME PAUSED", startOffsetX, WINDOW_HEIGHT/2-50, 40, TETRIS_YELLOW);
        DrawText("Press [ESC] to quit!", startOffsetX-50, WINDOW_HEIGHT/2+20, 40, TETRIS_DARK_RED);

    }

    EndDrawing();
}