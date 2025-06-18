#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "Boss.h"

struct Power* PowerCreate(int side, struct Position position)
{
    ALLEGRO_BITMAP *ray = al_load_bitmap("Assets/Sprites/rayBoss.png");
    ALLEGRO_BITMAP *light = al_load_bitmap("Assets/Sprites/lightBoss.png");
    struct Power* power =  malloc(sizeof(struct Power));

    if(power == NULL)
        return NULL;

    power->isFixed = FALSE;
    power->isActive = FALSE;
    power->timerToLive = POWER_COOLDOWN_BOSS;
    power->timer = POWER_COOLDOWN_BOSS;
    power->lineTimer = POWER_COOLDOWN_BOSS - 200;
    power->position = position;
    power->ray = ray;
    power->light = light;
    power->hitbox = HitboxCreate(10000, side, position.x, position.y);

    return power; 
}
struct Boss* BossCreate(struct Position position)
{
    struct Boss* boss;
    ALLEGRO_BITMAP *bossBullet = al_load_bitmap("Assets/Sprites/bulletBoss.png");

    boss = malloc(sizeof(struct Boss));

    if(boss == NULL)
        exit(EXIT_FAILURE);

    boss->health = 10;
    boss->position = position;
    boss->hitbox = HitboxCreate(FRAME_VER_BOSS - 100, FRAME_HOR_BOSS- 100, position.x, position.y);
    boss->pistol = PistolCreate(PISTOL_COOLDOWN_BOSS, bossBullet, 60);
    boss->sprite = al_load_bitmap("Assets/Sprites/boss.png");
    boss->animationTime = 0;
    boss->currentFrame = 0;
    boss->moveTimer = 0;
    boss->isAttacking = FALSE;
    boss->velocityX = 0;
    boss->velocityY = 0;
    struct Position powerPos = {-10000, -10000, -10000, -10000};
    boss->power = PowerCreate(POWER_WIDTH, powerPos);

    return boss;

}
void BossShot(struct Boss* boss, struct Player* player)
{
    struct Bullet* shot = NULL;
    short distanceX =  player->position.worldX - boss->position.worldX;
    short distanceY = player->position.worldY - boss->position.worldY;
    float angle = atan2(distanceY, distanceX);
    struct Vector2 traj = {cos(angle), sin(angle)};

    shot = PistolShot(boss->position, traj, 4., boss->pistol);

    if(shot)
        boss->pistol->shots = shot;
}
void CheckDistanceBoss(struct Boss* boss, struct Player* player)
{
    short distanceX = boss->position.worldX - player->position.worldX;
    short distanceY = boss->position.worldY - player->position.worldY;
    float distance = sqrt(distanceX*distanceX + distanceY*distanceY);
    
    if(distance < 600 && boss->pistol->timer == 0)
    {
        boss->isAttacking = TRUE; 
        boss->pistol->timer = boss->pistol->cooldown;
        BossShot(boss, player);
    }
    if(distance < 600 && boss->power->timer == 0 && boss->power->isFixed == FALSE)
    {
        boss->power->isFixed = TRUE;
        struct Position pos = {player->position.x, 0, player->position.worldX, 0};
        boss->power->position = pos;
        boss->power->hitbox->x = pos.worldX;
        boss->power->hitbox->y = pos.worldY;
    }
    if(distance >= 900)
        boss->isAttacking = FALSE;
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
void BossPowerUpdate(struct Boss* boss, struct Player* player)
{
    if(boss->power->timerToLive <= 0)
        boss->power->isActive = FALSE;
    
    if(boss->power->timer)
        boss->power->timer--;

    if(boss->power != NULL && boss->power->timer == 0)
    {
        al_draw_bitmap(boss->power->ray,
                       boss->power->hitbox->x - 75 - player->viewport->offsetX,
                       0, 0);

        boss->power->lineTimer--;
    }

    if(boss->power->lineTimer == 0)
    {
        boss->power->isFixed = FALSE;
        boss->power->isActive = TRUE;
        boss->power->timer = POWER_COOLDOWN_BOSS;
        boss->power->timerToLive = POWER_COOLDOWN_BOSS;
        boss->power->lineTimer = POWER_COOLDOWN_BOSS - 200;
    }

    if(boss->power->isActive == TRUE)
    {
        if(HitboxCheck(boss->power->hitbox, player->hitbox) && player->invencibility == 0)
        {
            player->invencibility = INV_FRAME;
            player->health--;
        }

        boss->power->timerToLive--;

        al_draw_bitmap(boss->power->light,
                       boss->power->hitbox->x - 75 - player->viewport->offsetX,
                       0, 0);
        //HitboxDraw(boss->power->hitbox, player);
    }

}
void BossMove(struct Boss* boss, struct Player* player)
{
    short distanceX = boss->position.worldX - player->position.worldX;

    boss->moveTimer++;
    if(boss->isAttacking && boss->moveTimer >= BOSS_MOVE_DELAY)
    {
        if(distanceX > 0)
            boss->velocityX = 12;
        else
            boss->velocityX = -12;

        boss->moveTimer = 0;
    }
    if(boss->health == 36 || boss->health == 24 || boss->health == 12)
    {
        boss->velocityY = 1.2;
    }
}
void BossDraw(struct Boss* boss, struct Player* player)
{
    if(boss->power->isActive)
    {
        al_draw_bitmap_region(boss->sprite, 4*FRAME_HOR_BOSS,
                    0, FRAME_HOR_BOSS, FRAME_VER_BOSS,
                    boss->position.x - FRAME_HOR_BOSS/2,
                    boss->position.y - FRAME_VER_BOSS/2, 0);
        return;
    }
    boss->animationTime++;
    if(boss->animationTime >= BOSS_FRAME_DELAY)
    {
        boss->animationTime = 0;
        boss->currentFrame++;

        if(boss->currentFrame > BOSS_FRAME_NUMBER)
            boss->currentFrame = 0;
    }

    al_draw_bitmap_region(boss->sprite, boss->currentFrame*FRAME_HOR_BOSS,
                0, FRAME_HOR_BOSS, FRAME_VER_BOSS,
                boss->position.x - FRAME_HOR_BOSS/2,
                boss->position.y - FRAME_VER_BOSS/2, 0);

}
void BossUpdate(struct Boss* boss, struct Player* player)
{
    if(boss == NULL || player == NULL)
        return;


    boss->position.worldX += boss->velocityX;
    boss->position.worldY += boss->velocityY;

    if(boss->velocityX > 0)
        boss->velocityX -= 0.12;
    if(boss->velocityX < 0)
        boss->velocityX += 0.12;

    if(boss->velocityY)
        boss->velocityY -= 0.2;
    if(boss->velocityY < 0)
        boss->velocityY = 0;

    if(boss->pistol != NULL && boss->pistol->timer)
        boss->pistol->timer--;
    

    /* If boss died */
    //if(boss->health == 0)
    //    exit(EXIT_SUCCESS);

    /*Updating our screen x and y positions*/
    boss->position.x = boss->position.worldX - player->viewport->offsetX;
    boss->position.y = boss->position.worldY - player->viewport->offsetY;
    /*Updating our hitbox based on the position of the word*/
    boss->hitbox->x = boss->position.worldX;
    boss->hitbox->y = boss->position.worldY - 60; //Subtracting 60 to move the hitbox up

    //HitboxDraw(boss->hitbox, player);
    CheckCollisionBoss(boss, player);
    CheckDistanceBoss(boss, player);
    BossBulletUpdate(boss, player);
    BossPowerUpdate(boss, player);
    BossMove(boss, player);
    BossDraw(boss, player);
}

void BossDestroy(struct Boss* boss)
{
    al_destroy_bitmap(boss->sprite);
    al_destroy_bitmap(boss->power->ray);
    al_destroy_bitmap(boss->power->light);
    PistolDestroy(boss->pistol);
    HitboxDestroy(boss->hitbox);
    free(boss->power);
    free(boss);
}
