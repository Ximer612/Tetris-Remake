#include <scenes.h>

void SwitchScene(GameScene* newScene, GameScene** actual_game_scene)
{
    if(*actual_game_scene && (*actual_game_scene)->OnExit)
    {
        (*actual_game_scene)->OnExit();
    }

    *actual_game_scene = newScene;

    if((*actual_game_scene)->OnEnter)
    {
        (*actual_game_scene)->OnEnter();
    }
}