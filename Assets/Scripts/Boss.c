#include <stdlib.h>
#include <math.h>
#include "Boss.h"
struct Boss* BossCreate(unsigned char side, struct Position position)
{
    struct Boss* boss;

    boss = malloc(sizeof(struct Boss));

    if(boss == NULL)
        exit(EXIT_FAILURE);

    boss->health = 50;
    boss->side = side;
    boss->position = position;
    boss->hitbox = HitboxCreate(side, side, position.x, position.y);
    boss->pistol = PistolCreate();

    return boss;

}
void BossShot(struct Boss* boss, struct Player* player)
{
    struct Bullet* shot = NULL;
    short distanceX =  player->position.worldX - boss->position.worldX;
    short distanceY = player->position.worldY - boss->position.worldY;
    float angle = atan2(distanceY, distanceX);
    struct Vector2 traj = {cos(angle), sin(angle)};

    shot = PistolShot(boss->position, traj, 6., boss->pistol);

    if(shot)
        boss->pistol->shots = shot;
}
void CheckDistanceBoss(struct Boss* boss, struct Player* player)
{
    short distanceX = boss->position.worldX - player->position.worldX;
    short distanceY = boss->position.worldY - player->position.worldY;
    float distance = sqrt(distanceX*distanceX + distanceY*distanceY);
    
    if(distance < 200 && boss->pistol->timer == 0)
    {
        boss->pistol->timer = PISTOL_COOLDOWN_BOSS;
        BossShot(boss, player);
    }
}
void CheckCollisionBoss(struct Boss* boss, struct Player* player)
{
    struct Bullet* previous = NULL;

    /* Checking if any bullet is colliding with me */
    for(struct Bullet* index = player->pistol->shots; index != NULL;)
    {
        if(HitboxCheck(index->hitbox, boss->hitbox))
        {
            boss->health--;

            if(previous != NULL)
            {
                previous->next = index->next;
                BulletDestroy(index);
                index = previous->next;
                break;
            }
            else
            {
                player->pistol->shots = index->next;
                BulletDestroy(index);
                index = player->pistol->shots;
                break;
            }
        }
        else
        {
            previous= index;
            index = index->next;
        }
    }

    previous = NULL;
    /* Checking if any bullet is colliding with the player */
    for(struct Bullet* index = boss->pistol->shots; index != NULL;)
    {
        if(HitboxCheck(index->hitbox, player->hitbox))
        {
            player->health--;

            if(previous != NULL)
            {
                previous->next = index->next;
                BulletDestroy(index);
                index = previous->next;
                break;
            }
            else
            {
                boss->pistol->shots = index->next;
                BulletDestroy(index);
                index = boss->pistol->shots;
                break;
            }
        }
        else
        {
            previous = index;
            index = index->next;
        }
    }
}
void BossUpdate(struct Boss* boss, struct Player* player)
{
    if(boss == NULL || player == NULL)
        return;

    unsigned char size = boss->side/2;
    if(boss->pistol->timer)
        boss->pistol->timer--;
    
    /* If boss died */
    if(boss->health == 0)
    {
        exit(EXIT_SUCCESS);
    }

    /*Updating our screen x and y positions*/
    boss->position.x = boss->position.worldX - player->viewport->offsetX;
    boss->position.y = boss->position.worldY - player->viewport->offsetY;
    /*Updating our hitbox based on the position of the word*/
    boss->hitbox->x = boss->position.worldX;
    boss->hitbox->y = boss->position.worldY;

    al_draw_filled_rectangle(boss->position.x - size, boss->position.y - size,
        boss->position.x + size, boss->position.y + size, al_map_rgb(0, 255, 0));

    CheckCollisionBoss(boss, player);
    CheckDistanceBoss(boss, player);
    BossBulletUpdate(boss, player);

}
