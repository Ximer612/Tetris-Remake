#include <raylib.h>
#include <time.h>
#include <string.h>
#define TETRIS_DEFINITIONS
#include <Tetris.h>
#include <math.h>
#include <raylib_custom_functions.h>

const Color clear_color = {40,40,40,255};

//CONST VARIABLES
const int startOffsetX = (WINDOW_WIDTH / 2) - ((STAGE_WIDTH * TILE_SIZE) / 2);
const int startOffsetY = (WINDOW_HEIGHT / 2) - ((STAGE_HEIGHT * TILE_SIZE) / 2);
const int tetrominoStartX = STAGE_WIDTH / 2 - 2;
const int tetrominoStartY = 0;

//GAME SCENES VARIABLES
typedef struct{

    char* scene_name;
    void (*OnEnter)();
    void (*Loop)();
    void (*OnExit)();

} GameScene;

int is_game_paused = 0;
int is_waiting_for_ending_effect = 0;

GameScene* actual_game_scene;

GameScene gameover_scene;
GameScene maingame_scene;
GameScene startmenu_scene;

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

void SwitchScene(GameScene* newScene)
{
    if(actual_game_scene && actual_game_scene->OnExit)
    {
        actual_game_scene->OnExit();
    }

    actual_game_scene = newScene;

    if(actual_game_scene->OnEnter)
    {
        actual_game_scene->OnEnter();
    }

}

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

void AddScore(const int score_to_add)
{
    actual_score +=score_to_add;

    actual_falling_speed = MAX_FALL_SPEED - (actual_score * 0.0001f); 
    actual_falling_speed = MIN(actual_falling_speed,MAX_FALL_SPEED);
    actual_falling_speed = MAX(actual_falling_speed,MIN_FALL_SPEED);

    float moveTetrominoDownTimer = actual_falling_speed;
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

            const int deleted_lines = DeleteCompletedLines(to_remove_lines);
            AddScore((deleted_lines * 1.5f) *LINE_POINTS);
            
            if(SpawnNewPlayerTetromino(tetrominoStartX, tetrominoStartY, &currentTetrominoX, &currentTetrominoY, &currentTetrominoType, &last_tetramino_type, &currentRotation, &currentColor))
            {
                SwitchScene(&gameover_scene);
            }
        }
    }
}

void MainGameOnEnter()
{
    memcpy(stage,stage_empty,sizeof(stage));

    actual_score = 0;

    float timeToMoveTetrominoDown = actual_falling_speed;
    counterToShowCompletedLineEffect = timerToShowCompletedLineEffect;

    currentMusic = &music_ingame2;
    PlayMusicStream(*currentMusic);

    SpawnNewPlayerTetromino(tetrominoStartX, tetrominoStartY, &currentTetrominoX, &currentTetrominoY, &currentTetrominoType, &last_tetramino_type, &currentRotation, &currentColor);
}

void MainGameLoop()
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

void GameOverLoop()
{
    UpdateMusicStream(*currentMusic);

    if(IsKeyPressed(KEY_ENTER))
    {
        SwitchScene(&maingame_scene);
        return;
    }

    BeginDrawing();

    ClearBackground(BLACK);

                                        // - (words count * font size) / 2
    DrawText("GAME OVER!", WINDOW_WIDTH/2 - 60, 10, 20, RED);
    DrawText("PRESS [ENTER] TO CONTINUE! ", WINDOW_WIDTH/2 - 150, 30, 20, RED);
    DrawText(TextFormat("Score: %08i", actual_score), 20, 70, 20, RED);
    DrawText(TextFormat("Highscore: %08i", highscore), 20, 90, 20, GREEN);

    EndDrawing();
}

void GameOverOnEnter()
{
    if(actual_score > highscore) highscore = actual_score;

    currentMusic = &music_gameover;
    PlayMusicStream(*currentMusic);
}

void StartMenuLoop()
{
    UpdateMusicStream(*currentMusic);

    if(IsKeyPressed(KEY_ENTER))
    {
        SwitchScene(&maingame_scene);
        return;
    }

    BeginDrawing();

    ClearBackground(BLACK);
                                        // - (words count * font size) / 2
    DrawText("TETRIS IN RAYLIB!", WINDOW_WIDTH/2 - 100, 10, 20, YELLOW);
    DrawText("PRESS [ENTER] TO START GAME! ", WINDOW_WIDTH/2 - 150, 30, 20, GREEN);
    DrawText(TextFormat("Last Highscore: %08i", highscore), 20, 90, 20, PURPLE);

    EndDrawing();
}

void StartMenuOnEnter()
{
    currentMusic = &music_startmenu;
    PlayMusicStream(*currentMusic);
}

int main(int argc, char** argv)
{
    //init random to current time
    time_t unixTime;
    time(&unixTime);
    SetRandomSeed(unixTime);

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Tetris In RayLib by Marco Baldini");
    
    InitAudioDevice();

    //load sfxs
    sfx_hit_piece = LoadSound("assets/audio/hit.wav");
    sfx_line_completed = LoadSound("assets/audio/line_completed.wav");

    //load musics
    music_ingame1 = LoadMusicStream("assets/music/in_game1.wav");
    SetMusicVolume(music_ingame1,0.3f);
    music_ingame2 = LoadMusicStream("assets/music/in_game2.wav");
    SetMusicVolume(music_ingame2,0.3f);
    music_gameover = LoadMusicStream("assets/music/game_over.wav");
    SetMusicVolume(music_gameover,0.3f);
    music_startmenu = LoadMusicStream("assets/music/start_menu.wav");
    SetMusicVolume(music_startmenu,0.3f);

    SetTargetFPS(60);

    //generate scenes
    maingame_scene.Loop = MainGameLoop;
    maingame_scene.OnEnter = MainGameOnEnter;
    maingame_scene.OnExit = NULL;
    maingame_scene.scene_name = "Main game";

    gameover_scene.Loop = GameOverLoop;
    gameover_scene.OnEnter = GameOverOnEnter;
    gameover_scene.OnExit = NULL;
    gameover_scene.scene_name = "Game over";

    startmenu_scene.Loop = StartMenuLoop;
    startmenu_scene.OnEnter = StartMenuOnEnter;
    startmenu_scene.OnExit = NULL;
    startmenu_scene.scene_name = "Start menu";

    //load scene
    actual_game_scene = NULL;
    SwitchScene(&startmenu_scene);

    while(!WindowShouldClose())
    {
        //CustomPrint(actual_game_scene->scene_name);
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