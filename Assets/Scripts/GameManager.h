#ifndef __MANAGER__
#define __MANAGER__

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "NormalEnemy.h"
#include "Player.h"
#include "Item.h"
#include "Boss.h"
/*Struct that will hold a lot of components, and logic of the game*/
struct GameManager
{
    struct NormalEnemy* normalEnemyList;
    struct Item* lifeItemList;
    struct Player* player;
    struct Boss* boss;
};

struct GameManager* GameManagerCreate(struct Player* player, struct Boss* boss);
void CreateNormalEnemy(struct GameManager* manager, struct Position position, ALLEGRO_BITMAP* sprite, ALLEGRO_BITMAP* bullet);
void CreateItem(struct GameManager* manager, struct Position position, unsigned char type, ALLEGRO_BITMAP* sprite);
void UpdateLogic(struct GameManager* manager);
#endif
