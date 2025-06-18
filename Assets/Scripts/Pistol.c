#include <stdlib.h>
#include "Pistol.h"
#include "Player.h"
#include "NormalEnemy.h"
#include "Boss.h"

struct Pistol* PistolCreate(unsigned char cooldown, ALLEGRO_BITMAP* sprite, unsigned char hitboxSize)
{
    struct Pistol* newPistol = malloc(sizeof(struct Pistol));
    if(newPistol == NULL)
        return NULL;

    newPistol->sprite = sprite;
    newPistol->timer = 0;
    newPistol->cooldown = cooldown;
    newPistol->shots = NULL;
    newPistol->hitboxSize = hitboxSize;

    return newPistol;
}

struct Bullet* PistolShot(struct Position position, struct Vector2 trajectory, float velocity, struct Pistol* gun)
{
    struct Bullet* newBullet = BulletCreate(position, trajectory, velocity, gun->shots, gun->hitboxSize);
    if(newBullet == NULL)
        return NULL;

    return newBullet;
}
void PistolDestroy(struct Pistol* pistol)
{
    struct Bullet* sentinel;
    for(struct Bullet* index = pistol->shots; index != NULL; index = sentinel)
    {
        sentinel = index->next;
        BulletDestroy(index);
    }

    free(pistol);
}
void PlayerBulletUpdate(struct Player* player)
{
    struct Bullet* previous = NULL;

    for(struct Bullet* index = player->pistol->shots; index != NULL;)
    {
        index->timerToLive--;

        index->position.x += index->trajectory.x * index->velocity;
        index->position.y += index->trajectory.y * index->velocity;
        index->position.worldX += index->trajectory.x * index->velocity;
        index->position.worldY += index->trajectory.y * index->velocity;
        index->hitbox->x = index->position.worldX;
        index->hitbox->y = index->position.worldY;
    
        //HitboxDraw(index->hitbox, player);
        al_draw_bitmap(player->pistol->sprite,
                       index->position.worldX - 20 - player->viewport->offsetX,
                       index->position.worldY - 20 - player->viewport->offsetY,
                       0);

        if(index->position.x > player->viewport->width || index->timerToLive == 0)
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
void BossBulletUpdate(struct Boss* boss, struct Player* player)
{
    struct Bullet* previous = NULL;

    for(struct Bullet* index = boss->pistol->shots; index != NULL;)
    {
        index->timerToLive--;

        index->position.x += index->trajectory.x * index->velocity;
        index->position.y += index->trajectory.y * index->velocity;
        index->position.worldX += index->trajectory.x * index->velocity;
        index->position.worldY += index->trajectory.y * index->velocity;
        index->hitbox->x = index->position.worldX;
        index->hitbox->y = index->position.worldY;
        
        HitboxDraw(index->hitbox, player);
        al_draw_scaled_bitmap(boss->pistol->sprite, 0, 0, 150, 150,
                       index->position.worldX - 50 - player->viewport->offsetX,
                       index->position.worldY - 50 - player->viewport->offsetY,
                       100, 100, 0);

        if(index->position.x > player->viewport->width || index->timerToLive == 0)
        {
            if(previous)
            {
                previous->next = index->next;
                BulletDestroy(index);
                index = previous->next;
            }
            else
            {
                boss->pistol->shots = index->next;
                BulletDestroy(index);
                index = boss->pistol->shots;
            }
        }
        else
        {
            previous = index;
            index = index->next;
        }
    }
}

void EnemyBulletUpdate(struct NormalEnemy* enemy, struct Player* player)
{
    struct Bullet* previous = NULL;

    for(struct Bullet* index = enemy->pistol->shots; index != NULL;)
    {
        index->timerToLive--;

        index->position.x += index->trajectory.x * index->velocity;
        index->position.y += index->trajectory.y * index->velocity;
        index->position.worldX += index->trajectory.x * index->velocity;
        index->position.worldY += index->trajectory.y * index->velocity;
        index->hitbox->x = index->position.worldX;
        index->hitbox->y = index->position.worldY;
       
        HitboxDraw(index->hitbox, player);
        al_draw_bitmap(enemy->pistol->sprite,
                       index->position.worldX - 12 - player->viewport->offsetX,
                       index->position.worldY - 12 - player->viewport->offsetY,
                       0);

        if(index->position.x > player->viewport->width || index->timerToLive == 0)
        {
            if(previous)
            {
                previous->next = index->next;
                BulletDestroy(index);
                index = previous->next;
            }
            else
            {
                enemy->pistol->shots = index->next;
                BulletDestroy(index);
                index = enemy->pistol->shots;
            }
        }
        else
        {
            previous = index;
            index = index->next;
        }
    }
}

