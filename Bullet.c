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
    newBullet->timerToLive = 180;
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
        index->timerToLive--;

        if(index->trajectory == LEFT)
            index->x -= BULLET_MOVE;
        else
            index->x += BULLET_MOVE;

        index->hitbox->x = index->x;

        al_draw_filled_circle(index->x, index->y, 2, al_map_rgb(255, 0, 0));

        if(index->x > player->position.screenX || index->timerToLive == 0)
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
