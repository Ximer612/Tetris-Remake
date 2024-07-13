#include <raylib.h>
#include <scenes.h>
#define TETRIS_DEFINITIONS
#include <tetris.h>
#include <extra_colors.h>
#include <string.h>
#include <game_settings.h>
#include <stdlib.h>
#include <singly_linked.h>

enum HorizontalKey {LEFT = -1,RIGHT = 1 };

//EXTERN VARIABLES
extern Music* current_music;
extern Music music_ingame2;
extern Sound sfx_hit_piece;
extern Sound sfx_rotate_tetromino;
extern Sound sfx_move_tetromino;
extern Texture2D tetromino_texture;
extern int highscore;
extern int actual_score;
extern GameScene gameover_scene;
extern GameScene* actual_game_scene;

//CONST VARIABLES
const int start_offset_x = (WINDOW_WIDTH / 2) - ((STAGE_WIDTH * TILE_SIZE) / 2);
const int start_offset_y = (WINDOW_HEIGHT / 2) - ((STAGE_HEIGHT * TILE_SIZE) / 2);
const int tetromino_start_x = STAGE_WIDTH / 2 - 2;
const int tetromino_start_y = 0;
const int next_tetromino_start_x = WINDOW_WIDTH - 130;
const int next_tetromino_start_y = 50;

//GAME VARIABLES
int is_game_paused = 0;
int is_waiting_for_ending_effect = 0;

//PLAYER VARIABLES
float actual_falling_speed = MAX_FALL_SPEED;
int current_player_color;
int current_tetromino_type;
int current_rotation;
int current_tetromino_x;
int current_tetromino_y;
int last_tetramino_type = -1;
KeyboardKey last_pressed_key;

//const int num_of_next_pieces = 4;
#define num_of_next_pieces 5
int_singly_list_item* next_tetrominos;

//TIMERS
float counter_to_move_tetromino_down;

//const float timer_to_show_completed_line_effect = 0.5f;
#define timer_to_show_completed_line_effect 0.5f
float counter_to_show_completed_line_effect = timer_to_show_completed_line_effect;

//const float timer_to_continuous_movement = 0.2f;
#define timer_to_continuous_movement 0.1f
#define default_counter_to_continous_movement_value -1
float counter_to_continuous_movement = default_counter_to_continous_movement_value;

//EFFECT VARIABLES
int to_remove_lines[STAGE_HEIGHT];
int index_color_effect = 0;

/// @brief Saves the actual score on file if it's greater than the old highscore
void MainGameOnExit()
{
    if (actual_score > highscore)
    {
        highscore = actual_score;
        SaveFileText(SAVE_FILE, (char*)TextFormat("%d", highscore));
    }
}

/// @brief Resets the current stage, plays the main game music and generates the next tetrominos
void MainGameOnEnter()
{
    memcpy(stage, stage_empty, STAGE_HEIGHT * STAGE_WIDTH * sizeof(int));

    actual_score = 0;

    float timeToMoveTetrominoDown = actual_falling_speed;
    counter_to_show_completed_line_effect = timer_to_show_completed_line_effect;

    current_music = &music_ingame2;
    PlayMusicStream(*current_music);

    //generate next pieces

    if (next_tetrominos) free(next_tetrominos);

    next_tetrominos = NULL;

    SetNextPieces(&next_tetrominos, num_of_next_pieces);

    SpawnNewPlayerTetromino(&next_tetrominos, tetromino_start_x, tetromino_start_y, &current_tetromino_x, &current_tetromino_y, &current_tetromino_type, &last_tetramino_type, &current_rotation, &current_player_color);
}

/// @brief Recalculate the falling speed with that formula MAX_FALL_SPEED - (actual_score * 0.0001f) and clamps it between the max and the min
void RecalculateFallingSpeed()
{
    actual_falling_speed = MAX_FALL_SPEED - (actual_score * 0.0001f);
    actual_falling_speed = min(actual_falling_speed, MAX_FALL_SPEED);
    actual_falling_speed = max(actual_falling_speed, MIN_FALL_SPEED);
}

/// @brief Adds the score to the actual score and recalculate the falling speed
/// @param amount amount of score to add
void AddScore(const int amount)
{
    actual_score += amount;
    RecalculateFallingSpeed();
}

/// @brief Play the input sound with a random pitch from 0.05f to 1.50f
/// @param sound Sound to play
void PlayWithRandomPitch(Sound sound)
{
    SetSoundPitch(sound, GetRandomValue(5, 150) * 0.01f); // from 0.05f to 1.50f
    PlaySound(sound);
}

/// @brief Sets the player tetromino's blocks to the stage with the right color
void SetPlayerTetrominoToStage()
{
    for (int y = 0; y < TETROMINO_SIZE; y++)
    {
        for (int x = 0; x < TETROMINO_SIZE; x++)
        {
            const int offset = y * TETROMINO_SIZE + x;

            const int* tetromino = tetromino_types[current_tetromino_type][current_rotation];

            if (tetromino[offset] == 1)
            {
                const int offset_stage = (y + current_tetromino_y) * STAGE_WIDTH + (x + current_tetromino_x);
                stage[offset_stage] = current_player_color + 1;
            }
        }
    }
}

/// @brief Checks for the tetromino to go down on timer or on key pressed
void TetrominoDownMovementCheck()
{
    if (counter_to_move_tetromino_down <= 0 || IsKeyPressed(KEY_DOWN))
    {
        AddScore(DOWN_POINTS);

        if (!CheckCollision(current_tetromino_x, current_tetromino_y + 1, tetromino_types[current_tetromino_type][current_rotation]))
        {
            current_tetromino_y++;
            counter_to_move_tetromino_down = actual_falling_speed;
            PlayWithRandomPitch(sfx_move_tetromino);
        }
        else
        {
            SetPlayerTetrominoToStage();

            PlayWithRandomPitch(sfx_hit_piece);

            const int deleted_lines = DeleteCompletedLines(to_remove_lines);
            AddScore((deleted_lines * 1.5f) * LINE_POINTS);

            if (SpawnNewPlayerTetromino(&next_tetrominos, tetromino_start_x, tetromino_start_y, &current_tetromino_x, &current_tetromino_y, &current_tetromino_type, &last_tetramino_type, &current_rotation, &current_player_color))
            {
                SwitchScene(&gameover_scene, &actual_game_scene);
            }
        }
    }
}

/// @brief Move the player in that direction horizontally
/// @param value 1 = right, -1 = left
void MoveHorizontal(enum HorizontalKey value)
{
    // No need to check overflow, wall is your protector
    if (!CheckCollision(current_tetromino_x + value, current_tetromino_y, tetromino_types[current_tetromino_type][current_rotation]))
    {
        current_tetromino_x += value;
        PlayWithRandomPitch(sfx_move_tetromino);
    }
}

/// @brief Ticks the timer to check if keep moving the player's tetromino in that direction
/// @param pressing_key the key is currently pressing the player
/// @return 1 = timer is ended, 0 = still ticking
int TickPressingKeyTimer(KeyboardKey pressing_key)
{
    //if (pressing_key != last_pressed_key) counter_to_continuous_movement = 0;
    //last_pressed_key = pressing_key;

    counter_to_continuous_movement -= GetFrameTime();

    if (counter_to_continuous_movement < 0)
    {
        // No need to check overflow, wall is your protector
        counter_to_continuous_movement = timer_to_continuous_movement;
        return 1;
    }
    return 0;
}

/// @brief Checks if the player is keeping down a key and move the tetromino in the key direction
void CheckRotationKeyDown()
{
    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP))
    {
        const int last_rotation = current_rotation;
        const int last_counter_to_move_tetromino_down = counter_to_move_tetromino_down;

        current_rotation = (current_rotation + 1) % 4;
        counter_to_move_tetromino_down = counter_to_move_tetromino_down + GetFrameTime();

        if (CheckCollision(current_tetromino_x, current_tetromino_y, tetromino_types[current_tetromino_type][current_rotation]))
        {
            current_rotation = last_rotation;
            counter_to_move_tetromino_down = last_counter_to_move_tetromino_down;
        }
        else
        {
            PlayWithRandomPitch(sfx_rotate_tetromino);
        }
    }
}

/// @brief Checks if the player is keeping down a key and move the tetromino in the key direction
void CheckMovementKeysDown()
{
    if (IsKeyDown(KEY_DOWN))
    {
        if (TickPressingKeyTimer(KEY_DOWN))
        {
            counter_to_move_tetromino_down = default_counter_to_continous_movement_value;
            PlayWithRandomPitch(sfx_move_tetromino);
        }
    }
    
    if (IsKeyDown(KEY_LEFT))
    {
        if (TickPressingKeyTimer(KEY_LEFT))
        {
            MoveHorizontal(LEFT);
        }
    }
    else if (IsKeyDown(KEY_RIGHT))
    {
        if (TickPressingKeyTimer(KEY_RIGHT))
        {
            MoveHorizontal(RIGHT);
        }
    }

    if (IsKeyReleased(KEY_DOWN) || IsKeyReleased(KEY_LEFT) || IsKeyReleased(KEY_RIGHT))
    {
        counter_to_continuous_movement = default_counter_to_continous_movement_value;
    }
}

/// @brief Checks for player inputs (rotation and movements)
void PlayerInputManager()
{
    CheckRotationKeyDown();
    CheckMovementKeysDown();
    TetrominoDownMovementCheck();
}

/// @brief Stops the game until the effect is completed
/// @return  1 = still playing, 0 = ended effect
int ManageCompletedLineEffect()
{
    if (is_waiting_for_ending_effect) //the line completed effect
    {
        for (int i = 0; i < STAGE_HEIGHT; i++)
        {
            if (to_remove_lines[i] == 0) continue;

            for (int j = 0; j < STAGE_WIDTH - 2; j++)
            {
                stage[to_remove_lines[i] + j] = index_color_effect;
            }
        }

        counter_to_show_completed_line_effect -= GetFrameTime();
        index_color_effect = (index_color_effect + 1) % 8;
        if (counter_to_show_completed_line_effect < 0)
        {
            is_waiting_for_ending_effect = false;
            counter_to_show_completed_line_effect = timer_to_show_completed_line_effect;

            for (int i = 0; i < STAGE_HEIGHT; i++)
            {
                if (to_remove_lines[i] == 0) continue;

                memset(stage + to_remove_lines[i], 0, (STAGE_WIDTH - 2) * sizeof(int)); //set the whole line to 0

                PushDownTetrominos(to_remove_lines[i] / STAGE_WIDTH - 1); //y * STAGE_WIDTH + 1
            }

            memset(to_remove_lines, 0, sizeof(to_remove_lines));

        }
        return 1;
    }
    return 0;
}

/// @brief Draws the score, highscore, vertical speed, all tetrominos and pause menu if enabled
void DrawMainGame()
{
    DrawText(TextFormat("Score: %08i", actual_score), 20, 10, 20, TETRIS_BLUE_PURPLE);
    DrawText(TextFormat("Highscore: %08i", highscore), 20, 30, 20, TETRIS_LIGHT_GREEN);
    DrawText(TextFormat("Vertical Speed: %.3f", (1 - actual_falling_speed)), 20, 50, 20, TETRIS_YELLOW);

    DrawStageTetrominos(tetromino_texture, current_tetromino_x, current_tetromino_y, start_offset_x, start_offset_y);
    DrawText("Next tetrominos:", next_tetromino_start_x - 50, next_tetromino_start_y - 30, 20, TETRIS_DARK_RED);
    DrawNextPieces(next_tetrominos, tetromino_texture, next_tetromino_start_x, next_tetromino_start_y, tetromino_types);
    DrawTetromino(tetromino_texture, tetromino_colors[current_player_color], current_tetromino_x, current_tetromino_y, start_offset_x, start_offset_y, tetromino_types[current_tetromino_type][current_rotation]);

    if (is_game_paused)
    {
        DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, TETRIS_BLACK_OPAQUE);
        DrawText("GAME PAUSED", start_offset_x, WINDOW_HEIGHT / 2 - 50, 40, TETRIS_YELLOW);
        DrawText("Press [ESC] to quit!", start_offset_x - 50, WINDOW_HEIGHT / 2 + 20, 40, TETRIS_DARK_RED);
    }

}

/// @brief Updates current music, checks pause, updates timers and does draw calls on screen
void MainGameLoop()
{
    UpdateMusicStream(*current_music);

    if (IsKeyPressed(KEY_P))
    {
        is_game_paused = !is_game_paused;
    }

    if (is_game_paused) goto game_loop_drawing;

    if (ManageCompletedLineEffect() == 1) goto game_loop_drawing;

    //ACTUAL GAME LOOP
    counter_to_move_tetromino_down -= GetFrameTime();

    //debug key for doubling points
    if (IsKeyPressed(KEY_E))
    {
        AddScore(actual_score);
    }

    PlayerInputManager();

game_loop_drawing:

    BeginDrawing();
    ClearBackground(BLACK);
    DrawMainGame();
    EndDrawing();
}