#include <raylib.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#define TETRIS_DEFINITIONS
#include <tetris.h>
#include <math.h>
#include <raylib_custom_functions.h>
#include <extra_colors.h>
#include <scenes.h>

//CONST VARIABLES
const int startOffsetX = (WINDOW_WIDTH / 2) - ((STAGE_WIDTH * TILE_SIZE) / 2);
const int startOffsetY = (WINDOW_HEIGHT / 2) - ((STAGE_HEIGHT * TILE_SIZE) / 2);
const int tetrominoStartX = STAGE_WIDTH / 2 - 2;
const int tetrominoStartY = 0;

//GAME SCENES VARIABLES

int is_game_paused = 0;
int is_waiting_for_ending_effect = 0;

GameScene* actual_game_scene;

GameScene gameover_scene;
GameScene maingame_scene;
GameScene startmenu_scene;

//AUDIOS

Sound sfx_hit_piece;
Sound sfx_line_completed;
Music music_ingame1;
Music music_ingame2;
Music music_gameover;
Music music_startmenu;

Music* current_music;

//TEXTURES

Texture2D tetrominoTexture;

//PLAYER VARIABLES

int actual_score;
int highscore = 0;

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


void MainGameOnEnter()
{
    memcpy(stage,stage_empty,sizeof(stage));

    actual_score = 0;

    float timeToMoveTetrominoDown = actual_falling_speed;
    counterToShowCompletedLineEffect = timerToShowCompletedLineEffect;

    current_music = &music_ingame2;
    PlayMusicStream(*current_music);

    SpawnNewPlayerTetromino(tetrominoStartX, tetrominoStartY, &currentTetrominoX, &currentTetrominoY, &currentTetrominoType, &last_tetramino_type, &currentRotation, &currentColor);
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

    DrawStageTetrominos(tetrominoTexture,currentTetrominoX,currentTetrominoY,startOffsetX,startOffsetY);

    DrawPlayerTetromino(tetrominoTexture,tetrominoColors[currentColor],currentTetrominoX,currentTetrominoY,startOffsetX,startOffsetY, tetrominoTypes[currentTetrominoType][currentRotation]);

    if(is_game_paused) 
    {
        DrawRectangle(0,0,WINDOW_WIDTH,WINDOW_HEIGHT,TETRIS_BLACK_OPAQUE);
        DrawText("GAME PAUSED", startOffsetX, WINDOW_HEIGHT/2-50, 40, TETRIS_YELLOW);
        DrawText("Press [ESC] to quit!", startOffsetX-50, WINDOW_HEIGHT/2+20, 40, TETRIS_DARK_RED);

    }

    EndDrawing();
}

void MainGameOnExit()
{
    if(actual_score > highscore)
    {
        highscore = actual_score;
        SaveFileText(SAVE_PATH,(char*)TextFormat("%d",highscore));
    } 
}

void StartMenuOnEnter()
{
    current_music = &music_startmenu;
    PlayMusicStream(*current_music);

}

void GameOverLoop()
{
    UpdateMusicStream(*current_music);

    if(IsKeyPressed(KEY_ENTER))
    {
        SwitchScene(&maingame_scene,&actual_game_scene);
        return;
    }

    BeginDrawing();

    ClearBackground(TETRIS_DARKEST_GRAY);
                                        // - (words count * font size) / 2
    DrawText("GAME OVER!", WINDOW_WIDTH/2 - 130, 20, 40, TETRIS_DARK_RED);
    DrawText("PRESS [ENTER] TO CONTINUE! ", WINDOW_WIDTH/2 - 150, WINDOW_HEIGHT/2, 20, TETRIS_YELLOW);
    DrawText(TextFormat("Last Score: %08i", actual_score), 20, 70, 20, TETRIS_BLUE_PURPLE);
    DrawText(TextFormat("Highscore: %08i", highscore), 20, 90, 20, TETRIS_LIGHT_GREEN);

    EndDrawing();
}

void GameOverOnEnter()
{
    current_music = &music_gameover;
    PlayMusicStream(*current_music);
}

void StartMenuLoop()
{
    UpdateMusicStream(*current_music);

    if(IsKeyPressed(KEY_ENTER))
    {
        SwitchScene(&maingame_scene,&actual_game_scene);
        return;
    }

    BeginDrawing();

    ClearBackground(TETRIS_CYAN);
                                        // - (words count * font size) / 2
    DrawText("TETRIS IN RAYLIB!", WINDOW_WIDTH/2 - 200, 20, 40, TETRIS_BLUE_PURPLE);
    DrawText("PRESS [ENTER] TO START GAME! ", WINDOW_WIDTH/2 - 200, WINDOW_HEIGHT/2, 20, TETRIS_YELLOW);
    DrawText(TextFormat("Last Highscore: %08i", highscore), 20, 90, 20, TETRIS_LIGHT_GREEN);

    EndDrawing();
}


void LoadAudios()
{
    InitAudioDevice();

    //load assets
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
}

void LoadTextures()
{
    //load textures
    tetrominoTexture = LoadTexture("assets/texture/tetrisPiece.png");
    tetrominoTexture.width=TILE_SIZE;
    tetrominoTexture.height=TILE_SIZE;
}

void InitScenes()
{
    //generate scenes
    maingame_scene.Loop = MainGameLoop;
    maingame_scene.OnEnter = MainGameOnEnter;
    maingame_scene.OnExit = MainGameOnExit;
    maingame_scene.scene_name = "Main game";

    gameover_scene.Loop = GameOverLoop;
    gameover_scene.OnEnter = GameOverOnEnter;
    gameover_scene.OnExit = NULL;
    gameover_scene.scene_name = "Game over";

    startmenu_scene.Loop = StartMenuLoop;
    startmenu_scene.OnEnter = StartMenuOnEnter;
    startmenu_scene.OnExit = NULL;
    startmenu_scene.scene_name = "Start menu";

}



int main(int argc, char** argv)
{
    //init random to current time
    time_t unixTime;
    time(&unixTime);
    SetRandomSeed(unixTime);

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Tetris In RayLib by Marco Baldini");

    LoadAudios();
    LoadTextures();
    InitScenes();
    
    SetTargetFPS(60);

    //load scene
    actual_game_scene = NULL;

    SwitchScene(&startmenu_scene,&actual_game_scene);

    //load file with highscore
    if(FileExists(SAVE_PATH))
    {
        char* text = LoadFileText(SAVE_PATH);
        if(text)
        {
            highscore = atoi(text);
        }
    }

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