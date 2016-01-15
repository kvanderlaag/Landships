#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <climits>
#include <map>
#include <ctime>

#include <dirent.h>

#include "Player.hpp"
#include "Tile.hpp"
#include "Map.hpp"
#include "Rectangle.hpp"
#include "Vector2D.hpp"
#include "Point.hpp"
#include "Collider.hpp"
#include "Explosion.hpp"
#include "Container.hpp"
#include "Powerup.hpp"
#include "Defines.hpp"
#include "Options.hpp"


bool gRunning = true;
int rndTiles = 0;
std::string wallTileNames[MAX_TILESET + 1] = { WALL_TILES_DIRT, WALL_TILES_ICE, WALL_TILES_URBAN };

Mix_Music* gameMusic[MAX_TILESET + 1] = { NULL, NULL, NULL };
Mix_Music* introMusic[MAX_TILESET + 1] = { NULL, NULL, NULL };
Mix_Music* menuMusic = { NULL };
Mix_Chunk* sfxFire = NULL;
Mix_Chunk* sfxBounce[3] = { NULL, NULL, NULL };
Mix_Chunk* sfxDie = NULL;
Mix_Chunk* sfxPowerupSpeed[MAX_TILESET + 1] = { NULL, NULL, NULL };
Mix_Chunk* sfxPowerupBounce[MAX_TILESET + 1] = { NULL, NULL, NULL };
Mix_Chunk* sfxPowerupBullet[MAX_TILESET + 1] = { NULL, NULL, NULL };
std::default_random_engine generator(time(0));



bool playersIn[4] = { false, false, false, false };
int playersInCount = 0;
int maxPlayers = 0;
std::string mapfilename;

bool altHeld = false;
bool fullscreen = true;

SDL_Window* win = NULL;
SDL_Renderer* ren = NULL;
SDL_Joystick* gController[4] = { NULL, NULL, NULL, NULL };

const int JOYTURRET_DEADZONE = 12000;
const int JOYMOVE_DEADZONE = 12000;
const int JOYFIRE_DEADZONE = 0;

const std::string basePath = SDL_GetBasePath();
void NewExplosion(const float x, const float y, SDL_Renderer* ren, std::map<int, RenderableObject*>& vRenderable, std::vector<Explosion*>& vExplosions);

int Title();
int Menu();
Options* OptionsMenu();
int WinScreen(bool (&winningPlayer)[4], Player (&players)[4]);
void Quit(int status);

int main(int argc, char** argv) {

    freopen("error.log", "w", stdout);

    uint32_t ticks = SDL_GetTicks();
    uint32_t old_time = SDL_GetTicks();

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER |SDL_INIT_JOYSTICK) < 0) {
        std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
        Quit(1);
    }

    if (IMG_Init(IMG_INIT_PNG) < 0) {
        std::cout << "Error initializing SDL_IMG: " << SDL_GetError() << std::endl;
        Quit(2);
    }

    if (TTF_Init() != 0) {
        std::cout << "Error initializing SDL_TTF: " << SDL_GetError() << std::endl;
        Quit(3);
    }


    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        std::cout << "Error initializing SDL_Mixer: " << Mix_GetError() << std::endl;
        Quit(4);
    }

    gameMusic[0] = Utility::LoadMusic(GAME_MUSIC1);
    gameMusic[1] = Utility::LoadMusic(GAME_MUSIC2);
    gameMusic[2] = Utility::LoadMusic(GAME_MUSIC3);

    introMusic[0] = Utility::LoadMusic(INTRO_MUSIC1);
    introMusic[1] = Utility::LoadMusic(INTRO_MUSIC2);
    introMusic[2] = Utility::LoadMusic(INTRO_MUSIC3);

    menuMusic = Utility::LoadMusic(MENU_MUSIC);
    sfxFire = Utility::LoadSound(SFX_FIRE);
    sfxBounce[0] = Utility::LoadSound(SFX_BOUNCE);
    sfxBounce[1] = Utility::LoadSound(SFX_BOUNCE2);
    sfxBounce[2] = Utility::LoadSound(SFX_BOUNCE3);
    sfxDie = Utility::LoadSound(SFX_DIE);

    sfxPowerupBounce[0] = Utility::LoadSound(SFX_POWERUP_BOUNCE1);
    sfxPowerupBounce[1] = Utility::LoadSound(SFX_POWERUP_BOUNCE1);
    sfxPowerupBounce[2] = Utility::LoadSound(SFX_POWERUP_BOUNCE1);

    sfxPowerupBullet[0] = Utility::LoadSound(SFX_POWERUP_BULLET1);
    sfxPowerupBullet[1] = Utility::LoadSound(SFX_POWERUP_BULLET2);
    sfxPowerupBullet[2] = Utility::LoadSound(SFX_POWERUP_BULLET3);

    sfxPowerupSpeed[0] = Utility::LoadSound(SFX_POWERUP_SPEED1);
    sfxPowerupSpeed[1] = Utility::LoadSound(SFX_POWERUP_SPEED2);
    sfxPowerupSpeed[2] = Utility::LoadSound(SFX_POWERUP_SPEED3);

    if (gameMusic[0] == nullptr || sfxFire == nullptr || sfxBounce[0] == nullptr || sfxBounce[1] == nullptr
        || sfxBounce[2] == nullptr || sfxDie == nullptr ) {
        std::cout << "Could not load sound effects. Exiting." << std::endl;
        Quit(5);
        return 5;
    }




    // Initialize joysticks
    //const int JOYSTICK_DEADZONE = 10000;


    if (SDL_NumJoysticks() > 0) {
        maxPlayers = std::min(SDL_NumJoysticks(), 4);
        for (int i = 0; i < std::min(SDL_NumJoysticks(), 4); ++i) {
            gController[i] = SDL_JoystickOpen(i);
            if (gController[i] == NULL) {
                std::cout << "Could not open joystick " << i << ". SDL Error: " << SDL_GetError() << std::endl;
                Quit(2);
            }
        }
    }

    int playerx, playery;
    playerx = (SCREEN_WIDTH / 2);
    playery = (SCREEN_HEIGHT / 2);

    win = SDL_CreateWindow("Tanks", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, SDL_WINDOW_SHOWN);
    //SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN);
    SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_ShowCursor(0);

    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);


    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");  // make the scaled rendering look smoother.
    SDL_RenderSetLogicalSize(ren, SCREEN_WIDTH, SCREEN_HEIGHT);

    uint32_t rmask, gmask, bmask, amask;
        #if SDL_BYTEORDER == SDL_BIG_ENDIAN
            rmask = 0xff000000;
            gmask = 0x00ff0000;
            bmask = 0x0000ff00;
            amask = 0x000000ff;
        #else
            amask = 0xff000000;
            bmask = 0x00ff0000;
            gmask = 0x0000ff00;
            rmask = 0x000000ff;
        #endif // SDL_BIG_ENDIAN

    SDL_Surface* rSurface = SDL_CreateRGBSurface(0, 1, 1, 32, rmask, gmask, bmask, amask);
        uint32_t backgroundColor[MAX_TILESET + 1] = { SDL_MapRGB(rSurface->format, 0x20, 0x20, 0x05),
                                                SDL_MapRGB(rSurface->format, 0x00, 0x00, 0x40),
                                                SDL_MapRGB(rSurface->format, 0x30, 0x30, 0x30)
                                            };
        SDL_FreeSurface(rSurface);

    bool loopGame = true;

    if (Title() == -1) {
            Quit(0);
        }

    while (loopGame) {


        Mix_FadeInMusic(menuMusic, -1, 1000);

        if (Menu() == -1) {
            Quit(0);
        }

        Options* gameOptions = OptionsMenu();
        if (gameOptions == nullptr) {
            Quit(0);
        }
        Mix_FadeOutMusic(500);
        SDL_Delay(500);
        uint32_t gameTime = gameOptions->GetTime() * 1000;

        if (argc > 1)
            mapfilename = argv[1];
        //else
            //mapfilename = "default.d";

        std::uniform_int_distribution<int> distTiles(0,MAX_TILESET);
        rndTiles = distTiles(generator);

        Map m(mapfilename, wallTileNames[rndTiles], ren);
        if (!m.LoadSuccess()) {
            std::cout << "Level file " << mapfilename << " not a valid map.";
            exit(10);
        }

        Player players[4] = { Player("Tank1.png", 1, gameOptions->GetStock(), ren),
                                Player("Tank2.png", 2, gameOptions->GetStock(), ren),
                                Player("Tank3.png", 3, gameOptions->GetStock(),ren),
                                Player("Tank4.png", 4, gameOptions->GetStock(),ren) };
        //Player& p = players[0];

        for (int i = 0; i < 4; ++i) {
            if (!playersIn[i]) {
                //players[i].Die();
            }
            Vector2D startPos = m.GetStartPos(i + 1);
            if (startPos.GetX() == 0 && startPos.GetY() == 0) {
                players[i].SetX(playerx);
                players[i].SetY(playery);
            } else {
                players[i].SetX(startPos.GetX());
                players[i].SetY(startPos.GetY());

            }
        }

        for (int i = 0; i < 4; i++) {
            if (players[i].GetTexture() == nullptr) {
                std::cout << "Error loading player sprite: " << SDL_GetError() << std::endl;
                SDL_DestroyWindow(win);
                SDL_DestroyRenderer(ren);
                IMG_Quit();
                TTF_Quit();
                SDL_Quit();
                return 1;
            }
        }

        std::vector<Player*> vPlayers, vPlayersDelete;
        std::vector<Collider*> vStationary, vStationaryDelete;
        std::map<int, RenderableObject*> vRenderable, vRenderableDelete;
        std::map<int, Bullet*> vBullets, vBulletsDelete;
        std::vector<Explosion*> vExplosions, vExplosionsDelete;
        std::vector<Container*> vContainers, vContainersDelete;
        std::vector<Powerup*> vPowerups, vPowerupsDelete;

        bool winningPlayer[4] = {false, false, false, false};

        for (int i = 0; i < std::max(maxPlayers, 1); ++i) {
            if (playersIn[i]) {
                if (gameOptions->GetMatchType() == STOCK_MATCH) {
                    winningPlayer[i] = true;
                }
                vPlayers.push_back(&players[i]);
                vRenderable.insert(std::pair<int, RenderableObject*>(RenderableObject::next, &players[i]));
                RenderableObject::next++;
            } else {
                players[i].Die();
            }
        }

        std::vector<Collider>& mapColliders = m.GetColliders();

        for (Collider& c : mapColliders) {
            vStationary.push_back(&c);
        }

        vRenderable.insert(std::pair<int, RenderableObject*>(RenderableObject::next, &m));
        RenderableObject::next++;

        //Mix_PlayMusic(introMusic[rndTiles], 0);
        //SDL_Delay(700);
        Utility::PlayMusic(gameMusic[rndTiles]);

        bool running = true;

        std::uniform_int_distribution<int> containerSpawnDist(5000, 30000);
        int32_t containerSpawnTicks = containerSpawnDist(generator);
        const int maxContainers = 5;
        int containers = 0;

        while (running == true) {
            uint32_t new_time = SDL_GetTicks();
            uint32_t frame_time = std::min(new_time - old_time, (uint32_t) 30);
            old_time = new_time;

            switch (gameOptions->GetMatchType()) {
                case TIME_MATCH:
                    if (gameTime > frame_time) {
                        gameTime -= frame_time;
                    } else {
                        int highScore = INT_MIN;
                        for (int i = 0; i < 4; ++i) {
                            if (players[i].GetScore() >= highScore && playersIn[i]) {
                                highScore = players[i].GetScore();
                            }
                        }
                        for (int i = 0; i < 4; ++i) {
                            if (players[i].GetScore() == highScore && playersIn[i]) {
                                winningPlayer[i] = true;
                            }
                        }
                        running = false;
                    }
                    break;
                case SCORE_MATCH:
                    for (int i = 0; i < 4; ++i) {
                        if (players[i].GetScore() >= gameOptions->GetScore() ) {
                            winningPlayer[i] = true;
                        }
                    }
                    for (int i = 0; i < 4; ++i) {
                        if (winningPlayer[i]) {
                            running = false;
                        }
                    }
                    break;
                case STOCK_MATCH:
                    int playersAlive = 0;
                    for (int i = 0; i < 4; ++i) {
                        if (players[i].IsDead()) {
                            winningPlayer[i] = false;
                        } else if (playersIn[i]) {
                            playersAlive++;
                        }
                    }
                    if (playersAlive < std::min(maxPlayers + 1, 2)) {
                        running = false;
                    }


                    break;
            }

            if (running == false) {
                break;
            }

            SDL_Event e;
            while (SDL_PollEvent(&e)) {

                int index = -1;
                if (e.type == SDL_JOYAXISMOTION) {
                    for (int i = 0; i < maxPlayers; ++i) {
                        if (SDL_JoystickInstanceID(gController[i]) == e.jaxis.which) {
                            index = i;
                            break;
                        }
                    }

                } else if (e.type == SDL_JOYBUTTONDOWN || e.type == SDL_JOYBUTTONUP) {
                    for (int i = 0; i < maxPlayers; ++i) {
                        if (SDL_JoystickInstanceID(gController[i]) == e.jbutton.which) {
                            index = i;
                            break;
                        }
                    }
                }

                if (e.type == SDL_QUIT) {
                    running = false;
                    loopGame = false;
                } else if (e.type == SDL_JOYBUTTONDOWN) {
                    if (e.jbutton.button == JBUTTON_FIRE && !players[index].FireHeld()) {
                        players[index].FireIsHeld(true);
                    } else if (e.jbutton.button == JBUTTON_BACK) {
                        running = false;
                    }
                    #ifdef _POWERUP_DEBUG
                    else if (e.jbutton.button == JBUTTON_A) {
                        players[index].IncreaseMaxSpeed();
                        players[index].IncreaseMaxSpeed();
                        players[index].IncreaseMaxBounce();
                        players[index].IncreaseMaxBounce();
                        players[index].IncreaseMaxBullets();
                        players[index].IncreaseMaxBullets();
                        players[index].IncreaseMaxBullets();
                        players[index].IncreaseMaxBullets();
                    }
                    #endif
                } else if (e.type == SDL_JOYBUTTONUP) {
                    if (e.jbutton.button == JBUTTON_FIRE) {
                        players[index].FireIsHeld(false);
                        players[index].FireIsReleased(true);
                    }
                } else if (e.type == SDL_JOYAXISMOTION) {
                    if (e.jaxis.axis == JAXIS_FIRE) {
                        if (e.jaxis.value > JOYFIRE_DEADZONE) {
                            if (!players[index].FireHeld())
                                players[index].FireIsHeld(true);
                        } else {
                            players[index].FireIsHeld(false);
                            players[index].FireIsReleased(true);
                        }

                    //std::cout << "Joystick " << e.jaxis.which << " - Axis " << e.jaxis.axis << ": " << e.jaxis.value << std::endl;
                    } else if (e.jaxis.axis == JAXIS_MOVEX || e.jaxis.axis == JAXIS_MOVEY) {
                        int32_t joyX, joyY;
                        joyX = SDL_JoystickGetAxis(gController[index], JAXIS_MOVEX);
                        joyY = SDL_JoystickGetAxis(gController[index], JAXIS_MOVEY);
                        if (std::sqrt(joyX * joyX + joyY * joyY) > JOYMOVE_DEADZONE) {
                            players[index].SetJoyMove(true);
                        //if (e.jaxis.value < -JOYSTICK_DEADZONE || e.jaxis.value > JOYSTICK_DEADZONE) {
                        } else {
                            players[index].SetJoyMove(false);
                            players[index].SetRotationVel(0);
                            players[index].SetForwardVel(0);

                        }
                    //if (e.jaxis.axis == JAXIS_ROTATE) {
                        // X-axis
                        /*
                        if (e.jaxis.value < -JOYSTICK_DEADZONE || e.jaxis.value > JOYSTICK_DEADZONE) {
                            players[e.jaxis.which].SetJoyRotate(true);

                            float scale = (e.jaxis.value > 0) - (e.jaxis.value < 0);
                            //std::cout << "Rotating " << scale << std::endl;
                            players[e.jaxis.which].SetRotationVel(MAX_ROTATE * scale);
                        } else {
                                players[e.jaxis.which].SetJoyRotate(false);
                                players[e.jaxis.which].SetRotationVel(0);
                        }
                        */
                    //} else if (e.jaxis.axis == JAXIS_MOVE) {
                        // Y-axis
                        /*
                        if (e.jaxis.value < -JOYSTICK_DEADZONE * 1.5 || e.jaxis.value > JOYSTICK_DEADZONE * 1.5) {
                            players[e.jaxis.which].SetJoyMove(true);

                            float scale = (e.jaxis.value > 0) - (e.jaxis.value < 0);
                            //std::cout << "Moving " << scale << std::endl;
                            players[e.jaxis.which].SetForwardVel(MAX_MOVE * -scale);
                        } else {
                            players[e.jaxis.which].SetJoyMove(false);
                            players[e.jaxis.which].SetForwardVel(0);

                        }
                        */
                    } else if (e.jaxis.axis == JAXIS_TURRETX || e.jaxis.axis == JAXIS_TURRETY) {
                        // Right X-axis
                        /*
                        if (e.jaxis.value < -JOYSTICK_DEADZONE || e.jaxis.value > JOYSTICK_DEADZONE) {
                            players[e.jaxis.which].SetJoyTurret(true);

                            float scale = (e.jaxis.value > 0) - (e.jaxis.value < 0);
                            std::cout << "Rotating Turret " << scale << std::endl;
                            players[e.jaxis.which].SetTurretRotationVel(MAX_ROTATE * scale);
                        } else {
                            players[e.jaxis.which].SetJoyTurret(false);
                            players[e.jaxis.which].SetTurretRotationVel(0);

                        }
                        */
                        int32_t joyX, joyY;
                        joyX = SDL_JoystickGetAxis(gController[index], JAXIS_TURRETX);
                        joyY = SDL_JoystickGetAxis(gController[index], JAXIS_TURRETY);
                        if (std::sqrt(joyX * joyX + joyY * joyY) > JOYTURRET_DEADZONE) {
                            players[index].SetJoyTurret(true);
                        //if (e.jaxis.value < -JOYSTICK_DEADZONE || e.jaxis.value > JOYSTICK_DEADZONE) {
                        } else {
                            players[index].SetJoyTurret(false);
                            players[index].SetTurretRotationVel(0);

                        }

                    }

                } else if (e.type == SDL_KEYDOWN) {
                    switch (e.key.keysym.sym) {
                    case SDLK_p:
                        #ifdef _POWERUP_DEBUG
                        players[0].IncreaseMaxSpeed();
                        players[0].IncreaseMaxSpeed();
                        players[0].IncreaseMaxBounce();
                        players[0].IncreaseMaxBounce();
                        players[0].IncreaseMaxBullets();
                        players[0].IncreaseMaxBullets();
                        players[0].IncreaseMaxBullets();
                        players[0].IncreaseMaxBullets();
                        #endif
                        break;
                    case SDLK_LALT:
                    case SDLK_RALT:
                        altHeld = true;
                        break;
                    case SDLK_RETURN:
                        if (altHeld) {
                            if (fullscreen) {
                                SDL_SetWindowFullscreen(win, SDL_WINDOW_SHOWN);
                                fullscreen = false;
                            } else {
                                SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN_DESKTOP);
                                fullscreen = true;
                            }
                        }
                        break;
                    case SDLK_PAUSE:
                    case SDLK_END:
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                    case SDLK_LEFT:
                        if (!players[0].JoyMove())
                            players[0].SetRotationVel(-MAX_ROTATE);
                        break;
                    case SDLK_RIGHT:
                        if (!players[0].JoyMove())
                            players[0].SetRotationVel(MAX_ROTATE);
                        break;
                    case SDLK_a:
                    case SDLK_TAB:
                        if (!players[0].JoyTurret())
                            players[0].SetTurretRotationVel(-MAX_ROTATE);
                        break;
                    case SDLK_d:
                    case SDLK_BACKSPACE:
                        if (!players[0].JoyTurret())
                            players[0].SetTurretRotationVel(MAX_ROTATE);
                        break;
                    case SDLK_UP:
                        if (!players[0].JoyMove())
                            players[0].SetForwardVel(MAX_MOVE);
                        break;
                    case SDLK_DOWN:
                        if (!players[0].JoyMove())
                            players[0].SetForwardVel(-MAX_MOVE);
                        break;
                    case SDLK_SPACE:
                        players[0].FireIsHeld(true);
                        break;
                    }

                } else if (e.type == SDL_KEYUP) {
                    switch (e.key.keysym.sym) {
                    case SDLK_LALT:
                    case SDLK_RALT:
                        altHeld = false;
                        break;
                    case SDLK_LEFT:
                        if (players[0].GetRotationVel() < 0 && !players[0].JoyMove())
                            players[0].SetRotationVel(0);
                        break;
                    case SDLK_RIGHT:
                        if (players[0].GetRotationVel() > 0 && !players[0].JoyMove())
                            players[0].SetRotationVel(0);
                        break;
                    case SDLK_TAB:
                    case SDLK_a:
                        if (players[0].GetTurretRotationVel() < 0)
                            players[0].SetTurretRotationVel(0);
                        break;
                    case SDLK_BACKSPACE:
                    case SDLK_d:
                        if (players[0].GetTurretRotationVel() > 0)
                            players[0].SetTurretRotationVel(0);
                        break;
                    case SDLK_UP:
                        if (players[0].GetForwardVel() > 0 && !players[0].JoyMove())
                            players[0].SetForwardVel(0);
                        break;
                    case SDLK_DOWN:
                        if (players[0].GetForwardVel() < 0 && !players[0].JoyMove())
                            players[0].SetForwardVel(0);
                        break;
                    case SDLK_SPACE:
                        players[0].FireIsHeld(false);
                        players[0].FireIsReleased(true);
                        break;
                    }

                }
            }

            containerSpawnTicks -= frame_time;
            if (containerSpawnTicks <= 0) {
                if (containers <= maxContainers) {
                    std::uniform_int_distribution<int> cXdist(1,38);
                    std::uniform_int_distribution<int> cYdist(1,28);
                    int cX = cXdist(generator);
                    int cY = cYdist(generator);
                    if (m.GetTileAt(cY, cX) == 0x00 && m.GetTileAt(cY-1, cX) == 0x00 &&
                        m.GetTileAt(cY, cX-1) == 0x00 && m.GetTileAt(cY-1, cX-1) == 0x00) {
                        bool overlap = false;
                        for (Container* c1 : vContainers) {
                            if ( (cX * 8) >= (c1->GetX() - 8) && (cX * 8) <= (c1->GetX() + 8) &&
                                (cY * 8) >= (c1->GetY() - 8) && (cX * 8) <= (c1->GetY() + 8)) {
                                    overlap = true;
                                }
                        }
                        for (Player* c1 : vPlayers) {
                            if ( (cX * 8) >= (c1->GetX() - 8) && (cX * 8) <= (c1->GetX() + 8) &&
                                (cY * 8) >= (c1->GetY() - 8) && (cX * 8) <= (c1->GetY() + 8)) {
                                    overlap = true;
                                }
                        }
                        if (!overlap) {
                            Container* c = new Container(cX * 8, cY * 8, ren);
                            std::cout << "Creating new container at (" << cX << ", " << cY << ")" << std::endl;
                            vContainers.push_back(c);
                            vRenderable.insert(std::pair<int, RenderableObject*>(RenderableObject::next, c));
                            RenderableObject::next++;
                            containers++;
                            containerSpawnTicks = containerSpawnDist(generator) + containerSpawnTicks;
                        }
                    }
                }
            }

            for (int i = 0; i < 4; i++) {
                if (players[i].IsDead() ) {
                    continue;
                }
                if (players[i].FireHeld() && players[i].FireReady() && players[i].FireReleased()) {
                    Bullet* b = players[i].Fire();
                        if (b != nullptr) {
                            vBullets.insert(std::pair<int, Bullet*>(Bullet::next, b));
                            vRenderable.insert(std::pair<int, RenderableObject*>(RenderableObject::next, b));
                            RenderableObject::next++;
                        }
                }
            }

            for (int i = 0; i < 4; i++) {
                if (players[i].IsDead() ) {
                    continue;
                }
                if (players[i].JoyTurret()) {
                    int32_t joyX, joyY;
                    joyX = SDL_JoystickGetAxis(gController[i], JAXIS_TURRETX);
                    joyY = SDL_JoystickGetAxis(gController[i], JAXIS_TURRETY);
                    Vector2D joyVec(-joyX, -joyY);
                    joyVec = joyVec.Normalized();


                    float ptAngle = players[i].GetTurretAngle() - 90;
                    while (ptAngle > 360) {
                        ptAngle -= 360;
                    }
                    while (ptAngle < 0) {
                        ptAngle += 360;
                    }
                    if (ptAngle > 180) {
                        ptAngle -= 360;
                    }

                    if (ptAngle < -180) {
                        ptAngle += 360;
                    }
                    //float joyAngle = std::atan2(joyY, joyX);

                    ptAngle = ptAngle * M_PI / 180;

                    if (ptAngle > M_PI) {
                        ptAngle = ptAngle - 2 * M_PI;
                    }
                    if (ptAngle < -M_PI) {
                        ptAngle = ptAngle + 2 * M_PI;
                    }

                    float ptX, ptY;
                    ptX = std::cos(ptAngle);
                    ptY = std::sin(ptAngle);

                    float scale;

                    double diff = (joyVec.GetX() * ptY) - (joyVec.GetY() * ptX);
                    scale = (diff > 0 ) - (diff < 0);

                    //std::cout << "Joystick angle: " << joyAngle / M_PI << ", Turret Angle: " << ptAngle / M_PI << ", Diff: " << diff << std::endl;


                    //std::cout << "Rotating Turret " << scale << std::endl;
                    //players[e.jaxis.which].SetTurretAngle(joyAngle + 90);
                    players[i].SetTurretRotationVel(MAX_ROTATE * scale * joyVec.GetMagnitude());
                }
            }

            for (int i = 0; i < 4; i++) {
                if (players[i].IsDead() ) {
                    continue;
                }
                if (players[i].JoyMove()) {
                    players[i].IsMoving(true);
                    int32_t joyX, joyY;
                    joyX = SDL_JoystickGetAxis(gController[i], JAXIS_MOVEX);
                    joyY = SDL_JoystickGetAxis(gController[i], JAXIS_MOVEY);
                    Vector2D joyVec(-joyX, -joyY);
                    joyVec = joyVec.Normalized();


                    float pAngle = players[i].GetAngle() - 90;
                    while (pAngle > 360) {
                        pAngle -= 360;
                    }
                    while (pAngle < 0) {
                        pAngle += 360;
                    }
                    if (pAngle > 180) {
                        pAngle -= 360;
                    }

                    if (pAngle < -180) {
                        pAngle += 360;
                    }
                    //float joyAngle = std::atan2(joyY, joyX);

                    pAngle = pAngle * M_PI / 180;

                    if (pAngle > M_PI) {
                        pAngle = pAngle - 2 * M_PI;
                    }
                    if (pAngle < -M_PI) {
                        pAngle = pAngle + 2 * M_PI;
                    }

                    float pX, pY;
                    pX = std::cos(pAngle);
                    pY = std::sin(pAngle);

                    float scale;

                    double diff = (joyVec.GetX() * pY) - (joyVec.GetY() * pX);
                    scale = (diff > 0 ) - (diff < 0);

                    //std::cout << "Joystick angle: " << joyAngle / M_PI << ", Player Angle: " << pAngle / M_PI << ", Diff: " << diff << std::endl;


                    //std::cout << "Rotating Turret " << scale << std::endl;
                    //players[e.jaxis.which].SetTurretAngle(joyAngle + 90);
                    players[i].SetRotationVel(MAX_ROTATE * scale);
                    players[i].SetForwardVel(MAX_MOVE * joyVec.GetMagnitude());
                } else if (players[i].Moving()) {
                    players[i].IsMoving(false);
                }
            }

            for (Player* p : vPlayers) {
                if (p->IsDead()) {
                    continue;
                }
                for (Collider* c : vStationary) {
                    if (!c->GetOwner().IsDead()) {
                        p->CheckCollision(*c, frame_time);
                    }
                }
                for (Player* p_other : vPlayers) {
                    p->CheckCollision(p_other->GetCollider(), frame_time);

                }
                for (Container* c : vContainers) {
                    if (!c->IsDead()) {
                        p->CheckCollision(c->GetCollider(), frame_time);
                    } else {
                        bool exists = false;
                            for (Container* c1 : vContainersDelete) {
                                if (c1 == c)
                                    exists = true;
                            }
                            if (!exists)
                                vContainersDelete.push_back(c);
                    }
                }

                for (Powerup* pow : vPowerups) {
                    if (!pow->IsDead()) {
                        CollisionInfo coll = p->GetCollider().CheckCollision(pow->GetCollider(), p->GetVelocity());

                        if (coll.Colliding()) {
                            pow->Apply(*p);
                        }
                    } else {
                        bool exists = false;
                            for (Powerup* pow1 : vPowerupsDelete) {
                                if (pow1 == pow)
                                    exists = true;
                            }
                            if (!exists)
                                vPowerupsDelete.push_back(pow);
                    }

                }
            }

            for (GameObject* g : vPlayers) {
                g->Update(frame_time);
                if (g->GetX() > ARENA_WIDTH || g->GetX() < 0 || g->GetY() < 0 || g->GetY() > ARENA_HEIGHT) {
                    g->SetX(m.GetStartPos( ((Player*) g)->GetID()).GetX());
                    g->SetY(m.GetStartPos( ((Player*) g)->GetID()).GetY());
                }
            }

            for (std::pair<int, Bullet*> b : vBullets) {
                if (!b.second->IsDead()) {
                    b.second->Update(frame_time);

                    for (Container* c : vContainers) {
                        if (!c->IsDead()) {
                            CollisionInfo coll = b.second->CheckCollision(*c, frame_time);
                            if (coll.Colliding()) {
                                c->Die();
                                b.second->Die();
                                b.second->GetOwner().DestroyBullet();
                                Powerup* pow = new Powerup(c->GetX(), c->GetY(), ren, c->GetContents());
                                vPowerups.push_back(pow);
                                vRenderable.insert(std::pair<int, RenderableObject*>(RenderableObject::next, pow));
                                RenderableObject::next++;
                                NewExplosion(c->GetX(), c->GetY(), ren, vRenderable, vExplosions);
                                containers--;
                            }
                        } else {
                            bool exists = false;
                            for (Container* c1 : vContainersDelete) {
                                if (c1 == c)
                                    exists = true;
                            }
                            if (!exists)
                                vContainersDelete.push_back(c);
                        }
                    }

                    for (Player* pl : vPlayers) {
                            if (pl->IsDead()) {
                                continue;
                            }
                            if (pl->IsInvincible()) {
                                continue;
                            }
                            if (pl->GetID() == b.second->GetOwner().GetID() && b.second->GetBounce() == 0) {
                                continue;
                            }
                            CollisionInfo coll = b.second->CheckCollision(*pl, frame_time);
                            if (coll.Colliding()) {
                                if (&(b.second->GetOwner()) == pl) {
                                    pl->AddScore(-1);
                                } else {
                                    b.second->GetOwner().AddScore(1);
                                }

                                NewExplosion(pl->GetX(), pl->GetY(), ren, vRenderable, vExplosions);

                                Utility::PlaySound(sfxDie);
                                if (gameOptions->GetMatchType() == STOCK_MATCH && pl->GetLives() == 1) {
                                    pl->Die();
                                } else {
                                    pl->LoseLife();
                                    pl->SetX(m.GetStartPos(pl->GetID()).GetX());
                                    pl->SetY(m.GetStartPos(pl->GetID()).GetY());
                                    pl->Invincible();
                                }
                                b.second->GetOwner().DestroyBullet();
                                b.second->Die();


                            }
                    }

                    if (b.second->IsDead()) {
                        break;
                    }

                    for (std::pair<int, Bullet*> other : vBullets) {
                        if (b.second != other.second && !other.second->IsDead() && b.second->GetDirection() != other.second->GetDirection()) {
                            CollisionInfo coll = b.second->CheckCollision(*other.second, frame_time);
                            if ((coll.Colliding() || coll.WillCollide())) {
                                b.second->GetOwner().DestroyBullet();
                                other.second->GetOwner().DestroyBullet();
                                NewExplosion(b.second->GetX(), b.second->GetY(), ren, vRenderable, vExplosions);
                                b.second->Die();
                                other.second->Die();
                                break;
                            }
                        }

                    }

                    if (b.second->IsDead()) {
                        break;
                    }

                    for (Collider* c : vStationary) {
                        CollisionInfo coll = b.second->CheckCollision(*c, frame_time);
                        if (b.second->IsDead() && (coll.Colliding() || coll.WillCollide()) ) {
                            b.second->GetOwner().DestroyBullet();
                            NewExplosion(b.second->GetX(), b.second->GetY(), ren, vRenderable, vExplosions);
                            b.second->Die();
                            break;
                        }
                    }
                    if (b.second->IsDead()) {
                        break;
                    }
                }
                if (b.second->IsDead() ) {
                    vBulletsDelete.insert(b);
                }
            }

            for (Explosion* e : vExplosions) {
                if (!e->IsDead()) {
                    e->Update(frame_time);
                } else {
                    vExplosionsDelete.push_back(e);
                }
            }


            while (!vExplosionsDelete.empty()) {
                std::vector<Explosion*>::iterator it = vExplosionsDelete.begin();
                Explosion* e = *it;
                for (std::vector<Explosion*>::iterator eIt = vExplosions.begin(); eIt != vExplosions.end(); eIt++) {
                    if (e == *eIt) {
                        vExplosions.erase(eIt);
                        break;
                    }
                }
                for (std::map<int, RenderableObject*>::iterator rIt = vRenderable.begin(); rIt != vRenderable.end(); rIt++) {
                    if (rIt->second == e) {
                        vRenderable.erase(rIt->first);
                        break;
                    }
                }
                vExplosionsDelete.erase(it);
                delete e;

            }

            while (!vContainersDelete.empty()) {
                std::vector<Container*>::iterator it = vContainersDelete.begin();
                Container* c = *it;
                for (std::vector<Container*>::iterator cIt = vContainers.begin(); cIt != vContainers.end(); cIt++) {
                    if (c == *cIt) {
                        vContainers.erase(cIt);
                        break;
                    }
                }
                for (std::map<int, RenderableObject*>::iterator rIt = vRenderable.begin(); rIt != vRenderable.end(); rIt++) {
                    if (rIt->second == c) {
                        vRenderable.erase(rIt->first);
                        delete c;
                        break;
                    }
                }
                vContainersDelete.erase(it);


            }

            while (!vPowerupsDelete.empty()) {
                std::vector<Powerup*>::iterator it = vPowerupsDelete.begin();
                Powerup* c = *it;
                for (std::vector<Powerup*>::iterator cIt = vPowerups.begin(); cIt != vPowerups.end(); cIt++) {
                    if (c == *cIt) {
                        vPowerups.erase(cIt);
                        break;
                    }
                }
                for (std::map<int, RenderableObject*>::iterator rIt = vRenderable.begin(); rIt != vRenderable.end(); rIt++) {
                    if (rIt->second == c) {
                        vRenderable.erase(rIt->first);
                        delete c;
                        break;
                    }
                }
                vPowerupsDelete.erase(it);


            }


            while (!vBulletsDelete.empty()) {
                std::map<int, Bullet*>::iterator it = vBulletsDelete.begin();
                Bullet* b = it->second;
                vBullets.erase(it->first);
                for (std::map<int, RenderableObject*>::iterator rIt = vRenderable.begin(); rIt != vRenderable.end(); rIt++) {
                    if (rIt->second == b) {
                        vRenderable.erase(rIt->first);
                        break;
                    }
                }
                vBulletsDelete.erase(it);
                delete b;

            }


            /* Render Loop */
            if (SDL_TICKS_PASSED(new_time - ticks, RENDER_INTERVAL)) {

                /* Background texture */

                SDL_Surface* surf = SDL_CreateRGBSurface(0, 320, SCREEN_HEIGHT, 32, rmask, gmask, bmask, amask);


                SDL_FillRect(surf, NULL, backgroundColor[rndTiles]);
                SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surf);
                SDL_FreeSurface(surf);



                /* Clear render target */
                SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0x00, 0xFF);
                SDL_RenderClear(ren);

                /* Copy background texture */
                SDL_Rect bgDestRect;
                bgDestRect.x = 0;
                bgDestRect.y = 0;
                bgDestRect.w = 320;
                bgDestRect.h = 240;
                SDL_RenderCopy(ren, tex, NULL, &bgDestRect);


                /* Delete background texture since it's recreated every frame */
                SDL_DestroyTexture(tex);

                /* Render all renderable objects */
                for (std::pair<int, RenderableObject*> r : vRenderable) {
                    if (!r.second->IsDead()) {
                        r.second->Render();
                    }
                }

                #ifdef _FPS_DEBUG
                /* FPS Texture */
                SDL_Color c = {255, 255, 255, 255};
                std::string basePath = SDL_GetBasePath();
                uint32_t fps = 1000 / (new_time + (SDL_GetTicks() - new_time) - ticks);
                //if (frame_time > 0)
                //    fps = 1000 / frame_time;
                std::stringstream strFPS;
                strFPS << "FPS: " << fps;

                SDL_Texture* fps_tex = Utility::RenderText(strFPS.str(), basePath + GAME_FONT, c, 10, ren);

                int fps_w, fps_h;

                SDL_QueryTexture(fps_tex, NULL, NULL, &fps_w, &fps_h);
                SDL_Rect srcRect, dstRect;
                srcRect.x = 0;
                srcRect.y = 0;
                srcRect.h = fps_h;
                srcRect.w = fps_w;

                dstRect.x = 8;
                dstRect.y = 8;
                dstRect.h = fps_h;
                dstRect.w = fps_w;

                SDL_RenderCopy(ren, fps_tex, &srcRect, &dstRect);

                /* End of FPS texture */
                #endif // _FPS_DEBUG

                #ifdef _ANGLE_DEBUG
                /* Angle text */

                std::stringstream strAngle;
                strAngle << "Ticks: " << frame_time;
                //SDL_Color c = {255, 255, 255, 255};
                SDL_Texture* angle_tex = Utility::RenderText(strAngle.str(), basePath + GAME_FONT, c, 10, ren);

                int angle_w, angle_h;
                SDL_QueryTexture(angle_tex, NULL, NULL, &angle_w, &angle_h);

                //SDL_Rect srcRect, dstRect;
                srcRect.x = 0;
                srcRect.y = 0;
                srcRect.h = angle_h;
                srcRect.w = angle_w;

                dstRect.x = 8;
                dstRect.y = 16;
                dstRect.h = angle_h;
                dstRect.w = angle_w;

                SDL_RenderCopy(ren, angle_tex, &srcRect, &dstRect);

                /* End of angle text */
                #endif // _ANGLE_DEBUG

                /* HUD */
                int verticalOffset = 0;
                {
                    SDL_Color c = { 0xFF, 0xFF, 0xFF, 0xFF };
                    {
                        SDL_SetRenderDrawColor(ren, 0xFF, 0xFF, 0xFF, 0xFF);

                        std::stringstream gtypeString, gvalueString;
                        gtypeString << "Match: ";
                        switch (gameOptions->GetMatchType()) {
                            case SCORE_MATCH:
                                gtypeString << "Score";
                                gvalueString << "Max score: " << gameOptions->GetScore();
                                break;
                            case STOCK_MATCH:
                                gtypeString << "Stock";
                                gvalueString << "Max lives: " << gameOptions->GetStock();
                                break;
                            case TIME_MATCH:
                                gtypeString << "Time";
                                gvalueString << "Time: " << gameTime / 1000;
                                break;
                        }

                        SDL_Texture* gtypeTexture = Utility::RenderText(gtypeString.str(), GAME_FONT, c, 12, ren);
                        SDL_Texture* gvalueTexture = Utility::RenderText(gvalueString.str(), GAME_FONT, c, 12, ren);

                        int gtypeW, gtypeH, gvalueW, gvalueH;

                        SDL_QueryTexture(gtypeTexture, NULL, NULL, &gtypeW, &gtypeH);
                        SDL_QueryTexture(gvalueTexture, NULL, NULL, &gvalueW, &gvalueH);

                        SDL_Rect frameRect, typeRect, valueRect;

                        frameRect.x = 320;
                        frameRect.y = 0;
                        frameRect.h = verticalOffset = gtypeH + gvalueH + 2 + 2 + 2;
                        frameRect.w = 106;

                        typeRect.x = 320 + 2;
                        typeRect.y = 0 + 2;
                        typeRect.w = gtypeW;
                        typeRect.h = gtypeH;

                        valueRect.x = 320 + 2;
                        valueRect.y = 2 + gtypeH + 2;
                        valueRect.w = gvalueW;
                        valueRect.h = gvalueH;

                        SDL_RenderDrawRect(ren, &frameRect);
                        SDL_RenderCopy(ren, gtypeTexture, NULL, &typeRect);
                        SDL_RenderCopy(ren, gvalueTexture, NULL, &valueRect);
                        //SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0x00, 0xFF);
                        //SDL_Surface* scoreboardSurface = SDL_CreateRGBSurface(0, 106, 240, 32, rmask, gmask, bmask, amask);
                        //SDL_FillRect(scoreboardSurface, NULL, SDL_MapRGB(scoreboardSurface->format, 0x00, 0x00, 0x00));
                        //SDL_Texture* scoreboardTexture = SDL_CreateTextureFromSurface(ren, scoreboardSurface);
                        //SDL_FreeSurface(scoreboardSurface);

                        //int scoreboardW, scoreboardH;
                        //SDL_QueryTexture(scoreboardTexture, NULL, NULL, &scoreboardW, &scoreboardH);

                        //SDL_Rect dstRect;
                        //dstRect.x = 320;
                        //dstRect.y = 0;
                        //dstRect.w = scoreboardW;
                        //dstRect.h = scoreboardH;

                        //SDL_RenderCopy(ren, scoreboardTexture, NULL, &dstRect);
                        //SDL_DestroyTexture(scoreboardTexture);
                        SDL_DestroyTexture(gtypeTexture);
                        SDL_DestroyTexture(gvalueTexture);
                    }
                    SDL_SetRenderDrawColor(ren, 0xFF, 0xFF, 0xFF, 0xFF);
                    for (int i = 0; i < std::max(maxPlayers, 1); ++i) {
                        if (!playersIn[i])
                            continue;
                        int rWidth, rHeight;
                        rWidth = 106;
                        rHeight = (240 - verticalOffset) / 4;

                        SDL_Rect srcRect, dstRect;
                        dstRect.x = 320;
                        dstRect.y = verticalOffset + i * rHeight;
                        dstRect.w = rWidth;
                        dstRect.h = rHeight;

                        SDL_RenderDrawRect(ren, &dstRect);



                        std::stringstream scoreString;

                        scoreString << "Player " << i + 1;
                        SDL_Texture* plTex = Utility::RenderText(scoreString.str(), GAME_FONT, c, 12, ren);
                        int plWidth, plHeight;
                        SDL_QueryTexture(plTex, NULL, NULL, &plWidth, &plHeight);
                        scoreString.str("");

                        switch (gameOptions->GetMatchType() ) {
                            case STOCK_MATCH:
                                scoreString << "Lives: " << players[i].GetLives();
                                break;
                            case SCORE_MATCH:
                            case TIME_MATCH:
                                scoreString << "Score: " << players[i].GetScore();
                                break;
                        }
                        int scoreWidth, scoreHeight;
                        SDL_Texture* scoreTex = Utility::RenderText(scoreString.str(), GAME_FONT, c, 12, ren);
                        SDL_QueryTexture(scoreTex, NULL, NULL, &scoreWidth, &scoreHeight);
                        scoreString.str("");

                        scoreString << "B: " << players[i].GetMaxBullets() << ", O: " << players[i].GetMaxBounce() << ", S: " << players[i].GetMaxSpeed();
                        SDL_Texture* PowerupsTex = Utility::RenderText(scoreString.str(), GAME_FONT, c, 12, ren);
                        int powWidth, powHeight;
                        SDL_QueryTexture(PowerupsTex, NULL, NULL, &powWidth, &powHeight);
                        scoreString.str("");

                        srcRect.x = 0;
                        srcRect.y = 0;
                        srcRect.w = plWidth;
                        srcRect.h = plHeight;

                        dstRect.x = 320 + 2;
                        dstRect.y = verticalOffset + (i * rHeight + 2);
                        dstRect.w = plWidth;
                        dstRect.h = plHeight;

                        SDL_RenderCopy(ren, plTex, &srcRect, &dstRect);

                        srcRect.x = 0;
                        srcRect.y = 0;
                        srcRect.w = scoreWidth;
                        srcRect.h = scoreHeight;

                        dstRect.x = 320 + 2;
                        dstRect.y = verticalOffset + (i * rHeight + plHeight + 4);
                        dstRect.w = scoreWidth;
                        dstRect.h = scoreHeight;

                        SDL_RenderCopy(ren, scoreTex, &srcRect, &dstRect);

                        srcRect.x = 0;
                        srcRect.y = 0;
                        srcRect.w = powWidth;
                        srcRect.h = powHeight;

                        dstRect.x = 320 + 2;
                        dstRect.y = verticalOffset + (i * rHeight + plHeight + scoreHeight + 6);
                        dstRect.w = powWidth;
                        dstRect.h = powHeight;

                        SDL_RenderCopy(ren, PowerupsTex, &srcRect, &dstRect);


                    }
                }
                /* End of HUD */


                SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0x00, 0xFF);
                SDL_RenderPresent(ren);
                ticks = SDL_GetTicks();
            }
            /* End of Render Loop */


        }

        if (loopGame) {
            Mix_FadeOutMusic(200);
            if (WinScreen(winningPlayer, players) == -1) {
                loopGame = false;
            }
        } else {
            Mix_HaltMusic();
        }
    }

    for (int i = 0; i < maxPlayers; ++i) {
        SDL_JoystickClose(gController[i]);
        gController[i] = NULL;
    }

    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(ren);

    Mix_FreeChunk(sfxFire);
    Mix_FreeChunk(sfxDie);
    for (int i = 0; i < 3; ++i)
        Mix_FreeChunk(sfxBounce[i]);
    for (int i = 0; i < MAX_TILESET + 1; ++i)
        Mix_FreeChunk(sfxPowerupBounce[i]);
    for (int i = 0; i < MAX_TILESET + 1; ++i)
        Mix_FreeChunk(sfxPowerupBullet[i]);
    for (int i = 0; i < MAX_TILESET + 1; ++i)
        Mix_FreeChunk(sfxPowerupSpeed[i]);
    for (int i = 0; i < MAX_TILESET + 1; ++i)
        Mix_FreeMusic(gameMusic[i]);
    for (int i = 0; i < MAX_TILESET + 1; ++i)
        Mix_FreeMusic(introMusic[i]);

    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();

    return 0;
}


void NewExplosion(const float x, const float y, SDL_Renderer* ren, std::map<int, RenderableObject*>& vRenderable, std::vector<Explosion*>& vExplosions) {
    Explosion* newExpl = new Explosion(x, y, ren);
    std::pair<int, RenderableObject*> newPair(RenderableObject::next, newExpl);
    RenderableObject::next++;
    vRenderable.insert(newPair);
    vExplosions.push_back(newExpl);
}


int Menu() {
    bool menuRunning = true;
    std::vector<std::string> levelFiles;

    dirent* de;
    DIR* dp;

    int numJoysticks = 0;

    std::string basePath = SDL_GetBasePath();
    dp = opendir(basePath.c_str());
    if (dp) {
        while (true) {
            errno = 0;

            de = readdir(dp);
            if (de == NULL)
                break;
            std::string filename = de->d_name;
            if (filename.at(filename.length() - 1) == 'd' && filename.at(filename.length() - 2) == '.') {
                std::string fullPath = basePath + filename;
                std::ifstream inFile(fullPath);
                if (inFile.good() ) {
                    char c;
                    char DE = 0xDE;
                    char AD = 0xAD;
                    char BE = 0xBE;
                    char EF = 0xEF;

                    inFile.get(c);
                    if (c != DE)
                        continue;

                    inFile.get(c);
                    if (c != AD)
                        continue;

                    inFile.get(c);
                    if (c != BE)
                        continue;

                    inFile.get(c);
                    if (c != EF)
                        continue;

                    levelFiles.push_back(filename);
                }

            }

        }
    }

    /*
    for (std::string file : levelFiles) {
        std::cout << file << std::endl;
    }
    */

    bool playersUpHeld[4] = { false, false, false, false };
    bool playersDownHeld[4] = {false, false, false, false };
    const int cursorRepeat = 500;
    int ticksSinceMove[4] = {0, 0, 0, 0};
    bool mapSelect = false;

    int mapCount = levelFiles.size();
    int mapSelected = 0;

    uint32_t time = SDL_GetTicks();

    while (menuRunning) {

        if (SDL_NumJoysticks() > 0 && SDL_NumJoysticks() != numJoysticks) {
            numJoysticks = SDL_NumJoysticks();
            for (int i = 0; i < 4; ++i) {
                playersIn[i] = false;
            }
            maxPlayers = std::min(SDL_NumJoysticks(), 4);
            for (int i = 0; i < std::min(SDL_NumJoysticks(), 4); ++i) {
                gController[i] = SDL_JoystickOpen(i);
                if (gController[i] == NULL) {
                    std::cout << "Could not open joystick " << i << ". SDL Error: " << SDL_GetError() << std::endl;
                    Quit(2);
                }
            }
        }

        uint32_t frameTime = SDL_GetTicks() - time;
        time = SDL_GetTicks();
        SDL_Event e;
        int index = -1;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                menuRunning = false;
                return -1;
            }
            if (e.type == SDL_JOYBUTTONDOWN || e.type == SDL_JOYBUTTONUP || e.type == SDL_JOYAXISMOTION) {
                for (int i = 0; i < maxPlayers; ++i) {
                    if (SDL_JoystickInstanceID(gController[i]) == e.jaxis.which) {
                        index = i;
                        break;
                    }
                }
            }
            if (e.type == SDL_JOYBUTTONDOWN) {
                switch (e.jbutton.button) {
                    case JBUTTON_A:
                        if (playersIn[index] == false)
                            playersIn[index] = true;
                        break;
                    case JBUTTON_B:
                        if (playersIn[index] == true)
                            playersIn[index] = false;
                        break;
                    case JBUTTON_BACK:
                        menuRunning = false;
                        return -1;
                        break;
                    case JBUTTON_START:
                        if (playersInCount > 1) {
                            mapSelect = true;
                        }
                        break;
                    case JBUTTON_DPADUP:
                        playersUpHeld[index] = true;
                        playersDownHeld[index] = false;
                        break;
                    case JBUTTON_DPADDOWN:
                        playersDownHeld[index] = true;
                        playersUpHeld[index] = false;
                        break;
                }
            } else if (e.type == SDL_JOYBUTTONUP) { // if e.type == SDL_JOYBUTTONDOWN
                switch (e.jbutton.button) {
                    case JBUTTON_DPADUP:
                        playersUpHeld[index] = false;
                        break;
                    case JBUTTON_DPADDOWN:
                        playersDownHeld[index] = false;
                        break;
                } // switch e.jbutton.button
            } else if (e.type == SDL_JOYAXISMOTION) {
                if (e.jaxis.axis == JAXIS_MOVEY) {
                    if (e.jaxis.value > JOYMOVE_DEADZONE) {
                        playersDownHeld[index] = true;
                        playersUpHeld[index] = false;
                    } else if (e.jaxis.value < -JOYMOVE_DEADZONE) {
                        playersUpHeld[index] = true;
                        playersDownHeld[index] = false;
                    } else {
                        playersDownHeld[index] = false;
                        playersUpHeld[index] = false;
                    }
                }
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE:
                    case SDLK_END:
                        menuRunning = false;
                        return -1;
                        break;
                    case SDLK_SPACE:
                        playersIn[0] = true;
                        break;
                    case SDLK_RETURN:
                        if (playersInCount > 0)
                            mapSelect = true;
                        break;
                    case SDLK_UP:
                        playersUpHeld[0] = true;
                        playersDownHeld[0] = false;
                        break;
                    case SDLK_DOWN:
                        playersDownHeld[0] = true;
                        playersUpHeld[0] = false;
                        break;
                }
            } else if (e.type == SDL_KEYUP) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        playersUpHeld[0] = false;
                        break;
                    case SDLK_DOWN:
                        playersDownHeld[0] = false;
                        break;
                }
            }


        } // End of SDL_PollEvent(e)

        for (int i = 0; i < 4; ++i) {
            if (ticksSinceMove[i] > 0) {
                ticksSinceMove[i] = std::max((uint32_t) 0, ticksSinceMove[i] - frameTime);
            } else  {
                if (playersUpHeld[i]) {
                    if (mapSelected > 0) {
                            mapSelected--;
                            ticksSinceMove[i] = cursorRepeat;
                    }
                } else if (playersDownHeld[i]) {
                    if (mapSelected < mapCount - 1) {
                            mapSelected++;
                            ticksSinceMove[i] = cursorRepeat;
                    }
                }
            }
        }

        int tempPlayersIn = 0;
        for (int i = 0; i < 4; ++i) {
            if (playersIn[i]) {
                tempPlayersIn++;
            }
        }
        playersInCount = tempPlayersIn;

        if (mapSelect) {
            mapfilename = levelFiles[mapSelected];
            return 0;
        }

        SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(ren);

        SDL_SetRenderDrawColor(ren, 0xFF, 0xFF, 0xFF, 0xFF);

        SDL_Rect frameRect;

        SDL_Color white = {0xFF, 0xFF, 0xFF, 0xFF};

        if (maxPlayers >= 0) {
            SDL_Texture* p1Text = Utility::RenderText("Player 1", GAME_FONT, white, 12, ren);
            SDL_Texture* p1Button = NULL;

            if (playersIn[0] == false) {
                p1Button = Utility::RenderText("Press A", GAME_FONT, white, 10, ren);
            } else {
                p1Button = Utility::RenderText("Ready!", GAME_FONT, white, 10, ren);
            }
            int p1TextWidth, p1TextHeight;
            SDL_QueryTexture(p1Text, NULL, NULL, &p1TextWidth, &p1TextHeight);
            int p1ButtonWidth, p1ButtonHeight;
            SDL_QueryTexture(p1Button, NULL, NULL, &p1ButtonWidth, &p1ButtonHeight);

            SDL_Rect p1DestRect;

            p1DestRect.x = 320 / 4 - (p1TextWidth / 2);
            p1DestRect.y = 240 / 4 - (p1TextHeight) - 2;
            p1DestRect.w = p1TextWidth;
            p1DestRect.h = p1TextHeight;
            SDL_RenderCopy(ren, p1Text, NULL, &p1DestRect);

            p1DestRect.x = 320 / 4 - (p1ButtonWidth / 2);
            p1DestRect.y = 240 / 4 + 2;
            p1DestRect.w = p1ButtonWidth;
            p1DestRect.h = p1ButtonHeight;
            SDL_RenderCopy(ren, p1Button, NULL, &p1DestRect);

            frameRect.x = 0;
            frameRect.y = 0;
            frameRect.h = SCREEN_HEIGHT / 2;
            frameRect.w = 320 / 2;


            SDL_RenderDrawRect(ren, &frameRect);

            SDL_DestroyTexture(p1Text);
            SDL_DestroyTexture(p1Button);
        }

        if (maxPlayers > 1) {

            SDL_Texture* p2Text = Utility::RenderText("Player 2", GAME_FONT, white, 12, ren);
            SDL_Texture* p2Button = NULL;

            if (playersIn[1] == false) {
                p2Button = Utility::RenderText("Press A", GAME_FONT, white, 10, ren);
            } else {
                p2Button = Utility::RenderText("Ready!", GAME_FONT, white, 10, ren);
            }
            int p2TextWidth, p2TextHeight;
            SDL_QueryTexture(p2Text, NULL, NULL, &p2TextWidth, &p2TextHeight);
            int p2ButtonWidth, p2ButtonHeight;
            SDL_QueryTexture(p2Button, NULL, NULL, &p2ButtonWidth, &p2ButtonHeight);

            SDL_Rect p2DestRect;

            p2DestRect.x = 3 * 320 / 4 - (p2TextWidth / 2);
            p2DestRect.y = 240 / 4 - (p2TextHeight) - 2;
            p2DestRect.w = p2TextWidth;
            p2DestRect.h = p2TextHeight;
            SDL_RenderCopy(ren, p2Text, NULL, &p2DestRect);

            p2DestRect.x = 3 * 320 / 4 - (p2ButtonWidth / 2);
            p2DestRect.y = 240 / 4 + 2;
            p2DestRect.w = p2ButtonWidth;
            p2DestRect.h = p2ButtonHeight;
            SDL_RenderCopy(ren, p2Button, NULL, &p2DestRect);

            frameRect.x = 320 / 2;
            frameRect.y = 0;
            frameRect.h = SCREEN_HEIGHT / 2;
            frameRect.w = 320 / 2;
            SDL_RenderDrawRect(ren, &frameRect);

            SDL_DestroyTexture(p2Text);
            SDL_DestroyTexture(p2Button);
        }

        if (maxPlayers > 2) {

            SDL_Texture* p3Text = Utility::RenderText("Player 3", GAME_FONT, white, 12, ren);
            SDL_Texture* p3Button = NULL;

            if (playersIn[2] == false) {
                p3Button = Utility::RenderText("Press A", GAME_FONT, white, 10, ren);
            } else {
                p3Button = Utility::RenderText("Ready!", GAME_FONT, white, 10, ren);
            }
            int p3TextWidth, p3TextHeight;
            SDL_QueryTexture(p3Text, NULL, NULL, &p3TextWidth, &p3TextHeight);
            int p3ButtonWidth, p3ButtonHeight;
            SDL_QueryTexture(p3Button, NULL, NULL, &p3ButtonWidth, &p3ButtonHeight);

            SDL_Rect p3DestRect;

            p3DestRect.x = 320 / 4 - (p3TextWidth / 2);
            p3DestRect.y = 3 * 240 / 4 - (p3TextHeight) - 2;
            p3DestRect.w = p3TextWidth;
            p3DestRect.h = p3TextHeight;
            SDL_RenderCopy(ren, p3Text, NULL, &p3DestRect);

            p3DestRect.x = 320 / 4 - (p3ButtonWidth / 2);
            p3DestRect.y = 3 * 240 / 4 + 2;
            p3DestRect.w = p3ButtonWidth;
            p3DestRect.h = p3ButtonHeight;
            SDL_RenderCopy(ren, p3Button, NULL, &p3DestRect);

            frameRect.x = 0;
            frameRect.y = 240 / 2;
            frameRect.h = SCREEN_HEIGHT / 2;
            frameRect.w = 320 / 2;
            SDL_RenderDrawRect(ren, &frameRect);

            SDL_DestroyTexture(p3Text);
            SDL_DestroyTexture(p3Button);
        }

        if (maxPlayers > 3) {

            SDL_Texture* p4Text = Utility::RenderText("Player 4", GAME_FONT, white, 12, ren);
            SDL_Texture* p4Button = NULL;

            if (playersIn[3] == false) {
                p4Button = Utility::RenderText("Press A", GAME_FONT, white, 10, ren);
            } else {
                p4Button = Utility::RenderText("Ready!", GAME_FONT, white, 10, ren);
            }
            int p4TextWidth, p4TextHeight;
            SDL_QueryTexture(p4Text, NULL, NULL, &p4TextWidth, &p4TextHeight);
            int p4ButtonWidth, p4ButtonHeight;
            SDL_QueryTexture(p4Button, NULL, NULL, &p4ButtonWidth, &p4ButtonHeight);

            SDL_Rect p4DestRect;

            p4DestRect.x = 3 * 320 / 4 - (p4TextWidth / 2);
            p4DestRect.y = 3 * 240 / 4 - (p4TextHeight) - 2;
            p4DestRect.w = p4TextWidth;
            p4DestRect.h = p4TextHeight;
            SDL_RenderCopy(ren, p4Text, NULL, &p4DestRect);

            p4DestRect.x = 3 * 320 / 4 - (p4ButtonWidth / 2);
            p4DestRect.y = 3 * 240 / 4 + 2;
            p4DestRect.w = p4ButtonWidth;
            p4DestRect.h = p4ButtonHeight;
            SDL_RenderCopy(ren, p4Button, NULL, &p4DestRect);

            frameRect.x = 320 / 2;
            frameRect.y = 240 / 2;
            frameRect.h = SCREEN_HEIGHT / 2;
            frameRect.w = 320 / 2;
            SDL_RenderDrawRect(ren, &frameRect);

            SDL_DestroyTexture(p4Text);
            SDL_DestroyTexture(p4Button);
        }


        // Level Select Frame
        frameRect.x = 320;
        frameRect.y = 0;
        frameRect.h = 240;
        frameRect.w = 106;

        {
            SDL_Color grey = { 0x60, 0x60, 0x60, 0xFF };
            int i = 0;
            for (std::string filename : levelFiles) {


                SDL_Texture* fileTex;

                if (mapSelected == i) {
                    fileTex = Utility::RenderText(filename, GAME_FONT, white, 10, ren);
                } else {
                    fileTex = Utility::RenderText(filename, GAME_FONT, grey, 10, ren);
                }

                int fileHeight, fileWidth;
                SDL_QueryTexture(fileTex, NULL, NULL, &fileWidth, &fileHeight);
                SDL_Rect fileRect;
                fileRect.x = 320 + 2;
                fileRect.y = i * fileHeight + i * 2;
                fileRect.w = fileWidth;
                fileRect.h = fileHeight;

                if (i * fileHeight + i * 2 < 240 - fileHeight - 2)
                    SDL_RenderCopy(ren, fileTex, NULL, &fileRect);
                i++;
                SDL_DestroyTexture(fileTex);
            }
        }
        SDL_RenderDrawRect(ren, &frameRect);



        SDL_RenderPresent(ren);
    }
    return 0;
}


Options* OptionsMenu() {
    bool optionsMenuRunning = true;

    int gameType = SCORE_MATCH;

    int score = std::min(10, MAX_SCORE);
    int time = std::min(120, MAX_TIME);
    int stock = std::min(10, MAX_STOCK);
    //int score = MIN_SCORE + ((MAX_SCORE - MIN_SCORE) / 2);
    //int time = MIN_TIME + ((MAX_TIME - MIN_TIME) / 2);
    //int stock = MIN_STOCK + ((MAX_STOCK - MIN_STOCK) / 2);

    const int cursorRepeat = 500;
    int ticksSinceMove[4] = {0, 0, 0, 0};
    bool playersUpHeld[4] = { false, false, false, false };
    bool playersDownHeld[4] = {false, false, false, false };
    bool playersLeftHeld[4] = { false, false, false, false };
    bool playersRightHeld[4] = {false, false, false, false };

    uint32_t nowTime, renderTime;

    renderTime = nowTime = SDL_GetTicks();

    while (optionsMenuRunning) {

        uint32_t frameTime = SDL_GetTicks() - nowTime;
        nowTime = SDL_GetTicks();


        int index = -1;
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_JOYBUTTONDOWN || e.type == SDL_JOYBUTTONUP || e.type == SDL_JOYAXISMOTION) {
                for (int i = 0; i < maxPlayers; ++i) {
                    if (SDL_JoystickInstanceID(gController[i]) == e.jaxis.which) {
                        index = i;
                        break;
                    }
                }
            }

            if (e.type == SDL_QUIT) {
              return nullptr;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_END:
                    case SDLK_ESCAPE:
                        return nullptr;
                        break;
                    case SDLK_RETURN:
                    case SDLK_SPACE:
                        optionsMenuRunning = false;
                        break;
                    case SDLK_RIGHT:
                        playersRightHeld[0] = true;
                        playersLeftHeld[0] = false;
                        break;
                    case SDLK_LEFT:
                        playersLeftHeld[0] = true;
                        playersRightHeld[0] = false;
                        break;
                    case SDLK_UP:
                        playersUpHeld[0] = true;
                        playersDownHeld[0] = false;
                        break;
                    case SDLK_DOWN:
                        playersUpHeld[0] = false;
                        playersDownHeld[0] = true;
                        break;
                } // switch e.key.keysym.sym
            } else if (e.type == SDL_KEYUP) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        playersUpHeld[0] = false;
                        break;
                    case SDLK_DOWN:
                        playersDownHeld[0] = false;
                        break;
                    case SDLK_LEFT:
                        playersLeftHeld[0] = false;
                        break;
                    case SDLK_RIGHT:
                        playersRightHeld[0] = false;
                        break;
                } // switch e.key.keysym.sym
            } else if (e.type == SDL_JOYAXISMOTION) {
                if (e.jaxis.axis == JAXIS_MOVEY) {
                    if (e.jaxis.value > JOYMOVE_DEADZONE) {
                        playersDownHeld[index] = true;
                        playersUpHeld[index] = false;
                    } else if (e.jaxis.value < -JOYMOVE_DEADZONE) {
                        playersUpHeld[index] = true;
                        playersDownHeld[index] = false;
                    } else {
                        playersDownHeld[index] = false;
                        playersUpHeld[index] = false;
                    }
                } else if (e.jaxis.axis == JAXIS_MOVEX) {
                    if (e.jaxis.value > JOYMOVE_DEADZONE) {
                        playersRightHeld[index] = true;
                        playersLeftHeld[index] = false;
                    } else if (e.jaxis.value < -JOYMOVE_DEADZONE) {
                        playersLeftHeld[index] = true;
                        playersRightHeld[index] = false;
                    } else {
                        playersLeftHeld[index] = false;
                        playersRightHeld[index] = false;
                    }
                }
            } else if (e.type == SDL_JOYBUTTONDOWN) {
                switch (e.jbutton.button) {
                    case JBUTTON_BACK:
                        return nullptr;
                        break;
                    case JBUTTON_START:
                    case JBUTTON_A:
                        optionsMenuRunning = false;
                        break;
                    case JBUTTON_DPADUP:
                        playersUpHeld[index] = true;
                        playersDownHeld[index] = false;
                        break;
                    case JBUTTON_DPADDOWN:
                        playersUpHeld[index] = false;
                        playersDownHeld[index] = true;
                        break;
                    case JBUTTON_DPADLEFT:
                        playersLeftHeld[index] = true;
                        playersRightHeld[index] = false;
                        break;
                    case JBUTTON_DPADRIGHT:
                        playersLeftHeld[index] = false;
                        playersRightHeld[index] = true;

                        break;
                } // switch e.jbutton.button
            } else if (e.type == SDL_JOYBUTTONUP) { // if e.type == SDL_JOYBUTTONDOWN
                switch (e.jbutton.button) {
                    case JBUTTON_DPADUP:
                        playersUpHeld[index] = false;
                        break;
                    case JBUTTON_DPADDOWN:
                        playersDownHeld[index] = false;
                        break;
                    case JBUTTON_DPADLEFT:
                        playersLeftHeld[index] = false;
                        break;
                    case JBUTTON_DPADRIGHT:
                        playersRightHeld[index] = false;
                        break;
                } // switch e.jbutton.button
            } // if e.type == SDL_JOYBUTTONUP
        } // While SDL_PollEvent()


        for (int i = 0; i < 4; ++i) {
            if (ticksSinceMove[i] > 0) {
                ticksSinceMove[i] = std::max((uint32_t) 0, ticksSinceMove[i] - frameTime);
            } else  {
                if (playersUpHeld[i]) {
                    if (gameType > SCORE_MATCH)
                            gameType--;
                    ticksSinceMove[i] = cursorRepeat;
                } else if (playersDownHeld[i]) {
                    if (gameType < TIME_MATCH)
                            gameType++;
                    ticksSinceMove[i] = cursorRepeat;
                } else if (playersLeftHeld[i]) {
                    switch (gameType) {
                        case SCORE_MATCH:
                            if (score > MIN_SCORE)
                                score--;
                            break;
                        case STOCK_MATCH:
                            if (stock > MIN_STOCK)
                                stock--;
                            break;
                        case TIME_MATCH:
                            if (time > MIN_TIME)
                                time--;
                            break;
                    } // switch gameType
                    ticksSinceMove[i] = cursorRepeat;
                } else if (playersRightHeld[i]) {
                    switch (gameType) {
                            case SCORE_MATCH:
                                if (score < MAX_SCORE)
                                    score++;
                                break;
                            case STOCK_MATCH:
                                if (stock < MAX_STOCK)
                                    stock++;
                                break;
                            case TIME_MATCH:
                                if (time < MAX_TIME)
                                    time++;
                                break;
                        } // switch gameType
                    ticksSinceMove[i] = cursorRepeat;
                }
            }
        }

        if (SDL_TICKS_PASSED(nowTime - renderTime, RENDER_INTERVAL) ) {
            SDL_Texture* gameTypeTexture = nullptr;
            int gameTypeWidth, gameTypeHeight;

            SDL_Texture* gameValueTexture = nullptr;
            int gameValueWidth, gameValueHeight;

            SDL_Texture* arrowTexture = Utility::LoadTexture(ren, "Arrows.png");
            int arrowWidth, arrowHeight;
            SDL_QueryTexture(arrowTexture, NULL, NULL, &arrowWidth, &arrowHeight);

            std::string gameTypeString = "Game Type: ";
            std::stringstream gameValueString;

            switch (gameType) {
                case SCORE_MATCH:
                    gameTypeString += "Score";
                    gameValueString << "Score: " << score;
                    break;
                case STOCK_MATCH:
                    gameTypeString += "Stock";
                    gameValueString << "Stock: " << stock;
                    break;
                case TIME_MATCH:
                    gameTypeString += "Time";
                    gameValueString << "Time: " << time;
                    break;
            } // switch gameType

            SDL_Color white = { 0xFF, 0xFF, 0xFF, 0xFF };

            gameTypeTexture = Utility::RenderText(gameTypeString, GAME_FONT, white, 12, ren);
            gameValueTexture = Utility::RenderText(gameValueString.str(), GAME_FONT, white, 12, ren);

            SDL_QueryTexture(gameTypeTexture, NULL, NULL, &gameTypeWidth, &gameTypeHeight);
            SDL_QueryTexture(gameValueTexture, NULL, NULL, &gameValueWidth, &gameValueHeight);

            SDL_Rect gameTypeRect, gameValueRect;

            SDL_Rect lArrowSrcRect, rArrowSrcRect;
            SDL_Rect uArrowSrcRect, dArrowSrcRect;

            lArrowSrcRect.x = 0;
            lArrowSrcRect.y = 0;
            lArrowSrcRect.h = arrowHeight;
            lArrowSrcRect.w = arrowWidth / 4;

            rArrowSrcRect.x = 8;
            rArrowSrcRect.y = 0;
            rArrowSrcRect.h = arrowHeight;
            rArrowSrcRect.w = arrowWidth / 4;

            uArrowSrcRect.x = 16;
            uArrowSrcRect.y = 0;
            uArrowSrcRect.h = arrowHeight;
            uArrowSrcRect.w = arrowWidth / 4;

            dArrowSrcRect.x = 24;
            dArrowSrcRect.y = 0;
            dArrowSrcRect.h = arrowHeight;
            dArrowSrcRect.w = arrowWidth / 4;


            SDL_Rect lArrowDestRect, rArrowDestRect;
            SDL_Rect uArrowDestRect, dArrowDestRect;



            gameTypeRect.x = (320 / 2) - (gameTypeWidth / 2);
            gameTypeRect.y = (240 / 2) - (gameTypeHeight / 2) - 2;
            gameTypeRect.w = gameTypeWidth;
            gameTypeRect.h = gameTypeHeight;

            gameValueRect.x = (320 / 2) - (gameValueWidth / 2);
            gameValueRect.y = (240 / 2) + (gameValueHeight / 2) + 2;
            gameValueRect.w = gameValueWidth;
            gameValueRect.h = gameValueHeight;

            uArrowDestRect.x = (320 / 2) - (arrowWidth / 4 / 2);
            uArrowDestRect.y = (240 / 2) - (gameTypeHeight / 2) - 2 - (arrowHeight);
            uArrowDestRect.h = arrowHeight;
            uArrowDestRect.w = arrowWidth / 4;

            dArrowDestRect.x = (320 / 2) - (arrowWidth / 4 / 2);
            dArrowDestRect.y = (240 / 2) + (gameTypeHeight / 2 ) + (gameValueHeight) + 1;
            dArrowDestRect.h = arrowHeight;
            dArrowDestRect.w = arrowWidth / 4;

            lArrowDestRect.x = gameValueRect.x - 2 - (arrowWidth / 4);
            lArrowDestRect.y = gameValueRect.y + 2;
            lArrowDestRect.h  = arrowHeight;
            lArrowDestRect.w = arrowWidth / 4;

            rArrowDestRect.x = gameValueRect.x + gameValueWidth + 2;
            rArrowDestRect.y = gameValueRect.y + 2;
            rArrowDestRect.h  = arrowHeight;
            rArrowDestRect.w = arrowWidth / 4;

            SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0x00, 0xFF);
            SDL_RenderClear(ren);



            SDL_RenderCopy(ren, gameTypeTexture, NULL, &gameTypeRect);
            SDL_RenderCopy(ren, gameValueTexture, NULL, &gameValueRect);
            SDL_RenderCopy(ren, arrowTexture, &uArrowSrcRect, &uArrowDestRect);
            SDL_RenderCopy(ren, arrowTexture, &dArrowSrcRect, &dArrowDestRect);
            SDL_RenderCopy(ren, arrowTexture, &lArrowSrcRect, &lArrowDestRect);
            SDL_RenderCopy(ren, arrowTexture, &rArrowSrcRect, &rArrowDestRect);

            SDL_DestroyTexture(gameTypeTexture);
            SDL_DestroyTexture(gameValueTexture);
            SDL_DestroyTexture(arrowTexture);

            SDL_RenderPresent(ren);
        }
    } // while optionsMenuRunning

    return new Options(gameType, score, stock, time);
}

int WinScreen(bool (&winningPlayer)[4], Player (&players)[4]) {
    bool WinScreenRunning = true;

    SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(ren);

    std::stringstream WinnerString;
    std::string scoreString = "Score";
    std::stringstream p1scoreString, p2scoreString, p3scoreString, p4scoreString;

    std::vector<int> vWinners;

    for (int i = 0; i < 4; ++i) {
        if (winningPlayer[i])
            vWinners.push_back(i + 1);
    }

    switch (vWinners.size()) {
        case 0:
            WinnerString << "No Contest!";
            break;
        case 1:
            WinnerString << "Player " << vWinners.at(0) << " wins!";
            break;
        case 2:
            WinnerString << "Players " << vWinners.at(0) << " & " << vWinners.at(1) << " tied!";
            break;
        case 3:
            WinnerString << "Players " << vWinners.at(0) << ", " << vWinners.at(1) << " & " << vWinners.at(2) << " tied!";
            break;
        case 4:
            WinnerString << "Tie game!";
            break;
    }

    p1scoreString << "Player 1: " << players[0].GetScore();
    p2scoreString << "Player 2: " << players[1].GetScore();
    p3scoreString << "Player 3: " << players[2].GetScore();
    p4scoreString << "Player 4: " << players[3].GetScore();

    int winnersW, winnersH;
    int scoreW, scoreH;
    int p1W, p1H;
    int p2W, p2H;
    int p3W, p3H;
    int p4W, p4H;

    SDL_Color c = { 0xFF, 0xFF, 0xFF, 0xFF };

    SDL_Texture* winnersTex = Utility::RenderText(WinnerString.str(), GAME_FONT, c, 18, ren);
    SDL_Texture* scoreTex = Utility::RenderText(scoreString, GAME_FONT, c, 16, ren);
    SDL_Texture* p1Tex = Utility::RenderText(p1scoreString.str(), GAME_FONT, c, 14, ren);
    SDL_Texture* p2Tex = Utility::RenderText(p2scoreString.str(), GAME_FONT, c, 14, ren);
    SDL_Texture* p3Tex = Utility::RenderText(p3scoreString.str(), GAME_FONT, c, 14, ren);
    SDL_Texture* p4Tex = Utility::RenderText(p4scoreString.str(), GAME_FONT, c, 14, ren);

    SDL_QueryTexture(winnersTex, NULL, NULL, &winnersW, &winnersH);
    SDL_QueryTexture(scoreTex, NULL, NULL, &scoreW, &scoreH);
    SDL_QueryTexture(p1Tex, NULL, NULL, &p1W, &p1H);
    SDL_QueryTexture(p2Tex, NULL, NULL, &p2W, &p2H);
    SDL_QueryTexture(p3Tex, NULL, NULL, &p3W, &p3H);
    SDL_QueryTexture(p4Tex, NULL, NULL, &p4W, &p4H);

    SDL_Rect winnersRect, scoreRect, p1Rect, p2Rect, p3Rect, p4Rect;

    winnersRect.x = (SCREEN_WIDTH / 2) - (winnersW / 2);
    winnersRect.y = (SCREEN_HEIGHT / 2) - (winnersH / 2);
    winnersRect.h = winnersH;
    winnersRect.w = winnersW;

    scoreRect.x = (SCREEN_WIDTH / 2) - (scoreW / 2);
    scoreRect.y = (SCREEN_HEIGHT / 2) + (winnersH / 2) + 4;
    scoreRect.w = scoreW;
    scoreRect.h = scoreH;

    int verticalOffset = (SCREEN_HEIGHT / 2) + (winnersH / 2) + 4 + scoreH + 2;

    if (playersIn[0]) {
        p1Rect.x = (SCREEN_WIDTH / 2) - (p1W / 2);
        p1Rect.y = verticalOffset;
        p1Rect.h = p1H;
        p1Rect.w = p1W;
        verticalOffset += p1H + 2;
        SDL_RenderCopy(ren, p1Tex, NULL, &p1Rect);
    }

    if (playersIn[1]) {
        p2Rect.x = (SCREEN_WIDTH / 2) - (p2W / 2);
        p2Rect.y = verticalOffset;
        p2Rect.h = p2H;
        p2Rect.w = p2W;
        verticalOffset += p2H + 2;
        SDL_RenderCopy(ren, p2Tex, NULL, &p2Rect);
    }

    if (playersIn[2]) {
        p3Rect.x = (SCREEN_WIDTH / 2) - (p3W / 2);
        p3Rect.y = verticalOffset;
        p3Rect.h = p3H;
        p3Rect.w = p3W;
        verticalOffset += p3H + 2;
        SDL_RenderCopy(ren, p3Tex, NULL, &p3Rect);
    }

    if (playersIn[3]) {
        p4Rect.x = (SCREEN_WIDTH / 2) - (p4W / 2);
        p4Rect.y = verticalOffset;
        p4Rect.h = p4H;
        p4Rect.w = p4W;
        verticalOffset += p4H + 2;
        SDL_RenderCopy(ren, p4Tex, NULL, &p4Rect);
    }

    SDL_RenderCopy(ren, winnersTex, NULL, &winnersRect);
    SDL_RenderCopy(ren, scoreTex, NULL, &scoreRect);

    SDL_RenderPresent(ren);

    SDL_DestroyTexture(winnersTex);
    SDL_DestroyTexture(scoreTex);
    SDL_DestroyTexture(p1Tex);
    SDL_DestroyTexture(p2Tex);
    SDL_DestroyTexture(p3Tex);
    SDL_DestroyTexture(p4Tex);

    while (WinScreenRunning) {
        SDL_Event e;

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                return -1;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_RETURN:
                    case SDLK_SPACE:
                    case SDLK_ESCAPE:
                    case SDLK_END:
                    case SDLK_PAUSE:
                        WinScreenRunning = false;
                    break;
                }
            } else if (e.type == SDL_JOYBUTTONDOWN) {
                switch (e.jbutton.button) {
                    case JBUTTON_A:
                    case JBUTTON_START:
                    case JBUTTON_BACK:
                        WinScreenRunning = false;
                    break;
                }
            }
        }
    }

    return 0;
}

void Quit(int status) {
    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();
    exit(status);

}

int Title() {

    uint32_t rmask, gmask, bmask, amask;
        #if SDL_BYTEORDER == SDL_BIG_ENDIAN
            rmask = 0xff000000;
            gmask = 0x00ff0000;
            bmask = 0x0000ff00;
            amask = 0x000000ff;
        #else
            amask = 0xff000000;
            bmask = 0x00ff0000;
            gmask = 0x0000ff00;
            rmask = 0x000000ff;
        #endif // SDL_BIG_ENDIAN

    bool titleRunning = true;
    SDL_Texture* titleTexture = Utility::LoadTexture(ren, TITLE_IMG);

    int ticksElapsed = 0;

    int time = SDL_GetTicks();
    int renderTime = SDL_GetTicks();

    const int fadeInTicks = 2000;
    const int flashTicks = 500;
    bool fadeInDone = false;
    bool flash = true;

    int numJoysticks = 0;

    while (titleRunning) {

        if (SDL_NumJoysticks() > 0 && SDL_NumJoysticks() != numJoysticks) {
            numJoysticks = SDL_NumJoysticks();
            for (int i = 0; i < 4; ++i) {
                playersIn[i] = false;
            }
            maxPlayers = std::min(SDL_NumJoysticks(), 4);
            for (int i = 0; i < std::min(SDL_NumJoysticks(), 4); ++i) {
                gController[i] = SDL_JoystickOpen(i);
                if (gController[i] == NULL) {
                    std::cout << "Could not open joystick " << i << ". SDL Error: " << SDL_GetError() << std::endl;
                    Quit(2);
                }
            }
        }

        uint32_t frameTime = SDL_GetTicks() - time;
        time = SDL_GetTicks();

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                Quit(0);
            } else if (e.type == SDL_JOYBUTTONDOWN) {
                if (e.jbutton.button == JBUTTON_START && fadeInDone) {
                    titleRunning = false;
                } else if (e.jbutton.button == JBUTTON_BACK) {
                    Quit(0);
                }
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_RETURN && fadeInDone) {
                    titleRunning = false;
                }
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    Quit(0);
                }
            }
        }

        if (!fadeInDone) {
            ticksElapsed += frameTime;
            if (ticksElapsed >= fadeInTicks) {
                fadeInDone = true;
                ticksElapsed = 0;
            }
        } else {
            ticksElapsed += frameTime;
            if (ticksElapsed > flashTicks) {
                ticksElapsed = 0;
                flash = !flash;
            }
        }

        int alpha = 0;
        if (!fadeInDone) {
            alpha = 255 - ((float) ticksElapsed / fadeInTicks * 255);
        }

        if (SDL_TICKS_PASSED(time - renderTime, RENDER_INTERVAL) ) {

            renderTime = SDL_GetTicks();


            SDL_Surface* surf = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, rmask, gmask, bmask, amask);
            SDL_FillRect(surf, NULL, SDL_MapRGBA(surf->format, 0x00, 0x00, 0x00, alpha));
            SDL_Texture* fadeTex = SDL_CreateTextureFromSurface(ren, surf);
            SDL_FreeSurface(surf);

            SDL_Color white = {0xFF, 0xFF, 0xFF, 0xFF};
            SDL_Texture* startTexture = Utility::RenderText("Press Start", GAME_FONT, white, 16, ren);
            int startW, startH;
            SDL_QueryTexture(startTexture, NULL, NULL, &startW, &startH);

            SDL_Rect startRect;

            startRect.x = (SCREEN_WIDTH / 2) - (startW / 2);
            startRect.y = (SCREEN_HEIGHT / 2) + (SCREEN_HEIGHT / 4);
            startRect.w = startW;
            startRect.h = startH;

            SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0x00, 0xFF);
            SDL_RenderClear(ren);
            SDL_RenderCopy(ren, titleTexture, NULL, NULL);
            SDL_RenderCopy(ren, fadeTex, NULL, NULL);

            if (fadeInDone & flash) {
                SDL_RenderCopy(ren, startTexture, NULL, &startRect);
            }

            SDL_RenderPresent(ren);
            SDL_DestroyTexture(fadeTex);
            SDL_DestroyTexture(startTexture);
        }

    }
    SDL_DestroyTexture(titleTexture);
    return 0;
}
