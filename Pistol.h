#ifndef __PISTOL__
#define __PISTOL__

#define X_SCREEN 320 
#define Y_SCREEN 320
#define LEFT 0 
#define RIGHT 1

#include "Bullet.h"

#define PISTOL_COOLDOWN 10

struct Pistol 
{
    unsigned char timer;
    struct Bullet* shots;
};

struct Pistol* PistolCreate();
struct Bullet* PistolShot(unsigned short x, unsigned short y, unsigned char trajectory, struct Pistol* gun);
void PistolDestroy(struct Pistol* pistol);

#endif
