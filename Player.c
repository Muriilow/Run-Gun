#include <stdlib.h>
#include "Player.h"

struct Player* PlayerCreate(unsigned char side, unsigned char face, unsigned short x, unsigned short y, unsigned short maxX, unsigned short maxY)
{
    struct Player* newPlayer;

    if((x - side/2 < 0) || (x + side/2 > maxX) || (y - side/2 < 0) || (y + side/2 > maxY))
        return NULL;

    newPlayer = malloc(sizeof(struct Player));

    if(newPlayer == NULL)
        exit(EXIT_FAILURE);

    newPlayer->side = side;
    newPlayer->face = face;
    newPlayer->isOnGround = 1;
    newPlayer->isRight = 0;
    newPlayer->isLeft = 0;
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
    player->state = JUMPING;
    player->isOnGround = 0;
    player->velocityY = player->jumpStrength;
    EventJumpHandler(player);
}

void EventJumpHandler(struct Player* player)
{
    if(player->isOnGround)
    {
        EnterIdleHandler(player);
        return;
    }
    if(player->control->left)
        PlayerMove(player, 1, LEFT);
    
    if(player->control->right)
        PlayerMove(player, 1, RIGHT);
}
void EventRunHandler(struct Player* player, enum State state)
{
    if(state == IDLE)
    {
        EnterIdleHandler(player);
        return;
    }
    else if(state == JUMPING)
    {
        EnterJumpHandler(player);
        return;
    }

    if(player->control->left)
        PlayerMove(player, 1, LEFT);
    
    if(player->control->right)
        PlayerMove(player, 1, RIGHT);

}
void EventIdleHandler(struct Player* player, enum State state)
{
    if(state == RUN)
        EnterRunHandler(player);

    else if(state == JUMPING)
        EnterJumpHandler(player);
}

void PlayerMove(struct Player* player, unsigned char steps, unsigned char trajectory)
{
    int leftZone = 300;
    int rightZone = player->maxX - 300;

    if(trajectory == LEFT)
    {
        player->hitbox->x -= steps*PLAYER_STEP;
        player->x -= steps*PLAYER_STEP;
        if(!(player->x >= leftZone && player->x <= rightZone))
        {
            player->isLeft = 1;
            player->hitbox->x += steps*PLAYER_STEP;
            player->x += steps*PLAYER_STEP;
        }
        else
            player->isRight = 0;
    }

    else if(trajectory == RIGHT)
    {
        player->hitbox->x += steps*PLAYER_STEP;
        player->x += steps*PLAYER_STEP;
        if(!(player->x >= leftZone && player->x <= rightZone))
        {
            player->isRight = 1;
            player->hitbox->x -= steps*PLAYER_STEP;
            player->x -= steps*PLAYER_STEP;
        }
        else
            player->isLeft = 0;
    }
}

void PlayerUpdateState(struct Player* player, enum State newState)
{
    if(player->state == IDLE)
        EventIdleHandler(player, newState);

    if(player->state == RUN)
        EventRunHandler(player, newState);

    if(player->state == JUMPING)
        EventJumpHandler(player);
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
        newState = JUMPING;
    
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
