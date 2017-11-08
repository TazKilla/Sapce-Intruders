#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif

#include <SDL/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <time.h>

struct donnees_vaisseaux_alien
{
    bool dead;
    SDL_Rect positionAlien;
};

struct donnees_missile
{
    SDL_Rect positionMissile;
};
