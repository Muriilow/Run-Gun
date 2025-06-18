#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "NormalEnemy.h"

struct NormalEnemy* NormalEnemyCreate(unsigned char vert, unsigned char hor, struct Position position, struct NormalEnemy* next, ALLEGRO_BITMAP* sprite, ALLEGRO_BITMAP* bullet)
{
    struct NormalEnemy* enemy;
    struct Vector2 vec2 = {0, 0};
    enemy = malloc(sizeof(struct NormalEnemy));

    if(enemy == NULL)
        exit(EXIT_FAILURE);

    enemy->health = 5;
    enemy->position = position;
    enemy->hitbox = HitboxCreate(vert, hor, position.x, position.y);
    enemy->pistol = PistolCreate(PISTOL_COOLDOWN_ENEMY, bullet, 5);
    enemy->next = next;
    enemy->sprite = sprite;
    enemy->spriteBullet = bullet;
    enemy->isFocused = FALSE;
    enemy->currentFrame = 1;
    enemy->animationTime = 0;
    enemy->velocity = vec2;
    return enemy;
}
void NormalEnemyShot(struct NormalEnemy* enemy, struct Player* player)
{
    struct Bullet* shot = NULL;
    short distanceX =  player->position.worldX - enemy->position.worldX;
    short distanceY = player->position.worldY - enemy->position.worldY;
    float angle = atan2(distanceY, distanceX);
    struct Vector2 traj = {cos(angle), sin(angle)};

    shot = PistolShot(enemy->position, traj, 6., enemy->pistol);

    if(shot)
        enemy->pistol->shots = shot;
}
void CheckDistance(struct NormalEnemy* enemy, struct Player* player)
{
    short distanceX = enemy->position.worldX - player->position.worldX;
    short distanceY = enemy->position.worldY - player->position.worldY;
    float distance = sqrt(distanceX*distanceX + distanceY*distanceY);
            
    if(distance < 650 && enemy->pistol->timer == 0)
    {
        enemy->velocity.x = -2;
        enemy->isFocused = TRUE;
        enemy->pistol->timer = enemy->pistol->cooldown;
        NormalEnemyShot(enemy, player);
    }
    else if(distance >= 700)
    {
        enemy->isFocused = FALSE;
    }
}
void CheckCollision(struct NormalEnemy* enemy, struct Player* player)
{
    struct Bullet* previous = NULL;

    /* Checking if any bullet is colliding with me */
    for(struct Bullet* index = player->pistol->shots; index != NULL;)
    {
        if(HitboxCheck(index->hitbox, enemy->hitbox))
        {
            enemy->health--;

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
    for(struct Bullet* index = enemy->pistol->shots; index != NULL;)
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
                enemy->pistol->shots = index->next;
                BulletDestroy(index);
                index = enemy->pistol->shots;
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
void EnemyDraw(struct NormalEnemy* enemy, struct Player* player)
{
    if(enemy->isFocused == TRUE)
    {
        al_draw_bitmap_region(enemy->sprite,
            0, 0, FRAME_HOR_EN, FRAME_VERT_EN, 
            enemy->position.x - 75, enemy->position.y - 125,
            0);
    }
    else
    { 
        enemy->animationTime++;
        if(enemy->animationTime >= ENEMY_FRAME_DELAY)
        {
            enemy->animationTime = 0;
            enemy->currentFrame++;

            if(enemy->currentFrame > ENEMY_FRAME_NUMBER)
                enemy->currentFrame = 1;

        }
        al_draw_bitmap_region(enemy->sprite, enemy->currentFrame*FRAME_HOR_EN,
                    0, FRAME_HOR_EN,FRAME_VERT_EN, enemy->position.x - 75, enemy->position.y - 125, 0);

    }

}
void NormalEnemyUpdate(struct NormalEnemy* enemy, struct Player* player)
{
    if(enemy == NULL || player == NULL)
        return;

    if(enemy->pistol->timer)
        enemy->pistol->timer--;
    
    /* If enemy died */
    if(enemy->health == 0)
    {
        NormalEnemyDestroy(enemy); 
        enemy = NULL;
        return;
    }
    if(enemy->velocity.x)
        enemy->velocity.x += 0.2;

    if(enemy->velocity.x > 0)
        enemy->velocity.x = 0;
    enemy->position.worldX += enemy->velocity.x;
    enemy->position.worldY += enemy->velocity.y;

    /*Updating our screen x and y positions*/
    enemy->position.x = enemy->position.worldX - player->viewport->offsetX;
    enemy->position.y = enemy->position.worldY - player->viewport->offsetY;
    /*Updating our hitbox based on the position of the word*/
    enemy->hitbox->x = enemy->position.worldX;
    enemy->hitbox->y = enemy->position.worldY;

    HitboxDraw(enemy->hitbox, player);


    CheckCollision(enemy, player);
    CheckDistance(enemy, player);
    EnemyBulletUpdate(enemy, player);
    EnemyDraw(enemy, player);
}

void NormalEnemyDestroy(struct NormalEnemy* enemy)
{
    PistolDestroy(enemy->pistol);
    HitboxDestroy(enemy->hitbox);
    free(enemy);
}
