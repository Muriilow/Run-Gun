#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "Bullet.h"
#include "Player.h"
struct Bullet* BulletCreate(unsigned short x, unsigned short y, struct Vector2 trajectory, float velocity, struct Bullet* next)
{
    struct Bullet* newBullet = malloc(sizeof(struct Bullet));
    if(newBullet == NULL)
        return NULL;

    newBullet->velocity = velocity;
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

        index->x += (unsigned short)index->trajectory.x * index->velocity;
        index->y += (unsigned short)index->trajectory.y * index->velocity;
        index->hitbox->x = index->x;
        index->hitbox->y = index->y;

        printf("%d - %d\n", index->hitbox->x, index->hitbox->y);

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
