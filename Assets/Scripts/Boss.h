#ifndef __BOSS__
#define __BOSS__

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "Hitbox.h"
#include "Pistol.h"
#include "Player.h"
#include "Utils.h"

#define FRAME_HOR_BOSS 783
#define FRAME_VER_BOSS 384
#define BOSS_FRAME_DELAY 20
#define BOSS_FRAME_NUMBER 3
#define BOSS_MOVE_DELAY 40

#define PISTOL_COOLDOWN_BOSS 40
#define POWER_COOLDOWN_BOSS 400
#define POWER_WIDTH 50
struct Power
{
    unsigned char isFixed;
    unsigned char isActive;
    unsigned short lineTimer;
    unsigned short timer;
    short timerToLive;
    struct Position position; 
    struct Hitbox* hitbox;
};
struct Boss
{
    ALLEGRO_BITMAP* sprite;
    unsigned char animationTime;
    unsigned char currentFrame;
    unsigned char moveTimer;
    unsigned char health;
    struct Hitbox* hitbox;
    struct Position position;
    struct Pistol* pistol;
    struct Power* power;
};

struct Boss* BossCreate(struct Position position);
void BossDestroy(struct Boss* boss);
void BossUpdate(struct Boss* boss, struct Player* player);
struct Power* PowerCreate(int side, struct Position position);
#endif
