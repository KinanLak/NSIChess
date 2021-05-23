#include <stdio.h>
#include <stdlib.h>
#include "move.c"

#define addMoveIfPossible(team, chessBoard, file, departure, arrival) if (!(kingIsInCheck(team, chessBoard)))\
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


int kingIsInCheck(int team, unsigned int* chessBoard);
int Min(int a, int b);
int Max(int a, int b);
int kingPosition(int team, unsigned int* chessBoard);
void legalMovePiece(unsigned int* chessBoard, int position, int enPassant, int rock, FileMoveStructure* file);
void legalMovesPawn(unsigned int* chessBoard, int position, int enPassant, FileMoveStructure* file);
void legalMovesKnight(unsigned int* chessBoard, int position, FileMoveStructure* file);
void legalMovesRook(unsigned int* chessBoard, int position, FileMoveStructure* file);
void legalMovesBishop(unsigned int* chessBoard, int position, FileMoveStructure* file);
void legalMovesQueen(unsigned int* chessBoard, int position, FileMoveStructure* file);
void legalMovesKing(unsigned int* chessBoard, int position, int rock, FileMoveStructure* file);


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
        if (chessBoard[position-8]/8==0)
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
        if (position/8==3 && position-1==enPassant/64)
        {
            ifEnPassantMovePossibleMakeIt(position, position-9, 1, file, enPassant/64);
        }
        else if (position/8==3 && position+1==enPassant/64)
        {
            ifEnPassantMovePossibleMakeIt(position, position-7, 1, file, enPassant/64);
        }
    }
    else // C'est un pion noir, donc doit descendre (monter en terme de position)
    {
        if (chessBoard[position+8]/8==0)
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
        if (position/8==4 && position+1==enPassant%64)
        {
            ifEnPassantMovePossibleMakeIt(position, position+9, 0, file, enPassant%64);
        }
        else if (position/8==4 && position-1==enPassant%64)
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


//------------------------------------------------------------
//----------------------Rock must be add----------------------
//------------------------------------------------------------

void legalMovesKing(unsigned int* chessBoard, int position, int rock, FileMoveStructure* file)
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
        if (column>0 && chessBoard[position-1] && chessBoard[position+9]/8==0)
        {
            ifSimpleMovePossibleMakeIt(position, position-1, 1, file);
        }
        if (column<7 && chessBoard[position+1] && chessBoard[position+9]/8==0)
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
    }
}

void legalMovesRook(unsigned int* chessBoard, int position, FileMoveStructure* file)
{
    int column = position%8;
    int row = position/8;
    int pawnTeam=chessBoard[position]/8;
    int i=position-8;
    while (i>0)
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

    i=position+1;
    if (position%8 != 7)
    {
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

    i=position-1;
    if (position%8 != 0)
    {
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

void legalMovePiece(unsigned int* chessBoard, int position, int enPassant, int rock, FileMoveStructure* file)
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


int kingIsInCheck(int team, unsigned int* chessBoard)
{
    int position = kingPosition(team, chessBoard);
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