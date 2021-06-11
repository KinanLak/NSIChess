#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <SDL2/SDL_ttf.h>
#include <winsock2.h>
//#pragma comment(lib, "ws2_32.lib")
#include "sqlite3.h"

//Creation of the structure for each move
typedef struct MoveStructure MoveStructure;
struct MoveStructure
{
    int departureCase;
    int arrivalCase;
    MoveStructure *nextMove;
};

//Creation of the structure wich store the first move
typedef struct FileMoveStructure FileMoveStructure;
struct FileMoveStructure
{
    MoveStructure *firstMove;
};

//Creation of the prototypes
FileMoveStructure *initialise(void);
void addMoveFile(FileMoveStructure *file, int newDepartureCase, int newArrivalCase);
void storeAllMovesSQL(FileMoveStructure *file, int gameId);


FileMoveStructure *initialise()
{
    FileMoveStructure *file = malloc(sizeof(*file));
    file->firstMove = NULL;

    return file;
}


void addMoveFile(FileMoveStructure *file, int newDepartureCase, int newArrivalCase)
{
    MoveStructure *new = malloc(sizeof(*new));
    if (file == NULL || new == NULL)
    {
        exit(EXIT_FAILURE);
    }

    new->departureCase = newDepartureCase;
    new->arrivalCase = newArrivalCase;
    new->nextMove = NULL;

    if (file->firstMove != NULL)
    {
        MoveStructure *MoveStructureActuel = file->firstMove;
        while (MoveStructureActuel->nextMove != NULL)
        {
            MoveStructureActuel = MoveStructureActuel->nextMove;
        }
        MoveStructureActuel->nextMove = new;
    }
    else
    {
        file->firstMove = new;
    }
}


void storeAllMovesSQL(FileMoveStructure *file, int gameId)
{
    if (file == NULL)
    {
        exit(EXIT_FAILURE);
    }

    MoveStructure *MoveStructure = file->firstMove;

    //récuperer le plus grand moveId qui existe pour le moment
    // et le rentrer dans la variable en dessous
    int moveId =12;

    while (MoveStructure != NULL)
    {
        moveId++;
        //Manque la requête qui ajoute le mouvement
        // INSERT INTO table_name
        // VALUES (moveId, MoveStructure->departureCase, MoveStructure->arrivalCase, gameId);
        MoveStructure = MoveStructure->nextMove;
        //We have to free -> elementDepile (check how to do that)
    }

}

//Faire une fonction pour vider tous les élements de la pile









int caseIsInCheck(int team, unsigned int* chessBoard, int position);
int Min(int a, int b);
int Max(int a, int b);
int kingPosition(int team, unsigned int* chessBoard);
void legalMovePiece(unsigned int* chessBoard, int position, int enPassant, int* rock, FileMoveStructure* file);
void legalMovesPawn(unsigned int* chessBoard, int position, int enPassant, FileMoveStructure* file);
void legalMovesKnight(unsigned int* chessBoard, int position, FileMoveStructure* file);
void legalMovesRook(unsigned int* chessBoard, int position, FileMoveStructure* file);
void legalMovesBishop(unsigned int* chessBoard, int position, FileMoveStructure* file);
void legalMovesQueen(unsigned int* chessBoard, int position, FileMoveStructure* file);
void legalMovesKing(unsigned int* chessBoard, int position, int* rock, FileMoveStructure* file);



#define addMoveIfPossible(team, chessBoard, file, departure, arrival) if (!(caseIsInCheck(team, chessBoard, kingPosition(team, chessBoard))))\
                                                                        {\
                                                                            addMoveFile(file, departure, arrival);\
                                                                        }

#define ifSimpleMovePossibleMakeIt(departure, arrival, team, file) unsigned int save = chessBoard[arrival];\
                                                chessBoard[arrival]=chessBoard[departure];\
                                                chessBoard[departure]=0;\
                                                addMoveIfPossible(team, chessBoard, file, departure, arrival)\
                                                chessBoard[departure]=chessBoard[arrival];\
                                                chessBoard[arrival]=save;

#define ifEnPassantMovePossibleMakeIt(departure, arrival, team, file, enPassant) unsigned int save = chessBoard[enPassant];\
                                                chessBoard[arrival]=chessBoard[departure];\
                                                chessBoard[departure]=0;\
                                                addMoveIfPossible(team, chessBoard, file, departure, arrival)\
                                                chessBoard[departure]=chessBoard[arrival];\
                                                chessBoard[arrival]=0;\
                                                chessBoard[enPassant]=save


int Min(int a, int b)
{
    if (a>b)
    {
        return b;
    }
    else
    {
        return a;
    }
}

int Max(int a, int b)
{
    if (a<b)
    {
        return b;
    }
    else
    {
        return a;
    }
}

int kingPosition(int team, unsigned int* chessBoard)
{
    if (team)
    {
        int i;
        for (i=0; i<64; i++)
        {
            if (chessBoard[i]==15)
            {
                return i;
            }
        }
        printf("Erreur, dans la recherche de la position du roi");
        return -1;
    }
    else
    {
       int i;
        for (i=63; i>-1; i--)
        {
            if (chessBoard[i]==7)
            {
                return i;
            }
        }
        printf("Erreur, dans la recherche de la position du roi");
        return -1;
    }
}

void legalMovesPawn(unsigned int* chessBoard, int position, int enPassant, FileMoveStructure* file)
{
    if (chessBoard[position]/8==1) // C'est un pion blanc, donc doit monter (baisser en terme de position)
    {
        if (chessBoard[position-8]==0)
        {
            ifSimpleMovePossibleMakeIt(position, position-8, 1, file);
            if (position/8==6 && chessBoard[position-16]==0)
            {
                ifSimpleMovePossibleMakeIt(position, position-16, 1, file);
            }
        }
        if (position%8>0 && chessBoard[position-9]!=0 && chessBoard[position-9]/8==0)
        {
            ifSimpleMovePossibleMakeIt(position, position-9, 1, file);
        }
        if (position%8<7 && chessBoard[position-7]!=0 && chessBoard[position-7]/8==0)
        {
            ifSimpleMovePossibleMakeIt(position, position-7, 1, file);
        }
        if (position/8==3 && position-1==enPassant)
        {
            ifEnPassantMovePossibleMakeIt(position, position-9, 1, file, enPassant/64);
        }
        else if (position/8==3 && position+1==enPassant)
        {
            ifEnPassantMovePossibleMakeIt(position, position-7, 1, file, enPassant/64);
        }
    }
    else // C'est un pion noir, donc doit descendre (monter en terme de position)
    {
        if (chessBoard[position+8]==0)
        {
            ifSimpleMovePossibleMakeIt(position, position+8, 0, file);
            if (position/8==1 && chessBoard[position+16]==0)
            {
                ifSimpleMovePossibleMakeIt(position, position+16, 0, file);
            }
        }
        if (position%8>0 && chessBoard[position+7]/8==1)
        {
            ifSimpleMovePossibleMakeIt(position, position+7, 0, file);
        }
        if (position%8<7 && chessBoard[position+9]/8==1)
        {
            ifSimpleMovePossibleMakeIt(position, position+9, 0, file);
        }
        if (position/8==4 && position+1==enPassant)
        {
            ifEnPassantMovePossibleMakeIt(position, position+9, 0, file, enPassant%64);
        }
        else if (position/8==4 && position-1==enPassant)
        {
            ifEnPassantMovePossibleMakeIt(position, position+7, 0, file, enPassant%64);
        }
    }
}




void legalMovesKnight(unsigned int* chessBoard, int position, FileMoveStructure* file)
{
    int column = position%8;
    int row = position/8;
    if (chessBoard[position]/8==1)
    {
        if (row>0)
        {
            if (column>1 && chessBoard[position-10]/8==0)
            {
                ifSimpleMovePossibleMakeIt(position, position-10, 1, file);
            }
            if (column<6 && chessBoard[position-6]/8==0)
            {
                ifSimpleMovePossibleMakeIt(position, position-6, 1, file);
            }
            if (row>1)
            {
                if (column>0 && chessBoard[position-17]/8==0)
                {
                    ifSimpleMovePossibleMakeIt(position, position-17, 1, file);
                }
                if (column<7 && chessBoard[position-15]/8==0)
                {
                    ifSimpleMovePossibleMakeIt(position, position-15, 1, file);
                }
            }
        }
        if (row<7)
        {
            if (column>1 && chessBoard[position+6]/8==0)
            {
                ifSimpleMovePossibleMakeIt(position, position+6, 1, file);
            }
            if (column<6 && chessBoard[position+10]/8==0)
            {
                ifSimpleMovePossibleMakeIt(position, position+10, 1, file);
            }
            if (row<6)
            {
                if (column>0 && chessBoard[position+15]/8==0)
                {
                    ifSimpleMovePossibleMakeIt(position, position+15, 1, file);
                }
                if (column<7 && chessBoard[position+17]/8==0)
                {
                    ifSimpleMovePossibleMakeIt(position, position+17, 1, file);
                }
            }
        }
    }
    else
    {
        if (row>0)
        {
            if (column>1 && (chessBoard[position-10]==0 || chessBoard[position-10]/8==1))
            {
                ifSimpleMovePossibleMakeIt(position, position-10, 0, file);
            }
            if (column<6 && (chessBoard[position-6]==0 || chessBoard[position-6]/8==1))
            {
                ifSimpleMovePossibleMakeIt(position, position-6, 0, file);
            }
            if (row>1)
            {
                if (column>0 && (chessBoard[position-17]==0 || chessBoard[position-17]/8==1))
                {
                    ifSimpleMovePossibleMakeIt(position, position-17, 0, file);
                }
                if (column<7 && (chessBoard[position-15]==0 || chessBoard[position-15]/8==1))
                {
                    ifSimpleMovePossibleMakeIt(position, position-15, 0, file);
                }
            }
        }
        if (row<7)
        {
            if (column>1 && (chessBoard[position+6]==0 || chessBoard[position+6]/8==1))
            {
                ifSimpleMovePossibleMakeIt(position, position+6, 0, file);
            }
            if (column<6 && (chessBoard[position+10]==0 || chessBoard[position+10]/8==1))
            {
                ifSimpleMovePossibleMakeIt(position, position+10, 0, file);
            }
            if (row<6)
            {
                if (column>0 && (chessBoard[position+15]==0 || chessBoard[position+15]/8==1))
                {
                    ifSimpleMovePossibleMakeIt(position, position+15, 0, file);
                }
                if (column<7 && (chessBoard[position+17]==0 || chessBoard[position+17]/8==1))
                {
                    ifSimpleMovePossibleMakeIt(position, position+17, 0, file);
                }
            }
        }

    }
}


//0/1------------------------------------------------------0/2
//0/4------------------------------------------------------0/8

void legalMovesKing(unsigned int* chessBoard, int position, int* rock, FileMoveStructure* file)
{
    int column = position%8;
    int row = position/8;
    if (chessBoard[position]/8==1)
    {
        if (row<7)
        {
            if (column>0 && chessBoard[position+7]/8==0)
            {
                ifSimpleMovePossibleMakeIt(position, position+7, 1, file);
            }
            if (chessBoard[position+8]/8==0)
            {
                ifSimpleMovePossibleMakeIt(position, position+8, 1, file);
            }
            if (column<7 && chessBoard[position+9]/8==0)
            {
                ifSimpleMovePossibleMakeIt(position, position+9, 1, file);
            }
        }
        else if (position==60)
        {
            if (((*rock)/4)%2==1)
            {
                if (chessBoard[56]==12)
                {
                    if (chessBoard[57]==0 && chessBoard[58]==0 && chessBoard[59]==0)
                    {
                        if ((!(caseIsInCheck(1, chessBoard, 58))) && (!(caseIsInCheck(1, chessBoard, 59))) && (!(caseIsInCheck(1, chessBoard, 60))))
                        {
                            addMoveIfPossible(1, chessBoard, file, 60,58)
                        }
                    }
                }
                else
                {
                    *rock -= 4;
                }
            }
            if (((*rock)/8)%2==1)
            {
                if (chessBoard[63]==12)
                {
                    if (chessBoard[61]==0 && chessBoard[62]==0)
                    {
                        if ((!(caseIsInCheck(1, chessBoard, 60))) && (!(caseIsInCheck(1, chessBoard, 61))) && (!(caseIsInCheck(1, chessBoard, 62))))
                        {
                            addMoveIfPossible(1, chessBoard, file, 60, 62)
                        }
                    }
                }
                else
                {
                    *rock -= 8;
                }
            }
        }
        else
        {
            if (((*rock)/4)%2==1)
            {
                *rock -= 4;
            }
            if (((*rock)/8)%2==1)
            {
                *rock -= 8;
            }
        }
        if (column>0 && chessBoard[position-1]/8==0)
        {
            ifSimpleMovePossibleMakeIt(position, position-1, 1, file);
        }
        if (column<7 && chessBoard[position+1]/8==0)
        {
            ifSimpleMovePossibleMakeIt(position, position+1, 1, file);
        }
        if (row>0)
        {
            if (column>0 && chessBoard[position-9]/8==0)
            {
                ifSimpleMovePossibleMakeIt(position, position-9, 1, file);
            }
            if (chessBoard[position-8]/8==0)
            {
                ifSimpleMovePossibleMakeIt(position, position-8, 1, file);
            }
            if (column<7 && chessBoard[position-7]/8==0)
            {
                ifSimpleMovePossibleMakeIt(position, position-7, 1, file);
            }
        }
    }
    else
    {
        if (row<7)
        {
            if (column>0 && (chessBoard[position+7]==0 || chessBoard[position+7]/8==1))
            {
                ifSimpleMovePossibleMakeIt(position, position+7, 0, file);
            }
            if ((chessBoard[position+8]==0 || chessBoard[position+8]/8==1))
            {
                ifSimpleMovePossibleMakeIt(position, position+8, 0, file);
            }
            if (column<7 && (chessBoard[position+9]==0 || chessBoard[position+9]/8==1))
            {
                ifSimpleMovePossibleMakeIt(position, position+9, 0, file);
            }
        }
        if (column>0 && (chessBoard[position-1]==0 || chessBoard[position-1]/8==1))
        {
            ifSimpleMovePossibleMakeIt(position, position-1, 0, file);
        }
        if (column<7 && (chessBoard[position+1]==0 || chessBoard[position+1]/8==1))
        {
            ifSimpleMovePossibleMakeIt(position, position+1, 0, file);
        }
        if (row>0)
        {
            if (column>0 && (chessBoard[position-9]==0 || chessBoard[position-9]/8==1))
            {
                ifSimpleMovePossibleMakeIt(position, position-9, 0, file);
            }
            if ((chessBoard[position-8]==0 || chessBoard[position-8]/8==1))
            {
                ifSimpleMovePossibleMakeIt(position, position-8, 0, file);
            }
            if (column<7 && (chessBoard[position-7]==0 || chessBoard[position-7]/8==1))
            {
                ifSimpleMovePossibleMakeIt(position, position-7, 0, file);
            }
        }
        else if (position==4)
        {
            if ((*rock)%2==1)
            {
                if (chessBoard[0]==4)
                {
                    if (chessBoard[1]==0 && chessBoard[2]==0 && chessBoard[3]==0)
                        if ((!(caseIsInCheck(0, chessBoard, 2))) && (!(caseIsInCheck(0, chessBoard, 3))) && (!(caseIsInCheck(0, chessBoard, 4))))
                        {
                            addMoveIfPossible(0, chessBoard, file, 4, 2)
                        }
                }
                else
                {
                    *rock -= 1;
                }
            }
            if (((*rock)/2)%2==1)
            {
                if (chessBoard[7]==4)
                {
                    if (chessBoard[5]==0 && chessBoard[6]==0)
                    {
                        if ((!(caseIsInCheck(0, chessBoard, 4))) && (!(caseIsInCheck(0, chessBoard, 5))) && (!(caseIsInCheck(0, chessBoard, 6))))
                        {
                            addMoveIfPossible(0, chessBoard, file, 4, 6)
                        }
                    }
                }
                else
                {
                    *rock -= 2;
                }
            }
        }
        else
        {
            if (((*rock))%2==1)
            {
                *rock -= 1;
            }
            if (((*rock)/2)%2==1)
            {
                *rock -= 2;
            }
        }
    }
}

void legalMovesRook(unsigned int* chessBoard, int position, FileMoveStructure* file)
{
    int column = position%8;
    int row = position/8;
    int pawnTeam=chessBoard[position]/8;
    int i=position-8;
    while (i>=0)
    {
        if (chessBoard[i]==0)
        {
            ifSimpleMovePossibleMakeIt(position, i, pawnTeam, file);
        }
        else if (chessBoard[i]/8 != pawnTeam)
        {
            ifSimpleMovePossibleMakeIt(position, i, pawnTeam, file);
            break;
        }
        else
        {
            break;
        }
        i-=8;
    }

    i=position+8;
    while (i<64)
    {
        if (chessBoard[i]==0)
        {
            ifSimpleMovePossibleMakeIt(position, i, pawnTeam, file);
        }
        else if (chessBoard[i]/8 != pawnTeam)
        {
            ifSimpleMovePossibleMakeIt(position, i, pawnTeam, file);
            break;
        }
        else
        {
            break;
        }
        i+=8;
    }

    if (position%8 != 7)
    {
        i=position+1;
        while (i%8 != 0)
        {
            if (chessBoard[i]==0)
            {
                ifSimpleMovePossibleMakeIt(position, i, pawnTeam, file);
            }
            else if (chessBoard[i]/8 != pawnTeam)
            {
                ifSimpleMovePossibleMakeIt(position, i, pawnTeam, file);
                break;
            }
            else
            {
                break;
            }
            i+=1;
        }
    }

    if (position%8 != 0)
    {
        i=position-1;
        while (i%8 != 7)
        {
            if (chessBoard[i]==0)
            {
                ifSimpleMovePossibleMakeIt(position, i, pawnTeam, file);
            }
            else if (chessBoard[i]/8 != pawnTeam)
            {
                ifSimpleMovePossibleMakeIt(position, i, pawnTeam, file);
                break;
            }
            else
            {
                break;
            }
            i-=1;
        }
    }
}


void legalMovesBishop(unsigned int* chessBoard, int position, FileMoveStructure* file)
{
    int i=position-7, pawnTeam=chessBoard[position]/8;
    if (position%8 != 7)
    {
        while (i>=0 && i%8!=0)
        {
            if (chessBoard[i]==0)
            {
                ifSimpleMovePossibleMakeIt(position, i, pawnTeam, file);
            }
            else if (chessBoard[i]/8 != pawnTeam)
            {
                ifSimpleMovePossibleMakeIt(position, i, pawnTeam, file);
                break;
            }
            else
            {
                break;
            }
            i-=7;
        }
    }

    i=position-9;
    if (position%8 != 0)
    {
        while (i>=0 && i%8!=7)
        {
            if (chessBoard[i]==0)
            {
                ifSimpleMovePossibleMakeIt(position, i, pawnTeam, file);
            }
            else if (chessBoard[i]/8 != pawnTeam)
            {
                ifSimpleMovePossibleMakeIt(position, i, pawnTeam, file);
                break;
            }
            else
            {
                break;
            }
            i-=9;
        }
    }

    i=position+9;
    if (position%8 != 7)
    {
        while (i<64 && i%8!=0)
        {
            if (chessBoard[i]==0)
            {
                ifSimpleMovePossibleMakeIt(position, i, pawnTeam, file);
            }
            else if (chessBoard[i]/8 != pawnTeam)
            {
                ifSimpleMovePossibleMakeIt(position, i, pawnTeam, file);
                break;
            }
            else
            {
                break;
            }
            i+=9;
        }
    }
    i=position+7;
    if (position%8 != 0)
    {
        while (i<64 && i%8!=7)
        {
            if (chessBoard[i]==0)
            {
                ifSimpleMovePossibleMakeIt(position, i, pawnTeam, file);
            }
            else if (chessBoard[i]/8 != pawnTeam)
            {
                ifSimpleMovePossibleMakeIt(position, i, pawnTeam, file);
                break;
            }
            else
            {
                break;
            }
            i+=7;
        }
    }
}

void legalMovesQueen(unsigned int* chessBoard, int position, FileMoveStructure* file)
{
    legalMovesRook(chessBoard, position, file);
    legalMovesBishop(chessBoard, position, file);
}

void legalMovePiece(unsigned int* chessBoard, int position, int enPassant, int* rock, FileMoveStructure* file)
{
    if (chessBoard[position]%8==1)
    {
        legalMovesPawn(chessBoard, position, enPassant, file);
    }
    else if (chessBoard[position]%8==2)
    {
        legalMovesKnight(chessBoard, position, file);
    }
    else if (chessBoard[position]%8==3)
    {
        legalMovesBishop(chessBoard, position, file);
    }
    else if (chessBoard[position]%8==4)
    {
        legalMovesRook(chessBoard, position, file);
    }
    else if (chessBoard[position]%8==6)
    {
        legalMovesQueen(chessBoard, position, file);
    }
    else if (chessBoard[position]%8==7)
    {
        legalMovesKing(chessBoard, position, rock, file);
    }
}

//Function that search in all the moves of the pawn

//Function that add a move to the list of moves

//Function that return the position of the king

//Function that return whether or not the king is in chess
void functionRunAllMoves(FileMoveStructure* file)
{
    MoveStructure *actualMove= file->firstMove;
    while (actualMove!=NULL)
    {
        printf("%d -> %d\n", actualMove->departureCase, actualMove->arrivalCase);
        actualMove = actualMove->nextMove;
    }
    
}


int caseIsInCheck(int team, unsigned int* chessBoard, int position)
{
    int cpt;
    if (!(team))
    {
        int pawnColumn=position%8;
        int pawnLine=position/8;
        if (position>7)
        {
            if ((pawnColumn)>1 && chessBoard[position-10]==10)//It's a white knight
            {
                return 1;
            }
            if ((pawnColumn)<6 && chessBoard[position-6]==10)//It's a white knight
            {
                return 1;
            }
            if (position>15)
            {
                if ((pawnColumn)>0 && chessBoard[position-17]==10)//It's a white knight
                {
                    return 1;
                }
                if ((pawnColumn)<7 && chessBoard[position-15]==10)//It's a white knight
                {
                    return 1;
                }
            }
        }
        if (position<56)
        {
            if ((pawnColumn)>1 && chessBoard[position+6]==10)//It's a white knight
            {
                return 1;
            }
            if ((pawnColumn)<6 && chessBoard[position+10]==10)//It's a white knight
            {
                return 1;
            }
            if (position<48)
            {
                if ((pawnColumn)<7 && chessBoard[position+17]==10)//It's a white knight
                {
                    return 1;
                }
                if ((pawnColumn)>0 && chessBoard[position+15]==10)//It's a white knight
                {
                    return 1;
                }
            }
        }
        //Check on the left line
        for (cpt=0; cpt<pawnColumn;cpt++)
        {
            if (chessBoard[position-cpt-1]==0)
            {
                //Nothing because it's an empty case
            }
            else if (chessBoard[position-cpt-1]/8==0)
            {
                break;
            }
            else if (chessBoard[position-cpt-1]==12 || chessBoard[position-cpt-1]==14)
            {
                return 1;
            }
            else
            {
                break;
            }
        }
        
        //Check on the right line
        
        for (cpt=0; cpt<7-pawnColumn; cpt++)
        {
            if (chessBoard[position+cpt+1]==0)
            {
                //Nothing because it's an empty case
            }
            else if (chessBoard[position+cpt+1]/8==0)
            {
                break;
            }
            else if (chessBoard[position+cpt+1]==12 || chessBoard[position+cpt+1]==14)
            {
                return 1;
            }
            else
            {
                break;
            }
        }
        //Check on the up line
        
        for (cpt=0; cpt<pawnLine;cpt++)
        {
            if (chessBoard[position-((cpt+1)*8)]==0)
            {
                //Nothing because it's an empty case
            }
            else if (chessBoard[position-((cpt+1)*8)]/8==0)
            {
                break;
            }
            else if (chessBoard[position-((cpt+1)*8)]==12 || chessBoard[position-((cpt+1)*8)]==14)
            {
                return 1;
            }
            else
            {
                break;
            }
        }
        //Check on the low line
        
        for (cpt=0; cpt<7-pawnLine;cpt++)
        {
            if (chessBoard[position+((cpt+1)*8)]==0)
            {
                //Nothing because it's an empty case
            }
            else if (chessBoard[position+((cpt+1)*8)]/8==0)
            {
                break;
            }
            else if (chessBoard[position+((cpt+1)*8)]==12 || chessBoard[position+((cpt+1)*8)]==14)
            {
                return 1;
            }
            else
            {
                break;
            }
        }
        //Check on the top left diagonal
        int topLeft=Min(pawnLine, pawnColumn);
        if (topLeft>0)
        {
            if (chessBoard[position-9]==15) //It's a white king
            {
                return 1;
            }
            for (cpt=0; cpt<topLeft;cpt++)
            {
                if (chessBoard[position-((cpt+1)*9)]==0)
                {
                    //Nothing because it's an empty case
                }
                else if (chessBoard[position-((cpt+1)*9)]/8==0)
                {
                    break;
                }
                else if (chessBoard[position-((cpt+1)*9)]==11 || chessBoard[position-((cpt+1)*9)]==14)
                {
                    return 1;
                }
                else
                {
                    break;
                }
            }
        }
        //Check on the top right diagonal
        int topRight=Min(pawnLine, 7-pawnColumn);
        if (topRight>0)
        {
            if (chessBoard[position-7]==15) //It's a white king
            {
                return 1;
            }
            for (cpt=0; cpt<topRight;cpt++)
            {
                if (chessBoard[position-((cpt+1)*7)]==0)
                {
                    //Nothing because it's an empty case
                }
                else if (chessBoard[position-((cpt+1)*7)]/8==0)
                {
                    break;
                }
                else if (chessBoard[position-((cpt+1)*7)]==11 || chessBoard[position-((cpt+1)*7)]==14)
                {
                    return 1;
                }
                else
                {
                    break;
                }
            }
        }
        //Check on the bottom left diagonal
        int bottomLeft=Min(7-pawnLine, pawnColumn);
        if (bottomLeft>0)
        {
            if (chessBoard[position+7]==15 ||chessBoard[position+7]==9) //It's a white pawn
            {
                return 1;
            }
            
            for (cpt=0; cpt<bottomLeft;cpt++)
            {
                if (chessBoard[position+((cpt+1)*7)]==0)
                {
                    //Nothing because it's an empty case
                }
                else if (chessBoard[position+((cpt+1)*7)]/8==0)
                {
                    break;
                }
                else if (chessBoard[position+((cpt+1)*7)]==11 || chessBoard[position+((cpt+1)*7)]==14)
                {
                    return 1;
                }
                else
                {
                    break;
                }
            }
        }
        //Check on the bottom right diagonal
        int bottomRight=Min(7-pawnLine, 7-pawnColumn);
        if (bottomRight>0)
        {
            if (chessBoard[position+9]==15 || chessBoard[position+9]==9) //It's a white pawn
            {
                return 1;
            }
            
            for (cpt=0; cpt<bottomRight;cpt++)
            {
                if (chessBoard[position+((cpt+1)*9)]==0)
                {
                    //Nothing because it's an empty case
                }
                else if (chessBoard[position+((cpt+1)*9)]/8==0)
                {
                    break;
                }
                else if (chessBoard[position+((cpt+1)*9)]==11 || chessBoard[position+((cpt+1)*9)]==14)
                {
                    return 1;
                }
                else
                {
                    break;
                }
            }
        }
        return 0;
    }
    else
    {
        int pawnColumn=position%8;
        int pawnLine=position/8;
        if (position>7)
        {
            if ((pawnColumn)>1 && chessBoard[position-10]==2)//It's a black knight
            {
                return 1;
            }
            if ((pawnColumn)<6 && chessBoard[position-6]==2)//It's a black knight
            {
                return 1;
            }
            if (position>15)
            {
                if ((pawnColumn)>0 && chessBoard[position-17]==2)//It's a black knight
                {
                    return 1;
                }
                if ((pawnColumn)<7 && chessBoard[position-15]==2)//It's a black knight
                {
                    return 1;
                }
            }
        }
        if (position<56)
        {
            if ((pawnColumn)>1 && chessBoard[position+6]==2)//It's a black knight
            {
                return 1;
            }
            if ((pawnColumn)<6 && chessBoard[position+10]==2)//It's a black knight
            {
                return 1;
            }
            if (position<48)
            {
                if ((pawnColumn)<7 && chessBoard[position+17]==2)//It's a black knight
                {
                    return 1;
                }
                if ((pawnColumn)>0 && chessBoard[position+15]==2)//It's a black knight
                {
                    return 1;
                }
            }
        }
        //Check on the left line
        
        for (cpt=0; cpt<pawnColumn;cpt++)
        {
            if (chessBoard[position-cpt-1]==0)
            {
                //Nothing because it's an empty case
            }
            else if (chessBoard[position-cpt-1]/8==1)
            {
                break;
            }
            else if (chessBoard[position-cpt-1]==4 || chessBoard[position-cpt-1]==6)
            {
                return 1;
            }
            else
            {
                break;
            }
        }
        //Check on the right line
        
        for (cpt=0; cpt<7-pawnColumn; cpt++)
        {
            if (chessBoard[position+cpt+1]==0)
            {
                //Nothing because it's an empty case
            }
            else if (chessBoard[position+cpt+1]/8==1)
            {
                break;
            }
            else if (chessBoard[position+cpt+1]==4 || chessBoard[position+cpt+1]==6)
            {
                return 1;
            }
            else
            {
                break;
            }
        }
        //Check on the up line
        
        for (cpt=0; cpt<pawnLine;cpt++)
        {
            if (chessBoard[position-((cpt+1)*8)]==0)
            {
                //Nothing because it's an empty case
            }
            else if (chessBoard[position-((cpt+1)*8)]/8==1)
            {
                break;
            }
            else if (chessBoard[position-((cpt+1)*8)]==4 || chessBoard[position-((cpt+1)*8)]==6)
            {
                return 1;
            }
            else
            {
                break;
            }
        }
        //Check on the low line
        
        
        for (cpt=0; cpt<7-pawnLine;cpt++)
        {
            if (chessBoard[position+((cpt+1)*8)]==0)
            {
                //Nothing because it's an empty case
            }
            else if (chessBoard[position+((cpt+1)*8)]/8==1)
            {
                break;
            }
            else if (chessBoard[position+((cpt+1)*8)]==4 || chessBoard[position+((cpt+1)*8)]==6)
            {
                return 1;
            }
            else
            {
                break;
            }
        }
        //Check on the top left diagonal
        int topLeft=Min(pawnLine, pawnColumn);
        if (topLeft>0)
        {
            if (chessBoard[position-9]==1 || chessBoard[position-9]==7) // It's a black pawn
            {
                return 1;
            }
            
            for (cpt=0; cpt<topLeft;cpt++)
            {
                if (chessBoard[position-((cpt+1)*9)]==0)
                {
                    //Nothing because it's an empty case
                }
                else if (chessBoard[position-((cpt+1)*9)]/8==1)
                {
                    break;
                }
                else if (chessBoard[position-((cpt+1)*9)]==3 || chessBoard[position-((cpt+1)*9)]==6)
                {
                    return 1;
                }
                else
                {
                    break;
                }
            }
        }
        //Check on the top right diagonal
        int topRight=Min(pawnLine, 7-pawnColumn);
        if (topRight>0)
        {
            if (chessBoard[position-7]==1 || chessBoard[position-7]==7) // It's a white pawn
            {
                return 1;
            }
            
            for (cpt=0; cpt<topRight;cpt++)
            {
                if (chessBoard[position-((cpt+1)*7)]==0)
                {
                    //Nothing because it's an empty case
                }
                else if (chessBoard[position-((cpt+1)*7)]/8==1)
                {
                    break;
                }
                else if (chessBoard[position-((cpt+1)*7)]==3 || chessBoard[position-((cpt+1)*7)]==6)
                {
                    return 1;
                }
                else
                {
                    break;
                }
            }
        }
        //Check on the bottom left diagonal
        int bottomLeft=Min(7-pawnLine, pawnColumn);
        if (bottomLeft>0)
        {
            if (chessBoard[position+7]==7)
            {
                return 1;
            }
            for (cpt=0; cpt<bottomLeft;cpt++)
            {
                if (chessBoard[position+((cpt+1)*7)]==0)
                {
                    //Nothing because it's an empty case
                }
                else if (chessBoard[position+((cpt+1)*7)]/8==1)
                {
                    break;
                }
                else if (chessBoard[position+((cpt+1)*7)]==3 || chessBoard[position+((cpt+1)*7)]==6)
                {
                    return 1;
                }
                else
                {
                    break;
                }
            }
        }
        //Check on the bottom right diagonal
        int bottomRight=Min(7-pawnLine, 7-pawnColumn);
        if (bottomRight>0)
        {
            if (chessBoard[position+9]==7)
            {
                return 1;
            }
            for (cpt=0; cpt<bottomRight;cpt++)
            {
                if (chessBoard[position+((cpt+1)*9)]==0)
                {
                    //Nothing because it's an empty case
                }
                else if (chessBoard[position+((cpt+1)*9)]/8==1)
                {
                    break;
                }
                else if (chessBoard[position+((cpt+1)*9)]==3 || chessBoard[position+((cpt+1)*9)]==6)
                {
                    return 1;
                }
                else
                {
                    break;
                }
            }
        }
        return 0;
    }
}


















//Definitions of variables
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define xMinBoard 488
#define xMaxBoard 1432
#define yMinBoard 69
#define yMaxBoard 1013
#define lenSquare 118
#define RED 255, 155, 155, 255
#define REDLight 255, 200, 200, 255
#define WHITE 255, 255, 255, 255
#define BLACK 0, 0, 0, 255
#define GREYINPUT 211, 209, 209, 255

#define NOTHING -1
#define transparentColor 239, 239, 239


//Definition of images path
#define BoardBgImageBMP "images/board/bg.bmp"
#define MenuBGImageBMP "images/menu.bmp"
#define ConnexionBGImageBMP "images/connexionBG.bmp"
#define OptionImageBMP "images/option.bmp"
#define PointImageBMP "images/board/point.bmp"
#define WhitePromoteBarBMP "images/board/whitePB.bmp"
#define BlackPromoteBarBMP "images/board/blackPB.bmp"
#define HoverButtonConnexionBMP "images/hoverButtonConnexion.bmp"
#define ButtonConnexionBMP "images/buttonConnexion.bmp"

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
    4,2,3,6,7,3,2,4, \
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
    SDL_Texture* textureBackground = NULL;\
    SDL_Texture* texturePoint = NULL;\
    SDL_Texture* textureWhitePromoteBar = NULL;\
    SDL_Texture* textureBlackPromoteBar = NULL;


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
    SDL_Surface* imageBackground = NULL;\
    SDL_Surface* imagePoint = NULL;\
    SDL_Surface* imageWhitePromoteBar = NULL;\
    SDL_Surface* imageBlackPromoteBar = NULL;

#define drawImageColor(dstrect, i, ii, textureBlack, textureWhite, nb) if (chessBoard[ii]%8==nb)\
            {\
                if (chessBoard[ii]/8==1)\
                {\
                    DrawImage(dstrect, &dstrect, i, textureWhite)\
                }\
                else\
                {\
                    DrawImage(dstrect, &dstrect, i, textureBlack)\
                }\
            }
#define initAllBoardImages() ALLImageINIT(imageBackground, textureBackground, BoardBgImageBMP, render)\
    ALLImageINIT(imagePoint, texturePoint, PointImageBMP, render)\
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
    ALLImageAndTransparencyINIT(imageWhiteKing ,textureWhiteKing, WhiteKingImageBMP, render)\
    ALLImageINIT(imageWhitePromoteBar, textureWhitePromoteBar, WhitePromoteBarBMP, render)\
    ALLImageINIT(imageBlackPromoteBar, textureBlackPromoteBar, BlackPromoteBarBMP, render)

#define showPreviousMovesNonInversed()  if (previousMove[0]!=NOTHING)\
                        {\
                            drawFullSquarePreviousMove(previousMove[0], render);\
                            drawFullSquarePreviousMove(previousMove[1], render);\
                        }

#define showPreviousMovesInversed()  if (previousMove[0]!=NOTHING)\
                        {\
                            drawFullSquarePreviousMove(63-previousMove[0], render);\
                            drawFullSquarePreviousMove(63-previousMove[1], render);\
                        }

#define showPreviousMoves() if (inverse==1)\
    {\
        showPreviousMovesInversed()\
    }\
    else\
    {\
        showPreviousMovesNonInversed()\
    }

#define displayAllpiecesInRenderNonInversed() for (int i=0; i<64; i++)\
    {\
        if (chessBoard[i]!=0)\
        {\
            drawImageColor(dstrect, i, i, textureBlackPawn, textureWhitePawn,1)\
            else drawImageColor(dstrect,i, i, textureBlackKnight, textureWhiteKnight,2)\
            else drawImageColor(dstrect, i,i, textureBlackBishop, textureWhiteBishop,3)\
            else drawImageColor(dstrect, i, i, textureBlackRook, textureWhiteRook,4)\
            else drawImageColor(dstrect, i, i, textureBlackQueen, textureWhiteQueen,6)\
            else drawImageColor(dstrect, i, i, textureBlackKing, textureWhiteKing,7)\
        }\
    }

#define displayAllpiecesInRenderInversed() for (int i=0; i<64; i++)\
    {\
        if (chessBoard[i]!=0)\
        {\
            int xb=63-i;\
            drawImageColor(dstrect, xb, i, textureBlackPawn, textureWhitePawn,1)\
            else drawImageColor(dstrect, xb, i,textureBlackKnight, textureWhiteKnight,2)\
            else drawImageColor(dstrect, xb, i,textureBlackBishop, textureWhiteBishop,3)\
            else drawImageColor(dstrect, xb, i,textureBlackRook, textureWhiteRook,4)\
            else drawImageColor(dstrect, xb, i,textureBlackQueen, textureWhiteQueen,6)\
            else drawImageColor(dstrect, xb, i,textureBlackKing, textureWhiteKing,7)\
        }\
    }

#define displayAllpiecesInRender() if (inverse==1)\
    {\
        displayAllpiecesInRenderInversed()\
    }\
    else\
    {\
        displayAllpiecesInRenderNonInversed()\
    }

#define freeAllBoardSurfaces() SDL_FreeSurface(imagePoint); \
    SDL_FreeSurface(imageBackground);\
    SDL_FreeSurface(imageBlackPawn);\
    SDL_FreeSurface(imageBlackRook);\
    SDL_FreeSurface(imageBlackKnight);\
    SDL_FreeSurface(imageBlackBishop);\
    SDL_FreeSurface(imageBlackQueen);\
    SDL_FreeSurface(imageBlackKing);\
    SDL_FreeSurface(imageWhitePawn);\
    SDL_FreeSurface(imageWhiteRook);\
    SDL_FreeSurface(imageWhiteKnight);\
    SDL_FreeSurface(imageWhiteBishop);\
    SDL_FreeSurface(imageWhiteQueen);\
    SDL_FreeSurface(imageWhiteKing);

#define destroyAllBoardStructures()     SDL_DestroyTexture(textureBackground);\
    SDL_DestroyTexture(texturePoint);\
    SDL_DestroyTexture(textureBlackPawn);\
    SDL_DestroyTexture(textureBlackRook);\
    SDL_DestroyTexture(textureBlackKnight);\
    SDL_DestroyTexture(textureBlackBishop);\
    SDL_DestroyTexture(textureBlackQueen);\
    SDL_DestroyTexture(textureBlackKing);\
    SDL_DestroyTexture(textureWhitePawn);\
    SDL_DestroyTexture(textureWhiteRook);\
    SDL_DestroyTexture(textureWhiteKnight);\
    SDL_DestroyTexture(textureWhiteBishop);\
    SDL_DestroyTexture(textureWhiteQueen);\
    SDL_DestroyTexture(textureWhiteKing);


#define showWhitePromoteBar(rect, rectP, i, texture) rect.x= (xMinBoard+(i%8)*lenSquare);\
            rect.y= (yMinBoard+(i/8)*lenSquare);\
            rect.h= (lenSquare*4)+59;\
            rect.w= lenSquare;\
            SDL_RenderCopy(render, texture, NULL, rectP);\
            DrawImage(rect, &rect, i, textureWhiteQueen)\
            DrawImage(rect, &rect, (i+8), textureWhiteKnight)\
            DrawImage(rect, &rect, (i+16), textureWhiteRook)\
            DrawImage(rect, &rect, (i+24), textureWhiteBishop)

#define showBlackPromoteBar(rect, rectP, i, texture) rect.x= (xMinBoard+(i%8)*lenSquare);\
            rect.y= (yMinBoard+((i/8)-3)*lenSquare)-59;\
            rect.h= (lenSquare*4)+59;\
            rect.w= lenSquare;\
            SDL_RenderCopy(render, texture, NULL, rectP);\
            DrawImage(rect, &rect, i, textureBlackQueen)\
            DrawImage(rect, &rect, (i-8), textureBlackKnight)\
            DrawImage(rect, &rect, (i-16), textureBlackRook)\
            DrawImage(rect, &rect, (i-24), textureBlackBishop)
            
#define playSound(sound) SDL_LoadWAV(sound, &wavSpec, &wavBuffer, &wavLength);\
                    int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);\
                    SDL_PauseAudioDevice(deviceId, 0);

//Prototypes
int giveCaseNumber(int eventX, int eventY);
void drawSquare(int squareNumber, SDL_Renderer* render, int inverse);
void drawFullSquarePreviousMove(int squareNumber, SDL_Renderer* render);











int isCheckMate(unsigned int* chessBoard, int team, int* rock, int enPassant)
{
    FileMoveStructure* file = NULL;
    for (int i=0; i<64; i++)
    {
        if (chessBoard[i]!=0)
        {
            if (chessBoard[i]/8==team)
            {
                file = initialise();
                legalMovePiece(chessBoard, i, enPassant, rock, file);
                MoveStructure *actualMove= file->firstMove;
                while (actualMove!=NULL)
                {
                    return 0;
                };
            }
        }
    }
    return 1;
}

void updateRock(unsigned int* chessBoard, int team, int* rock)
{
    int posKing=kingPosition(team, chessBoard);
    FileMoveStructure* file = initialise();
    legalMovesKing(chessBoard, posKing, rock, file);
    MoveStructure *actualMove= file->firstMove;
}

int isMovePossible(int moveCandidat, FileMoveStructure* file)
{
    if (file!=NULL)
    {
        MoveStructure *actualMove= file->firstMove;
        while (actualMove!=NULL)
        {
            if (actualMove->arrivalCase == moveCandidat)
            {
                return 1;
            }
            actualMove = actualMove->nextMove;
        };
    }
    return 0;
}

#define showTextes() changeValueConnexion1()\
                SDL_RenderCopy(render, textureConnexion2, NULL, &sdlRectConnexion2);\
                surfaceConnexion1 = TTF_RenderText_Solid(font, strConnexion1, color);\
                textureConnexion1 = SDL_CreateTextureFromSurface(render, surfaceConnexion1);\
                SDL_QueryTexture(textureConnexion1, NULL, NULL, &texWConnexion1, &texHConnexion1);\
                SDL_Rect sdlRectConnexion1 = {597, 529, texWConnexion1, texHConnexion1};\
                SDL_RenderCopy(render, textureConnexion1, NULL, &sdlRectConnexion1);\
                changeValueConnexion2()\
                SDL_RenderCopy(render, textureConnexion1, NULL, &sdlRectConnexion1);\
                surfaceConnexion2 = TTF_RenderText_Solid(fontPassword, strConnexion2Hidder, color);\
                textureConnexion2 = SDL_CreateTextureFromSurface(render, surfaceConnexion2);\
                SDL_QueryTexture(textureConnexion2, NULL, NULL, &texWConnexion2, &texHConnexion2);\
                SDL_Rect sdlRectConnexion2 = {594, 643, texWConnexion2, texHConnexion2};\
                SDL_RenderCopy(render, textureConnexion2, NULL, &sdlRectConnexion2);

#define drawSquareTimer(a,b) rect.x = a;\
    rect.y = a;\
    rect.w = b;\
    rect.h = b;\
    SDL_SetRenderDrawColor(render, WHITE);\
    SDL_RenderFillRect(render, &rect);

#define focusConnexion1() focus=1;\
    SDL_SetRenderDrawColor(render, WHITE);\
    SDL_Rect rect;\
    rect.x = 593;\
    rect.y = 529;\
    rect.w = 731;\
    rect.h = 40;\
    SDL_RenderDrawRect(render, &rect);\
    rect.x = 592;\
    rect.y = 528;\
    rect.w = 733;\
    rect.h = 42;\
    SDL_RenderDrawRect(render, &rect);

#define focusConnexion2() focus=2;\
    SDL_SetRenderDrawColor(render, WHITE);\
    SDL_Rect rect;\
    rect.x = 591;\
    rect.y = 644;\
    rect.w = 734;\
    rect.h = 40;\
    SDL_RenderDrawRect(render, &rect);\
    rect.x = 590;\
    rect.y = 643;\
    rect.w = 736;\
    rect.h = 42;\
    SDL_RenderDrawRect(render, &rect);

#define changeValueConnexion1() SDL_SetRenderDrawColor(render, GREYINPUT);\
    SDL_Rect rectFill1;\
    rectFill1.x = 594;\
    rectFill1.y = 530;\
    rectFill1.w = 729;\
    rectFill1.h = 38;\
    SDL_RenderFillRect(render, &rectFill1);

#define changeValueConnexion2() SDL_SetRenderDrawColor(render, GREYINPUT);\
    SDL_Rect rectFill2;\
    rectFill2.x = 592;\
    rectFill2.y = 645;\
    rectFill2.w = 732;\
    rectFill2.h = 38;\
    SDL_RenderFillRect(render, &rectFill2);



#define keyPressedConnexion(key, valueKey, valueKeyShift, valueKeyAlt, valueMax) case key:\
                        if (focus==1)\
                        {\
                            if (cptNumberOfValuesConnexion1<valueMax)\
                            {\
                                if ((rightAlt==1 || leftAlt==1) && (leftShift==1 || rightShift==1))\
                                {\
                                }\
                                else if (rightAlt==1 || leftAlt==1)\
                                {\
                                    if (valueKeyAlt!=-1)\
                                    {\
                                        strPointeurConnexion1[cptNumberOfValuesConnexion1]= valueKeyAlt;\
                                        cptNumberOfValuesConnexion1+=1;\
                                    }\
                                }\
                                else if (leftShift==1 || rightShift==1)\
                                {\
                                    if (valueKeyShift!=-1)\
                                    {\
                                        strPointeurConnexion1[cptNumberOfValuesConnexion1]= valueKeyShift;\
                                        cptNumberOfValuesConnexion1+=1;\
                                    }\
                                }\
                                else\
                                {\
                                    if (valueKey!=-1)\
                                    {\
                                        strPointeurConnexion1[cptNumberOfValuesConnexion1]= valueKey;\
                                        cptNumberOfValuesConnexion1+=1;\
                                    }\
                                }\
                            }\
                        }\
                        else if (focus==2)\
                        {\
                            if (cptNumberOfValuesConnexion2<valueMax)\
                            {\
                                if ((rightAlt==1 || leftAlt==1) && (leftShift==1 || rightShift==1))\
                                {\
                                }\
                                else if (rightAlt==1 || leftAlt==1)\
                                {\
                                    if (valueKeyAlt!=-1)\
                                    {\
                                        strPointeurConnexion2[cptNumberOfValuesConnexion2]= valueKeyAlt;\
                                        strPointeurConnexion2Hidder[cptNumberOfValuesConnexion2]=charhidder;\
                                        cptNumberOfValuesConnexion2+=1;\
                                    }\
                                }\
                                else if (leftShift==1 || rightShift==1)\
                                {\
                                    if (valueKeyShift!=-1)\
                                    {\
                                        strPointeurConnexion2[cptNumberOfValuesConnexion2]= valueKeyShift;\
                                        strPointeurConnexion2Hidder[cptNumberOfValuesConnexion2]=charhidder;\
                                        cptNumberOfValuesConnexion2+=1;\
                                    }\
                                }\
                                else\
                                {\
                                    if (valueKey!=-1)\
                                    {\
                                        strPointeurConnexion2[cptNumberOfValuesConnexion2]= valueKey;\
                                        strPointeurConnexion2Hidder[cptNumberOfValuesConnexion2]=charhidder;\
                                        cptNumberOfValuesConnexion2+=1;\
                                    }\
                                }\
                            }\
                        }\
                        break;

//---------------------------------------------------------------------------------
//------------------------------------All Pages------------------------------------
//---------------------------------------------------------------------------------

void loginPage(SDL_Window* window, SDL_Renderer* render)
{
    
    CreateRenderInNewWindow(window, render)

    SDL_Surface* imageConnexionBackground = NULL;
    SDL_Texture* textureConnexionBackground = NULL;
    ALLImageINIT(imageConnexionBackground, textureConnexionBackground, ConnexionBGImageBMP, render)

    SDL_Surface* imageButtonBackground = NULL;
    SDL_Texture* textureButtonBackground = NULL;
    ALLImageAndTransparencyINIT(imageButtonBackground, textureButtonBackground, ButtonConnexionBMP, render)

    SDL_Surface* imageHoverButtonBackground = NULL;
    SDL_Texture* textureHoverButtonBackground = NULL;
    ALLImageAndTransparencyINIT(imageHoverButtonBackground, textureHoverButtonBackground, HoverButtonConnexionBMP, render)
    SDL_Rect rectButton;
    rectButton.x= 797;
    rectButton.y= 740;
    rectButton.h= 81;
    rectButton.w= 323;

    TTF_Font * font = TTF_OpenFont("fonts/arial.ttf", 34);
    TTF_Font * fontPassword = TTF_OpenFont("fonts/arial.ttf", 62);
    SDL_Color color = { 0, 0, 0};
    SDL_Surface * surfaceConnexion1 = TTF_RenderText_Solid(font,"", color);
    SDL_Texture * textureConnexion1 = SDL_CreateTextureFromSurface(render, surfaceConnexion1);    
    int texWConnexion1 = 729;
    int texHConnexion1 = 38;
    SDL_QueryTexture(textureConnexion1, NULL, NULL, &texWConnexion1, &texHConnexion1);
    SDL_Rect sdlRectConnexion1 = {597, 529, texWConnexion1, texHConnexion1};
    SDL_RenderCopy(render, textureConnexion1, NULL, &sdlRectConnexion1);
    
    SDL_Surface * surfaceConnexion2 = TTF_RenderText_Solid(fontPassword,"", color);
    SDL_Texture * textureConnexion2 = SDL_CreateTextureFromSurface(render, surfaceConnexion2);    
    int texWConnexion2 = 729;
    int texHConnexion2 = 38;
    SDL_QueryTexture(textureConnexion2, NULL, NULL, &texWConnexion2, &texHConnexion2);
    SDL_Rect sdlRectConnexion2 = {594, 643, texWConnexion2, texHConnexion2};
    SDL_RenderCopy(render, textureConnexion2, NULL, &sdlRectConnexion2);
    
    char strConnexion1[38]="                                      ";
    char* strPointeurConnexion1 = strConnexion1;
    char strConnexion2[38]="                                      ";
    char* strPointeurConnexion2 = strConnexion2;
    char strConnexion2Hidder[38]="                                      ";
    char* strPointeurConnexion2Hidder = strConnexion2Hidder;
    int cptNumberOfValuesConnexion1 = 0;
    int cptNumberOfValuesConnexion2 = 0;
    int focus = 0;
    int leftShift=0;
    int rightShift=0;
    int leftAlt=0;
    int rightAlt=0;
    int charhidder=42;
    int limitChar=30;
    SDL_RenderCopy(render, textureConnexionBackground, NULL, NULL);
    SDL_RenderPresent(render);
    SDL_Delay(1000);

    SDL_Event event;
    int continuer = 1;
    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_MOUSEMOTION:
                if (event.motion.x >799 && event.motion.x <1122 && event.motion.y >739 && event.motion.y <822)
                {
                    SDL_RenderCopy(render, textureHoverButtonBackground, NULL, &rectButton);
                    SDL_RenderPresent(render);
                }
                else
                {
                    SDL_RenderCopy(render, textureButtonBackground, NULL, &rectButton);
                    SDL_RenderPresent(render);
                }
                break;
            case SDL_QUIT:
                continuer = 0;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.x >=1875 && event.button.y <=45)
                {
                    continuer=0;
                }
                if (event.button.x >593 && event.button.y > 529 && event.button.x <1324 && event.button.y < 569)
                {
                    SDL_RenderClear(render);
                    SDL_RenderCopy(render, textureConnexionBackground, NULL, NULL);
                    focusConnexion1()
                    showTextes()
                    SDL_RenderPresent(render);
                }
                else if (event.button.x >591 && event.button.y > 644 && event.button.x <1325 && event.button.y < 684)
                {
                    SDL_RenderClear(render);
                    SDL_RenderCopy(render, textureConnexionBackground, NULL, NULL);
                    focusConnexion2()
                    showTextes()
                    SDL_RenderPresent(render);
                }
                else if (event.button.x>797 && event.button.y>740 && event.button.x <1120 && event.button.y<821)
                {
                    //Send request
                }
                else
                {
                    focus=0;
                    SDL_RenderClear(render);
                    SDL_RenderCopy(render, textureConnexionBackground, NULL, NULL);
                    showTextes()
                    SDL_RenderPresent(render);
                }
                break;
//----------------------------------------------
//---Have to take into account the 'lock' key---
//----------------------------------------------
            case SDL_KEYDOWN: 
                switch( event.key.keysym.sym ){
                    case SDLK_LSHIFT:
                        leftShift=1;
                        break;
                    case SDLK_RSHIFT:
                        rightShift=1;
                        break;
                    case SDLK_RALT:
                        rightAlt=1;
                        break;
                    case SDLK_LALT:
                        leftAlt=1;
                        break;
                    case SDLK_BACKSPACE:
                        if (focus==1)
                        {
                            if (cptNumberOfValuesConnexion1>0)
                            {
                                cptNumberOfValuesConnexion1-=1;
                                strPointeurConnexion1[cptNumberOfValuesConnexion1]=32;
                            }
                        }
                        else if (focus==2)
                        {
                            if (cptNumberOfValuesConnexion1>0)
                            {
                                cptNumberOfValuesConnexion2-=1;
                                strPointeurConnexion2[cptNumberOfValuesConnexion2]=32;
                                strPointeurConnexion2Hidder[cptNumberOfValuesConnexion2]=32;
                            }
                        }
                        break;
                    case SDLK_TAB:
                        if (focus==0)
                        {
                            SDL_RenderClear(render);
                            SDL_RenderCopy(render, textureConnexionBackground, NULL, NULL);
                            changeValueConnexion1()
                            focusConnexion1()
                            int x,y;
                            SDL_GetGlobalMouseState(&x, &y);
                            if (x >799 && x <1122 && y >739 && y <822)
                            {
                                SDL_RenderCopy(render, textureHoverButtonBackground, NULL, &rectButton);
                            }
                            SDL_RenderPresent(render);
                        }
                        else if (focus==1)
                        {
                            SDL_RenderClear(render);
                            SDL_RenderCopy(render, textureConnexionBackground, NULL, NULL);
                            changeValueConnexion2()
                            focusConnexion2()
                            int x,y;
                            SDL_GetGlobalMouseState(&x, &y);
                            if (x >799 && x <1122 && y >739 && y <822)
                            {
                                SDL_RenderCopy(render, textureHoverButtonBackground, NULL, &rectButton);
                            }
                            SDL_RenderPresent(render);
                        }
                        break;
                    keyPressedConnexion(SDLK_a, 97, 65, -1, limitChar)
                    keyPressedConnexion(SDLK_b, 98, 66, -1, limitChar)
                    keyPressedConnexion(SDLK_c, 99, 67, -1, limitChar)
                    keyPressedConnexion(SDLK_d, 100, 68, -1, limitChar)
                    keyPressedConnexion(SDLK_e, 101, 69, -1, limitChar)
                    keyPressedConnexion(SDLK_f, 102, 70, -1, limitChar)
                    keyPressedConnexion(SDLK_g, 103, 71, -1, limitChar)
                    keyPressedConnexion(SDLK_h, 104, 72, -1, limitChar)
                    keyPressedConnexion(SDLK_i, 105, 73, -1, limitChar)
                    keyPressedConnexion(SDLK_j, 106, 74, -1, limitChar)
                    keyPressedConnexion(SDLK_k, 107, 75, -1, limitChar)
                    keyPressedConnexion(SDLK_l, 108, 76, -1, limitChar)
                    keyPressedConnexion(SDLK_m, 109, 77, -1, limitChar)
                    keyPressedConnexion(SDLK_n, 110, 78, -1, limitChar)
                    keyPressedConnexion(SDLK_o, 111, 79, -1, limitChar)
                    keyPressedConnexion(SDLK_p, 112, 80, -1, limitChar)
                    keyPressedConnexion(SDLK_q, 113, 81, -1, limitChar)
                    keyPressedConnexion(SDLK_r, 114, 82, -1, limitChar)
                    keyPressedConnexion(SDLK_s, 115, 83, -1, limitChar)
                    keyPressedConnexion(SDLK_t, 116, 84, -1, limitChar)
                    keyPressedConnexion(SDLK_u, 117, 85, -1, limitChar)
                    keyPressedConnexion(SDLK_v, 118, 86, -1, limitChar)
                    keyPressedConnexion(SDLK_w, 119, 87, -1, limitChar)
                    keyPressedConnexion(SDLK_x, 120, 88, -1, limitChar)
                    keyPressedConnexion(SDLK_y, 121, 89, -1, limitChar)
                    keyPressedConnexion(SDLK_z, 122, 90, -1, limitChar)
                    keyPressedConnexion(SDLK_1, 38, 49, -1, limitChar)
                    keyPressedConnexion(SDLK_2, 233, 50, -1, limitChar)
                    keyPressedConnexion(SDLK_3, -1, 51, 35, limitChar)
                    keyPressedConnexion(SDLK_4, -1, 52, -1, limitChar)
                    keyPressedConnexion(SDLK_5, -1, 53, -1, limitChar)
                    keyPressedConnexion(SDLK_6, 45, 54, -1, limitChar)
                    keyPressedConnexion(SDLK_7, 232, 55, -1, limitChar)
                    keyPressedConnexion(SDLK_8, 95, 56, -1, limitChar)
                    keyPressedConnexion(SDLK_9, 231, 57, -1, limitChar)
                    keyPressedConnexion(SDLK_0, 224, 48, 64, limitChar)
                    
                    keyPressedConnexion(SDLK_COMMA, -1, 63, -1, limitChar)
                    keyPressedConnexion(SDLK_EXCLAIM, 33, -1, -1, limitChar)
                    keyPressedConnexion(SDLK_SEMICOLON, -1, 46, -1, limitChar)
                    
                }
                showTextes()
                SDL_RenderPresent(render);
                
                break;
            case SDL_KEYUP:
                switch( event.key.keysym.sym ){
                    case SDLK_LSHIFT:
                        leftShift=0;
                        break;
                    case SDLK_RSHIFT:
                        rightShift=0;
                        break;
                    case SDLK_RALT:
                        rightAlt=0;
                        break;
                    case SDLK_LALT:
                        leftAlt=0;
                        break;
                }
        }
        SDL_Delay(5);
    }
    SDL_RenderClear(render);
    SDL_RenderPresent(render);
}


void menuPage(SDL_Window* window, SDL_Renderer* render)
{
    
    CreateRenderInNewWindow(window, render)


    SDL_Surface* imageBackgroundMenu = NULL;
    SDL_Texture* textureBackgroundMenu = NULL;
    ALLImageINIT(imageBackgroundMenu, textureBackgroundMenu, MenuBGImageBMP, render)

    SDL_Surface* imageOption = NULL;
    SDL_Texture* textureOption = NULL;
    ALLImageINIT(imageOption, textureOption, OptionImageBMP, render)

    SDL_RenderCopy(render, textureBackgroundMenu, NULL, NULL);
    SDL_RenderCopy(render, textureOption, NULL, NULL);
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
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.x >=1875 && event.button.y <=45)
                {
                    continuer=0;
                }
                break;
            case SDL_KEYDOWN: 
                break;
        }
        SDL_Delay(5);
    }
    SDL_RenderClear(render);
    SDL_RenderPresent(render);
}


void mainBoard(SDL_Window* window,SDL_Renderer* render)
{

    CreateRenderInNewWindow(window, render)

    initAllSurfaces()
    initAllTextures() 

    //Create all images
    initAllBoardImages()


    SDL_Rect dstrect;
    int previousMove[2]={NOTHING, NOTHING};

    //Initialisation of the audio
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;
    SDL_LoadWAV(MoveSound, &wavSpec, &wavBuffer, &wavLength);
    SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
    

    unsigned int chessBoard[64] = departPosition;

    int change = NOTHING;
    int enPassant = 0;
    int rock=15;
    int teamToPlay = 1;
    int noPromotion = NOTHING;

    //Initialisation of the timer
    int timerIsOn=1;
    int inverse=1;
    int add_time=2;


    TTF_Font * font = TTF_OpenFont("fonts/arial.ttf", 50);
    SDL_Color color = { 0, 0, 0};
    time_t endTime;
    time(&endTime);
    endTime += 100;
    
    int leftOverTimeWhite = 100;
    int leftOverTimeBlack = 100;
    char stringTimeToShowWhite[6]="1:40";
    char stringTimeToShowBlack[6]="1:40";
    SDL_Surface * surfaceTimerWhite = TTF_RenderText_Solid(font,stringTimeToShowWhite, color);
    SDL_Texture * textureTimerWhite = SDL_CreateTextureFromSurface(render, surfaceTimerWhite);
    SDL_Surface * surfaceTimerBlack = TTF_RenderText_Solid(font,stringTimeToShowBlack, color);
    SDL_Texture * textureTimerBlack = SDL_CreateTextureFromSurface(render, surfaceTimerBlack);
    int texWWhite = 200;
    int texHWhite = 100;
    SDL_QueryTexture(textureTimerWhite, NULL, NULL, &texWWhite, &texHWhite);
    SDL_Rect sdlRectTimerWhite = {300, 300, texWWhite, texHWhite};
    int texWBlack = 200;
    int texHBlack = 100;
    SDL_QueryTexture(textureTimerBlack, NULL, NULL, &texWBlack, &texHBlack);
    SDL_Rect sdlRectTimerBlack = {200, 200, texWBlack, texHBlack};
    SDL_RenderCopy(render, textureBackground, NULL, NULL);
    displayAllpiecesInRender()
    SDL_RenderPresent(render);


    SDL_Event event;
    FileMoveStructure* file = NULL;
    int continuer = 1;
    while (continuer)
    {
        SDL_PollEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.x >=1875 && event.button.y <=45)
                {
                    continuer=0;
                }
                if (event.button.x >= xMinBoard && event.button.x <= xMaxBoard && event.button.y <=yMaxBoard && event.button.y >= yMinBoard)
                {
                    int caseNumber = 0;
                    if (inverse)
                    {
                        caseNumber = 63-giveCaseNumber(event.button.x, event.button.y);
                    }
                    else
                    {
                        caseNumber = giveCaseNumber(event.button.x, event.button.y);
                    }

                    if (noPromotion!=NOTHING) // If the pawn is about to be promoted
                    {
                        if (teamToPlay)
                        {
                            if (caseNumber==noPromotion)
                            {
                                chessBoard[change]=0;
                                chessBoard[noPromotion]=14;
                            } 
                            else if (caseNumber==noPromotion+8)
                            {
                                chessBoard[change]=0;
                                chessBoard[noPromotion]=10;
                            }
                            else if (caseNumber==noPromotion+16)
                            {
                                chessBoard[change]=0;
                                chessBoard[noPromotion]=12;
                            }
                            else if (caseNumber==noPromotion+24)
                            {
                                chessBoard[change]=0;
                                chessBoard[noPromotion]=11;
                            }
                            else
                            {
                                change=NOTHING;
                                noPromotion=NOTHING;
                                SDL_RenderClear(render);
                                SDL_RenderCopy(render, textureBackground, NULL, NULL);
                                showPreviousMoves()
                                displayAllpiecesInRender()
                                SDL_RenderPresent(render);
                                continue;
                            }
                            previousMove[0] = change;
                            previousMove[1] = noPromotion;
                            noPromotion=NOTHING;
                            change=NOTHING;
                            teamToPlay=0;
                            if (isCheckMate(chessBoard, teamToPlay, &rock, enPassant)==1)
                            {
                                continuer=0;
                            }
                            enPassant=0;
                            updateRock(chessBoard, teamToPlay, &rock);
                            SDL_RenderClear(render);
                            SDL_RenderCopy(render, textureBackground, NULL, NULL);
                            showPreviousMoves()
                            displayAllpiecesInRender()
                            SDL_RenderPresent(render);
                        } else {
                            if (caseNumber==noPromotion)
                            {
                                chessBoard[change]=0;
                                chessBoard[noPromotion]=6;
                            } 
                            else if (caseNumber==noPromotion-8)
                            {
                                chessBoard[change]=0;
                                chessBoard[noPromotion]=2;
                            }
                            else if (caseNumber==noPromotion-16)
                            {
                                chessBoard[change]=0;
                                chessBoard[noPromotion]=4;
                            }
                            else if (caseNumber==noPromotion-24)
                            {
                                chessBoard[change]=0;
                                chessBoard[noPromotion]=3;
                            }
                            else
                            {
                                change=NOTHING;
                                noPromotion=NOTHING;
                                SDL_RenderClear(render);
                                SDL_RenderCopy(render, textureBackground, NULL, NULL);
                                showPreviousMoves()
                                displayAllpiecesInRender()
                                SDL_RenderPresent(render);
                                continue;
                            }
                            previousMove[0] = change;
                            previousMove[1] = noPromotion;
                            noPromotion=NOTHING;
                            change=NOTHING;
                            teamToPlay=1;
                            if (isCheckMate(chessBoard, teamToPlay, &rock, enPassant)==1)
                            {
                                continuer=0;
                            }
                            enPassant=0;
                            updateRock(chessBoard, teamToPlay, &rock);
                            SDL_RenderClear(render);
                            SDL_RenderCopy(render, textureBackground, NULL, NULL);
                            showPreviousMoves()
                            displayAllpiecesInRender()
                            SDL_RenderPresent(render);
                        }

                    }
                    else if (change==NOTHING && chessBoard[caseNumber]!=0 && chessBoard[caseNumber]/8!=teamToPlay)
                    {

                    }
                    else if (change==NOTHING || caseNumber==change)
                    {
                        if (caseNumber==change)
                        {
                            change=NOTHING;
                            SDL_RenderClear(render);
                            SDL_RenderCopy(render, textureBackground, NULL, NULL);
                            if (previousMove[0]!=NOTHING)
                            {
                                showPreviousMoves()
                            }
                            displayAllpiecesInRender()
                            SDL_RenderPresent(render);
                        }
                        else if (chessBoard[caseNumber]!=0)
                        {
                            
                            SDL_RenderClear(render);
                            SDL_RenderCopy(render, textureBackground, NULL, NULL);
                            showPreviousMoves()
                            change = caseNumber;
                            SDL_SetRenderDrawColor(render, BLACK);
                            drawSquare(change, render, inverse);
                            file = initialise(); 
                            legalMovePiece(chessBoard, change, enPassant,  &rock, file);
                            MoveStructure *actualMove= file->firstMove;
                            displayAllpiecesInRender()
                            while (actualMove!=NULL)
                            {
                                if (inverse==1)
                                {
                                    int a=63 - (actualMove->arrivalCase);
                                    DrawImage(dstrect, &dstrect, a, texturePoint)
                                }
                                else
                                {
                                    DrawImage(dstrect, &dstrect, actualMove->arrivalCase, texturePoint)
                                }
                                actualMove = actualMove->nextMove;
                            };
                            SDL_RenderPresent(render);
                            
                        }
                    }
                    else
                    {
                        if (chessBoard[caseNumber]!=0 && (chessBoard[caseNumber]/8 == chessBoard[change]/8))
                        {
                            SDL_RenderClear(render);
                            SDL_RenderCopy(render, textureBackground, NULL, NULL);
                            showPreviousMoves()
                            displayAllpiecesInRender()
                            change = caseNumber;
                            SDL_SetRenderDrawColor(render, BLACK);
                            drawSquare(change, render, inverse);
                            file = initialise();
                            legalMovePiece(chessBoard, change, enPassant, &rock, file);
                            MoveStructure *actualMove= file->firstMove;
                            while (actualMove!=NULL)
                            {

                                if (inverse==1)
                                {
                                    int a=63 - (actualMove->arrivalCase);
                                    DrawImage(dstrect, &dstrect, a, texturePoint)
                                }
                                else
                                {
                                    DrawImage(dstrect, &dstrect, actualMove->arrivalCase, texturePoint)
                                }
                                actualMove = actualMove->nextMove;
                            };
                            SDL_RenderPresent(render);
                        }
                        else if (isMovePossible(caseNumber, file))
                        {
                            
                            //Change the pieces of position
                            if ((chessBoard[change]==7 || chessBoard[change]==15) && (change-2==caseNumber || change+2==caseNumber))
                            {
                                if (change-2==caseNumber)
                                {
                                    chessBoard[change-1]=chessBoard[change-4];
                                    chessBoard[change-4]=0;
                                    chessBoard[caseNumber] = chessBoard[change];
                                    chessBoard[change] = 0;
                                }
                                else
                                {
                                    chessBoard[change+1]=chessBoard[change+3];
                                    chessBoard[change+3]=0;
                                    chessBoard[caseNumber] = chessBoard[change];
                                    chessBoard[change] = 0;
                                }
                                enPassant=0;
                            }
                            else if (chessBoard[change]==1 || chessBoard[change]==9)
                            {
                                if (change+16==caseNumber || change==caseNumber+16)
                                {
                                    chessBoard[caseNumber] = chessBoard[change];
                                    chessBoard[change] = 0;
                                    enPassant=caseNumber;
                                    playSound(MoveSound)
                                }
                                else if ((change-9==caseNumber || change+9==caseNumber || change-7==caseNumber || change+7==caseNumber ) && (chessBoard[caseNumber]==0))
                                {
                                    chessBoard[caseNumber] = chessBoard[change];
                                    chessBoard[change] = 0;
                                    chessBoard[enPassant]=0;
                                    playSound(CaptureSound)
                                }
                                else if (caseNumber/8==0)//White promotion
                                {
                                    noPromotion = caseNumber;
                                    showWhitePromoteBar(dstrect, &dstrect, caseNumber, textureWhitePromoteBar)
                                    SDL_RenderPresent(render);
                                    continue;
                                }
                                else if (caseNumber/8==7) //Black promotion
                                {
                                    noPromotion = caseNumber;
                                    showBlackPromoteBar(dstrect, &dstrect, caseNumber, textureBlackPromoteBar)
                                    SDL_RenderPresent(render);
                                    continue;
                                }
                                else
                                {
                                    if (chessBoard[caseNumber]!=0)
                                    {
                                        playSound(CaptureSound)
                                    }
                                    else
                                    {
                                        playSound(MoveSound)
                                    }
                                    chessBoard[caseNumber] = chessBoard[change];
                                    chessBoard[change] = 0;
                                    enPassant=0;
                                }
                            }
                            else
                            {
                                chessBoard[caseNumber] = chessBoard[change];
                                chessBoard[change] = 0;
                                playSound(MoveSound)
                                enPassant=0;
                            }
                            //Change the moves in the previousMove array
                            previousMove[0] = change;
                            previousMove[1] = caseNumber;
                            

                            //Clear the window
                            SDL_RenderClear(render);

                            //Show the background
                            SDL_RenderCopy(render, textureBackground, NULL, NULL);

                            //Show the previous move
                            showPreviousMoves()

                            //Show all the pieces
                            displayAllpiecesInRender()

                            SDL_RenderPresent(render);

                            //Reset change
                            change = NOTHING;
                            if (teamToPlay)
                            {
                                teamToPlay=0;
                                endTime=time(NULL)+leftOverTimeBlack+add_time;
                            }
                            else
                            {
                                teamToPlay=1;
                                endTime=time(NULL)+leftOverTimeWhite+add_time;
                            }
                            if (isCheckMate(chessBoard, teamToPlay, &rock, enPassant)==1)
                            {
                                continuer=0;
                            }
                            updateRock(chessBoard, teamToPlay, &rock);

                        }
                        else
                        {
                            //Clear the window
                            SDL_RenderClear(render);

                            //Show the background
                            SDL_RenderCopy(render, textureBackground, NULL, NULL);

                            //Show the previous move
                            showPreviousMoves()

                            //Show all the pieces
                            displayAllpiecesInRender()

                            SDL_RenderPresent(render);

                            change = NOTHING;
                        }
                    }

                }
                else
                {
                    //Reset change
                    change = NOTHING;
                }
                break;
        }
        if (timerIsOn)
        {
            if (teamToPlay)
            {
                leftOverTimeWhite = endTime-time(NULL);
                if (leftOverTimeWhite<=0)
                {
                    continuer = 0;
                }
                else
                {
                    itoa(leftOverTimeWhite/60, stringTimeToShowWhite, 10);
                    strcat(stringTimeToShowWhite, ":");
                    char stringTens[1];
                    itoa((leftOverTimeWhite%60)/10, stringTens, 10);
                    char stringUnity[2];
                    itoa((leftOverTimeWhite%60)%10, stringUnity, 10);
                    strcat(stringTimeToShowWhite, stringTens);
                    strcat(stringTimeToShowWhite, stringUnity);
                }
            }
            else
            {
                leftOverTimeBlack = endTime-time(NULL);
                if (leftOverTimeBlack<=0)
                {
                    continuer = 0;
                }
                else
                {
                    itoa(leftOverTimeBlack/60, stringTimeToShowBlack, 10);
                    strcat(stringTimeToShowBlack, ":");
                    char stringTens[1];
                    itoa((leftOverTimeBlack%60)/10, stringTens, 10);
                    char stringUnity[2];
                    itoa((leftOverTimeBlack%60)%10, stringUnity, 10);
                    strcat(stringTimeToShowBlack, stringTens);
                    strcat(stringTimeToShowBlack, stringUnity);
                }
            }
            SDL_Rect rect;
            drawSquareTimer(300, 100)
            surfaceTimerWhite = TTF_RenderText_Solid(font, stringTimeToShowWhite, color);
            textureTimerWhite = SDL_CreateTextureFromSurface(render, surfaceTimerWhite);
            SDL_RenderCopy(render, textureTimerWhite, NULL, &sdlRectTimerWhite);
            
            drawSquareTimer(200, 100)
            surfaceTimerBlack = TTF_RenderText_Solid(font, stringTimeToShowBlack, color);
            textureTimerBlack = SDL_CreateTextureFromSurface(render, surfaceTimerBlack);
            SDL_RenderCopy(render, textureTimerBlack, NULL, &sdlRectTimerBlack);
            SDL_RenderPresent(render);
            SDL_Delay(50);
        }
    }
    SDL_RenderClear(render);
    SDL_RenderPresent(render);

    destroyAllBoardStructures()
    freeAllBoardSurfaces()
}


void puzzleBoard(SDL_Window* window, SDL_Renderer* renderer)
{

}


int main(int argc, char* argv[])
{
    /*//Initialisation socket and shits
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2, 0), &WSAData);*/

    //Initialisation of the window
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "10");
    SDL_Window* window = NULL;
    SDL_Renderer* render = NULL;
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
    TTF_Init();
    
    loginPage(window, render);
    //Launch the mainBoard
    //mainBoard(window, render);
    //menuPage(window, render);

    
    //Destruction of the window
    TTF_Quit();
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);

    return 1;
}




int giveCaseNumber(int eventX, int eventY)
{
    return (eventX-xMinBoard)/lenSquare + (((eventY-yMinBoard)/lenSquare)*8);
}


void drawSquare(int squareNumber, SDL_Renderer* render, int inverse)
{
    if (inverse==1)
    {
        squareNumber = 63-squareNumber;
    }
    SDL_Rect rect;
    rect.x = (xMinBoard+(squareNumber%8)*lenSquare);
    rect.y = (yMinBoard+(squareNumber/8)*lenSquare);
    rect.w = lenSquare;
    rect.h = lenSquare;
    SDL_RenderDrawRect(render, &rect);
    rect.x = (xMinBoard + (squareNumber % 8) * lenSquare+1);
    rect.y = (yMinBoard + (squareNumber / 8) * lenSquare+1);
    rect.w = lenSquare-2;
    rect.h = lenSquare-2;
    SDL_RenderDrawRect(render, &rect);
    rect.x = (xMinBoard + (squareNumber % 8) * lenSquare+2);
    rect.y = (yMinBoard + (squareNumber / 8) * lenSquare)+2;
    rect.w = lenSquare-4;
    rect.h = lenSquare-4;
    SDL_RenderDrawRect(render, &rect);
    rect.x = (xMinBoard + (squareNumber % 8) * lenSquare+3);
    rect.y = (yMinBoard + (squareNumber / 8) * lenSquare+3);
    rect.w = lenSquare-6;
    rect.h = lenSquare-6;
    SDL_RenderDrawRect(render, &rect);
    rect.x = (xMinBoard + (squareNumber % 8) * lenSquare+4);
    rect.y = (yMinBoard + (squareNumber / 8) * lenSquare+4);
    rect.w = lenSquare-8;
    rect.h = lenSquare-8;
    SDL_RenderDrawRect(render, &rect);
}

void drawFullSquarePreviousMove(int squareNumber, SDL_Renderer* render)
{
    if (((squareNumber/8)+(squareNumber%8))%2==0)
    {
        SDL_SetRenderDrawColor(render, REDLight);
    }
    else
    {
        SDL_SetRenderDrawColor(render, RED);
    }
    SDL_Rect rect;
    rect.x = (xMinBoard+(squareNumber%8)*lenSquare);
    rect.y = (yMinBoard+(squareNumber/8)*lenSquare);
    rect.w = lenSquare;
    rect.h = lenSquare;
    SDL_RenderFillRect(render, &rect);
}
