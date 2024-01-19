#include <raylib.h>
#include <scenes.h>
#include <tetris.h>
#include <extra_colors.h>
#include <game_settings.h>

extern Sound sfx_explosion;
extern Music* current_music;
extern Music music_gameover;
extern GameScene maingame_scene;
extern GameScene* actual_game_scene;
extern int highscore;
extern int actual_score;

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

    PlaySound(sfx_explosion);
}