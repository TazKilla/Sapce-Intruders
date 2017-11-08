#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif

#include <SDL/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <time.h>
#include <stdio.h>
#include <fmod.h>
#include <string.h>

#include "functions.h"

void pause();

int main ( int argc, char** argv )
{
    int i = 0, j = 0, temps_precedent = 0, temps_actuel = 0, pos_x = 0, pos_y = 0, numero_alien, lateral = 0;
    bool missile_tire = false, missile_tire_alien = false, droite = true;
    char chemin_image_anim[64];
    srand(time(NULL));

    FMOD_SYSTEM *system;
    FMOD_System_Create(&system);
    FMOD_System_Init(system, 2, FMOD_INIT_NORMAL, NULL);

    FMOD_SOUND *tir_alien = NULL;
    FMOD_SOUND *mon_tir = NULL;
    FMOD_SOUND *explosion_son = NULL;

    FMOD_System_CreateSound(system, "sounds/alien_weapon.mp3", FMOD_CREATESAMPLE, 0, &tir_alien);
    FMOD_System_CreateSound(system, "sounds/my_weapon.mp3", FMOD_CREATESAMPLE, 0, &mon_tir);
    FMOD_System_CreateSound(system, "sounds/boom.mp3", FMOD_CREATESAMPLE, 0, &explosion_son);

    donnees_vaisseaux_alien grille[4][6];
    donnees_missile shoot;

    pos_x = 11;
    pos_y = -200;

    for(i = 0; i < 4; i++)
    {
        pos_x = 11;
        for(j = 0; j < 6; j++)
        {
            grille[i][j].positionAlien.x = pos_x;
            grille[i][j].positionAlien.y = pos_y;
            grille[i][j].dead = false;
            pos_x += 66;
        }
        pos_y += 61;
    }

    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    SDL_WM_SetIcon(IMG_Load("imgs/ico.bmp"), NULL);
    SDL_WM_SetCaption("Space Intruder", NULL);
    SDL_EnableKeyRepeat(100, 100);

    // create a new window
    SDL_Surface* screen = SDL_SetVideoMode(500, 600, 16, SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( !screen )
    {
        printf("Unable to set 500x600 video: %s\n", SDL_GetError());
        return 1;
    }

    // load an image
    SDL_Surface* fond_accueil = IMG_Load("imgs/space.jpg");
    SDL_Surface* fond = IMG_Load("imgs/stars.jpg");
    SDL_Surface* fond_mort = IMG_Load("imgs/fond_mort.png");
    SDL_Surface* my_ship = IMG_Load("imgs/my_ship.png");
    SDL_Surface* alien_ship = IMG_Load("imgs/alien_ship.png");
    SDL_Surface* missile = IMG_Load("imgs/missile.png");
    SDL_Surface* shot = IMG_Load("imgs/shot.png");
    SDL_Surface* explosion[11];

    for(i = 0; i < 11; i++)
    {
        sprintf(chemin_image_anim, "imgs/explosion_%d.png", i + 1);
        explosion[i] = IMG_Load(chemin_image_anim);
    }

    /*explosion[0] = IMG_Load("imgs/explosion_1.png");
    explosion[1] = IMG_Load("imgs/explosion_2.png");
    explosion[2] = IMG_Load("imgs/explosion_3.png");
    explosion[3] = IMG_Load("imgs/explosion_4.png");
    explosion[4] = IMG_Load("imgs/explosion_5.png");
    explosion[5] = IMG_Load("imgs/explosion_6.png");
    explosion[6] = IMG_Load("imgs/explosion_7.png");
    explosion[7] = IMG_Load("imgs/explosion_8.png");
    explosion[8] = IMG_Load("imgs/explosion_9.png");
    explosion[9] = IMG_Load("imgs/explosion_10.png");
    explosion[10] = IMG_Load("imgs/explosion_11.png");*/

    if (!fond_accueil || !fond_mort || !fond || !my_ship || !alien_ship || !missile || !shot)
    {
        printf("Unable to load sprite: %s\n", SDL_GetError());
        return 1;
    }


    // centre the bitmap on screen
    /*SDL_Rect dstrect, positionFond;
    dstrect.x = (screen->w - bmp->w) / 2;
    dstrect.y = (screen->h - bmp->h) / 2;*/
    SDL_Rect positionFond, positionVaisseau, positionAlien, positionAlienDepart, positionMissile, positionMissileAlien, positionExplosion;
    positionFond.x = 0;
    positionFond.y = 0;
    positionVaisseau.x = 225;
    positionVaisseau.y = 650;
    positionAlienDepart.y = 11;
    positionAlienDepart.x = 11;
    positionMissile.x = 0;
    positionMissile.y = 0;
    positionMissileAlien.x = 0;
    positionMissileAlien.y = 0;

    SDL_BlitSurface(fond_accueil, 0, screen, &positionFond);
    SDL_Flip(screen);
    pause();

    while(positionVaisseau.y > 520)
    {
        SDL_BlitSurface(fond, 0, screen, &positionFond);
        SDL_BlitSurface(my_ship, 0, screen, &positionVaisseau);
        SDL_Flip(screen);
        positionVaisseau.y--;
        SDL_Delay(10);
    }

    int decallage = 250;
    pos_y = -239;
    while(pos_y < 11)
    {
        for(i = 0; i < 4; i++)
        {
            pos_x = 11;
            for(j = 0; j < 6; j++)
            {
                grille[i][j].positionAlien.x = pos_x;
                grille[i][j].positionAlien.y = pos_y;
                pos_x += 66;
            }
            pos_y += 61;
        }
        SDL_BlitSurface(fond, 0, screen, &positionFond);
        SDL_BlitSurface(my_ship, 0, screen, &positionVaisseau);
        for(i = 0; i < 4; i++)
        {
            for(j = 0; j < 6; j++)
            {
                SDL_BlitSurface(alien_ship, 0, screen, &grille[i][j].positionAlien);
            }
        }

        SDL_Flip(screen);
        decallage--;
        pos_y = 11 - decallage;
        //SDL_Delay(1);
    }

    // program main loop
    bool dead = false, run = false, right = false, done = false;
    while(!done)
    {
        while (!dead)
        {
            //temps actuel = SDL_GetTicks();
            // message processing loop
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                // check for messages
                switch (event.type)
                {
                    // exit if the window is closed
                case SDL_QUIT:
                    dead = true;
                    break;

                    // check for keypresses
                case SDL_KEYDOWN:
                    {
                        // exit if ESCAPE is pressed
                        switch (event.key.keysym.sym)
                        {
                        case SDLK_ESCAPE:
                            dead = true;
                            break;
                        case SDLK_LEFT:
                            positionVaisseau.x -= 10;
                            break;
                        case SDLK_RIGHT:
                            positionVaisseau.x += 10;
                            break;
                        case SDLK_UP:
                        case SDLK_SPACE:
                            if(!missile_tire)
                            {
                                positionMissile.y = positionVaisseau.y - 10;
                                positionMissile.x = positionVaisseau.x + 20;
                                missile_tire = true;
                                FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, mon_tir, 0, NULL);
                            }
                            break;
                        default:
                            break;
                        /*case SDLK_DOWN:
                            positionVaisseau.y += 5;
                            break;*/
                        }
                    }
                } // end switch

            } // end of message processing
            /*if(left == true)
                positionVaisseau.x -= 1;
            if(right == true)
                positionVaisseau.x += 1;*/
            SDL_BlitSurface(fond, 0, screen, &positionFond);
            SDL_BlitSurface(my_ship, 0, screen, &positionVaisseau);
            temps_actuel = SDL_GetTicks();
            if(temps_actuel - temps_precedent > 2500)
            {
                numero_alien = (rand() % (6 - 1 + 1) - 1);
                positionMissileAlien.y = grille[3][numero_alien + 1].positionAlien.y + 10;
                positionMissileAlien.x = grille[3][numero_alien + 1].positionAlien.x + 20;
                missile_tire_alien = true;
                FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, tir_alien, 0, NULL);
                temps_precedent = temps_actuel;
            }

            if(missile_tire_alien && positionMissileAlien.y < 600)
            {
                SDL_BlitSurface(shot, 0, screen, &positionMissileAlien);
                positionMissileAlien.y += 2;

                if((positionMissileAlien.x + 3 >= positionVaisseau.x && positionMissileAlien.x + 3 < positionVaisseau.x + 50) && (positionMissileAlien.y + 30 == positionVaisseau.y || positionMissileAlien.y + 30 == positionVaisseau.y + 1))
                {
                    FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, explosion_son, 0, NULL);
                    for(i = 0; i < 11; i++)
                    {
                        positionExplosion.x = positionMissileAlien.x + 3 - (explosion[i]->w / 2);
                        positionExplosion.y = positionMissileAlien.y + 10 - (explosion[i]->h / 2);
                        SDL_BlitSurface(explosion[i], 0, screen, &positionExplosion);
                        SDL_Flip(screen);
                        SDL_Delay(50);
                    }
                    dead = true;
                    missile_tire_alien = false;
                }
            }
            else
                missile_tire_alien = false;

            if(missile_tire && positionMissile.y > 0)
            {
                SDL_BlitSurface(missile, 0, screen, &positionMissile);
                positionMissile.y -= 2;

                for(j = 0; j < 6; j++)
                {
                    if((positionMissile.x + 6 >= grille[3][j].positionAlien.x && positionMissile.x + 6 < grille[3][j].positionAlien.x + 30) && (positionMissile.y == grille[3][j].positionAlien.y + 50 || positionMissile.y == grille[3][j].positionAlien.y + 49))
                    {
                        FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, explosion_son, 0, NULL);
                        for(i = 0; i < 11; i++)
                        {
                            positionExplosion.x = positionMissile.x + 6 - (explosion[i]->w / 2);
                            positionExplosion.y = positionMissile.y - (explosion[i]->h / 2);
                            SDL_BlitSurface(explosion[i], 0, screen, &positionExplosion);
                            SDL_Flip(screen);
                            SDL_Delay(25);
                        }
                        grille[3][j].dead = true;
                        missile_tire = false;
                    }
                }
            }
            else
                missile_tire = false;
            /*if(temps_actuel - temps_precedent >= 500)
                positionAlien.x += 1;
            else
                positionAlien.x = 11;*/
            /*grille[i][j].positionAlien.y = 11;
            grille[i][j].positionAlien.y = 11;*/

            if(lateral < 80 && droite)
            {
                for(i = 0; i < 4; i++)
                {
                    for(j = 0; j < 6; j++)
                    {
                        if(!grille[i][j].dead)
                            SDL_BlitSurface(alien_ship, 0, screen, &grille[i][j].positionAlien);
                            grille[i][j].positionAlien.x++;
                            //SDL_Delay(1);
                    }
                }
                lateral++;
                SDL_Delay(1);
            }
            else if(lateral == 80)
            {
                droite = false;
            }

            if(lateral > 0 && !droite)
            {
                for(i = 0; i < 4; i++)
                {
                    for(j = 0; j < 6; j++)
                    {
                        if(!grille[i][j].dead)
                            SDL_BlitSurface(alien_ship, 0, screen, &grille[i][j].positionAlien);
                            grille[i][j].positionAlien.x--;
                            //SDL_Delay(1);
                    }
                }
                lateral--;
                SDL_Delay(1);
            }
            else if(lateral == 0)
            {
                droite = true;
            }

            // DRAWING STARTS HERE

            // clear screen
            //SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));

            // draw bitmap
            //SDL_BlitSurface(fond, 0, screen, &positionFond);
            //SDL_BlitSurface(bmp, 0, screen, &dstrect);

            // DRAWING ENDS HERE

            // finally, update the screen :)
            SDL_Flip(screen);
        } // end main loop

        SDL_BlitSurface(fond_mort, 0, screen, &positionFond);
        SDL_Flip(screen);

        SDL_Event event;
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_KEYDOWN:
            {
                // exit if Enter is pressed
                switch (event.key.keysym.sym)
                {
                    case SDLK_RETURN:
                        done = false;
                        dead = false;
                        break;
                    case SDLK_ESCAPE:
                        done = true;
                        break;
                }
            }
        }
    }

    // free loaded bitmap
    SDL_FreeSurface(fond);
    SDL_FreeSurface(fond_accueil);
    SDL_FreeSurface(fond_mort);
    SDL_FreeSurface(my_ship);
    SDL_FreeSurface(alien_ship);
    SDL_FreeSurface(missile);
    SDL_FreeSurface(shot);
    for(i = 0; i < 11; i++)
    {
        SDL_FreeSurface(explosion[i]);
    }
    SDL_FreeSurface(screen);
    FMOD_System_Close(system);
    FMOD_System_Release(system);

    // all is well ;)
    printf("Exited cleanly\n");
    return 0;
}
