#ifndef H_FILE
#define H_FILE

//Definitions of variables
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define xMinBoard 494
#define xMaxBoard 1427
#define yMinBoard 77
#define yMaxBoard 1010
#define lenSquare 117
#define RED 255, 100, 100, 255
#define REDLight 255, 155, 155, 255
#define BLACK 0, 0, 0, 255
#define NOTHING -1
#define transparentColor 239, 239, 239


//Definition of images path
#define BoardBgImageBMP "images/board/bg.bmp"
#define BlueBoardBMP "images/board/blue_board.bmp"

#define BlackPawnImageBMP "images/board/black/pawn.bmp"
#define BlackKnightImageBMP "images/board/black/knight.bmp"
#define BlackKingImageBMP "images/board/black/king.bmp"
#define BlackQueenImageBMP "images/board/black/queen.bmp"
#define BlackRookImageBMP "images/board/black/rook.bmp"
#define BlackBishopImageBMP "images/board/black/bishop.bmp"

#define WhitePawnImageBMP "images/board/white/pawn.bmp"
#define WhiteKnightImageBMP "images/board/white/knight.bmp"
#define WhiteKingImageBMP "images/board/white/king.bmp"
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


#define departPosition { \
    4,2,3,7,6,3,2,4, \
    1,1,1,1,1,1,1,1,\
    0,0,0,0,0,0,0,0,\
    0,0,0,0,0,0,0,0,\
    0,0,0,0,0,0,0,0,\
    0,0,0,0,0,0,0,0,\
    9,9,9,9,9,9,9,9,\
    12,10,11,14,15,11,10,12}

#define initAllTextures() SDL_Texture* textureBlackPawn = NULL;\
    SDL_Texture* textureBlackKnight = NULL; \
    SDL_Texture* textureBlackRook = NULL; \
    SDL_Texture* textureBlackBishop = NULL;\
    SDL_Texture* textureBlackQueen = NULL; \
    SDL_Texture* textureBlackKing = NULL; \
    SDL_Texture* textureWhitePawn = NULL; \
    SDL_Texture* textureWhiteKnight = NULL;\
    SDL_Texture* textureWhiteRook = NULL; \
    SDL_Texture* textureWhiteBishop = NULL;\
    SDL_Texture* textureWhiteQueen = NULL; \
    SDL_Texture* textureWhiteKing = NULL;\
    SDL_Texture* textureBackground = NULL;

#define initAllSurfaces() SDL_Surface* imageBlackPawn = NULL; \
    SDL_Surface* imageBlackKnight = NULL; \
    SDL_Surface* imageBlackRook = NULL; \
    SDL_Surface* imageBlackBishop = NULL; \
    SDL_Surface* imageBlackQueen = NULL; \
    SDL_Surface* imageBlackKing = NULL; \
    SDL_Surface* imageWhitePawn = NULL; \
    SDL_Surface* imageWhiteKnight = NULL;\
    SDL_Surface* imageWhiteRook = NULL; \
    SDL_Surface* imageWhiteBishop = NULL;\
    SDL_Surface* imageWhiteQueen = NULL; \
    SDL_Surface* imageWhiteKing = NULL;\
    SDL_Surface* imageBackground = NULL;

#define drawImageColor(dstrect, i, textureBlack, textureWhite, nb) if (chessBoard[i]%8==nb)\
            {\
                if (chessBoard[i]/8==1)\
                {\
                    DrawImage(dstrect, &dstrect, i, textureBlack)\
                }\
                else\
                {\
                    DrawImage(dstrect, &dstrect, i, textureWhite)\
                }\
            }
#define initAllImages() ALLImageINIT(imageBackground, textureBackground, BoardBgImageBMP, render)\
    ALLImageAndTransparencyINIT(imageBlackPawn ,textureBlackPawn, BlackPawnImageBMP, render)\
    ALLImageAndTransparencyINIT(imageBlackRook ,textureBlackRook, BlackRookImageBMP, render)\
    ALLImageAndTransparencyINIT(imageBlackBishop ,textureBlackBishop, BlackBishopImageBMP, render)\
    ALLImageAndTransparencyINIT(imageBlackKnight ,textureBlackKnight, BlackKnightImageBMP, render)\
    ALLImageAndTransparencyINIT(imageBlackQueen ,textureBlackQueen, BlackQueenImageBMP, render)\
    ALLImageAndTransparencyINIT(imageBlackKing ,textureBlackKing, BlackKingImageBMP, render)\
    ALLImageAndTransparencyINIT(imageWhitePawn ,textureWhitePawn, WhitePawnImageBMP, render)\
    ALLImageAndTransparencyINIT(imageWhiteRook ,textureWhiteRook, WhiteRookImageBMP, render)\
    ALLImageAndTransparencyINIT(imageWhiteBishop ,textureWhiteBishop, WhiteBishopImageBMP, render)\
    ALLImageAndTransparencyINIT(imageWhiteKnight ,textureWhiteKnight, WhiteKnightImageBMP, render)\
    ALLImageAndTransparencyINIT(imageWhiteQueen ,textureWhiteQueen, WhiteQueenImageBMP, render)\
    ALLImageAndTransparencyINIT(imageWhiteKing ,textureWhiteKing, WhiteKingImageBMP, render)

#define showPreviousMoves()  if (previousMove[0]!=NOTHING)\
                        {\
                            drawFullSquarePreviousMove(previousMove[0], render);\
                            drawFullSquarePreviousMove(previousMove[1], render);\
                        }
#define displayAllpiecesInRender() for (int i=0; i<64; i++)\
    {\
        if (chessBoard[i]!=0)\
        {\
            drawImageColor(dstrect, i, textureBlackPawn, textureWhitePawn,1)\
            else drawImageColor(dstrect, i, textureBlackKnight, textureWhiteKnight,2)\
            else drawImageColor(dstrect, i, textureBlackBishop, textureWhiteBishop,3)\
            else drawImageColor(dstrect, i, textureBlackRook, textureWhiteRook,4)\
            else drawImageColor(dstrect, i, textureBlackQueen, textureWhiteQueen,6)\
            else drawImageColor(dstrect, i, textureBlackKing, textureWhiteKing,7)\
        }\
    }


//Prototypes
int giveCaseNumber(int eventX, int eventY);
void drawSquare(int squareNumber, SDL_Renderer* render);
void drawFullSquarePreviousMove(int squareNumber, SDL_Renderer* render);

#endif
