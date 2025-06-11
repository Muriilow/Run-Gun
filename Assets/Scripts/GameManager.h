#ifndef __MANAGER__
#define __MANAGER__

#include "NormalEnemy.h"
#include "Player.h"
#include "Item.h"
/*Struct that will hold a lot of components, and logic of the game*/
struct GameManager
{
    struct NormalEnemy* normalEnemyList;
    struct Item* lifeItemList;
    struct Player* player;
};

struct GameManager* GameManagerCreate(struct Player* player);
void CreateNormalEnemy(struct GameManager* manager, unsigned char side, struct Position position);
void CreateLifeItem(struct GameManager* manager, unsigned char side, struct Position position);
void UpdateLogic(struct GameManager* manager);
#endif
