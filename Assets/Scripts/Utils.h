#ifndef __UTILS__
#define __UTILS__

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
enum Entities
{
    PLAYER,
    NORMAL_ENEMY,
};

#endif
