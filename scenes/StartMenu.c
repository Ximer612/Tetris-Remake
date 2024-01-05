// #include <raylib.h>
// #include <scenes.h>
// #include <Tetris.h>
// #include <extra_colors.h>
// #include <raylib_custom_functions.h>

// extern Music* current_music;
// extern Music music_startmenu;
// extern GameScene* maingame_scene;
// extern GameScene* actual_game_scene;
// extern int highscore;

// void StartMenuLoop()
// {
//     CustomPrint("loop!");

//     UpdateMusicStream(*current_music);

//     if(IsKeyPressed(KEY_ENTER))
//     {
//         SwitchScene(maingame_scene,actual_game_scene);
//         return;
//     }

//     BeginDrawing();

//     ClearBackground(TETRIS_CYAN);
//                                         // - (words count * font size) / 2
//     DrawText("TETRIS IN RAYLIB!", WINDOW_WIDTH/2 - 200, 20, 40, TETRIS_BLUE_PURPLE);
//     DrawText("PRESS [ENTER] TO START GAME! ", WINDOW_WIDTH/2 - 200, WINDOW_HEIGHT/2, 20, TETRIS_YELLOW);
//     DrawText(TextFormat("Last Highscore: %08i", highscore), 20, 90, 20, TETRIS_LIGHT_GREEN);

//     EndDrawing();
// }

// void StartMenuOnEnter()
// {
//     current_music = &music_startmenu;
//     PlayMusicStream(*current_music);

// }
