#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "Player.h"
#include "NormalEnemy.h"
#include "Ground.h"
#include "Item.h"
#include "GameManager.h"
#include "Boss.h"
#include "Utils.h"

#define SCREEN_W 1366
#define SCREEN_H 768 
#define GREY al_map_rgb(164, 164, 164)
#define BLACK al_map_rgb(64, 64, 64)
#define WHITE al_map_rgb(204, 204, 204)
#define RED al_map_rgb(153, 19, 19)
#define GREEN al_map_rgb(89, 116, 32)

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

unsigned char MenuControl(ALLEGRO_EVENT event, enum GameState* mode, enum Menu* option)
{
    if(event.type != ALLEGRO_EVENT_KEY_DOWN)
        return 1;
    if(event.keyboard.keycode == ALLEGRO_KEY_ENTER)
    {
        if(*option == PLAY)
            *mode = GAME;
        else if(*option == EXIT)
            return 0;
    }
    else if(event.keyboard.keycode == ALLEGRO_KEY_S || event.keyboard.keycode == ALLEGRO_KEY_W)
    {
        if(*option == PLAY)
            *option = EXIT;

        else if(*option == EXIT)
            *option = PLAY;
    }
    return 1;
};

unsigned char PauseControl(ALLEGRO_EVENT event, enum GameState* mode, enum Menu* option)
{
    if(event.type != ALLEGRO_EVENT_KEY_DOWN)
        return 2;

    if(event.keyboard.keycode == ALLEGRO_KEY_ENTER)
    {
        if(*option == CONTINUE)
            *mode = GAME;
        else if(*option == BACK)
        {
            *option = PLAY;
            *mode = MENU;
        }
        return 1;
    }
    else if(event.keyboard.keycode == ALLEGRO_KEY_S || event.keyboard.keycode == ALLEGRO_KEY_W)
    {
        if(*option == CONTINUE)
            *option = BACK;

        else if(*option == BACK)
            *option = CONTINUE;
    }

    return 0;
};

int main()
{
    al_init();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_init_primitives_addon();
    al_install_keyboard();
    al_set_new_display_flags(ALLEGRO_OPENGL);
 

    //Initializing variables 
    ALLEGRO_TIMER* timer;
    ALLEGRO_EVENT_QUEUE* queue; 
    ALLEGRO_EVENT event;
    unsigned char xHeartOffset = 40;
    unsigned char ignoreKeyboard = 1;
    unsigned char running = 1;
    struct Player* player;
    struct GameManager* manager;
    int imgW, imgH;
    enum GameState mode = MENU;
    enum Menu option = PLAY;

    timer = al_create_timer(1.0 / 60.0);
    queue = al_create_event_queue();

    //Creating a screen with the correct display size
    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_W, SCREEN_H);
    ALLEGRO_BITMAP *background = al_load_bitmap("Assets/Sprites/background.png");
    ALLEGRO_BITMAP *healthPW = al_load_bitmap("Assets/Sprites/healthPw.png");
    ALLEGRO_BITMAP *doubleJumpPW = al_load_bitmap("Assets/Sprites/doubleJumpPw.png");
    ALLEGRO_BITMAP *menu = al_load_bitmap("Assets/Sprites/menu.png");
    ALLEGRO_BITMAP *playerMove = al_load_bitmap("Assets/Sprites/playerMove.png");
    ALLEGRO_BITMAP *playerBullet = al_load_bitmap("Assets/Sprites/bulletPlayer.png");
    ALLEGRO_BITMAP *enemySprite = al_load_bitmap("Assets/Sprites/normalEnemy.png");
    ALLEGRO_BITMAP *enemyBullet = al_load_bitmap("Assets/Sprites/bulletEnemy.png");
    ALLEGRO_BITMAP *ground = al_load_bitmap("Assets/Sprites/ground.png");
    ALLEGRO_BITMAP *heart = al_load_bitmap("Assets/Sprites/heart.png");
    ALLEGRO_FONT *font = al_load_font("Assets/Fonts/Canterbury.ttf", 84, 0);
    
    imgW = al_get_bitmap_width(background);
    imgH = al_get_bitmap_height(background);
    
    int menuW = al_get_bitmap_width(menu);
    int menuH = al_get_bitmap_height(menu);

    //Any keyboard, screen or timer events will be inserted in our queue
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
   
    /*Initializing the game */
    struct Viewport viewport = {0, 0, 0, 0, SCREEN_W, SCREEN_H};
    struct Position pos = {301, 100, 301, 100};
    player = PlayerCreate(100, pos, &viewport, playerMove, playerBullet);

    struct Position posBoss = {5001, 200, 5001, 200};
    struct Boss* boss = BossCreate(posBoss);  
    manager = GameManagerCreate(player, boss);

    struct Position posEnemy1 = {1301, SCREEN_H - 125, 1301, SCREEN_H - 125};
    CreateNormalEnemy(manager, posEnemy1, enemySprite, enemyBullet);
    struct Position posEnemy2 = {1601, SCREEN_H - 125, 1601, SCREEN_H - 125}; 
    CreateNormalEnemy(manager, posEnemy2, enemySprite, enemyBullet);
    struct Position posEnemy3 = {2001, SCREEN_H - 125, 2001, SCREEN_H - 125};
    CreateNormalEnemy(manager, posEnemy3, enemySprite, enemyBullet);
    struct Position posEnemy4 = {2401, SCREEN_H - 125, 2401, SCREEN_H - 125};
    CreateNormalEnemy(manager, posEnemy4, enemySprite, enemyBullet);
    struct Position posEnemy5 = {2601, SCREEN_H - 125, 2601, SCREEN_H - 125};
    CreateNormalEnemy(manager, posEnemy5, enemySprite, enemyBullet);
    struct Position posEnemy6 = {3001, SCREEN_H - 125, 3001, SCREEN_H - 125};
    CreateNormalEnemy(manager, posEnemy6, enemySprite, enemyBullet);

    struct Position posItem1 = {501, SCREEN_H - 125, 501, SCREEN_H - 125};
    CreateItem(manager, posItem1, 0, healthPW);

    struct Position posItem2 = {1001, SCREEN_H - 125, 1001, SCREEN_H - 125};
    CreateItem(manager, posItem2, 0, healthPW);

    struct Position posItem3 = {2001, SCREEN_H - 125, 2001, SCREEN_H - 125};
    CreateItem(manager, posItem3, 1, doubleJumpPW);

    struct Position posItem4 = {3001, 400, 3001, 400};
    CreateItem(manager, posItem4, 0, healthPW);

    int xAxisBack = 0;
    int xAxisPlayer = 0;
    int xAxis = 0;

    al_start_timer(timer);
    while(running)
    {
        al_wait_for_event(queue, &event);

        if(event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_KEY_UP)
        {
            if(mode == GAME)
            {
                //if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                //{
                //    option = CONTINUE;
                //    mode = PAUSE;
                //    continue;
                //}

                /*We need to ignore the first key event because it will unsync the joystick with event_key_up*/
                if(ignoreKeyboard == 1)
                {
                    al_flush_event_queue(queue);
                    ignoreKeyboard = 0;
                    continue;
                }

                InputControl(event, player);
            }
            else if(mode == MENU)
                running = MenuControl(event, &mode, &option);
            else if(mode == PAUSE)
                ignoreKeyboard = PauseControl(event, &mode, &option);
            else if(mode == GAME_OVER || mode == GAME_WIN)
            {
                if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                    running = FALSE;
            }

        }
        else if(event.type == ALLEGRO_EVENT_TIMER)
        {
            if(mode == GAME)
            {
                if(player->health <= 0)
                {
                    mode = GAME_OVER;
                    option = PLAY;
                    continue;
                }
                else if(boss->health <= 0)
                {
                    mode = GAME_WIN;
                    continue;
                }
            
                xAxis--; 
                xAxisBack = xAxis % -imgW;
                xAxisPlayer = (int)-player->viewport->offsetX % -imgW;
                    
                al_draw_scaled_bitmap(background, 0, 0, imgW, imgH, xAxisBack, 0, imgW, SCREEN_H, 0);
                al_draw_scaled_bitmap(background, 0, 0, imgW, imgH, xAxisBack + imgW, 0, imgW, SCREEN_H, 0);
                al_draw_scaled_bitmap(ground, 0, 0, imgW, imgH, xAxisPlayer, 0, imgW, SCREEN_H, 0);
                al_draw_scaled_bitmap(ground, 0, 0, imgW, imgH, xAxisPlayer + imgW, 0, imgW, SCREEN_H, 0);
                for(int i = 0; i < player->health; i++)
                    al_draw_scaled_bitmap(heart, 0, 0, 150, 150, 50 + xHeartOffset*i, 50, 50, 50, 0);


                PlayerUpdate(player);
                UpdateLogic(manager); 
            }
            else if(mode == MENU)
            {
                al_draw_scaled_bitmap(menu, 0, 0, menuW, menuH, 0, 0, SCREEN_W, SCREEN_H, 0);

                if(option == PLAY)
                    al_draw_text(font, WHITE, SCREEN_W - 400, 200, ALLEGRO_ALIGN_LEFT, "Play");
                else
                    al_draw_text(font, GREY, SCREEN_W - 400, 200, ALLEGRO_ALIGN_LEFT, "Play");

                if(option == EXIT)
                    al_draw_text(font, WHITE, SCREEN_W - 400, 350, ALLEGRO_ALIGN_LEFT, "Exit");
                else
                    al_draw_text(font, GREY, SCREEN_W - 400, 350, ALLEGRO_ALIGN_LEFT, "Exit");
                
            }
            else if(mode == PAUSE)
            {
                if(option == CONTINUE)
                    al_draw_text(font, GREY, SCREEN_W/2, 200, ALLEGRO_ALIGN_CENTER, "Continue");
                else
                    al_draw_text(font, BLACK, SCREEN_W/2, 200, ALLEGRO_ALIGN_CENTER, "Continue");
                if(option == BACK)
                    al_draw_text(font, GREY, SCREEN_W/2, 350, ALLEGRO_ALIGN_CENTER, "Menu");
                else
                    al_draw_text(font, BLACK, SCREEN_W/2, 350, ALLEGRO_ALIGN_CENTER, "Menu");
            }
            else if(mode == GAME_OVER)
            {
                al_clear_to_color(WHITE);
                al_draw_text(font, RED, SCREEN_W/2, 200, ALLEGRO_ALIGN_CENTER, "GAME OVER!");
                al_draw_text(font, BLACK, SCREEN_W/2, 300, ALLEGRO_ALIGN_CENTER, "Press ESC to quit");
            }
            else if(mode == GAME_WIN)
            {
                al_clear_to_color(WHITE);
                al_draw_text(font, GREEN, SCREEN_W/2, 200, ALLEGRO_ALIGN_CENTER, "VICTORY!");
                al_draw_text(font, BLACK, SCREEN_W/2, 300, ALLEGRO_ALIGN_CENTER, "Press ESC to quit");
            }

            al_flip_display();
        }

        //Close the program
        if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            running = FALSE;
    }

    al_destroy_bitmap(background);
    al_destroy_bitmap(menu);
    al_destroy_bitmap(heart);
    al_destroy_bitmap(healthPW);
    al_destroy_bitmap(doubleJumpPW);
    al_destroy_bitmap(ground);
    al_destroy_bitmap(playerMove);
    al_destroy_bitmap(playerBullet);
    al_destroy_bitmap(enemyBullet);
    al_destroy_bitmap(enemySprite);
    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    PlayerDestroy(player);
    BossDestroy(boss);
    return 0;
}
