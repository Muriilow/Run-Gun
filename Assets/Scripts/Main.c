#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "Player.h"
#include "NormalEnemy.h"
#include "Ground.h"
#include "Item.h"
#include "GameManager.h"

#define WHITE al_map_rgb(255, 255, 255)

void InputControl(ALLEGRO_EVENT event, struct Player* player)
{
    switch(event.keyboard.keycode)
    {
        case ALLEGRO_KEY_A:
            JoystickActivate(player->control, LEFT);
            break;
        case ALLEGRO_KEY_D:
            JoystickActivate(player->control, RIGHT);
            break;
        case ALLEGRO_KEY_W:
            JoystickActivate(player->control, UP);
            break;
        case ALLEGRO_KEY_S:
            JoystickActivate(player->control, DOWN);
            break;
        case ALLEGRO_KEY_SPACE:
            if(event.type == ALLEGRO_EVENT_KEY_DOWN)
                JoystickActivate(player->control, JUMP);
            break;
        case ALLEGRO_KEY_ENTER:
            JoystickActivate(player->control, FIRE);
            break;
    }
};

void BackgroundUpdate(ALLEGRO_BITMAP* bg, int width, int xAxis)
{
    al_draw_bitmap(bg, xAxis, 0, 0);
    al_draw_bitmap(bg, xAxis + width, 0, 0);
};

int main()
{
    al_init();
    al_init_image_addon();
    al_init_primitives_addon();
    al_install_keyboard();
    al_set_new_display_flags(ALLEGRO_FULLSCREEN | ALLEGRO_OPENGL);
    
    //Initializing variables 
    ALLEGRO_MONITOR_INFO info;
    ALLEGRO_TIMER* timer;
    ALLEGRO_EVENT_QUEUE* queue; 
    ALLEGRO_FONT* font; 
    ALLEGRO_EVENT event;
    struct Item* item;
    struct Player* player;
    struct Ground* ground;
    struct GameManager* manager;
    int screenW, screenH, imgW, imgH;


    timer = al_create_timer(1.0 / 60.0);
    queue = al_create_event_queue();
    font = al_create_builtin_font();

    //Finding information about the screen
    al_get_monitor_info(0, &info);
    screenW = info.x2 - info.x1;
    screenH = info.y2 - info.y1;

    //Creating a screen with the correct display size
    ALLEGRO_DISPLAY* display = al_create_display(screenW, screenH);
    ALLEGRO_BITMAP *background = al_load_bitmap("Assets/background.jpg");
    imgW = al_get_bitmap_width(background);
    imgH = al_get_bitmap_height(background);

    //Any keyboard, screen or timer events will be inserted in our queue
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    
    struct Viewport viewport = {0, 0, 0, 0, screenW, screenH};
    struct Position pos = {301, 100, 301, 100};
    player = PlayerCreate(20, pos, &viewport);

    manager = GameManagerCreate(player);

    struct Position posEnemy1 = {1301, screenW/2, 1301, screenW/2};
    CreateNormalEnemy(manager, 20, posEnemy1);
    struct Position posEnemy2 = {1601, screenW/2, 1601, screenW/2}; 
    CreateNormalEnemy(manager, 20, posEnemy2);
    struct Position posEnemy3 = {2001, screenW/2, 2001, screenW/2};
    CreateNormalEnemy(manager, 20, posEnemy3);

    struct Position posItem = {501, screenW/2, 501, screenW/2};
    CreateLifeItem(manager, 10, posItem, 1);

    al_start_timer(timer);

    int xAxis = 0;

    while(1)
    {
        al_wait_for_event(queue, &event);
        
        if(player->health <= 0)
            break;
        //Clock event
        if(event.type == ALLEGRO_EVENT_TIMER)
        {
            xAxis--; 

            if (xAxis <= -imgW)
                xAxis = 0;

            BackgroundUpdate(background, imgW, xAxis);
            PlayerUpdate(player);
            UpdateLogic(manager);  

            al_draw_textf(font, WHITE, 10, 10, ALLEGRO_ALIGN_LEFT, "STATE: %d", player->state);
            al_draw_textf(font, WHITE, 10, 100, ALLEGRO_ALIGN_LEFT, "HEALTH: %d", player->health);
            al_draw_textf(font, WHITE, 10, 20, ALLEGRO_ALIGN_LEFT, "X-pos: %d", player->position.x);
            al_draw_textf(font, WHITE, 10, 30, ALLEGRO_ALIGN_LEFT, "Y-pos: %d", player->position.y);
            al_draw_textf(font, WHITE, 200, 10, ALLEGRO_ALIGN_LEFT, "XWorld-pos: %d", player->position.worldX);
            al_draw_textf(font, WHITE, 200, 20, ALLEGRO_ALIGN_LEFT, "YWorld-pos: %d", player->position.worldY);
            al_draw_textf(font, WHITE, 10, 40, ALLEGRO_ALIGN_LEFT, "X-hitbox: %f", player->hitbox->x);
            al_draw_textf(font, WHITE, 10, 50, ALLEGRO_ALIGN_LEFT, "Y-hitbox: %f", player->hitbox->y);
            al_draw_textf(font, WHITE, 10, 60, ALLEGRO_ALIGN_LEFT, "isOnGround: %d", player->isOnGround);
            al_draw_textf(font, WHITE, 10, 70, ALLEGRO_ALIGN_LEFT, "isLeft: %d", player->isLeft);
            al_draw_textf(font, WHITE, 10, 80, ALLEGRO_ALIGN_LEFT, "isRight: %d", player->isRight);
            al_draw_textf(font, WHITE, 10, 90, ALLEGRO_ALIGN_LEFT, "velocityY: %f", player->velocityY);
            al_draw_textf(font, WHITE, 200, 30, ALLEGRO_ALIGN_LEFT, "Xviewport: %f", viewport.offsetX);
            al_draw_textf(font, WHITE, 200, 40, ALLEGRO_ALIGN_LEFT, "Yviewport: %f", viewport.offsetY);

            al_flip_display();
        }
        else if(event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_KEY_UP)
            InputControl(event, player);
        //Close the program
        else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;
    }

    al_destroy_bitmap(background);
    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    PlayerDestroy(player);
    return 0;
}
