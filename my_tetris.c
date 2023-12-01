#include "raylib.h"
#include <time.h>

#define STAGE_WIDTH 12
#define STAGE_HEIGHT 22
#define TILE_SIZE 16
#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 512

#define iTetrominoColor 0x00F0F0
#define oTetrominoColor 0xF0F000
#define tTetrominoColor 0xA000F0
#define jTetrominoColor 0x0000F0
#define lTetrominoColor 0xF0A000
#define sTetrominoColor 0x00F000
#define zTetrominoColor 0xF00000

int stage[] = {
1,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,1,
1,1,1,1,1,1,1,1,1,1,1,1,
};

Color stageColors[STAGE_WIDTH*STAGE_HEIGHT];

const int lTetromino0[] =
{
    0, 1, 0, 0,
    0, 1, 0, 0,
    0, 1, 1, 0,
    0, 0, 0, 0,
};

const int lTetromino90[] =
{
    0, 0, 0, 0,
    1, 1, 1, 0,
    1, 0, 0, 0,
    0, 0, 0, 0,
};

const int lTetromino180[] =
{
    1, 1, 0, 0,
    0, 1, 0, 0,
    0, 1, 0, 0,
    0, 0, 0, 0,
};

const int lTetromino270[] =
{
    0, 0, 1, 0,
    1, 1, 1, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
};

const int jTetromino0[] =
{
    0, 1, 0, 0,
    0, 1, 0, 0,
    1, 1, 0, 0,
    0, 0, 0, 0,
};

const int jTetromino90[] =
{
    1, 0, 0, 0,
    1, 1, 1, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
};

const int jTetromino180[] =
{
    0, 1, 1, 0,
    0, 1, 0, 0,
    0, 1, 0, 0,
    0, 0, 0, 0,
};

const int jTetromino270[] =
{
    0, 0, 0, 0,
    1, 1, 1, 0,
    0, 0, 1, 0,
    0, 0, 0, 0,
};


const int oTetromino[] =
{
    1, 1, 0, 0,
    1, 1, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
};

const int sTetromino0[] =
{
    0, 0, 0, 0,
    0, 1, 1, 0,
    1, 1, 0, 0,
    0, 0, 0, 0,
};

const int sTetromino90[] =
{
    1, 0, 0, 0,
    1, 1, 0, 0,
    0, 1, 0, 0,
    0, 0, 0, 0,
};

const int sTetromino180[] =
{
    0, 1, 1, 0,
    1, 1, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
};

const int sTetromino270[] =
{
    0, 1, 0, 0,
    0, 1, 1, 0,
    0, 0, 1, 0,
    0, 0, 0, 0,
};

const int tTetromino0[] =
{
    0, 0, 0, 0,
    1, 1, 1, 0,
    0, 1, 0, 0,
    0, 0, 0, 0,
};

const int tTetromino90[] =
{
    0, 1, 0, 0,
    1, 1, 0, 0,
    0, 1, 0, 0,
    0, 0, 0, 0,
};

const int tTetromino180[] =
{
    0, 1, 0, 0,
    1, 1, 1, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
};

const int tTetromino270[] =
{
    0, 1, 0, 0,
    0, 1, 1, 0,
    0, 1, 0, 0,
    0, 0, 0, 0,
};

const int iTetromino0[] =
{
    0, 1, 0, 0,
    0, 1, 0, 0,
    0, 1, 0, 0,
    0, 1, 0, 0,
};

const int iTetromino90[] =
{
    0, 0, 0, 0,
    1, 1, 1, 1,
    0, 0, 0, 0,
    0, 0, 0, 0,
};

const int zTetromino0[] =
{
    0, 0, 0, 0,
    1, 1, 0, 0,
    0, 1, 1, 0,
    0, 0, 0, 0,
};

const int zTetromino90[] =
{
    0, 1, 0, 0,
    0, 1, 1, 0,
    0, 0, 1, 0,
    0, 0, 0, 0,
};

const int zTetromino180[] =
{
    0, 0, 0, 0,
    0, 1, 1, 0,
    1, 1, 0, 0,
    0, 0, 0, 0,
};

const int zTetromino270[] =
{
    1, 0, 0, 0,
    1, 1, 0, 0,
    0, 1, 0, 0,
    0, 0, 0, 0,
};

const int *tetrominoTypes[7][4] =
{
    {zTetromino0, zTetromino90, zTetromino180, zTetromino270},
    {sTetromino0, sTetromino90, sTetromino180, sTetromino270},
    {tTetromino0, tTetromino90, tTetromino180, tTetromino270},
    {oTetromino, oTetromino, oTetromino, oTetromino},
    {iTetromino0, iTetromino90, iTetromino0, iTetromino90},
    {jTetromino0, jTetromino90, jTetromino180, jTetromino270},
    {lTetromino0, lTetromino90, lTetromino180, lTetromino270},
};


const int stageOffsetX = WINDOW_WIDTH / 2 - (STAGE_WIDTH * TILE_SIZE) / 2;
const int stageOffsetY = (STAGE_HEIGHT / 2 - (STAGE_HEIGHT * TILE_SIZE) / 2) /2;
const int startPieceX = STAGE_WIDTH/2 - 2; //2 = widthPiece/2 = 4/2 = 2
const int widthPiece = 4;
int myPieces[16];
int myPieceX = 0;
int myPieceY = 0;
int oldTetraminoType;
int currentTetraminoRotation = 0;
int myTetraminoType = 0;
Color playerColor;
float pieceDownTimer = 1.f;
float pieceDownCounter = 0.f;

void DrawStage();
void SetTetraminoPiece();
void SetRandomPlayerColor();
void InputManage();
void DrawPlayer();
void RespawnTetramino();
int CheckPieceCollisions(int upOrDown, int leftOrRight);

int main(int argc, char** argv, char** environ)
{    
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Tetris in C");
    SetTargetFPS(60);    

    pieceDownCounter = pieceDownTimer;

    RespawnTetramino();
    SetRandomPlayerColor();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        
        ClearBackground(GRAY);

        //KEYS
        pieceDownCounter -= GetFrameTime();
        InputManage();

        DrawStage();
        DrawPlayer();

        EndDrawing();
    }

    return 0;
}

void InputManage()
{
    if(pieceDownCounter < 0 || IsKeyPressed(KEY_DOWN))
    {
        pieceDownCounter = pieceDownTimer;  

        int hasCollide = CheckPieceCollisions(1,0);

        if(!hasCollide)
        {
            myPieceY += 1;     
            return;           
        }
    
        RespawnTetramino();
              
    }

    int moveDirection = 0;
    if(IsKeyPressed(KEY_RIGHT)){
        moveDirection=1;
    }
    else if(IsKeyPressed(KEY_LEFT)){
        moveDirection=-1;
    }

    if (IsKeyPressed(KEY_SPACE))
    {
        currentTetraminoRotation++;
        if (currentTetraminoRotation > 3)
        {
            currentTetraminoRotation = 0;
        }

        SetTetraminoPiece();

        int hasCollide = CheckPieceCollisions(0,0);

        if(hasCollide){
            currentTetraminoRotation--;
            if(currentTetraminoRotation<0)
            {
                currentTetraminoRotation=3;
            }
            SetTetraminoPiece();
        }

    }

    if(moveDirection != 0)
    {
        int hasCollide = CheckPieceCollisions(0,moveDirection);

        if(!hasCollide)
        {
            myPieceX += moveDirection;                
        }  
    }
}

void RespawnTetramino(){
    const int myIndex = (myPieceY)*STAGE_WIDTH+myPieceX;

        for (int y = 0; y < widthPiece; y++)
            {
                for (int x = 0; x < widthPiece; x++)
                {
                    if(myPieces[x+y*widthPiece] == 1)
                    {
                        const int index = myIndex+x+y*STAGE_WIDTH;

                        stage[index] = 1;
                    }    
                }
            } 

        myPieceX = startPieceX;
        myPieceY = 0;
        currentTetraminoRotation = 0;
        
        while(oldTetraminoType == myTetraminoType)
        {
            time_t unixTime;
            time(&unixTime);
            SetRandomSeed(unixTime);
            myTetraminoType = GetRandomValue(0,6);
        }

        oldTetraminoType = myTetraminoType;

        SetTetraminoPiece();
        SetRandomPlayerColor();  
}

void SetTetraminoPiece(){
    
    for (int i = 0; i < TILE_SIZE; i++)
    {
        myPieces[i] = tetrominoTypes[myTetraminoType][currentTetraminoRotation][i];
    }
}

void SetRandomPlayerColor()
{
    playerColor.r = GetRandomValue(0,255);
    playerColor.g = GetRandomValue(0,255);
    playerColor.b = GetRandomValue(0,255);
    playerColor.a = 255;
}

void DrawStage()
{
    for (int y = 0; y < STAGE_HEIGHT; y++)
    {
        for (int x = 0; x < STAGE_WIDTH; x++)
        {
            Color myColor;

            if(stage[x+y*STAGE_WIDTH] > 0)
            {
                //wall
                myColor = BLUE;                 
            }
            else
            {
                //empty
                myColor = WHITE;
            }

            DrawRectangle(x * TILE_SIZE + stageOffsetX,y*TILE_SIZE - stageOffsetY,TILE_SIZE,TILE_SIZE,myColor);
            DrawRectangleLines(x * TILE_SIZE + stageOffsetX,y*TILE_SIZE - stageOffsetY,TILE_SIZE,TILE_SIZE,BLACK);
        }
    }      

       
}

void DrawPlayer()
{
    for (int y = 0; y < widthPiece; y++)
    {
        for (int x = 0; x < widthPiece; x++)
        {
            if(myPieces[x+y*widthPiece] > 0)
            {
                DrawRectangle((myPieceX + x) * TILE_SIZE + stageOffsetX, (myPieceY + y) *TILE_SIZE - stageOffsetY,TILE_SIZE,TILE_SIZE,playerColor);  
            }    
        }
    }  
}

int CheckPieceCollisions(int upOrDown, int leftOrRight)
{
    const int myIndex = (myPieceY)*STAGE_WIDTH+myPieceX;

    int hasCollide = 0;

    for (int y = 0; y < widthPiece; y++)
    {
        for (int x = 0; x < widthPiece; x++)
        {
            if(myPieces[x+y*widthPiece] > 0)
            {
                const int index = myIndex+x+y*STAGE_WIDTH+leftOrRight+STAGE_WIDTH*upOrDown;

                if(stage[index] > 0)
                {
                    hasCollide = 1;
                    break;
                }
            }    
        }
    } 

    return hasCollide;
}