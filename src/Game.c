#include <raylib.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <raylib_custom_functions.h>
#include <scenes.h>
#include <game_settings.h>
#include <console_utilities.h>

//GAME SCENES VARIABLES
GameScene* actual_game_scene;

GameScene gameover_scene;
GameScene maingame_scene;
GameScene startmenu_scene;

//AUDIOS
Sound sfx_hit_piece;
Sound sfx_line_completed;
Sound sfx_explosion;
Sound sfx_move_tetromino;
Sound sfx_rotate_tetromino;
Sound sfx_start_game;

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

/// @brief Loads all audios (musics and sfxs)
void LoadAudios()
{
    //load sfxs
    sfx_hit_piece = LoadSound("assets/audio/hit.wav");
    sfx_line_completed = LoadSound("assets/audio/line_completed.wav");
    sfx_explosion = LoadSound("assets/audio/explosion.wav");
    sfx_move_tetromino = LoadSound("assets/audio/move_tetromino.wav");
    sfx_start_game = LoadSound("assets/audio/start_game.wav");
    sfx_rotate_tetromino = LoadSound("assets/audio/rotate_tetromino.wav");

    SetSoundVolume(sfx_hit_piece, 0.2f);
    SetSoundVolume(sfx_line_completed, 0.2f);
    SetSoundVolume(sfx_explosion, 0.2f);
    SetSoundVolume(sfx_move_tetromino, 0.2f);
    SetSoundVolume(sfx_start_game, 0.1f);
    SetSoundVolume(sfx_rotate_tetromino, 0.3f);

    //load musics
    music_ingame1 = LoadMusicStream("assets/music/in_game1.wav");
    music_ingame2 = LoadMusicStream("assets/music/in_game2.wav");
    music_gameover = LoadMusicStream("assets/music/game_over.wav");
    music_startmenu = LoadMusicStream("assets/music/start_menu.wav");

    SetMusicVolume(music_gameover, 0.1f);
    SetMusicVolume(music_ingame1, 0.1f);
    SetMusicVolume(music_ingame2, 0.1f);
    SetMusicVolume(music_startmenu, 0.1f);
}

/// @brief Loads all textures
void LoadTextures()
{
    tetromino_texture = LoadTexture("assets/texture/tetrisPiece.png");
}

//get extern scenes & functions
extern void StartMenuOnExit();
extern void StartMenuLoop();
extern void StartMenuOnEnter();

extern void GameOverLoop();
extern void GameOverOnEnter();

extern void MainGameOnExit();
extern void MainGameOnEnter();
extern void MainGameLoop();

/// @brief Creates scenes and assigns their functions and names
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
    startmenu_scene.OnExit = StartMenuOnExit;
    startmenu_scene.scene_name = "Start menu";
}

/// @brief Unload all audios
void UnloadAudios() {
    UnloadSound(sfx_hit_piece);
    UnloadSound(sfx_line_completed);
    UnloadMusicStream(music_ingame1);
    UnloadMusicStream(music_ingame2);
    UnloadMusicStream(music_gameover);
    UnloadMusicStream(music_startmenu);
}

/// @brief Checks if there is a save in the save path and loads the highscore
void LoadLastHighScore()
{
    //load file with highscore
    if (FileExists(SAVE_PATH))
    {
        char* text = LoadFileText(SAVE_PATH);
        if (text)
        {
            highscore = atoi(text);
        }
    }
}

int main(int argc, char** argv)
{
    //init random to current time
    time_t unixTime;
    time(&unixTime);
    SetRandomSeed(unixTime);

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Tetris In RayLib by Marco Baldini");

    InitAudioDevice();
    LoadAudios();
    LoadTextures();
    InitScenes();

    SetTargetFPS(60);

    LoadLastHighScore();

    //load scene
    actual_game_scene = NULL;
    SwitchScene(&startmenu_scene, &actual_game_scene);

    //game scene loop
    while (!WindowShouldClose())
    {
        actual_game_scene->Loop();
    }

    UnloadAudios();
    CloseAudioDevice();     // Close audio device
    CloseWindow();
    return 0;
}