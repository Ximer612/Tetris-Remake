#ifndef SCENE_DEFINE
#define SCENE_DEFINE

typedef struct{

    char* scene_name;
    void (*OnEnter)();
    void (*Loop)();
    void (*OnExit)();

} GameScene;

void SwitchScene(GameScene* newScene, GameScene** actual_game_scene);
#endif