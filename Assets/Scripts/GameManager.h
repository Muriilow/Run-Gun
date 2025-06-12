#ifndef __MANAGER__
#define __MANAGER__

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
void CreateNormalEnemy(struct GameManager* manager, unsigned char side, struct Position position);
void CreateLifeItem(struct GameManager* manager, unsigned char side, struct Position position, unsigned char type);
void UpdateLogic(struct GameManager* manager);
#endif
