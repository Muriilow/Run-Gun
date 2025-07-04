#ifndef __UTILS__
#define __UTILS__

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3
#define TRUE 1
#define FALSE 0

#define INV_FRAME 30
struct Position
{
    short x;
    short y;
    int worldX;
    int worldY;
};
struct Viewport
{
    float x;
    float y;
    float offsetX;
    float offsetY;
    float width;
    float height;
};
struct Vector2
{
    float x;
    float y;
};
enum State
{
    IDLE,
    RUN,
    JUMPING,
    DOUBLE_JUMP,
    CROUCHED, 
};
enum GameState
{
    MENU, 
    GAME,
    PAUSE,
    GAME_OVER,
    GAME_WIN
};
enum Entities
{
    PLAYER,
    NORMAL_ENEMY,
};
enum Menu 
{
    PLAY, 
    EXIT,
    CONTINUE,
    BACK
};

#endif
