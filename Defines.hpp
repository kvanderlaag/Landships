#ifndef _DEFINES_H
#define _DEFINES_H

#define SCREEN_WIDTH 428
#define SCREEN_HEIGHT 240

#define MAX_MOVE 1
#define MAX_ROTATE 3

#define ARENA_WIDTH 320
#define ARENA_HEIGHT 240

#define GAME_FONT "8bit.ttf"

#define TITLE_IMG "title.png"

#define WALL_TILES_DIRT "WallTiles.png"
#define WALL_TILES_ICE "WallTiles2.png"
#define WALL_TILES_URBAN "WallTiles3.png"

#define MAX_TILESET 2

#define GAME_END_TICKS 2000

#define GAME_MUSIC1 "music1.ogg"
#define GAME_MUSIC2 "music1.ogg"
#define GAME_MUSIC3 "music1.ogg"

#define INTRO_MUSIC1 "intro1.ogg"
#define INTRO_MUSIC2 "intro1.ogg"
#define INTRO_MUSIC3 "intro1.ogg"

#define MENU_MUSIC  "menu.ogg"
#define SFX_FIRE "sfx_fire.ogg"
#define SFX_BOUNCE "sfx_bounce.ogg"
#define SFX_BOUNCE2 "sfx_bounce2.ogg"
#define SFX_BOUNCE3 "sfx_bounce3.ogg"
#define SFX_DIE "sfx_die.ogg"

#define SFX_POWERUP_BOUNCE1 "sfx_powerupBounce1.ogg"
#define SFX_POWERUP_BOUNCE2 "sfx_powerupBounce1.ogg"
#define SFX_POWERUP_BOUNCE3 "sfx_powerupBounce1.ogg"

#define SFX_POWERUP_BULLET1 "sfx_powerupBullet1.ogg"
#define SFX_POWERUP_BULLET2 "sfx_powerupBullet1.ogg"
#define SFX_POWERUP_BULLET3 "sfx_powerupBullet1.ogg"

#define SFX_POWERUP_SPEED1 "sfx_powerupSpeed1.ogg"
#define SFX_POWERUP_SPEED2 "sfx_powerupSpeed1.ogg"
#define SFX_POWERUP_SPEED3 "sfx_powerupSpeed1.ogg"

#define RENDER_INTERVAL 16

#define MENU_REPEAT_HORIZ_TICKS 100
#define MENU_REPEAT_VERT_TICKS 300

#ifndef _PS4_CONTROLLER

#define JBUTTON_DPADUP 10
#define JBUTTON_DPADDOWN 11
#define JBUTTON_DPADLEFT 12
#define JBUTTON_DPADRIGHT 13
#define JBUTTON_START 7
#define JBUTTON_BACK 6
#define JBUTTON_FIRE 5
#define JBUTTON_A 0
#define JBUTTON_B 1
#define JAXIS_MOVE   0x01
#define JAXIS_ROTATE 0x00
#define JAXIS_LTRIGGER 0x02
#define JAXIS_TURRETX 0x03
#define JAXIS_TURRETY 0x04
#define JAXIS_MOVEX 0x00
#define JAXIS_MOVEY 0x01
#define JAXIS_FIRE 0x05

#else

#define JBUTTON_DPADUP 13
#define JBUTTON_DPADDOWN 11
#define JBUTTON_DPADLEFT 12
#define JBUTTON_DPADRIGHT 3
#define JBUTTON_START 9
#define JBUTTON_BACK 8
#define JBUTTON_FIRE 5
#define JBUTTON_A 1
#define JBUTTON_B 2
#define JAXIS_MOVE   0x01
#define JAXIS_ROTATE 0x00
#define JAXIS_TURRET 0x03
#define JAXIS_TURRETX 0x02
#define JAXIS_TURRETY 0x05
#define JAXIS_MOVEX 0x00
#define JAXIS_MOVEY 0x01
#define JAXIS_FIRE 0x04

#endif // _PS4_CONTROLLER

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
