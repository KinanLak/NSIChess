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


int main(int argc, char* argv[])
{
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
    SDL_Window* window = NULL;
    SDL_Renderer* render = NULL;
    SDL_Surface* imageBlackPawn = NULL; 
    SDL_Surface* imageBlackKnight = NULL; 
    SDL_Surface* imageBlackRook = NULL; 
    SDL_Surface* imageBlackBishop = NULL; 
    SDL_Surface* imageBlackQueen = NULL; 
    SDL_Surface* imageBlackKing = NULL; 
    SDL_Surface* imageWhitePawn = NULL; 
    SDL_Surface* imageWhiteKnight = NULL; 
    SDL_Surface* imageWhiteRook = NULL; 
    SDL_Surface* imageWhiteBishop = NULL; 
    SDL_Surface* imageWhiteQueen = NULL; 
    SDL_Surface* imageWhiteKing = NULL; 
    SDL_Surface* imageBackground = NULL;
    SDL_Surface* imageBlueBoard = NULL;

    SDL_Texture* textureBlackPawn = NULL; 
    SDL_Texture* textureBlackKnight = NULL; 
    SDL_Texture* textureBlackRook = NULL; 
    SDL_Texture* textureBlackBishop = NULL; 
    SDL_Texture* textureBlackQueen = NULL; 
    SDL_Texture* textureBlackKing = NULL; 
    SDL_Texture* textureWhitePawn = NULL; 
    SDL_Texture* textureWhiteKnight = NULL; 
    SDL_Texture* textureWhiteRook = NULL; 
    SDL_Texture* textureWhiteBishop = NULL; 
    SDL_Texture* textureWhiteQueen = NULL; 
    SDL_Texture* textureWhiteKing = NULL; 
    SDL_Texture* textureBackground = NULL;
    SDL_Texture* textureBlueBoard = NULL;

    //Initialisation of the SDL mode(s) that we will use
    SDL_Init(SDL_INIT_VIDEO);
    CreateRenderInNewWindow(window, render)

    //Create all images
    ALLImageINIT(imageBackground, textureBackground, BoardBgImageBMP, render)
    ALLImageINIT(imageBlueBoard, textureBlueBoard, BlueBoardBMP, render)
    

    //ALLImageAndTransparencyINIT(imageBlackPawn ,textureBlackPawn, BlackPawnImageBMP, render)
    ALLImageAndTransparencyINIT(imageBlackRook ,textureBlackRook, BlackRookImageBMP, render)
    ALLImageAndTransparencyINIT(imageBlackBishop ,textureBlackBishop, BlackBishopImageBMP, render)
    ALLImageAndTransparencyINIT(imageBlackKnight ,textureBlackKnight, BlackKnightImageBMP, render)
    ALLImageAndTransparencyINIT(imageBlackQueen ,textureBlackQueen, BlackQueenImageBMP, render)
    ALLImageAndTransparencyINIT(imageBlackKing ,textureBlackKing, BlackKingImageBMP, render)

    ALLImageAndTransparencyINIT(imageWhitePawn ,textureWhitePawn, WhitePawnImageBMP, render)
    ALLImageAndTransparencyINIT(imageWhiteRook ,textureWhiteRook, WhiteRookImageBMP, render)
    ALLImageAndTransparencyINIT(imageWhiteBishop ,textureWhiteBishop, WhiteBishopImageBMP, render)
    ALLImageAndTransparencyINIT(imageWhiteKnight ,textureWhiteKnight, WhiteKnightImageBMP, render)
    ALLImageAndTransparencyINIT(imageWhiteQueen ,textureWhiteQueen, WhiteQueenImageBMP, render)
    ALLImageAndTransparencyINIT(imageWhiteKing ,textureWhiteKing, WhiteKingImageBMP, render)


    SDL_Rect dstrect;
    int previousMove[2]={NOTHING, NOTHING};

    //Initialisation of the audio
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;
    SDL_LoadWAV(MoveSound, &wavSpec, &wavBuffer, &wavLength);
    SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
    


    unsigned int chessBoard[64] = {
    4,2,3,7,6,3,2,4,
    1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    9,9,9,9,9,9,9,9,
    12,10,11,14,15,11,10,12};

    int change = NOTHING;

    SDL_RenderCopy(render, textureBackground, NULL, NULL);
    for (int i=0; i<64; i++)
    {
        if (chessBoard[i]%8==1)
        {
            if (chessBoard[i]/8==1)
            {
                DrawImage(dstrect, &dstrect, i, textureBlackPawn)
            }
            else
            {
                DrawImage(dstrect, &dstrect, i, textureWhitePawn)
            }
        }
        else if (chessBoard[i]%8==2)
        {
            if (chessBoard[i]/8==1)
            {
                DrawImage(dstrect, &dstrect, i, textureBlackKnight)
            }
            else
            {
                DrawImage(dstrect, &dstrect, i, textureWhiteKnight)
            }
        }
        else if (chessBoard[i]%8==3)
        {
            if (chessBoard[i]/8==1)
            {
                DrawImage(dstrect, &dstrect, i, textureBlackBishop)
            }
            else
            {
                DrawImage(dstrect, &dstrect, i, textureWhiteBishop)
            }
        }
        else if (chessBoard[i]%8==4)
        {
            if (chessBoard[i]/8==1)
            {
                DrawImage(dstrect, &dstrect, i, textureBlackRook)
            }
            else
            {
                DrawImage(dstrect, &dstrect, i, textureWhiteRook)
            }
        }
        else if (chessBoard[i]%8==6)
        {
            if (chessBoard[i]/8==1)
            {
                DrawImage(dstrect, &dstrect, i, textureBlackQueen)
            }
            else
            {
                DrawImage(dstrect, &dstrect, i, textureWhiteQueen)
            }
        }
        else if (chessBoard[i]%8==7)
        {
            if (chessBoard[i]/8==1)
            {
                DrawImage(dstrect, &dstrect, i, textureBlackKing)
            }
            else
            {
                DrawImage(dstrect, &dstrect, i, textureWhiteKing)
            }
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
                    if (change ==NOTHING || giveCaseNumber(event.button.x, event.button.y)==change)
                    {
                        if (chessBoard[giveCaseNumber(event.button.x, event.button.y)]!=0)
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

                        //Show the background
                        SDL_RenderCopy(render, textureBackground, NULL, NULL);

                        //Show all the pieces
                        for (int i=0; i<64; i++)
                        {
                            if (chessBoard[i]!=0)
                            {
                                if (chessBoard[i]%8==1)
                                {
                                    if (chessBoard[i]/8==1)
                                    {
                                        DrawImage(dstrect, &dstrect, i, textureBlackPawn)
                                    }
                                    else
                                    {
                                        DrawImage(dstrect, &dstrect, i, textureWhitePawn)
                                    }
                                }
                                else if (chessBoard[i]%8==2)
                                {
                                    if (chessBoard[i]/8==1)
                                    {
                                        DrawImage(dstrect, &dstrect, i, textureBlackKnight)
                                    }
                                    else
                                    {
                                        DrawImage(dstrect, &dstrect, i, textureWhiteKnight)
                                    }
                                }
                                else if (chessBoard[i]%8==3)
                                {
                                    if (chessBoard[i]/8==1)
                                    {
                                        DrawImage(dstrect, &dstrect, i, textureBlackBishop)
                                    }
                                    else
                                    {
                                        DrawImage(dstrect, &dstrect, i, textureWhiteBishop)
                                    }
                                }
                                else if (chessBoard[i]%8==4)
                                {
                                    if (chessBoard[i]/8==1)
                                    {
                                        DrawImage(dstrect, &dstrect, i, textureBlackRook)
                                    }
                                    else
                                    {
                                        DrawImage(dstrect, &dstrect, i, textureWhiteRook)
                                    }
                                }
                                else if (chessBoard[i]%8==6)
                                {
                                    if (chessBoard[i]/8==1)
                                    {
                                        DrawImage(dstrect, &dstrect, i, textureBlackQueen)
                                    }
                                    else
                                    {
                                        DrawImage(dstrect, &dstrect, i, textureWhiteQueen)
                                    }
                                }
                                else if (chessBoard[i]%8==7)
                                {
                                    if (chessBoard[i]/8==1)
                                    {
                                        DrawImage(dstrect, &dstrect, i, textureBlackKing)
                                    }
                                    else
                                    {
                                        DrawImage(dstrect, &dstrect, i, textureWhiteKing)
                                    }
                                }
                            }
                        }
                        if (previousMove[0]!=NOTHING)
                        {
                            SDL_SetRenderDrawColor(render, RED);
                            drawSquare(previousMove[0], render);
                            drawSquare(previousMove[1], render);
                        }
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
    return (eventX-xMinBoard)/lenSquare + (((eventY-yMinBoard)/lenSquare)*8);
}

void drawSquare(int squareNumber, SDL_Renderer* render)
{
    SDL_Rect rect;
    rect.x = (xMinBoard+(squareNumber%8)*lenSquare);
    rect.y = (yMinBoard+(squareNumber/8)*lenSquare);
    rect.w = lenSquare;
    rect.h = lenSquare;
    SDL_RenderDrawRect(render, &rect);
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
-the computer must choose the white and the black pawn (can be influence be a parameter)
--It has to put a variable which is 1 if the user is white else it's 0 so black
-Display all the pawn
-initialisation of the timer
-launch the timer for the white
----launch the relevant 



*/
