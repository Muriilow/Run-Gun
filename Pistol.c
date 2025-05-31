#include <stdlib.h>
#include "Pistol.h"

struct Pistol* PistolCreate()
{
    struct Pistol* newPistol = malloc(sizeof(struct Pistol));
    if(newPistol == NULL)
        return NULL;

    newPistol->timer = 0;
    newPistol->shots = NULL;

    return newPistol;
}

struct Bullet* PistolShot(unsigned short x, unsigned short y, unsigned char trajectory, struct Pistol* gun)
{
    struct Bullet* newBullet = BulletCreate(x, y, trajectory, gun->shots);
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

