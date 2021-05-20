#ifndef H_FILE
#define H_FILE

//Definitions of variables
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define xMinBoard 494
#define yMinBoard 77
#define xMaxBoard 1427
#define yMaxBoard 1010
#define lenSquare 117
#define RED 255, 0, 0, 255
#define BLACK 0, 0, 0, 255
#define NOTHING -1
#define transparentColor 239, 239, 239


//Definition of images path
#define BoardBgImageBMP "images/board/bg.bmp"

#define BlackPawnImageBMP "images/board/black/pawn.bmp"
#define BlackKnightImageBMP "images/board/black/knight.bmp"
#define BlackKingImageBMP "images/board/black/king.bmp"
#define BlackQueenImageBMP "images/board/black/queen.bmp"
#define BlackRookImageBMP "images/board/black/rook.bmp"
#define BlackBishopImageBMP "images/board/black/bishop.bmp"

#define WhitePawnImageBMP "images/board/white/pawn.bmp"
#define WhiteKnightImageBMP "images/board/white/knight.bmp"
#define WhiteKingImageBMP "images/board/white/reine2.bmp"
#define WhiteQueenImageBMP "images/board/white/queen.bmp"
#define WhiteRookImageBMP "images/board/white/rook.bmp"
#define WhiteBishopImageBMP "images/board/white/bishop.bmp"


//Definition of files path
#define MoveSound "sounds/move.wav"
#define CheckSound "sounds/check.wav"
#define CaptureSound "sounds/capture.wav"
#define RockSound "sounds/rock.wav"
#define GameStartSound "sounds/gameStart.wav"
#define GameOverSound "sounds/gameOver.wav"

//Definition of pseudo-function
#define ALLImageAndTransparencyINIT(X,Y, Z, render)   X = SDL_LoadBMP(Z);\
                            SDL_SetColorKey(X, SDL_TRUE, SDL_MapRGB(X->format, transparentColor));\
                            Y = SDL_CreateTextureFromSurface(render, X);

#define ALLImageINIT(X,Y, Z, render)   X = SDL_LoadBMP(Z);\
                            Y = SDL_CreateTextureFromSurface(render, X);

#define DrawImage(rect, rectP, i, texture) rect.x= (xMinBoard+(i%8)*lenSquare);\
            rect.y= (yMinBoard+(i/8)*lenSquare);\
            rect.h= 118;\
            rect.w= 118;\
            SDL_RenderCopy(render, texture, NULL, rectP);

#define CreateRenderInNewWindow(window, render) window = SDL_CreateWindow("Hello world", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_SWSURFACE);\
    render = SDL_CreateRenderer(window, NOTHING, 0);


//Prototypes
int giveCaseNumber(int eventX, int eventY);
void drawSquare(int squareNumber, SDL_Renderer* render);

#endif