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
    newPlayer->x = x;
    newPlayer->y = y;
    newPlayer->maxX = maxX;
    newPlayer->maxY = maxY;
    newPlayer->hitbox = HitboxCreate(side, side, x, y);
    newPlayer->control = JoystickCreate();
    newPlayer->pistol = PistolCreate();

    return newPlayer;
}
void PlayerMove(struct Player* player, unsigned char steps, unsigned char trajectory, unsigned short maxX, unsigned short maxY)
{
   if(trajectory == LEFT)
   {
        player->x = player->x - steps*PLAYER_STEP;
   }
   else if(trajectory == RIGHT)
   {
        player->x = player->x + steps*PLAYER_STEP;
   }
   else if(trajectory == UP)
   {
        player->y = player->y - steps*PLAYER_STEP;
   }
}

void PlayerUpdate(struct Player* player)
{
    if(player->control->left)
    {
        PlayerMove(player, 1, LEFT, X_SCREEN, Y_SCREEN);
    }
    if(player->control->right)
    {
        PlayerMove(player, 1, RIGHT, X_SCREEN, Y_SCREEN);
    }
   if(player->control->fire)
   {
        if(player->pistol->timer == 0)
        {
            PlayerShot(player);
            player->pistol->timer = PISTOL_COOLDOWN;
        }
   }
   if(player->control->jump)
   {
        PlayerMove(player, 1, UP, X_SCREEN, Y_SCREEN);
   }
   BulletUpdate(player);
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
