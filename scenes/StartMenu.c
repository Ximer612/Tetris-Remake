#include <raylib.h>
#include <scenes.h>
#include <tetris.h>
#include <extra_colors.h>
#include <game_settings.h>

extern Sound sfx_start_game;
extern Music* current_music;
extern Music music_startmenu;
extern GameScene maingame_scene;
extern GameScene* actual_game_scene;
extern int highscore;
extern Texture2D tetromino_texture;

const Vector2 back_tetromino_pos = {WINDOW_WIDTH/2,WINDOW_HEIGHT/2+150};
const float back_tetromino_size = 5;

float back_tetromino_rot = 0;
float back_tetromino_rot_speed = 10;

void StartMenuLoop()
{
    UpdateMusicStream(*current_music);

    if(IsKeyPressed(KEY_ENTER))
    {
        SwitchScene(&maingame_scene,&actual_game_scene);
        return;
    }

    back_tetromino_rot += GetFrameTime() * back_tetromino_rot_speed;

    BeginDrawing();

    ClearBackground(TETRIS_CYAN);
                                        // - (words count * font size) / 2
    DrawTextureEx(tetromino_texture,back_tetromino_pos,back_tetromino_rot,back_tetromino_size,TETROMINO_RED);
    DrawTextureEx(tetromino_texture,back_tetromino_pos,back_tetromino_rot+90,back_tetromino_size,TETROMINO_GREEN);
    DrawTextureEx(tetromino_texture,back_tetromino_pos,back_tetromino_rot+180,back_tetromino_size,TETROMINO_BLUE);
    DrawTextureEx(tetromino_texture,back_tetromino_pos,back_tetromino_rot+270,back_tetromino_size,TETROMINO_YELLOW);
    DrawText("TETRIS IN RAYLIB!", WINDOW_WIDTH/2 - 200, 20, 40, TETRIS_BLUE_PURPLE);
    DrawText("PRESS [ENTER] TO START GAME! ", WINDOW_WIDTH/2 - 180, WINDOW_HEIGHT/2-100, 20, TETRIS_YELLOW);
    DrawText(TextFormat("Last Highscore: %08i", highscore), 20, 90, 20, TETRIS_LIGHT_GREEN);

    EndDrawing();
}

void StartMenuOnEnter()
{
    current_music = &music_startmenu;
    PlayMusicStream(*current_music);
}

void StartMenuOnExit()
{
    SetSoundVolume(sfx_start_game,2.f);
    PlaySound(sfx_start_game);
}