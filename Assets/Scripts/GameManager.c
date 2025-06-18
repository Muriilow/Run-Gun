#include <stdlib.h>
#include <stdio.h>
#include "GameManager.h"

struct GameManager* GameManagerCreate(struct Player* player, struct Boss* boss)
{
    struct GameManager* manager = malloc(sizeof(struct GameManager));

    if(manager == NULL)
        return NULL; 

    manager->normalEnemyList = NULL;
    manager->lifeItemList = NULL;
    manager->player = player;

    if(player==NULL)
        fprintf(stderr, "erro no player\n");

    manager->boss = boss;
    return manager;
}

void CreateItem(struct GameManager* manager, struct Position position, unsigned char type, ALLEGRO_BITMAP* sprite)
{
    struct Item* item = ItemCreate(position, manager->lifeItemList, type, sprite);

    if(item != NULL)
        manager->lifeItemList = item;
}
void CreateNormalEnemy(struct GameManager* manager, struct Position position, ALLEGRO_BITMAP* sprite, ALLEGRO_BITMAP* bullet)
{
    struct NormalEnemy* enemy = NormalEnemyCreate(250, 25, position, manager->normalEnemyList, sprite, bullet);
    
    if(enemy != NULL)
        manager->normalEnemyList = enemy;
}
void UpdateLogic(struct GameManager* manager)
{
    struct NormalEnemy* previousEnemy = NULL;
    struct Item* previousLifeItem = NULL;
    
    /*Checking if I need to free something*/
    for(struct Item* index = manager->lifeItemList; index != NULL;)
    {
        if(index->hasCollide)
        {
            if(previousLifeItem)
            {
                previousLifeItem->next = index->next;
                free(index);
                index = previousLifeItem->next;
            }
            else
            {
                manager->lifeItemList = index->next;
                free(index);
                index = manager->lifeItemList;
            }
        }
        else
        {
            previousLifeItem = index;
            index = index->next;
        }
        
    }
    for(struct NormalEnemy* index = manager->normalEnemyList; index != NULL;)
    {
        /*Checking if any enemy is dead*/
        if(index->health <= 0)
        {
            
            if(previousEnemy)
            {
                previousEnemy->next = index->next;
                NormalEnemyDestroy(index);
                index = previousEnemy->next;
            }
            else
            {
                manager->normalEnemyList = index->next;
                NormalEnemyDestroy(index);
                index = manager->normalEnemyList;
            }
        }
        else
        {
            previousEnemy = index;
            index = index->next;
        }
        
    }

    /*Updating the enemy*/
    for(struct NormalEnemy* index = manager->normalEnemyList; index != NULL; index = index->next)
        NormalEnemyUpdate(index, manager->player);


    for(struct Item* index = manager->lifeItemList; index != NULL; index = index->next)
        ItemUpdate(index, manager->player);
    
    if(manager->normalEnemyList == NULL)
        BossUpdate(manager->boss, manager->player);
}
