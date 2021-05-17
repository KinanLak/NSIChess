//For minimisation of the window with a '-' -> SDL_WM_IconifyWindow()



//To play a song (?), doesn't work anymore
//int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
//SDL_PauseAudioDevice(deviceId, 0);
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>

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

    image = SDL_LoadBMP(BlackPawnImage);
    SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 239, 239, 239));
    texture = SDL_CreateTextureFromSurface(render, image);
    SDL_Rect dstrect;

    //Initialisation of the audio
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;
    SDL_LoadWAV(MoveSound, &wavSpec, &wavBuffer, &wavLength);
    SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);

    
    int chessBoard[64] = {1,1,1,0};
    int change = -1;
    for (int i=0; i<64; i++)
    {
        if (chessBoard[i]==1)
        {
            dstrect.x= (xMinBoard+(i%8)*117);
            dstrect.y= (yMinBoard+(i/8)*117);
            dstrect.h= 118;
            dstrect.w= 118;
            SDL_RenderCopy(render, texture, NULL, &dstrect);
        }
    }
    SDL_RenderPresent(render);


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
                    if (change ==-1 || giveCaseNumber(event.button.x, event.button.y)==change)
                    {
                        if (chessBoard[giveCaseNumber(event.button.x, event.button.y)]==1)
                        {
                            change = giveCaseNumber(event.button.x, event.button.y);
                            SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
                            SDL_Rect rect;
                            rect.x = (xMinBoard+(change%8)*117);
                            rect.y = (yMinBoard+(change/8)*117);
                            rect.w = 118;
                            rect.h = 118;
                            SDL_RenderDrawRect(render, &rect);
                            SDL_RenderPresent(render);
                        }
                    }
                    else
                    {
                        //Change the spieces
                        chessBoard[giveCaseNumber(event.button.x, event.button.y)] = 1;
                        chessBoard[change] = 0;

                        //Clear the window
                        SDL_RenderClear(render);

                        //Show the background
                        SDL_RenderCopy(render, texture2, NULL, NULL);

                        //Show all the pieces
                        for (int i=0; i<64; i++)
                        {
                            if (chessBoard[i]==1)
                            {
                                dstrect.x= (xMinBoard+(i%8)*117);
                                dstrect.y= (yMinBoard+(i/8)*117);
                                dstrect.h= 118;
                                dstrect.w= 118;
                                SDL_RenderCopy(render, texture, NULL, &dstrect);
                            }
                        }
                        SDL_RenderPresent(render);

                        //Reset change
                        change = -1;
                    }

                }
                else
                {
                    //Reset change
                    change = -1;

                    //Clear the window
                    //SDL_RenderClear(render);

                    //Show the background
                    //SDL_RenderCopy(render, texture2, NULL, NULL);

                    //Change the modification
                    //SDL_RenderPresent(render);
                }
        }
        
    }


    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    return 1;
}




int giveCaseNumber(int eventX, int eventY)
{
    return (eventX-xMinBoard)/lenSquare + (((eventY-yMinBoard)/lenSquare)*8);
}

/*void placePieces()
{       
    //Placement of black pawn
    for (int x=0; x<8; x++)
    {

    }
}*/
