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

void pause()
{
    int continuer = 1;
    SDL_Event event;
    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_KEYDOWN:
                {
                    // exit if Enter is pressed
                    switch(event.key.keysym.sym)
                    {
                    case SDLK_RETURN:
                        continuer = 0;
                        break;
                    default:
                        break;
                    }
                }
        }
    }
}
