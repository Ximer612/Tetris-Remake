#include <raylib.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <raylib_custom_functions.h>
#include <scenes.h>
#include <game_settings.h>

//GAME SCENES VARIABLES
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
Texture2D tetromino_texture;

//PLAYER VARIABLES
int actual_score;
int highscore = 0;

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
    tetromino_texture = LoadTexture("assets/texture/tetrisPiece.png");
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