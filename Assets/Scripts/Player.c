#include <stdlib.h>
#include <stdio.h>
#include "Player.h"

struct Player* PlayerCreate(unsigned char side, struct Position position, struct Viewport* viewport, ALLEGRO_BITMAP* sprite, ALLEGRO_BITMAP* bullet)
{
    struct Player* newPlayer;

    newPlayer = malloc(sizeof(struct Player));

    if(newPlayer == NULL)
        exit(EXIT_FAILURE);

    newPlayer->health = 5;
    newPlayer->side = side;
    newPlayer->face = RIGHT;
    newPlayer->isOnGround = FALSE;
    newPlayer->isRight = FALSE;
    newPlayer->isLeft = FALSE;
    newPlayer->canDoubleJump = FALSE;
    newPlayer->invencibility = INV_FRAME;
    newPlayer->spriteWalking = sprite;
    newPlayer->spriteBullet = bullet;
    newPlayer->position = position;
    newPlayer->currentFrame = 0;
    newPlayer->animationTime = 0;

    newPlayer->velocityY = 0;
    newPlayer->jumpStrength = -15;
    newPlayer->hitbox = HitboxCreate(side, side, position.x + 50, position.y + 50);
    newPlayer->control = JoystickCreate();
    newPlayer->pistol = PistolCreate(PISTOL_COOLDOWN_PLAYER, bullet, 40);
    newPlayer->viewport = viewport;
    newPlayer->state = IDLE;

    return newPlayer;
}

void EnterGroundedHandler(struct Player* player)
{
    player->isLeft = FALSE;
    player->isRight = FALSE;
    player->state = CROUCHED;
    player->hitbox->vert = HITBOX_CROUCHED;

}
void EnterRunHandler(struct Player* player)
{
    //Can run animations or sound 
    player->state = RUN;
    EventRunHandler(player, RUN);
}
void EnterIdleHandler(struct Player* player)
{
    player->isLeft = FALSE;
    player->isRight = FALSE;
    player->state = IDLE;
    player->hitbox->vert = HITBOX;
}
void EnterDoubleJumpHandler(struct Player* player)
{
    player->control->jump = 0;
    player->state = DOUBLE_JUMP;
    player->isOnGround = FALSE;
    player->velocityY = player->jumpStrength;
    //EventDoubleJumpHandler(player);
}
void EnterJumpHandler(struct Player* player)
{
    player->control->jump = 0;
    player->state = JUMPING;
    player->isOnGround = FALSE;
    player->velocityY = player->jumpStrength;
    //EventJumpHandler(player);
}

void EventGroundedHandler(struct Player* player)
{
    if(player->control->down == FALSE)
        EnterIdleHandler(player);
}
void EventJumpHandler(struct Player* player)
{
    if(player->isOnGround == TRUE)
    {
        EnterIdleHandler(player);
        return;
    }
    if(player->control->jump && player->canDoubleJump)
    {
        EnterDoubleJumpHandler(player);
        return;
    }
    if(player->control->left)
        PlayerMove(player, 1, LEFT);
    
    if(player->control->right)
        PlayerMove(player, 1, RIGHT);
}
void EventDoubleJumpHandler(struct Player* player)
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
    else if(state == CROUCHED)
    {
        EnterGroundedHandler(player);
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

    else if(state == CROUCHED)
        EnterGroundedHandler(player);
}

void PlayerMove(struct Player* player, unsigned char steps, unsigned char trajectory)
{
    if(trajectory == LEFT)
    {
        player->position.worldX -= steps*PLAYER_STEP;
        player->face = LEFT;

        /*If the player is at the right corner but has pressed left, we stop
         * moving the background*/

        if(player->position.x > RIGHT_CORNER)
        {
            player->isRight = FALSE;
            player->position.x -= steps*PLAYER_STEP;
            return;
        }
        
        /*If the player is forcing its position to the left corner*/
        if(player->position.x < LEFT_CORNER)
        {
            player->viewport->offsetX -= steps*PLAYER_STEP;
            player->isLeft = TRUE;
        }
        else
        {
            player->isRight = FALSE;
            player->position.x -= steps*PLAYER_STEP;
        }
    }

    else if(trajectory == RIGHT)
    {
        player->position.worldX += steps*PLAYER_STEP;
        player->face = RIGHT;

        /*If the player is at the left corner but has pressed right, we stop
         * moving the background*/
        if(player->position.x < LEFT_CORNER)
        {
            player->isLeft = FALSE;
            player->position.x += steps*PLAYER_STEP;
            return;
        }
        
        /*If the player is forcing its position to the right corner*/
        if(player->position.x > RIGHT_CORNER)
        {
            player->viewport->offsetX += steps*PLAYER_STEP;
            player->isRight = TRUE;
        }
        else
        {
            player->isLeft = FALSE;
            player->position.x += steps*PLAYER_STEP;
        }
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

    if(player->state == DOUBLE_JUMP)
        EventDoubleJumpHandler(player);

    if(player->state == CROUCHED)
        EventGroundedHandler(player);
}

void PlayerDraw(struct Player* player)
{
        //HitboxDraw(player->hitbox, player);
    int posX = player->position.x - 50 + player->viewport->x;
    int posY = player->position.y - 50 + player->viewport->y;

    if(player->state == IDLE && player->control->fire)
    {
        al_draw_scaled_bitmap(player->spriteWalking,
                  1*FRAME_SIZE, 0, FRAME_SIZE, FRAME_SIZE, posX, posY, 100, 100, 1 ^ player->face);
    }
    else if(player->state == IDLE)
    {
        al_draw_scaled_bitmap(player->spriteWalking,
                  8*FRAME_SIZE, 0, FRAME_SIZE, FRAME_SIZE, posX, posY, 100, 100, 1 ^ player->face);
    }
    
    if(player->state == RUN)
    {
        /*Drawing the player and bullets*/
        player->animationTime++;
        if(player->animationTime >= FRAME_DELAY)
        {
            player->animationTime = 0;
            player->currentFrame++;

            if(player->currentFrame > FRAME_NUMBER)
                player->currentFrame = 0;

        }

        al_draw_scaled_bitmap(player->spriteWalking,player->currentFrame*FRAME_SIZE,
                    0, FRAME_SIZE,FRAME_SIZE, posX, posY, 100, 100, 1 ^ player->face);
    }

    if(player->state == JUMPING || player->state == DOUBLE_JUMP)
    {
        if(player->control->fire)
        {
            if(player->velocityY < 0)
            {
                al_draw_scaled_bitmap(player->spriteWalking,
                          13*FRAME_SIZE, 0, FRAME_SIZE, FRAME_SIZE, posX, posY, 100, 100, 1 ^ player->face);
            }
            else
            {
                al_draw_scaled_bitmap(player->spriteWalking,
                          14*FRAME_SIZE, 0, FRAME_SIZE, FRAME_SIZE, posX, posY, 100, 100, 1 ^ player->face);

            }
        }
        else
        {
            if(player->velocityY < 0)
            {
                al_draw_scaled_bitmap(player->spriteWalking,
                          9*FRAME_SIZE, 0, FRAME_SIZE, FRAME_SIZE, posX, posY, 100, 100, 1 ^ player->face);
            }
            else
            {
                al_draw_scaled_bitmap(player->spriteWalking,
                          10*FRAME_SIZE, 0, FRAME_SIZE, FRAME_SIZE, posX, posY, 100, 100, 1 ^ player->face);

            }
        }
    }
    if(player->state == CROUCHED)
    {
        if(player->control->fire)
        {
            al_draw_scaled_bitmap(player->spriteWalking,
                      11*FRAME_SIZE, 0, FRAME_SIZE, FRAME_SIZE, posX, posY, 100, 100, 1 ^ player->face);
        }
        else
        {
            al_draw_scaled_bitmap(player->spriteWalking,
                      12*FRAME_SIZE, 0, FRAME_SIZE, FRAME_SIZE, posX, posY, 100, 100, 1 ^ player->face);
        }
    }
}
void PlayerUpdate(struct Player* player)
{
    enum State newState = IDLE;
   
    player->position.y += player->velocityY;
    player->position.worldY += player->velocityY;
    player->hitbox->y = player->position.worldY;
    player->hitbox->x = player->position.worldX;

    if(player->invencibility > 0)
        player->invencibility--;

    if(player->isOnGround == FALSE)
        player->velocityY += GRAVITY;

    if(player->position.y >= player->viewport->width/2)
    {
        player->position.worldY = player->viewport->width/2;
        player->position.y = player->viewport->width/2;
        player->velocityY = 0;
        player->isOnGround = TRUE;
    }

    if(player->control->left || player->control->right)
        newState = RUN;
    
    if(player->control->down)
        newState = CROUCHED;

    if(player->control->jump)
        newState = JUMPING;
    
    PlayerUpdateState(player, newState);

    if(player->control->fire && player->pistol->timer == 0)
    {
        PlayerShot(player);
        player->pistol->timer = player->pistol->cooldown;
    }

    PlayerBulletUpdate(player);

    if(player->pistol->timer)
        player->pistol->timer--;


    PlayerDraw(player);
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

    if(player->control->up)
    {
        struct Vector2 traj = {0, -1};
        shot = PistolShot(player->position, traj, 6., player->pistol);
    }
    else if(player->face == LEFT)
    {
        struct Vector2 traj = {-1, 0};
        shot = PistolShot(player->position, traj, 6., player->pistol);
    }
    else if(player->face == RIGHT)
    {
        struct Vector2 traj = {1, 0};
        shot = PistolShot(player->position, traj, 6.,player->pistol);
    }
    if(shot)
        player->pistol->shots = shot;
}
