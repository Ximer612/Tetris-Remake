#include <scenes.h>

/// @brief Switches scene from the current to the newScene
/// @param newScene scene to be the new actual scene and to call OnEnter()
/// @param actual_game_scene pointer to the current scene to call OnExit()
void SwitchScene(GameScene* newScene, GameScene** actual_game_scene)
{
    if (*actual_game_scene && (*actual_game_scene)->OnExit)
    {
        (*actual_game_scene)->OnExit();
    }

    *actual_game_scene = newScene;

    if ((*actual_game_scene)->OnEnter)
    {
        (*actual_game_scene)->OnEnter();
    }
}