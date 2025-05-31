#include <stdlib.h>
#include "Bullet.h"
#include "Player.h"
struct Bullet* BulletCreate(unsigned short x, unsigned short y, unsigned char trajectory, struct Bullet* next)
{
    struct Bullet* newBullet = malloc(sizeof(struct Bullet));
    if(newBullet == NULL)
        return NULL;

    newBullet->x = x;
    newBullet->y = y;
    newBullet->trajectory = trajectory;
    newBullet->hitbox = HitboxCreate(5, 5, x, y);
    newBullet->next = next;

    return newBullet;
}

void BulletUpdate(struct Player* player)
{
    struct Bullet* previous = NULL;

    for(struct Bullet* index = player->pistol->shots; index != NULL;)
    {
        if(index->trajectory == LEFT)
            index->x -= BULLET_MOVE;
        else
            index->x += BULLET_MOVE;

        if(index->x < 0 || index->x > player->maxX)
        {
            if(previous)
            {
                previous->next = index->next;
                BulletDestroy(index);
                index = previous->next;
            }
            else
            {
                player->pistol->shots = index->next;
                BulletDestroy(index);
                index = player->pistol->shots;
            }
        }
        else
        {
            previous = index;
            index = index->next;
        }
    }
}

void BulletDestroy(struct Bullet* bullet)
{
    free(bullet);
}

//void BulletCollision()
//{
//
//}
