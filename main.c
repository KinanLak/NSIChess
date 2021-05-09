//For minimisation of the window with a '-' -> SDL_WM_IconifyWindow()


#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL_image.h>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080


void pause();


int main(int argc, char* argv[])
{
  //Initialisation of the video mode that we will use
  SDL_Init(SDL_INIT_VIDEO);
  
  SDL_CreateWindow("Hello world", 50, 50, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_SWSURFACE);

  pause();

  //Quit SDL
  SDL_Quit();
  return EXIT_SUCCESS;
}

void pause()
{
    int continuer = 1;
    SDL_Event event;
 
    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
        }
    }
}
