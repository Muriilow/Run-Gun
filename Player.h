#ifndef __PLAYER__
#define __PLAYER__

#define X_SCREEN 320 
#define Y_SCREEN 320
#define LEFT 0 
#define RIGHT 1
#define UP 2 
#define DOWN 3 
#define GRAVITY 0.8

#include "Hitbox.h"
#include "Joystick.h"
#include "Pistol.h"

#define PLAYER_STEP 3

enum State
{
    IDLE,
    RUN,
    JUMP,
    DOUBLE_JUMP,
};

struct Player 
{
    unsigned char side; 
    unsigned char face; //Am I facing right or left?
    unsigned char isOnGround;
    unsigned short x;
    unsigned short y;
    unsigned short maxX;
    unsigned short maxY;
    float velocityY;
    char jumpStrength;
    struct Joystick* control;
    struct Pistol* pistol;
    struct Hitbox* hitbox;
    enum State state;
};

//"Constructor"
struct Player* PlayerCreate(unsigned char side, unsigned char face, unsigned short x, unsigned short y, unsigned short maxX, unsigned short maxY);
void PlayerMove(struct Player* player, unsigned char steps, unsigned char trajectory, unsigned short maxX, unsigned short maxY);
void PlayerUpdate(struct Player* player);
void PlayerDestroy(struct Player* player);
void PlayerShot(struct Player* player);
void PlayerUpdateState(struct Player* player, enum State newState);

//EVENTS

void EnterRunHandler(struct Player* player);
void EnterIdleHandler(struct Player* player);
void EnterJumpHandler(struct Player* player);
void EventRunHandler(struct Player* player, enum State state);
void EventIdleHandler(struct Player* player, enum State state);
void EventJumpHandler(struct Player* player, enum State state);
#endif
