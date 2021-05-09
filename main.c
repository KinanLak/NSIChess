//For minimisation of the window with a '-' -> SDL_WM_IconifyWindow()


//For minimisation of the window with a '-' -> SDL_WM_IconifyWindow()


#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 500


void pause();


int main(int argc, char* argv[])
{
    //Variables initialisation
    SDL_Window *window = NULL;
    SDL_Surface *windowSurface = NULL;
    SDL_Surface *bgImage = NULL;
    SDL_Surface *currentImage = NULL;


    //Initialisation of the video mode that we will use
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Hello world", 50, 50, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_SWSURFACE);
    
    windowSurface = SDL_GetWindowSurface(window);

    bgImage = SDL_LoadBMP("bg.bmp");

    currentImage = bgImage;



    SDL_Event event;
 
    int continuer = 1;
    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
            case SDL_KEYDOWN:
                continuer = 0;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.x <=100)
                {
                    continuer = 0;
                }
        }
        SDL_BlitSurface(currentImage, NULL, windowSurface, NULL);
        SDL_UpdateWindowSurface(window);
    }

    //We free the surface
    SDL_FreeSurface(bgImage);
    SDL_DestroyWindow(window);

    bgImage = windowSurface = NULL;
    window = NULL;

    //Quit SDL
    SDL_Quit();
    return EXIT_SUCCESS;
}