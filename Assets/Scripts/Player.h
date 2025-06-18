#ifndef __PLAYER__
#define __PLAYER__
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "Hitbox.h"
#include "Joystick.h"
#include "Pistol.h"
#include "Utils.h"

/* Defines to simplify the code */
#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3
#define FIRE 4
#define JUMP 5
#define GRAVITY 0.8
#define TRUE 1
#define FALSE 0
#define LEFT_CORNER 300
#define RIGHT_CORNER 500
#define HITBOX 100
#define HITBOX_CROUCHED 50
#define PISTOL_COOLDOWN_PLAYER 30
#define FRAME_SIZE 256
#define FRAME_NUMBER 7
#define FRAME_DELAY 8

#define PLAYER_STEP 3

struct Player 
{
    unsigned char currentFrame;
    unsigned char animationTime;
    unsigned char canDoubleJump;
    unsigned char health;
    unsigned char side; //The size of my character
    unsigned char face; //Am I facing right or left?
    unsigned char isOnGround;
    unsigned char isLeft; //Am I at the left corner of the camera
    unsigned char isRight; //Am I at the right corner of the camera
    unsigned short invencibility;
    struct Position position;
    char jumpStrength;
    float velocityY;
    ALLEGRO_BITMAP* spriteWalking;
    ALLEGRO_BITMAP* spriteBullet;
    struct Viewport* viewport;
    struct Joystick* control;
    struct Pistol* pistol;
    struct Hitbox* hitbox;
    enum State state; //Finite state machine
};

/* Creates the character
 * side = size of the character
 * position = The coordenates X and Y relative to the camera or the world 
 * viewport = The struct that have the offset of the "camera", and the size of the screen 
 * Returns the pointer of the player
*/

struct Player* PlayerCreate(unsigned char side, struct Position position, struct Viewport* viewport, ALLEGRO_BITMAP* sprite, ALLEGRO_BITMAP* bullet);

/* Moves the character
 * player = The pointer to the player struct
 * steps = integer number that define how the player is going to walk (THIS IS NOT THE AMMOUNT OF PIXELS)
 * trajectory = RIGHT or LEFT, define the trajectory 
*/
void PlayerMove(struct Player* player, unsigned char steps, unsigned char trajectory);

/* Update all the player's logic, should be used in combination with the timer event
 * player = The pointer to the player struct
*/
void PlayerUpdate(struct Player* player);

/* Free all the malloc'd elements the player holds 
 * player = The pointer to the player struct
*/
void PlayerDestroy(struct Player* player);

/* Uses the PistolShot() function to create a new bullet and put it as the head of the list 
 * player = The pointer to the player struct
*/
void PlayerShot(struct Player* player);


/* This events holds all the logic to organize the finite state machine the player has.
 * The state machine events are:
 * Run, Idle, Jump, Grounded*/
void PlayerUpdateState(struct Player* player, enum State newState);
void EnterRunHandler(struct Player* player);
void EnterIdleHandler(struct Player* player);
void EnterJumpHandler(struct Player* player);
void EnterDoubleJumpHandler(struct Player* player);
void EnterGroundedHandler(struct Player* player);

void EventRunHandler(struct Player* player, enum State state);
void EventIdleHandler(struct Player* player, enum State state);
void EventJumpHandler(struct Player* player, enum State state);
void EventDoubleJumpHandler(struct Player* player);
void EventGroundedHandler(struct Player* player);

#endif
