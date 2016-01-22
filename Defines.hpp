#ifndef _DEFINES_H
#define _DEFINES_H

#include <string>

const int SCREEN_WIDTH = 428;
const int SCREEN_HEIGHT = 240;

const int MAX_MOVE = 1;
const int MAX_ROTATE = 3;

const int ARENA_WIDTH = 320;
const int ARENA_HEIGHT = 240;

const std::string GAME_FONT = "8bit.ttf";

const std::string TITLE_IMG = "title.png";

const std::string PLAYER1_TANK = "Tank1.png";
const std::string PLAYER2_TANK = "Tank2.png";
const std::string PLAYER3_TANK = "Tank3.png";
const std::string PLAYER4_TANK = "Tank4.png";

const std::string SCOREBOARD_BULLETS = "bullets.png";
const std::string SCOREBOARD_SPEED = "tankspeed.png";
const std::string SCOREBOARD_BOUNCE = "ricochet.png";

const std::string XBOX_CONTROLS_IMG = "360Controls.png";
const std::string PS4_CONTROLS_IMG = "PS4Controls.png";
const std::string CONTROLS_IMG = "Controls.png";

const std::string WALL_TILES_DIRT = "WallTiles.png";
const std::string WALL_TILES_ICE = "WallTiles2.png";
const std::string WALL_TILES_URBAN = "WallTiles3.png";



const int MAX_TILESET = 2;

const uint32_t GAME_END_TICKS = 2000;

#ifdef _WIN32
const std::string GFX_PATH = "gfx\\";
const std::string SFX_PATH = "sfx\\";
const std::string MUS_PATH = "mus\\";
const std::string FONTS_PATH = "fonts\\";
const std::string MAPS_PATH = "maps\\";
#else
const std::string GFX_PATH = "gfx/";
const std::string SFX_PATH = "sfx/";
const std::string MUS_PATH = "mus/";
const std::string FONTS_PATH = "fonts/";
const std::string MAPS_PATH = "maps/";
#endif // _WIN32

const std::string GAME_MUSIC1 = "music1.ogg";
const std::string GAME_MUSIC2 = "music2.ogg";
const std::string GAME_MUSIC3 = "music3.ogg";

const std::string INTRO_MUSIC1 = "intro1.ogg";
const std::string INTRO_MUSIC2 = "intro2.ogg";
const std::string INTRO_MUSIC3 = "intro3.ogg";

const std::string MENU_MUSIC = "menu.ogg";
const std::string SFX_FIRE = "sfx_fire.ogg";
const std::string SFX_BOUNCE = "sfx_bounce.ogg";
const std::string SFX_BOUNCE2 = "sfx_bounce2.ogg";
const std::string SFX_BOUNCE3 = "sfx_bounce3.ogg";
const std::string SFX_DIE = "sfx_die.ogg";
const std::string SFX_BULLET_WALL = "sfx_bulletWall.ogg";
const std::string SFX_BULLET_BRICK = "sfx_bulletBrick.ogg";
const std::string SFX_MENU = "sfx_menu.ogg";
const std::string SFX_MENU_CONFIRM = "sfx_menuConfirm.ogg";
const std::string SFX_PAUSE = "sfx_pause.ogg";
const std::string SFX_UNPAUSE = "sfx_unpause.ogg";
const std::string SFX_READY = "sfx_ready.ogg";
const std::string SFX_NOTREADY = "sfx_notready.ogg";

const std::string SFX_POWERUP_BOUNCE1 = "sfx_powerupBounce1.ogg";
const std::string SFX_POWERUP_BOUNCE2 = "sfx_powerupBounce2.ogg";
const std::string SFX_POWERUP_BOUNCE3 = "sfx_powerupBounce3.ogg";

const std::string SFX_POWERUP_BULLET1 = "sfx_powerupBullet1.ogg";
const std::string SFX_POWERUP_BULLET2 = "sfx_powerupBullet2.ogg";
const std::string SFX_POWERUP_BULLET3 = "sfx_powerupBullet3.ogg";

const std::string SFX_POWERUP_SPEED1 = "sfx_powerupSpeed1.ogg";
const std::string SFX_POWERUP_SPEED2 = "sfx_powerupSpeed2.ogg";
const std::string SFX_POWERUP_SPEED3 = "sfx_powerupSpeed3.ogg";

const uint32_t RENDER_INTERVAL = 16;

const uint32_t MENU_REPEAT_HORIZ_TICKS = 100;
const uint32_t MENU_REPEAT_VERT_TICKS = 300;

const int XBOX_360_CONTROLLER = 0;
const int PS4_CONTROLLER = 1;

const int SCORE_MATCH = 1;
const int STOCK_MATCH = 2;
const int TIME_MATCH = 3;

const int MIN_STOCK = 1;
const int MAX_STOCK = 100;
const int MIN_TIME = 30;
const int MAX_TIME = 600;
const int MIN_SCORE = 1;
const int MAX_SCORE = 100;

#endif // _DEFINES_H
