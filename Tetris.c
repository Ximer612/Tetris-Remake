#include <raylib.h>
#include <time.h>
#include <string.h>
#include <Tetris.h>
#include <math.h>

#pragma region stage & tetrominos
int stage[] = 
{
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

// L

const int lTetromino0[] =
{
    0, 1, 0, 0,
    0, 1, 0, 0,
    0, 1, 1, 0,
    0, 0, 0, 0,
};

const int lTetromino90[] =
{
    0, 0, 0, 0,
    1, 1, 1, 0,
    1, 0, 0, 0,
    0, 0, 0, 0,
};

const int lTetromino180[] =
{
    1, 1, 0, 0,
    0, 1, 0, 0,
    0, 1, 0, 0,
    0, 0, 0, 0,
};

const int lTetromino270[] =
{
    0, 0, 1, 0,
    1, 1, 1, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
};

// J

const int jTetromino0[] =
{
    0, 1, 1, 0,
    0, 1, 0, 0,
    0, 1, 0, 0,
    0, 0, 0, 0,
};

const int jTetromino90[] =
{
    0, 0, 0, 0,
    1, 1, 1, 0,
    0, 0, 1, 0,
    0, 0, 0, 0,
};

const int jTetromino180[] =
{
    0, 1, 0, 0,
    0, 1, 0, 0,
    1, 1, 0, 0,
    0, 0, 0, 0,
};

const int jTetromino270[] =
{
    1, 0, 0, 0,
    1, 1, 1, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
};

// O

const int oTetromino[] =
{
    1, 1, 0, 0,
    1, 1, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
};

// S

const int sTetromino0[] =
{
    0, 1, 0, 0,
    0, 1, 1, 0,
    0, 0, 1, 0,
    0, 0, 0, 0,
};

const int sTetromino90[] =
{
    0, 0, 0, 0,
    0, 1, 1, 0,
    1, 1, 0, 0,
    0, 0, 0, 0,
};

const int sTetromino180[] =
{
    1, 0, 0, 0,
    1, 1, 0, 0,
    0, 1, 0, 0,
    0, 0, 0, 0,
};

const int sTetromino270[] =
{
    0, 1, 1, 0,
    1, 1, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
};

// T

const int tTetromino0[] =
{
    0, 0, 0, 0,
    1, 1, 1, 0,
    0, 1, 0, 0,
    0, 0, 0, 0,
};

const int tTetromino90[] =
{
    0, 1, 0, 0,
    1, 1, 0, 0,
    0, 1, 0, 0,
    0, 0, 0, 0,
};

const int tTetromino180[] =
{
    0, 1, 0, 0,
    1, 1, 1, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
};

const int tTetromino270[] =
{
    0, 1, 0, 0,
    0, 1, 1, 0,
    0, 1, 0, 0,
    0, 0, 0, 0,
};

// I

const int iTetromino0[] =
{
    0, 1, 0, 0,
    0, 1, 0, 0,
    0, 1, 0, 0,
    0, 1, 0, 0,
};

const int iTetromino90[] =
{
    0, 0, 0, 0,
    1, 1, 1, 1,
    0, 0, 0, 0,
    0, 0, 0, 0,
};

const int iTetromino180[] =
{
    1, 0, 0, 0,
    1, 0, 0, 0,
    1, 0, 0, 0,
    1, 0, 0, 0,
};

const int iTetromino270[] =
{
    1, 1, 1, 1,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
};

// Z

const int zTetromino0[] =
{
    0, 0, 1, 0,
    0, 1, 1, 0,
    0, 1, 0, 0,
    0, 0, 0, 0,
};

const int zTetromino90[] =
{
    0, 0, 0, 0,
    1, 1, 0, 0,
    0, 1, 1, 0,
    0, 0, 0, 0,
};

const int zTetromino180[] =
{
    0, 1, 0, 0,
    1, 1, 0, 0,
    1, 0, 0, 0,
    0, 0, 0, 0,
};

const int zTetromino270[] =
{
    1, 1, 0, 0,
    0, 1, 1, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
};

const Color tetrominoColors[8] =
{
    {200,200,200,255},{240,0,0,255},{0,240,0,255},{160,0,240,255},{240,240,0,255},{0,240,240,255},{0,0,240,255},{240,160,0,255}
};

const Color clear_color = {40,40,40,255};

const int *tetrominoTypes[7][4] =
{
    {zTetromino0, zTetromino90, zTetromino180, zTetromino270},
    {sTetromino0, sTetromino90, sTetromino180, sTetromino270},
    {tTetromino0, tTetromino90, tTetromino180, tTetromino270},
    {oTetromino, oTetromino, oTetromino, oTetromino},
    {iTetromino0, iTetromino90, iTetromino180, iTetromino270},
    {jTetromino0, jTetromino90, jTetromino180, jTetromino270},
    {lTetromino0, lTetromino90, lTetromino180, lTetromino270},
};

#pragma endregion

//CONST VARIABLES
const int startOffsetX = (WINDOW_WIDTH / 2) - ((STAGE_WIDTH * TILE_SIZE) / 2);
const int startOffsetY = (WINDOW_HEIGHT / 2) - ((STAGE_HEIGHT * TILE_SIZE) / 2);
const int tetrominoStartX = STAGE_WIDTH / 2 - 2;
const int tetrominoStartY = 0;

//GAME SCENES VARIABLES
typedef struct{

    void (*OnEnter)();
    void (*Loop)();
    void (*OnExit)();

} GameScene;

int is_game_paused = 0;
int is_waiting_for_ending_effect = 0;

GameScene* actual_game_scene;

GameScene gameover_Scene;
GameScene maingame_Scene;
GameScene startmenu_Scene;
GameScene pause_Scene;

Sound sfx_hit_piece;
Sound sfx_line_completed;
Music music_ingame1;
Music music_ingame2;
Music music_gameover;
Music music_startmenu;

Music* currentMusic;

//PLAYER VARIABLES

int actual_score;
int highscore = 12345678;

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

void DrawPlayerTetromino(const Color current_color, const int *tetromino)
{
    for(int y = 0; y < TETROMINO_SIZE; y++)
    {
        for(int x = 0; x < TETROMINO_SIZE; x++)
        {
            const int offset = y * TETROMINO_SIZE + x;

            if(tetromino[offset] == 1)
            {
                DrawRectangle((x + currentTetrominoX) * TILE_SIZE + startOffsetX, (y + currentTetrominoY) * TILE_SIZE + startOffsetY, TILE_SIZE, TILE_SIZE, current_color);
            }
        }
    }
}

void DrawStageTetrominos()
{
    for(int y = 0; y < STAGE_HEIGHT; y++)
    {
        for(int x = 0; x < STAGE_WIDTH; x++)
        {
            const int offset = y * STAGE_WIDTH + x;
            const int color = stage[offset];

            if(stage[offset] != 0)
            {
                DrawRectangle(x * TILE_SIZE + startOffsetX, y * TILE_SIZE + startOffsetY, TILE_SIZE, TILE_SIZE, tetrominoColors[color-1]);
            }

            DrawRectangleLines(x * TILE_SIZE + startOffsetX, y * TILE_SIZE + startOffsetY, TILE_SIZE, TILE_SIZE, BLACK);
        }
    }
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

int DeleteCompletedLines()
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

void AddScore(const int score_to_add)
{
    actual_score +=score_to_add;

    actual_falling_speed = MAX_FALL_SPEED - (actual_score * 0.0001f); 
    actual_falling_speed = MIN(actual_falling_speed,MAX_FALL_SPEED);
    actual_falling_speed = MAX(actual_falling_speed,MIN_FALL_SPEED);

    float moveTetrominoDownTimer = actual_falling_speed;
}

void SpawnNewPlayerTetromino()
{
    currentTetrominoX = tetrominoStartX;
    currentTetrominoY = tetrominoStartY;

    do
    {
        currentTetrominoType = GetRandomValue(0, 6);
    } while (currentTetrominoType == last_tetramino_type);
    last_tetramino_type = currentTetrominoType;
    
    currentRotation = 0;
    currentColor = currentTetrominoType+1;

    if(CheckCollision(currentTetrominoX,currentTetrominoY,&currentTetrominoType))
    {
        actual_game_scene->OnExit(); //the game now is in game over loop
        TraceLog(LOG_INFO,"Game over!");
    }
}

void PlayerInputManager()
{
    if (IsKeyPressed(KEY_SPACE))
    {
        const int lastRotation = currentRotation;
        currentRotation++;

        if (currentRotation > 3)
        {
            currentRotation = 0;
        }

        if (CheckCollision(currentTetrominoX,currentTetrominoY,tetrominoTypes[currentTetrominoType][currentRotation]))
        {
            currentRotation = lastRotation;
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

            const int deleted_lines = DeleteCompletedLines();
            AddScore((deleted_lines * 1.5f) *LINE_POINTS);

            
            SpawnNewPlayerTetromino();
        }
    }
}

void GameLoopOnEnter()
{
    float timeToMoveTetrominoDown = actual_falling_speed;
    counterToShowCompletedLineEffect = timerToShowCompletedLineEffect;

    currentMusic = &music_ingame2;
    PlayMusicStream(*currentMusic);

    SpawnNewPlayerTetromino();
}

void GameLoopOnExit()
{
    actual_game_scene = &gameover_Scene;
}

void GameLoop()
{
    UpdateMusicStream(*currentMusic);

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
    ClearBackground(clear_color);

    DrawText(TextFormat("Score: %08i", actual_score), 20, 10, 20, RED);
    DrawText(TextFormat("Highscore: %08i", highscore), 20, 30, 20, GREEN);
    DrawText(TextFormat("Vertical Speed: %.3f", (1 - actual_falling_speed) ), 20, 50, 20, YELLOW);

    DrawStageTetrominos();

    DrawPlayerTetromino(tetrominoColors[currentColor], tetrominoTypes[currentTetrominoType][currentRotation]);

    if(is_game_paused) 
        DrawText("GAME PAUSED", startOffsetX, WINDOW_HEIGHT/2, 40, YELLOW);

    EndDrawing();
}

void GamePausedLoop()
{

}

void GameIntroLoop()
{

}

void GameOverLoop()
{

}

void SwitchScene(GameScene newScene)
{
    if(actual_game_scene && actual_game_scene->OnExit)actual_game_scene->OnExit();

    actual_game_scene = &newScene;

    if(actual_game_scene->OnEnter)actual_game_scene->OnEnter();
}

int main(int argc, char** argv)
{
    time_t unixTime;
    time(&unixTime);
    SetRandomSeed(unixTime);

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Tetris In RayLib by Marco Baldini");
    
    InitAudioDevice();

    //load audios
    sfx_hit_piece = LoadSound("assets/audio/hit.wav");
    sfx_line_completed = LoadSound("assets/audio/line_completed.wav");

    music_ingame1 = LoadMusicStream("assets/music/in_game1.wav");
    music_ingame2 = LoadMusicStream("assets/music/in_game2.wav");
    music_gameover = LoadMusicStream("assets/music/game_over.wav");
    music_startmenu = LoadMusicStream("assets/music/start_menu.wav");

    SetTargetFPS(60);

    //generate scenes
    maingame_Scene.Loop = GameLoop;
    maingame_Scene.OnEnter = GameLoopOnEnter;
    maingame_Scene.OnExit = GameLoopOnExit;

    //load scene
    actual_game_scene = NULL;

    SwitchScene(maingame_Scene);

    while(!WindowShouldClose())
    {
        actual_game_scene->Loop();        
    }

    UnloadSound(sfx_hit_piece);     // Unload sound data
    UnloadSound(sfx_line_completed);     // Unload sound data
    UnloadMusicStream(music_ingame1);
    UnloadMusicStream(music_ingame2);
    UnloadMusicStream(music_gameover);
    UnloadMusicStream(music_startmenu);

    CloseAudioDevice();     // Close audio device

    CloseWindow();

    return 0;
}