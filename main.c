#include<gb/gb.h>
#include<stdio.h>
#include "hero_miles.c"

#define HERO_STAND 4
#define HERO_WALK 8

#define HERO_WIDTH 16
#define HERO_HEIGHT 16

#define FACE_RIGHT 0
#define FACE_LEFT 1

#define FLOOR_Y 128

void game_delay(UINT8 cycles);
void game_init();
void game_update_switches();
void game_update_input();
void game_update_hero();
void game_update_hero_anim();
void game_update_hero_dir(UINT8 prop);
void game_update_hero_pos(UBYTE dir);
UBYTE game_get_hero_action();
UBYTE game_get_hero_dir();

UINT8 position[2];
UBYTE speed = 5;
UBYTE frame = 0;
UBYTE state = HERO_STAND;

void main()
{
    game_init();

    while(1)
    {
        game_update_input();
        game_update_hero();
        game_update_switches();
        game_delay(4);
    }
}

void game_delay(UINT8 cycles)
{
    UINT8 i;
    for(i = 0; i < cycles; i++)
    {
        wait_vbl_done();
    }
}

void game_init()
{
    DISPLAY_ON;

    SPRITES_8x16;
    set_sprite_data(0, 12, hero_miles);
    set_sprite_tile(0, HERO_STAND);
    set_sprite_tile(1, HERO_STAND+2);

    position[0] = 80;
    position[1] = FLOOR_Y;

    state = HERO_STAND | FACE_RIGHT;
    game_update_hero_pos(FACE_RIGHT);
}

void game_update_switches()
{
    HIDE_WIN;
    SHOW_SPRITES;
    //SHOW_BKG;
}

void game_update_input()
{
    // if(joypad() & J_B)
    // {
    //     set_bkg_tiles(0, 0, 20, 18, screen_blank);
    // }
    // if(joypad() & J_UP)
    // {

    // }
    if(joypad() & J_LEFT)
    {
        state = HERO_WALK | FACE_LEFT;
    }
    else if(joypad() & J_RIGHT)
    {
        state = HERO_WALK | FACE_RIGHT;
    }
    else
    {
        state = HERO_STAND | game_get_hero_dir();
    }
    
}

void game_update_hero()
{
    if(state & HERO_WALK)
    {
        if((state ^ HERO_WALK) & FACE_LEFT)
        {
            position[0] -= speed;
            game_update_hero_dir(S_FLIPX | FACE_LEFT);
            game_update_hero_pos(FACE_LEFT);
        }
        else if(!((state ^ HERO_WALK) & FACE_RIGHT))
        {
            position[0] += speed;
            game_update_hero_dir(FACE_RIGHT);
            game_update_hero_pos(FACE_RIGHT);
        }
    }

    game_update_hero_anim();
} 

void game_update_hero_anim()
{
    if(state & HERO_WALK)
    {
        if(frame == 0)
        {
            frame = 1;
            set_sprite_tile(0, HERO_WALK);
            set_sprite_tile(1, HERO_WALK+2);
            return;
        }
    }

    frame = 0;
    set_sprite_tile(0, HERO_STAND);
    set_sprite_tile(1, HERO_STAND+2);
}

void game_update_hero_dir(UINT8 prop)
{
    set_sprite_prop(0, prop);
    set_sprite_prop(1, prop);
}

void game_update_hero_pos(UBYTE dir)
{
    move_sprite(0+dir, position[0], position[1]);
    move_sprite(1-dir, position[0]+8, position[1]);
}

UBYTE game_get_hero_action()
{
    if((state^FACE_LEFT == HERO_WALK) ||
       (state^FACE_RIGHT == HERO_WALK))
    {
        return HERO_WALK;
    }
    else if((state^FACE_LEFT == HERO_STAND) ||
       (state^FACE_RIGHT == HERO_STAND))
    {
        return HERO_STAND;
    }

    return HERO_STAND;
}

UBYTE game_get_hero_dir()
{
    if(state^game_get_hero_action() == FACE_LEFT)
    {
        return FACE_LEFT;
    }

    return FACE_RIGHT;
}