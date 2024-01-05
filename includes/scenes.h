
#ifndef SCENE_DEFINE
#define SCENE_DEFINE

typedef struct{

    char* scene_name;
    void (*OnEnter)();
    void (*Loop)();
    void (*OnExit)();

} GameScene;

#endif

void SwitchScene(GameScene* newScene, GameScene** actual_game_scene2);

//void StartMenuLoop();
//void StartMenuOnEnter();

