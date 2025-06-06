#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include "Player.h"

struct Player* PlayerCreate(unsigned char side, unsigned char face, struct Position position)
{
    struct Player* newPlayer;
    unsigned char boundariesX;
    unsigned char boundariesY;

    boundariesX = (position.x - side/2 < 0) || (position.x + side/2 > position.screenX);
    boundariesY = (position.y - side/1 < 0) || (position.y + side/2 > position.screenY);

    if(boundariesX || boundariesY)
        return NULL;

    newPlayer = malloc(sizeof(struct Player));

    if(newPlayer == NULL)
        exit(EXIT_FAILURE);

    newPlayer->side = side;
    newPlayer->face = face;
    newPlayer->isOnGround = TRUE;
    newPlayer->isRight = FALSE;
    newPlayer->isLeft = FALSE;

    newPlayer->position = position;

    newPlayer->velocityY = 0;
    newPlayer->jumpStrength = -10;
    newPlayer->hitbox = HitboxCreate(side, side, position.x, position.y);
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
    player->isOnGround = FALSE;
    player->velocityY = player->jumpStrength;
    EventJumpHandler(player);
}

void EventJumpHandler(struct Player* player)
{
    if(player->isOnGround == TRUE)
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
    int rightZone = player->position.screenX - 300;

    if(trajectory == LEFT)
    {
        player->position.x -= steps*PLAYER_STEP;
        player->face = LEFT;
        if(!(player->position.x >= leftZone && player->position.x <= rightZone))
        {
            player->isLeft = TRUE;
            player->position.x += steps*PLAYER_STEP;
        }
        else
            player->isRight = FALSE;
    }

    else if(trajectory == RIGHT)
    {
        player->position.x += steps*PLAYER_STEP;
        player->face = RIGHT;
        if(!(player->position.x >= leftZone && player->position.x <= rightZone))
        {
            player->isRight = TRUE;
            player->position.x -= steps*PLAYER_STEP;
        }
        else
            player->isLeft = FALSE;
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
    unsigned char size = player->side/2;
    enum State newState = IDLE;
   
    player->position.y += player->velocityY;
    player->hitbox->y = player->position.y;
    player->hitbox->x = player->position.x;

    if(player->isOnGround == FALSE)
        player->velocityY += GRAVITY;

    if(player->position.y >= player->position.screenX/2)
    {
        player->position.y = player->position.screenX/2;
        player->velocityY = 0;
        player->isOnGround = TRUE;
    }

    if(player->control->left || player->control->right)
        newState = RUN;

    if(player->control->jump)
        newState = JUMPING;
    
    PlayerUpdateState(player, newState);

    if(player->control->fire && player->pistol->timer == 0 && player->state != JUMPING)
    {
        PlayerShot(player);
        player->pistol->timer = PISTOL_COOLDOWN;
    }

    BulletUpdate(player);

    if(player->pistol->timer)
        player->pistol->timer--;

    /*Drawing the player and bullets*/
    al_draw_filled_rectangle(player->position.x - size, player->position.y - size,
        player->position.x + size, player->position.y + size, al_map_rgb(255, 0, 0));

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
    {
        struct Vector2 traj = {-1, 0};
        shot = PistolShot(player->position.x - player->side/2, player->position.y, traj, 3., player->pistol);
    }
    else if(player->face == RIGHT)
    {
        struct Vector2 traj = {1, 0};
        shot = PistolShot(player->position.x + player->side/2, player->position.y, traj, 3.,player->pistol);
    }
    if(shot)
        player->pistol->shots = shot;
}
