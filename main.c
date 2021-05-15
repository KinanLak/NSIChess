//For minimisation of the window with a '-' -> SDL_WM_IconifyWindow()


#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define xMinBoard 494
#define yMinBoard 77
#define xMaxBoard 1427
#define yMaxBoard 1010
#define lenSquare 117


//Definition of images path
#define BoardBgImage "images/board/bg.bmp"
#define BlackPawnImage "images/board/pion.bmp"

//Definition of files path
#define MoveSound "sounds/move.wav"
#define CheckSound "sounds/check.wav"
#define CaptureSound "sounds/capture.wav"
#define RockSound "sounds/rock.wav"
#define GameStartSound "sounds/gameStart.wav"
#define GameOverSound "sounds/gameOver.wav"


int giveCaseNumber(int eventX, int eventY);




int main(int argc, char* argv[])
{
    SDL_Window* window = NULL;
    SDL_Renderer* render = NULL;
    SDL_Surface* image = NULL; 
    SDL_Surface* image2 = NULL; 
    SDL_Texture* texture = NULL;
    SDL_Texture* texture2 = NULL;

    //Initialisation of the SDL mode(s) that we will use
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Hello world", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_SWSURFACE);
    render = SDL_CreateRenderer(window, -1, 0);

    image2 = SDL_LoadBMP(BoardBgImage);
    texture2 = SDL_CreateTextureFromSurface(render, image2);
    SDL_RenderCopy(render, texture2, NULL, NULL);
    SDL_RenderPresent(render);
    image = SDL_LoadBMP(BlackPawnImage);
    SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 239, 239, 239));
    texture = SDL_CreateTextureFromSurface(render, image);

    /*SDL_Rect dstrect;
    dstrect.x= 300;
    dstrect.y= 300;
    dstrect.h= 118;
    dstrect.w= 118;
    SDL_RenderCopy(render, texture, NULL, &dstrect);
    SDL_RenderPresent(render);*/

    SDL_Event event;
    int continuer = 1;
    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.x >=1875 && event.button.y <=45)
                {
                    continuer=0;
                }
            case SDL_KEYDOWN:
                if (event.button.x >= xMinBoard && event.button.x <= xMaxBoard && event.button.y <=yMaxBoard && event.button.y >= yMinBoard)
                {
                    int x=event.button.x;
                    int y=event.button.y;
                    SDL_Rect dstrect;
                    dstrect.x= (xMinBoard+(x-xMinBoard)/117*117);
                    dstrect.y= (yMinBoard+(y-yMinBoard)/117*117);
                    dstrect.h= 118;
                    dstrect.w= 118;
                    SDL_RenderCopy(render, texture, NULL, &dstrect);
                    SDL_RenderPresent(render);
                    /*SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
                    SDL_RenderClear(render);
                    SDL_RenderPresent(render);*/

                }
                else
                {
                    /*int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
                    SDL_PauseAudioDevice(deviceId, 0);
                    SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
                    SDL_Rect rect;
                    rect.x = 0;
                    rect.y = 0;
                    rect.w = 32;
                    rect.h = 32;
                    SDL_RenderFillRect(render, &rect);
                    SDL_RenderPresent(render);*/
                }
        }
    }


    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    return 1;
}
