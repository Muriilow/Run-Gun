#ifndef __NORMAL_EN__
#define __NORMAL_EN__

#define ENEMY_FRAME_NUMBER 5
#define ENEMY_FRAME_DELAY 20
#define PISTOL_COOLDOWN_ENEMY 120
#define FRAME_VERT_EN 250 
#define FRAME_HOR_EN 150
 
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "Hitbox.h"
#include "Pistol.h"
#include "Player.h"
#include "Utils.h"

#define ENEMY_WALK_TIMER 400
enum EnemyState
{
    WALKING,
    SHOOTING,
    NONE
};
struct NormalEnemy
{
    ALLEGRO_BITMAP* sprite;
    ALLEGRO_BITMAP* spriteBullet;
    unsigned char animationTime;
    unsigned char currentFrame; 
    unsigned short walkTimer; 
    unsigned char isFocused;
    unsigned char health;
    unsigned char face;
    struct Vector2 velocity;
    struct Hitbox* hitbox;
    struct Position position;
    struct Pistol* pistol;
    struct NormalEnemy* next;
    enum EnemyState state;
};

struct NormalEnemy* NormalEnemyCreate(unsigned char vert, unsigned char hor, struct Position position, struct NormalEnemy* next, ALLEGRO_BITMAP* sprite, ALLEGRO_BITMAP* bullet);
void NormalEnemyUpdate(struct NormalEnemy* enemy, struct Player* player);
void NormalEnemyDestroy(struct NormalEnemy* enemy);
#endif
