#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>

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












#define addMoveIfPossible(team, chessBoard, file, departure, arrival) if (!(caseIsInCheck(team, chessBoard)))\
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


int caseIsInCheck(int team, unsigned int* chessBoard);
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


int caseIsInCheck(int team, unsigned int* chessBoard)
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
#define BLACK 0, 0, 0, 255
#define NOTHING -1
#define transparentColor 239, 239, 239


//Definition of images path
#define BoardBgImageBMP "images/board/bg.bmp"
#define PointImageBMP "images/board/point.bmp"

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
    SDL_Texture* textureBackground = NULL;
    SDL_Texture* texturePoint = NULL;    

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
    SDL_Surface* imagePoint = NULL;

#define drawImageColor(dstrect, i, textureBlack, textureWhite, nb) if (chessBoard[i]%8==nb)\
            {\
                if (chessBoard[i]/8==1)\
                {\
                    DrawImage(dstrect, &dstrect, i, textureWhite)\
                }\
                else\
                {\
                    DrawImage(dstrect, &dstrect, i, textureBlack)\
                }\
            }
#define initAllImages() ALLImageINIT(imageBackground, textureBackground, BoardBgImageBMP, render)\
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

/*#define movePosssibles() FileMoveStructure* file = initialise(); \
                            legalMovePiece(chessBoard, change, 0, 0, file);\
                            MoveStructure *actualMove= file->firstMove;\
                            while (actualMove!=NULL)\
                            {\
                                drawSquare(actualMove->arrivalCase, render);\
                                actualMove = actualMove->nextMove;\
                            };*/


//Prototypes
int giveCaseNumber(int eventX, int eventY);
void drawSquare(int squareNumber, SDL_Renderer* render);
void drawFullSquarePreviousMove(int squareNumber, SDL_Renderer* render);











int isCheckMate(unsigned int* chessBoard, int team)
{
    FileMoveStructure* file = NULL;
    for (int i=0; i<64; i++)
    {
        if (chessBoard[i]!=0)
        {
            if (chessBoard[i]/8==team)
            {
                file = initialise();
                legalMovePiece(chessBoard, i, 0, 0, file);
                MoveStructure *actualMove= file->firstMove;
                while (actualMove!=NULL)
                {
                    return 0;
                    actualMove = actualMove->nextMove;
                };
            }
        }
    }
    return 1;
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


int main(int argc, char* argv[])
{
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "10");
    SDL_Window* window = NULL;
    SDL_Renderer* render = NULL;
    initAllSurfaces()
    initAllTextures() 

    //Initialisation of the SDL mode(s) that we will use
    SDL_Init(SDL_INIT_VIDEO);
    CreateRenderInNewWindow(window, render)


    //Create all images
    initAllImages()


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
    int whiteToPlay = 1;

    SDL_RenderCopy(render, textureBackground, NULL, NULL);
    displayAllpiecesInRender()
    SDL_RenderPresent(render);


    SDL_Event event;
    FileMoveStructure* file = NULL;
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
                    int caseNumber = giveCaseNumber(event.button.x, event.button.y);
                    if (change==NOTHING && chessBoard[caseNumber]!=0 && chessBoard[caseNumber]/8!=whiteToPlay)
                    {

                    }
                    else if (change ==NOTHING || caseNumber==change)
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
                            drawSquare(change, render);
                            file = initialise(); 
                            legalMovePiece(chessBoard, change, 0, 0, file);
                            MoveStructure *actualMove= file->firstMove;
                            displayAllpiecesInRender()
                            while (actualMove!=NULL)
                            {
                                DrawImage(dstrect, &dstrect, actualMove->arrivalCase, texturePoint)
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
                            drawSquare(change, render);
                            file = initialise(); \
                            legalMovePiece(chessBoard, change, 0, 0, file);
                            MoveStructure *actualMove= file->firstMove;
                            while (actualMove!=NULL)
                            {
                                DrawImage(dstrect, &dstrect, actualMove->arrivalCase, texturePoint)
                                actualMove = actualMove->nextMove;
                            };
                            SDL_RenderPresent(render);
                        }
                        else if (isMovePossible(caseNumber, file))
                        {
                            //Change the moves in the previousMove array
                            previousMove[0] = change;
                            previousMove[1] = caseNumber;
                            
                            //Change the pieces of position
                            chessBoard[caseNumber] = chessBoard[change];
                            chessBoard[change] = 0;

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
                            if (whiteToPlay)
                            {
                                whiteToPlay=0;
                            }
                            else
                            {
                                whiteToPlay=1;
                            }
                            if (isCheckMate(chessBoard, whiteToPlay)==1)
                            {
                                continuer=0;
                            }

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
        }
        SDL_Delay(5);
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
