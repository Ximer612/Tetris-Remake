// #include <raylib.h>

// Sound sfx_hit_piece;
// Sound sfx_line_completed;
// Music music_ingame1;
// Music music_ingame2;
// Music music_gameover;
// Music music_startmenu;

// Music* currentMusic;

// int actual_score;
// int highscore;

// float actual_falling_speed = MAX_FALL_SPEED;

// bool is_game_paused = false;

// int currentColor;
// int currentTetrominoType;
// int currentRotation;
// int currentTetrominoX;
// int currentTetrominoY;

// const int startOffsetX = (WINDOW_WIDTH / 2) - ((STAGE_WIDTH * TILE_SIZE) / 2);
// const int startOffsetY = (WINDOW_HEIGHT / 2) - ((STAGE_HEIGHT * TILE_SIZE) / 2);

// const int tetrominoStartX = STAGE_WIDTH / 2 - 2;
// const int tetrominoStartY = 0;

// float timeToMoveTetrominoDown;

// typedef struct{

//     void (*OnEnter)();
//     void (*Loop)();
//     void (*OnExit)();

// } GameScene;

// GameScene* actual_game_scene;

// GameScene gameover_Scene;
// GameScene maingame_Scene;
// GameScene startmenu_Scene;
// GameScene pause_Scene;


// void GameLoopOnEnter()
// {
    
// }

// void GameLoopOnExit()
// {
//     actual_game_scene = &gameover_Scene;
// }

// void GameLoop()
// {
//     // if(IsKeyPressed(KEY_P))
//     // {
//     //     is_game_paused = !is_game_paused;
//     // }

//     UpdateMusicStream(*currentMusic);

//     timeToMoveTetrominoDown -= GetFrameTime();

//     if(IsKeyPressed(KEY_E))
//     {
//         AddScore(actual_score*2);
//     }

//     if (IsKeyPressed(KEY_SPACE))
//     {
//         const int lastRotation = currentRotation;
//         currentRotation++;

//         if (currentRotation > 3)
//         {
//             currentRotation = 0;
//         }

//         if (CheckCollision(currentTetrominoX,currentTetrominoY,tetrominoTypes[currentTetrominoType][currentRotation]))
//         {
//             currentRotation = lastRotation;
//         }
//     }

//     if (IsKeyPressed(KEY_RIGHT))
//     {
//         // No need to check overflow, wall is your protector
//         if (!CheckCollision(currentTetrominoX+1,currentTetrominoY,tetrominoTypes[currentTetrominoType][currentRotation]))
//         {
//             currentTetrominoX++;
//         }
//     }
//     if (IsKeyPressed(KEY_LEFT))
//     {
//         // No need to check overflow, wall is your protector
//         if (!CheckCollision(currentTetrominoX-1,currentTetrominoY,tetrominoTypes[currentTetrominoType][currentRotation]))
//         {
//             currentTetrominoX--;
//         }
//     }

//     if(timeToMoveTetrominoDown <= 0 || IsKeyPressed(KEY_DOWN))
//     {            
//         AddScore(DOWN_POINTS);

//         if(!CheckCollision(currentTetrominoX,currentTetrominoY+1,tetrominoTypes[currentTetrominoType][currentRotation]))
//         {
//             currentTetrominoY++;
//             timeToMoveTetrominoDown = actual_falling_speed;
//         }

//         else
//         {
//             for(int y = 0; y < TETROMINO_SIZE; y++)
//             {
//                 for(int x = 0; x < TETROMINO_SIZE; x++)
//                 {
//                     const int offset = y * TETROMINO_SIZE + x;

//                     const int *tetromino = tetrominoTypes[currentTetrominoType][currentRotation];

//                     if(tetromino[offset] == 1)
//                     {
//                         const int offset_stage = (y + currentTetrominoY) * STAGE_WIDTH + (x + currentTetrominoX);
//                         stage[offset_stage] = currentColor+1;
//                     }
//                 }
//             }

//             SetSoundPitch(sfx_hit_piece,GetRandomValue(5,150)*0.01f); // from 0.05f to 1.50f
//             PlaySound(sfx_hit_piece);

//             const int deleted_lines = DeleteCompletedLines();
//             AddScore((deleted_lines * 1.5f) *LINE_POINTS);
            
//             SpawnNewPlayerTetromino();
//         }
//     }

//     BeginDrawing();
//     ClearBackground(clear_color);

//     DrawText(TextFormat("Score: %08i", actual_score), 20, 10, 20, RED);
//     DrawText(TextFormat("Highscore: %08i", highscore), 20, 30, 20, GREEN);
//     DrawText(TextFormat("Vertical Speed: %.3f", (1 - actual_falling_speed) ), 20, 50, 20, YELLOW);
//     DrawText("GAME PAUSED", 0, WINDOW_HEIGHT/2, 40, YELLOW);

//     //if(is_game_paused) 

//     DrawStageTetrominos();

//     DrawPlayerTetromino(tetrominoColors[currentColor], tetrominoTypes[currentTetrominoType][currentRotation]);

//     EndDrawing();
// }

// void GamePausedLoop()
// {

// }

// void GameIntroLoop()
// {

// }

// void GameOverLoop()
// {

// }

// int main(int argc, char** argv)
// {
//     float timeToMoveTetrominoDown = actual_falling_speed;

//     time_t unixTime;
//     time(&unixTime);
//     SetRandomSeed(unixTime);

//     SpawnNewPlayerTetromino();

//     InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Tetris In RayLib by Marco Baldini");
    
//     InitAudioDevice();

//     sfx_hit_piece = LoadSound("assets/audio/hit.wav");
//     sfx_line_completed = LoadSound("assets/audio/line_completed.wav");

//     music_ingame1 = LoadMusicStream("assets/music/in_game1.wav");
//     music_ingame2 = LoadMusicStream("assets/music/in_game2.wav");
//     music_gameover = LoadMusicStream("assets/music/game_over.wav");
//     music_startmenu = LoadMusicStream("assets/music/start_menu.wav");

//     currentMusic = &music_ingame1;

//     PlayMusicStream(*currentMusic);

//     SetTargetFPS(60);

//     maingame_Scene.Loop = GameLoop;
//     maingame_Scene.OnEnter = GameLoopOnEnter;
//     maingame_Scene.OnExit = GameLoopOnExit;

//     actual_game_scene = &maingame_Scene;

//     actual_game_scene->OnEnter();

//     while(!WindowShouldClose())
//     {
//         actual_game_scene->Loop();
        
//     }

//     UnloadSound(sfx_hit_piece);     // Unload sound data
//     UnloadSound(sfx_line_completed);     // Unload sound data
//     UnloadMusicStream(music_ingame1);
//     UnloadMusicStream(music_ingame2);
//     UnloadMusicStream(music_gameover);
//     UnloadMusicStream(music_startmenu);

//     CloseAudioDevice();     // Close audio device

//     CloseWindow();

//     return 0;
// }