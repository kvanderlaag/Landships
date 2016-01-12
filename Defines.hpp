#ifndef _DEFINES_H
#define _DEFINES_H

#define SCREEN_WIDTH 428
#define SCREEN_HEIGHT 240

#define MAX_MOVE 1
#define MAX_ROTATE 3

#define ARENA_WIDTH 320
#define ARENA_HEIGHT 240

#define GAME_FONT "8bit.ttf"

#define GAME_MUSIC "music.ogg"
#define SFX_FIRE "sfx_fire.ogg"
#define SFX_BOUNCE "sfx_bounce.ogg"
#define SFX_BOUNCE2 "sfx_bounce2.ogg"
#define SFX_BOUNCE3 "sfx_bounce3.ogg"
#define SFX_DIE "sfx_die.ogg"

#define RENDER_INTERVAL 16


#define JBUTTON_DPADUP 0
#define JBUTTON_DPADDOWN 1
#define JBUTTON_DPADLEFT 2
#define JBUTTON_DPADRIGHT 3
#define JBUTTON_START 4
#define JBUTTON_BACK 5
#define JBUTTON_FIRE 9
#define JBUTTON_A 10
#define JAXIS_MOVE   0x01
#define JAXIS_ROTATE 0x00
#define JAXIS_TURRET 0x02
#define JAXIS_TURRETX 0x02
#define JAXIS_TURRETY 0x03
#define JAXIS_MOVEX 0x00
#define JAXIS_MOVEY 0x01
#define JAXIS_FIRE 0x05

#define SCORE_MATCH 1
#define STOCK_MATCH 2
#define TIME_MATCH 3

#define MIN_STOCK 1
#define MAX_STOCK 100
#define MIN_TIME 30
#define MAX_TIME 600
#define MIN_SCORE 1
#define MAX_SCORE 100

#endif // _DEFINES_H
