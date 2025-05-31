#include <stdlib.h>
#include "Player.h"

#define X_SCREEN 320 
#define Y_SCREEN 320
#define LEFT 0 
#define RIGHT 1
struct Player* PlayerCreate(unsigned char side, unsigned char face, unsigned short x, unsigned short y, unsigned short maxX, unsigned short maxY)
{
    struct Player* newPlayer;

    if((x - side/2 < 0) || (x + side/2 > maxX) || (y - side/2 < 0) || (y + side/2 > maxY))
        return NULL;

    newPlayer = malloc(sizeof(struct Player));
    newPlayer->side = side;
    newPlayer->face = face;
    newPlayer->isOnGround = 1;
    newPlayer->x = x;
    newPlayer->y = y;
    newPlayer->velocityY = 0;
    newPlayer->jumpStrength = -10;
    newPlayer->maxX = maxX;
    newPlayer->maxY = maxY;
    newPlayer->hitbox = HitboxCreate(side, side, x, y);
    newPlayer->control = JoystickCreate();
    newPlayer->pistol = PistolCreate();
    newPlayer->state = IDLE;

    return newPlayer;
}

void EnterRunHandler(struct Player* player)
{
    //Can run animations or sound 
    player->state = RUN;
    EventRunHandler(player, RUN);
}
void EnterIdleHandler(struct Player* player)
{
    player->state = IDLE;
}
void EnterJumpHandler(struct Player* player)
{
    player->state = JUMP;
    player->isOnGround = 0;
    player->velocityY = player->jumpStrength;
    EventJumpHandler(player, JUMP);
}

void EventJumpHandler(struct Player* player, enum State state)
{
    if(player->isOnGround)
    {
        EnterIdleHandler(player);
        return;
    }
    if(player->control->left)
        PlayerMove(player, 1, LEFT, X_SCREEN, Y_SCREEN);
    
    if(player->control->right)
        PlayerMove(player, 1, RIGHT, X_SCREEN, Y_SCREEN);
}
void EventRunHandler(struct Player* player, enum State state)
{
    if(state == IDLE)
    {
        EnterIdleHandler(player);
        return;
    }
    else if(state == JUMP)
    {
        EnterJumpHandler(player);
        return;
    }

    if(player->control->left)
        PlayerMove(player, 1, LEFT, X_SCREEN, Y_SCREEN);
    
    if(player->control->right)
        PlayerMove(player, 1, RIGHT, X_SCREEN, Y_SCREEN);

}
void EventIdleHandler(struct Player* player, enum State state)
{
    if(state == RUN)
        EnterRunHandler(player);

    else if(state == JUMP)
        EnterJumpHandler(player);
}

void PlayerMove(struct Player* player, unsigned char steps, unsigned char trajectory, unsigned short maxX, unsigned short maxY)
{
    if(trajectory == LEFT)
         player->x = player->x - steps*PLAYER_STEP;

    else if(trajectory == RIGHT)
         player->x = player->x + steps*PLAYER_STEP;
}

void PlayerUpdateState(struct Player* player, enum State newState)
{
    if(player->state == IDLE)
        EventIdleHandler(player, newState);

    if(player->state == RUN)
        EventRunHandler(player, newState);

    if(player->state == JUMP)
        EventJumpHandler(player, newState);
}
void PlayerUpdate(struct Player* player)
{
    enum State newState = IDLE;
    
    player->y += player->velocityY;
    if(!player->isOnGround)
        player->velocityY += GRAVITY;

    if(player->y >= player->maxX/2)
    {
        player->y = player->maxX/2;
        player->velocityY = 0;
        player->isOnGround = 1;
    }

    if(player->control->left || player->control->right)
        newState = RUN;

    if(player->control->jump)
        newState = JUMP;
    
    PlayerUpdateState(player, newState);

    if(player->control->fire && player->pistol->timer == 0)
    {
        PlayerShot(player);
        player->pistol->timer = PISTOL_COOLDOWN;
    }

    BulletUpdate(player);

    if(player->pistol->timer)
        player->pistol->timer--;
}

void PlayerDestroy(struct Player* player)
{
    JoystickDestroy(player->control);
    PistolDestroy(player->pistol);
    HitboxDestroy(player->hitbox);

    free(player);
}

void PlayerShot(struct Player* player)
{
    struct Bullet* shot;
    if(player->face == LEFT)
        shot = PistolShot(player->x - player->side/2, player->y, player->face, player->pistol);
    else if(player->face == RIGHT)
        shot = PistolShot(player->x + player->side/2, player->y, player->face, player->pistol);

    if(shot)
        player->pistol->shots = shot;
}
