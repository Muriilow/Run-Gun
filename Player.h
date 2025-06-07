#ifndef __PLAYER__
#define __PLAYER__

#include "Hitbox.h"
#include "Joystick.h"
#include "Pistol.h"
#include "Utils.h"

#define X_SCREEN 320 
#define Y_SCREEN 320
#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3
#define FIRE 4
#define JUMP 5
#define GRAVITY 0.8
#define TRUE 1
#define FALSE 0


#define PLAYER_STEP 3

struct Player 
{
    unsigned char side; 
    unsigned char face; //Am I facing right or left?
    unsigned char isOnGround;
    unsigned char isLeft; //Am I at the left end of the camera
    unsigned char isRight;
    struct Position position;
    char jumpStrength;
    float velocityY;
    struct Viewport* viewport;
    struct Joystick* control;
    struct Pistol* pistol;
    struct Hitbox* hitbox;
    enum State state;
};

//"Constructor"
struct Player* PlayerCreate(unsigned char side, unsigned char face, struct Position position, struct Viewport* viewport);
void PlayerMove(struct Player* player, unsigned char steps, unsigned char trajectory);
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
void EventJumpHandler(struct Player* player);
#endif
