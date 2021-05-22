//To play a song (?), doesn't work anymore
//int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
//SDL_PauseAudioDevice(deviceId, 0);
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>
#include "main.h"
#include "move.h"

/*
x001 -> pawn
x010 -> knight
x011 -> bishop
x100 -> rook
x110 -> queen
x111 -> king
*/

int main(int argc, char *argv[])
{
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
    SDL_Window *window = NULL;
    SDL_Renderer *render = NULL;
    initAllSurfaces()
    initAllTextures()

    //Initialisation of SDL_ttf
    TTF_init();

    //Initialisation of the SDL mode(s) that we will use
    SDL_Init(SDL_INIT_VIDEO);
    CreateRenderInNewWindow(window, render)

    //Create all images
    initAllImages()

    SDL_Rect dstrect;
    int previousMove[2] = {NOTHING, NOTHING};

    //Initialisation of the audio
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;
    SDL_LoadWAV(MoveSound, &wavSpec, &wavBuffer, &wavLength);
    SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);

    unsigned int chessBoard[64] = departPosition;

    int change = NOTHING;

    SDL_RenderCopy(render, textureBackground, NULL, NULL);
    displayAllpiecesInRender()
        SDL_RenderPresent(render);

    SDL_Event event;
    int continuer = 1;
    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            continuer = 0;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.x >= 1875 && event.button.y <= 45)
            {
                continuer = 0;
            }
        case SDL_KEYDOWN:
            if (event.button.x >= xMinBoard && event.button.x <= xMaxBoard && event.button.y <= yMaxBoard && event.button.y >= yMinBoard)
            {
                if (change == NOTHING || giveCaseNumber(event.button.x, event.button.y) == change)
                {
                    if (giveCaseNumber(event.button.x, event.button.y)==change)
                    {
                        change=NOTHING;
                        SDL_RenderClear(render);
                        SDL_RenderCopy(render, textureBackground, NULL, NULL);
                        displayAllpiecesInRender()
                        SDL_RenderPresent(render);
                    }
                    else if (chessBoard[giveCaseNumber(event.button.x, event.button.y)] != 0)
                    {
                        change = giveCaseNumber(event.button.x, event.button.y);
                        SDL_SetRenderDrawColor(render, BLACK);
                        drawSquare(change, render);
                        SDL_RenderPresent(render);
                    }
                }
                else
                {
                    //Change the moves in the previousMove array
                    previousMove[0] = change;
                    previousMove[1] = giveCaseNumber(event.button.x, event.button.y);

                    //Change the pieces of position
                    chessBoard[giveCaseNumber(event.button.x, event.button.y)] = chessBoard[change];
                    chessBoard[change] = 0;

                    //Clear the window
                    SDL_RenderClear(render);

                    //Quit SLD_ttf
                    TTF_quit();

                    //Show the background
                    SDL_RenderCopy(render, textureBackground, NULL, NULL);

                    //Show the previous move
                    showPreviousMoves()

                        //Show all the pieces
                        displayAllpiecesInRender()

                            SDL_RenderPresent(render);

                    //Reset change
                    change = NOTHING;
                }
            }
            else
            {
                //Reset change
                change = NOTHING;
            }
        }
    }

    //Destroy all textures
    SDL_DestroyTexture(textureBlackPawn);
    SDL_DestroyTexture(textureBlackRook);
    SDL_DestroyTexture(textureBlackKnight);
    SDL_DestroyTexture(textureBlackBishop);
    SDL_DestroyTexture(textureBlackQueen);
    SDL_DestroyTexture(textureBlackKing);
    SDL_DestroyTexture(textureWhitePawn);
    SDL_DestroyTexture(textureWhiteRook);
    SDL_DestroyTexture(textureWhiteKnight);
    SDL_DestroyTexture(textureWhiteBishop);
    SDL_DestroyTexture(textureWhiteQueen);
    SDL_DestroyTexture(textureWhiteKing);

    //Destroy all the images
    SDL_FreeSurface(imageBlackPawn);
    SDL_FreeSurface(imageBlackRook);
    SDL_FreeSurface(imageBlackKnight);
    SDL_FreeSurface(imageBlackBishop);
    SDL_FreeSurface(imageBlackQueen);
    SDL_FreeSurface(imageBlackKing);
    SDL_FreeSurface(imageWhitePawn);
    SDL_FreeSurface(imageWhiteRook);
    SDL_FreeSurface(imageWhiteKnight);
    SDL_FreeSurface(imageWhiteBishop);
    SDL_FreeSurface(imageWhiteQueen);
    SDL_FreeSurface(imageWhiteKing);

    //Destroy the window
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    return 1;
}

int giveCaseNumber(int eventX, int eventY)
{
    return (eventX - xMinBoard) / lenSquare + (((eventY - yMinBoard) / lenSquare) * 8);
}

void drawSquare(int squareNumber, SDL_Renderer *render)
{
    SDL_Rect rect;
    rect.x = (xMinBoard + (squareNumber % 8) * lenSquare);
    rect.y = (yMinBoard + (squareNumber / 8) * lenSquare);
    rect.w = lenSquare;
    rect.h = lenSquare;
    SDL_RenderDrawRect(render, &rect);
    rect.x = (xMinBoard + (squareNumber % 8) * lenSquare) + 1;
    rect.y = (yMinBoard + (squareNumber / 8) * lenSquare) + 1;
    rect.w = lenSquare - 2;
    rect.h = lenSquare - 2;
    SDL_RenderDrawRect(render, &rect);
    rect.x = (xMinBoard + (squareNumber % 8) * lenSquare) + 2;
    rect.y = (yMinBoard + (squareNumber / 8) * lenSquare) + 2;
    rect.w = lenSquare - 4;
    rect.h = lenSquare - 4;
    SDL_RenderDrawRect(render, &rect);
    rect.x = (xMinBoard + (squareNumber % 8) * lenSquare) + 3;
    rect.y = (yMinBoard + (squareNumber / 8) * lenSquare) + 3;
    rect.w = lenSquare - 6;
    rect.h = lenSquare - 6;
    SDL_RenderDrawRect(render, &rect);
}

void displayTimer()
{


}

void drawFullSquarePreviousMove(int squareNumber, SDL_Renderer *render)
{
    if (((squareNumber / 8) + (squareNumber % 8)) % 2 == 0)
    {
        SDL_SetRenderDrawColor(render, REDLight);
    }
    else
    {
        SDL_SetRenderDrawColor(render, RED);
    }
    SDL_Rect rect;
    rect.x = (xMinBoard + (squareNumber % 8) * lenSquare);
    rect.y = (yMinBoard + (squareNumber / 8) * lenSquare);
    rect.w = lenSquare;
    rect.h = lenSquare;
    SDL_RenderFillRect(render, &rect);
}

// Recap of all the things that has to be done

/*
To Do:
--Change the function give case number, in order to take into account the color of the player
--Create a function that collecte all the legal moves from a case
--Implement a chained list of all legals moves
--Create a function that show all the legals moves
--Create a function that displays all the pieces
--Create a double function that collect the 
Initialisation of the game:
-the computer must choose the white and the black pawn (can be influence by a parameter)
--It has to put a variable which is 1 if the user is white else it's 0 so black
-Display all the pawn
-initialisation of the timer
-launch the timer for the white
----launch the relevant 
*/
