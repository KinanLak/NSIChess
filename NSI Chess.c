#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <SDL2/SDL_ttf.h>
#include <mysql.h>
//#include <SDL2/SDL_image.h>
//#include <winsock2.h>


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
#define ConnexionBGImageBMP "images/connexion/connexionBG.bmp"
#define InscriptionBGImageBMP "images/inscription/inscriptionBG.bmp"
#define OptionsBGImageBMP "images/options/optionsBG.bmp"
#define PointImageBMP "images/board/point.bmp"
#define WhitePromoteBarBMP "images/board/whitePB.bmp"
#define BlackPromoteBarBMP "images/board/blackPB.bmp"
#define HoverButtonConnexionBMP "images/connexion/hoverButtonConnexion.bmp"
#define ButtonConnexionBMP "images/connexion/buttonConnexion.bmp"
#define HoverButtonInscriptionBMP "images/inscription/hoverButtonInscription.bmp"
#define ButtonInscriptionBMP "images/inscription/buttonInscription.bmp"
#define ModeSelectionBGImageBMP "images/main_menu/gameModeBG.bmp"
#define TempsPartieBGImageBMP "images/main_menu/tempsPartieBG.bmp"
#define MenuBGImageBMP "images/main_menu/menuBG.bmp"
#define WhiteBorderBGImageBMP "images/main_menu/whiteBorder.bmp"
#define HoverRetourTimeBMP "images/main_menu/hoverRetour.bmp"
#define RetourTimeBMP "images/main_menu/retour.bmp"
#define LancerTimeBMP "images/main_menu/lancer.bmp"
#define HoverLancerTimeBMP "images/main_menu/hoverLancer.bmp"
#define HoverPuzzleMainMenuBMP "images/main_menu/puzzleButtonHover.bmp"
#define PuzzleMainMenuBMP "images/main_menu/puzzleButton.bmp"
#define FriendListMainMenuBMP "images/main_menu/friendList.bmp"
#define FriendListHiderMainMenuBMP "images/main_menu/friendListHider.bmp"
#define JouerMainMenuBMP "images/main_menu/jouer.bmp"
#define HoverJouerMainMenuBMP "images/main_menu/hoverJouer.bmp"
#define AmisMainMenuBMP "images/main_menu/amis.bmp"
#define HoverAmisMainMenuBMP "images/main_menu/hoverAmis.bmp"
#define OptionsMainMenuBMP "images/main_menu/options.bmp"
#define HoverOptionsMainMenuBMP "images/main_menu/hoverOptions.bmp"
#define HoverAmisMainMenuBMP "images/main_menu/hoverAmis.bmp"
#define QuitterMainMenuBMP "images/main_menu/quitter.bmp"
#define HoverQuitterMainMenuBMP "images/main_menu/hoverQuitter.bmp"
#define ExitConfirmationBMP "images/exitConfirmation.bmp"
#define OptionsSwitchOnImageBMP "images/options/switchButtonOn.bmp"
#define OptionsSwitchOffImageBMP "images/options/switchButtonOff.bmp"
#define RightArrowOptionsImageBMP "images/options/rightArrow.bmp"
#define HoverRightArrowOptionsImageBMP "images/options/hoverRightArrow.bmp"
#define LeftArrowOptionsImageBMP "images/options/leftArrow.bmp"
#define HoverLeftArrowOptionsImageBMP "images/options/hoverLeftArrow.bmp"
#define DeconnexionOptionsImageBMP "images/options/deconnexionButton.bmp"
#define HoverDeconnexionOptionsImageBMP "images/options/hoverDeconnexionButton.bmp"
#define TypePieces1OptionsImageBMP "images/options/pieces/type1.bmp"
#define TypePieces2OptionsImageBMP "images/options/pieces/type2.bmp"
#define TypePieces3OptionsImageBMP "images/options/pieces/type3.bmp"
#define TypePieces4OptionsImageBMP "images/options/pieces/type4.bmp"


#define BlackPawnImageBMP "images/pieces/type1/black/pawn.bmp"
#define BlackKnightImageBMP "images/pieces/type1/black/knight.bmp"
#define BlackKingImageBMP "images/pieces/type1/black/king.bmp"
#define BlackQueenImageBMP "images/pieces/type1/black/queen.bmp"
#define BlackRookImageBMP "images/pieces/type1/black/rook.bmp"
#define BlackBishopImageBMP "images/pieces/type1/black/bishop.bmp"

#define WhitePawnImageBMP "images/pieces/type1/white/pawn.bmp"
#define WhiteKnightImageBMP "images/pieces/type1/white/knight.bmp"
#define WhiteKingImageBMP "images/pieces/type1/white/king.bmp"
#define WhiteQueenImageBMP "images/pieces/type1/white/queen.bmp"
#define WhiteRookImageBMP "images/pieces/type1/white/rook.bmp"
#define WhiteBishopImageBMP "images/pieces/type1/white/bishop.bmp"


/*#define defineAllPiecesType1() #define BlackPawnImageBMP "images/pieces/type1/black/pawn.bmp"\
    #define BlackKnightImageBMP "images/pieces/type1/black/knight.bmp"\
    #define BlackKingImageBMP "images/pieces/type1/black/king.bmp"\
    #define BlackQueenImageBMP "images/pieces/type1/black/queen.bmp"\
    #define BlackRookImageBMP "images/pieces/type1/black/rook.bmp"\
    #define BlackBishopImageBMP "images/pieces/type1/black/bishop.bmp"\
    #define WhitePawnImageBMP "images/pieces/type1/white/pawn.bmp"\
    #define WhiteKnightImageBMP "images/pieces/type1/white/knight.bmp"\
    #define WhiteKingImageBMP "images/pieces/type1/white/king.bmp"\
    #define WhiteQueenImageBMP "images/pieces/type1/white/queen.bmp"\
    #define WhiteRookImageBMP "images/pieces/type1/white/rook.bmp"\
    #define WhiteBishopImageBMP "images/pieces/type1/white/bishop.bmp"

#define undefinedAllPieces() #undef BlackPawnImageBMP\
    #undef BlackKnightImageBMP\
    #undef BlackKingImageBMP\
    #undef BlackQueenImageBMP\
    #undef BlackRookImageBMP\
    #undef BlackBishopImageBMP\
    #undef WhitePawnImageBMP\
    #undef WhiteKnightImageBMP\
    #undef WhiteKingImageBMP\
    #undef WhiteQueenImageBMP\
    #undef WhiteRookImageBMP\
    #undef WhiteBishopImageBMP*/


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

#define CreateRenderInNewWindow(window, render) window = SDL_CreateWindow("Logames", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_SWSURFACE);\
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
int dayCorrectInThisMonth(int day,int month, int year)
{
    if ((year%4==0) && ((year%400==0) || (year%100!=0)))
    {
        int maxDay[13] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (day<=maxDay[month])
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        int maxDay[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (day<=maxDay[month])
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

int verificationOfTheDayOfBirth(char* strPointeurInscription3)
{
    int day = (strPointeurInscription3[0]-48)*10 + (strPointeurInscription3[1]-48);
    int month = (strPointeurInscription3[3]-48)*10 + (strPointeurInscription3[4]-48);
    int year = (strPointeurInscription3[6]-48)*1000 + (strPointeurInscription3[7]-48)*100 + (strPointeurInscription3[8]-48)*10 + (strPointeurInscription3[9]-48);
    return dayCorrectInThisMonth(day, month, year);
}


#define showTextesConnexion() changeValueConnexion1() \
                surfaceConnexion1 = TTF_RenderText_Solid(font, strConnexion1, color); \
                textureConnexion1 = SDL_CreateTextureFromSurface(render, surfaceConnexion1); \
                SDL_QueryTexture(textureConnexion1, NULL, NULL, &texWConnexion1, &texHConnexion1); \
                SDL_Rect sdlRectConnexion1 = {597, 529, texWConnexion1, texHConnexion1}; \
                SDL_RenderCopy(render, textureConnexion1, NULL, &sdlRectConnexion1); \
                changeValueConnexion2() \
                surfaceConnexion2 = TTF_RenderText_Solid(fontPassword, strConnexion2Hidder, color); \
                textureConnexion2 = SDL_CreateTextureFromSurface(render, surfaceConnexion2); \
                SDL_QueryTexture(textureConnexion2, NULL, NULL, &texWConnexion2, &texHConnexion2); \
                SDL_Rect sdlRectConnexion2 = {594, 643, texWConnexion2, texHConnexion2}; \
                SDL_RenderCopy(render, textureConnexion2, NULL, &sdlRectConnexion2);

#define showTextesInscription() changeValueInscription1()\
                surfaceInscription1 = TTF_RenderText_Solid(font, strInscription1, color);\
                textureInscription1 = SDL_CreateTextureFromSurface(render, surfaceInscription1);\
                SDL_QueryTexture(textureInscription1, NULL, NULL, &texWInscription1, &texHInscription1);\
                SDL_Rect sdlRectInscription1 = {595, 349, texWInscription1, texHInscription1};\
                SDL_RenderCopy(render, textureInscription1, NULL, &sdlRectInscription1);\
                changeValueInscription2()\
                surfaceInscription2 = TTF_RenderText_Solid(font, strInscription2, color);\
                textureInscription2 = SDL_CreateTextureFromSurface(render, surfaceInscription2);\
                SDL_QueryTexture(textureInscription2, NULL, NULL, &texWInscription2, &texHInscription2);\
                SDL_Rect sdlRectInscription2 = {593, 462, texWInscription2, texHInscription2};\
                SDL_RenderCopy(render, textureInscription2, NULL, &sdlRectInscription2);\
                changeValueInscription3()\
                surfaceInscription3 = TTF_RenderText_Solid(font, strInscription3, color);\
                textureInscription3 = SDL_CreateTextureFromSurface(render, surfaceInscription3);\
                SDL_QueryTexture(textureInscription3, NULL, NULL, &texWInscription3, &texHInscription3);\
                SDL_Rect sdlRectInscription3 = {993, 462, texWInscription3, texHInscription3};\
                SDL_RenderCopy(render, textureInscription3, NULL, &sdlRectInscription3);\
                changeValueInscription4()\
                surfaceInscription4 = TTF_RenderText_Solid(font, strInscription4, color);\
                textureInscription4 = SDL_CreateTextureFromSurface(render, surfaceInscription4);\
                SDL_QueryTexture(textureInscription4, NULL, NULL, &texWInscription4, &texHInscription4);\
                SDL_Rect sdlRectInscription4 = {595, 578, texWInscription4, texHInscription4};\
                SDL_RenderCopy(render, textureInscription4, NULL, &sdlRectInscription4);\
                changeValueInscription5()\
                surfaceInscription5 = TTF_RenderText_Solid(fontPassword, strInscription5Hidder, color);\
                textureInscription5 = SDL_CreateTextureFromSurface(render, surfaceInscription5);\
                SDL_QueryTexture(textureInscription5, NULL, NULL, &texWInscription5, &texHInscription5);\
                SDL_Rect sdlRectInscription5 = {593, 693, texWInscription5, texHInscription5};\
                SDL_RenderCopy(render, textureInscription5, NULL, &sdlRectInscription5);\
                changeValueInscription6()\
                surfaceInscription6 = TTF_RenderText_Solid(fontPassword, strInscription6Hidder, color);\
                textureInscription6 = SDL_CreateTextureFromSurface(render, surfaceInscription6);\
                SDL_QueryTexture(textureInscription6, NULL, NULL, &texWInscription6, &texHInscription6);\
                SDL_Rect sdlRectInscription6 = {593, 804, texWInscription6, texHInscription6};\
                SDL_RenderCopy(render, textureInscription6, NULL, &sdlRectInscription6);

#define drawSquareTimer(a,b) rect.x = a;\
    rect.y = a;\
    rect.w = b;\
    rect.h = b;\
    SDL_SetRenderDrawColor(render, WHITE);\
    SDL_RenderFillRect(render, &rect);

#define focusInscription1() focus=1;\
    SDL_SetRenderDrawColor(render, WHITE);\
    SDL_Rect rect;\
    rect.x = 594;\
    rect.y = 348;\
    rect.w = 731;\
    rect.h = 40;\
    SDL_RenderDrawRect(render, &rect);\
    rect.x = 593;\
    rect.y = 347;\
    rect.w = 733;\
    rect.h = 42;\
    SDL_RenderDrawRect(render, &rect);

#define focusInscription2() focus=2;\
    SDL_SetRenderDrawColor(render, WHITE);\
    SDL_Rect rect;\
    rect.x = 592;\
    rect.y = 461;\
    rect.w = 328;\
    rect.h = 40;\
    SDL_RenderDrawRect(render, &rect);\
    rect.x = 591;\
    rect.y = 460;\
    rect.w = 330;\
    rect.h = 42;\
    SDL_RenderDrawRect(render, &rect);

#define focusInscription3() focus=3;\
    SDL_SetRenderDrawColor(render, WHITE);\
    SDL_Rect rect;\
    rect.x = 992;\
    rect.y = 461;\
    rect.w = 328;\
    rect.h = 40;\
    SDL_RenderDrawRect(render, &rect);\
    rect.x = 991;\
    rect.y = 460;\
    rect.w = 330;\
    rect.h = 42;\
    SDL_RenderDrawRect(render, &rect);

#define focusInscription4() focus=4;\
    SDL_SetRenderDrawColor(render, WHITE);\
    SDL_Rect rect;\
    rect.x = 594;\
    rect.y = 577;\
    rect.w = 731;\
    rect.h = 40;\
    SDL_RenderDrawRect(render, &rect);\
    rect.x = 593;\
    rect.y = 576;\
    rect.w = 733;\
    rect.h = 42;\
    SDL_RenderDrawRect(render, &rect);

#define focusInscription5() focus=5;\
    SDL_SetRenderDrawColor(render, WHITE);\
    SDL_Rect rect;\
    rect.x = 592;\
    rect.y = 692;\
    rect.w = 731;\
    rect.h = 40;\
    SDL_RenderDrawRect(render, &rect);\
    rect.x = 591;\
    rect.y = 691;\
    rect.w = 733;\
    rect.h = 42;\
    SDL_RenderDrawRect(render, &rect);

#define focusInscription6() focus=6;\
    SDL_SetRenderDrawColor(render, WHITE);\
    SDL_Rect rect;\
    rect.x = 592;\
    rect.y = 803;\
    rect.w = 731;\
    rect.h = 40;\
    SDL_RenderDrawRect(render, &rect);\
    rect.x = 591;\
    rect.y = 802;\
    rect.w = 733;\
    rect.h = 42;\
    SDL_RenderDrawRect(render, &rect);

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


#define changeValueInscription1() SDL_SetRenderDrawColor(render, GREYINPUT);\
    SDL_Rect rectFill1;\
    rectFill1.x = 595;\
    rectFill1.y = 349;\
    rectFill1.w = 729;\
    rectFill1.h = 38;\
    SDL_RenderFillRect(render, &rectFill1);

#define changeValueInscription2() SDL_SetRenderDrawColor(render, GREYINPUT);\
    SDL_Rect rectFill2;\
    rectFill2.x = 593;\
    rectFill2.y = 462;\
    rectFill2.w = 326;\
    rectFill2.h = 38;\
    SDL_RenderFillRect(render, &rectFill2);

#define changeValueInscription3() SDL_SetRenderDrawColor(render, GREYINPUT);\
    SDL_Rect rectFill3;\
    rectFill3.x = 993;\
    rectFill3.y = 462;\
    rectFill3.w = 326;\
    rectFill3.h = 38;\
    SDL_RenderFillRect(render, &rectFill3);

#define changeValueInscription4() SDL_SetRenderDrawColor(render, GREYINPUT);\
    SDL_Rect rectFill4;\
    rectFill4.x = 595;\
    rectFill4.y = 578;\
    rectFill4.w = 729;\
    rectFill4.h = 38;\
    SDL_RenderFillRect(render, &rectFill4);

#define changeValueInscription5() SDL_SetRenderDrawColor(render, GREYINPUT);\
    SDL_Rect rectFill5;\
    rectFill5.x = 593;\
    rectFill5.y = 693;\
    rectFill5.w = 729;\
    rectFill5.h = 38;\
    SDL_RenderFillRect(render, &rectFill5);

#define changeValueInscription6() SDL_SetRenderDrawColor(render, GREYINPUT);\
    SDL_Rect rectFill6;\
    rectFill6.x = 593;\
    rectFill6.y = 804;\
    rectFill6.w = 729;\
    rectFill6.h = 38;\
    SDL_RenderFillRect(render, &rectFill6);

#define createRectTimeChoice(x1, y1, w1, h1) SDL_Rect rectTimeChoice;\
    rectTimeChoice.x= x1;\
    rectTimeChoice.y= y1;\
    rectTimeChoice.w= w1;\
    rectTimeChoice.h= h1;\

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

#define keyPressedInscription(key, valueKey, valueKeyShift, valueKeyAlt) case key:\
                        if (focus==1)\
                        {\
                            if (cptNumberOfValuesInscription1<limitChar)\
                            {\
                                if ((rightAlt==1 || leftAlt==1) && (leftShift==1 || rightShift==1))\
                                {\
                                }\
                                else if (rightAlt==1 || leftAlt==1)\
                                {\
                                    if (valueKeyAlt!=-1)\
                                    {\
                                        strPointeurInscription1[cptNumberOfValuesInscription1]= valueKeyAlt;\
                                        cptNumberOfValuesInscription1+=1;\
                                    }\
                                }\
                                else if (leftShift==1 || rightShift==1)\
                                {\
                                    if (valueKeyShift!=-1)\
                                    {\
                                        strPointeurInscription1[cptNumberOfValuesInscription1]= valueKeyShift;\
                                        cptNumberOfValuesInscription1+=1;\
                                    }\
                                }\
                                else\
                                {\
                                    if (valueKey!=-1)\
                                    {\
                                        strPointeurInscription1[cptNumberOfValuesInscription1]= valueKey;\
                                        cptNumberOfValuesInscription1+=1;\
                                    }\
                                }\
                            }\
                        }\
                        else if (focus==2)\
                        {\
                            if (cptNumberOfValuesInscription2<limitCharSmall)\
                            {\
                                if ((rightAlt==1 || leftAlt==1) && (leftShift==1 || rightShift==1))\
                                {\
                                }\
                                else if (rightAlt==1 || leftAlt==1)\
                                {\
                                    if (valueKeyAlt!=-1)\
                                    {\
                                        strPointeurInscription2[cptNumberOfValuesInscription2]= valueKeyAlt;\
                                        cptNumberOfValuesInscription2+=1;\
                                    }\
                                }\
                                else if (leftShift==1 || rightShift==1)\
                                {\
                                    if (valueKeyShift!=-1)\
                                    {\
                                        strPointeurInscription2[cptNumberOfValuesInscription2]= valueKeyShift;\
                                        cptNumberOfValuesInscription2+=1;\
                                    }\
                                }\
                                else\
                                {\
                                    if (valueKey!=-1)\
                                    {\
                                        strPointeurInscription2[cptNumberOfValuesInscription2]= valueKey;\
                                        cptNumberOfValuesInscription2+=1;\
                                    }\
                                }\
                            }\
                        }\
                        else if (focus==3)\
                        {\
                            if (cptNumberOfValuesInscription3<limitCharDate)\
                            {\
                                if ((rightAlt==1 || leftAlt==1) && (leftShift==1 || rightShift==1))\
                                {\
                                }\
                                else if (rightAlt==1 || leftAlt==1)\
                                {\
                                    if ((valueKeyAlt!=-1) && (valueKeyAlt>47 && valueKeyAlt<58))\
                                    {\
                                        strPointeurInscription3[cptNumberOfValuesInscription3]= valueKeyAlt;\
                                        cptNumberOfValuesInscription3+=1;\
                                        if (cptNumberOfValuesInscription3==2 || cptNumberOfValuesInscription3==5)\
                                        {\
                                            strPointeurInscription3[cptNumberOfValuesInscription3]=47;\
                                            cptNumberOfValuesInscription3+=1;\
                                        }\
                                    }\
                                }\
                                else if (leftShift==1 || rightShift==1)\
                                {\
                                    if ((valueKeyShift!=-1) && (valueKeyShift>47 && valueKeyShift<58))\
                                    {\
                                        strPointeurInscription3[cptNumberOfValuesInscription3]= valueKeyShift;\
                                        cptNumberOfValuesInscription3+=1;\
                                        if (cptNumberOfValuesInscription3==2 || cptNumberOfValuesInscription3==5)\
                                        {\
                                            strPointeurInscription3[cptNumberOfValuesInscription3]=47;\
                                            cptNumberOfValuesInscription3+=1;\
                                        }\
                                    }\
                                }\
                                else\
                                {\
                                    if ((valueKey!=-1) && (valueKey>47 && valueKey<58))\
                                    {\
                                        strPointeurInscription3[cptNumberOfValuesInscription3]= valueKey;\
                                        cptNumberOfValuesInscription3+=1;\
                                        if (cptNumberOfValuesInscription3==2 || cptNumberOfValuesInscription3==5)\
                                        {\
                                            strPointeurInscription3[cptNumberOfValuesInscription3]=47;\
                                            cptNumberOfValuesInscription3+=1;\
                                        }\
                                    }\
                                }\
                            }\
                        }\
                        else if (focus==4)\
                        {\
                            if (cptNumberOfValuesInscription4<limitChar)\
                            {\
                                if ((rightAlt==1 || leftAlt==1) && (leftShift==1 || rightShift==1))\
                                {\
                                }\
                                else if (rightAlt==1 || leftAlt==1)\
                                {\
                                    if (valueKeyAlt!=-1)\
                                    {\
                                        strPointeurInscription4[cptNumberOfValuesInscription4]= valueKeyAlt;\
                                        cptNumberOfValuesInscription4+=1;\
                                    }\
                                }\
                                else if (leftShift==1 || rightShift==1)\
                                {\
                                    if (valueKeyShift!=-1)\
                                    {\
                                        strPointeurInscription4[cptNumberOfValuesInscription4]= valueKeyShift;\
                                        cptNumberOfValuesInscription4+=1;\
                                    }\
                                }\
                                else\
                                {\
                                    if (valueKey!=-1)\
                                    {\
                                        strPointeurInscription4[cptNumberOfValuesInscription4]= valueKey;\
                                        cptNumberOfValuesInscription4+=1;\
                                    }\
                                }\
                            }\
                        }\
                        else if (focus==5)\
                        {\
                            if (cptNumberOfValuesInscription5<limitChar)\
                            {\
                                if ((rightAlt==1 || leftAlt==1) && (leftShift==1 || rightShift==1))\
                                {\
                                }\
                                else if (rightAlt==1 || leftAlt==1)\
                                {\
                                    if (valueKeyAlt!=-1)\
                                    {\
                                        strPointeurInscription5[cptNumberOfValuesInscription5]= valueKeyAlt;\
                                        strPointeurInscription5Hidder[cptNumberOfValuesInscription5]=charhidder;\
                                        cptNumberOfValuesInscription5+=1;\
                                    }\
                                }\
                                else if (leftShift==1 || rightShift==1)\
                                {\
                                    if (valueKeyShift!=-1)\
                                    {\
                                        strPointeurInscription5[cptNumberOfValuesInscription5]= valueKeyShift;\
                                        strPointeurInscription5Hidder[cptNumberOfValuesInscription5]=charhidder;\
                                        cptNumberOfValuesInscription5+=1;\
                                    }\
                                }\
                                else\
                                {\
                                    if (valueKey!=-1)\
                                    {\
                                        strPointeurInscription5[cptNumberOfValuesInscription5]= valueKey;\
                                        strPointeurInscription5Hidder[cptNumberOfValuesInscription5]=charhidder;\
                                        cptNumberOfValuesInscription5+=1;\
                                    }\
                                }\
                            }\
                        }\
                        else if (focus==6)\
                        {\
                            if (cptNumberOfValuesInscription6<limitChar)\
                            {\
                                if ((rightAlt==1 || leftAlt==1) && (leftShift==1 || rightShift==1))\
                                {\
                                }\
                                else if (rightAlt==1 || leftAlt==1)\
                                {\
                                    if (valueKeyAlt!=-1)\
                                    {\
                                        strPointeurInscription6[cptNumberOfValuesInscription6]= valueKeyAlt;\
                                        strPointeurInscription6Hidder[cptNumberOfValuesInscription6]=charhidder;\
                                        cptNumberOfValuesInscription6+=1;\
                                    }\
                                }\
                                else if (leftShift==1 || rightShift==1)\
                                {\
                                    if (valueKeyShift!=-1)\
                                    {\
                                        strPointeurInscription6[cptNumberOfValuesInscription6]= valueKeyShift;\
                                        strPointeurInscription6Hidder[cptNumberOfValuesInscription6]=charhidder;\
                                        cptNumberOfValuesInscription6+=1;\
                                    }\
                                }\
                                else\
                                {\
                                    if (valueKey!=-1)\
                                    {\
                                        strPointeurInscription6[cptNumberOfValuesInscription6]= valueKey;\
                                        strPointeurInscription6Hidder[cptNumberOfValuesInscription6]=charhidder;\
                                        cptNumberOfValuesInscription6+=1;\
                                    }\
                                }\
                            }\
                        }\
                        break;

#define allKeyConnexion() switch( event.key.keysym.sym ){\
                    case SDLK_LSHIFT:\
                        leftShift=1;\
                        break;\
                    case SDLK_RSHIFT:\
                        rightShift=1;\
                        break;\
                    case SDLK_RALT:\
                        rightAlt=1;\
                        break;\
                    case SDLK_LALT:\
                        leftAlt=1;\
                        break;\
                    case SDLK_BACKSPACE:\
                        if (focus==1)\
                        {\
                            if (cptNumberOfValuesConnexion1>0)\
                            {\
                                cptNumberOfValuesConnexion1-=1;\
                                strPointeurConnexion1[cptNumberOfValuesConnexion1]=32;\
                            }\
                        }\
                        else if (focus==2)\
                        {\
                            if (cptNumberOfValuesConnexion1>0)\
                            {\
                                cptNumberOfValuesConnexion2-=1;\
                                strPointeurConnexion2[cptNumberOfValuesConnexion2]=32;\
                                strPointeurConnexion2Hidder[cptNumberOfValuesConnexion2]=32;\
                            }\
                        }\
                        break;\
                    case SDLK_TAB:\
                        if (focus==0)\
                        {\
                            SDL_RenderClear(render);\
                            SDL_RenderCopy(render, textureConnexionBackground, NULL, NULL);\
                            changeValueConnexion1()\
                            focusConnexion1()\
                            int x,y;\
                            SDL_GetGlobalMouseState(&x, &y);\
                            if (x >799 && x <1122 && y >739 && y <822)\
                            {\
                                SDL_RenderCopy(render, textureHoverButtonBackground, NULL, &rectButton);\
                            }\
                            SDL_RenderPresent(render);\
                        }\
                        else if (focus==1)\
                        {\
                            SDL_RenderClear(render);\
                            SDL_RenderCopy(render, textureConnexionBackground, NULL, NULL);\
                            changeValueConnexion2()\
                            focusConnexion2()\
                            int x,y;\
                            SDL_GetGlobalMouseState(&x, &y);\
                            if (x >799 && x <1122 && y >739 && y <822)\
                            {\
                                SDL_RenderCopy(render, textureHoverButtonBackground, NULL, &rectButton);\
                            }\
                            SDL_RenderPresent(render);\
                        }\
                        break;\
                    keyPressedConnexion(SDLK_a, 97, 65, -1, limitChar)\
                    keyPressedConnexion(SDLK_b, 98, 66, -1, limitChar)\
                    keyPressedConnexion(SDLK_c, 99, 67, -1, limitChar)\
                    keyPressedConnexion(SDLK_d, 100, 68, -1, limitChar)\
                    keyPressedConnexion(SDLK_e, 101, 69, -1, limitChar)\
                    keyPressedConnexion(SDLK_f, 102, 70, -1, limitChar)\
                    keyPressedConnexion(SDLK_g, 103, 71, -1, limitChar)\
                    keyPressedConnexion(SDLK_h, 104, 72, -1, limitChar)\
                    keyPressedConnexion(SDLK_i, 105, 73, -1, limitChar)\
                    keyPressedConnexion(SDLK_j, 106, 74, -1, limitChar)\
                    keyPressedConnexion(SDLK_k, 107, 75, -1, limitChar)\
                    keyPressedConnexion(SDLK_l, 108, 76, -1, limitChar)\
                    keyPressedConnexion(SDLK_m, 109, 77, -1, limitChar)\
                    keyPressedConnexion(SDLK_n, 110, 78, -1, limitChar)\
                    keyPressedConnexion(SDLK_o, 111, 79, -1, limitChar)\
                    keyPressedConnexion(SDLK_p, 112, 80, -1, limitChar)\
                    keyPressedConnexion(SDLK_q, 113, 81, -1, limitChar)\
                    keyPressedConnexion(SDLK_r, 114, 82, -1, limitChar)\
                    keyPressedConnexion(SDLK_s, 115, 83, -1, limitChar)\
                    keyPressedConnexion(SDLK_t, 116, 84, -1, limitChar)\
                    keyPressedConnexion(SDLK_u, 117, 85, -1, limitChar)\
                    keyPressedConnexion(SDLK_v, 118, 86, -1, limitChar)\
                    keyPressedConnexion(SDLK_w, 119, 87, -1, limitChar)\
                    keyPressedConnexion(SDLK_x, 120, 88, -1, limitChar)\
                    keyPressedConnexion(SDLK_y, 121, 89, -1, limitChar)\
                    keyPressedConnexion(SDLK_z, 122, 90, -1, limitChar)\
                    keyPressedConnexion(SDLK_1, 38, 49, -1, limitChar)\
                    keyPressedConnexion(SDLK_2, 233, 50, -1, limitChar)\
                    keyPressedConnexion(SDLK_3, -1, 51, 35, limitChar)\
                    keyPressedConnexion(SDLK_4, -1, 52, -1, limitChar)\
                    keyPressedConnexion(SDLK_5, -1, 53, -1, limitChar)\
                    keyPressedConnexion(SDLK_6, 45, 54, -1, limitChar)\
                    keyPressedConnexion(SDLK_7, 232, 55, -1, limitChar)\
                    keyPressedConnexion(SDLK_8, 95, 56, -1, limitChar)\
                    keyPressedConnexion(SDLK_9, 231, 57, -1, limitChar)\
                    keyPressedConnexion(SDLK_0, 224, 48, 64, limitChar)\
                    keyPressedConnexion(SDLK_COMMA, -1, 63, -1, limitChar)\
                    keyPressedConnexion(SDLK_EXCLAIM, 33, -1, -1, limitChar)\
                    keyPressedConnexion(SDLK_SEMICOLON, -1, 46, -1, limitChar)\
                }

#define emptyChamps(x, y) SDL_Surface * surfaceChampsVide = TTF_RenderText_Solid(fontBold,"Champs vide", colorIncorrect);\
                        SDL_Texture * textureChampsVide = SDL_CreateTextureFromSurface(render, surfaceChampsVide);\
                        int texWChamps1 = 729;\
                        int texHChamps1 = 38;\
                        SDL_QueryTexture(textureChampsVide, NULL, NULL, &texWChamps1, &texHChamps1);\
                        SDL_Rect sdlRectChamps1 = {x, y, texWChamps1, texHChamps1};\
                        SDL_RenderCopy(render, textureChampsVide, NULL, &sdlRectChamps1);

#define champsErrorText(x, y, text) SDL_Surface * surfaceChampsVide = TTF_RenderText_Solid(fontBold,text, colorIncorrect);\
                        SDL_Texture * textureChampsVide = SDL_CreateTextureFromSurface(render, surfaceChampsVide);\
                        int texWChamps1 = 729;\
                        int texHChamps1 = 38;\
                        SDL_QueryTexture(textureChampsVide, NULL, NULL, &texWChamps1, &texHChamps1);\
                        SDL_Rect sdlRectChamps1 = {x, y, texWChamps1, texHChamps1};\
                        SDL_RenderCopy(render, textureChampsVide, NULL, &sdlRectChamps1);


int emailFormatCorrect(char* string, int stringSize)
{
    int at=0;
    int dot=0;
    if (stringSize <5)
    {
        return 0;
    }
    for (int i=0; i<stringSize; i++)
    {
        if (string[i]==64)
        {
            at+=1;
        }
        else if (string[i]==46)
        {
            dot+=1;
        }
    }
    if (at==1 && dot>0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int passwordSame(char* string1, char* string2, int sizeString1, int sizeString2)
{
    if (sizeString1 != sizeString2)
    {
        return 0;
    }
    for (int i=0; i<sizeString1; i++)
    {
        if (string1[i] != string2[i])
        {
            return 0;
        }
    }
    return 1;
}


int doYouWantToQuitNoTime(SDL_Renderer* render)
{
    SDL_Surface* imageExitConfirmationBackground = NULL;
    SDL_Texture* textureExitConfirmationBackground = NULL;
    ALLImageAndTransparencyINIT(imageExitConfirmationBackground, textureExitConfirmationBackground, ExitConfirmationBMP, render)
    SDL_RenderCopy(render, textureExitConfirmationBackground, NULL, NULL);
    SDL_RenderPresent(render);
    SDL_Event event;
    int continuer = 1;
    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.x>604 && event.button.x<1315 && event.button.y>386 && event.button.y<609)
                {
                    if (event.button.x>756 && event.button.x<901 && event.button.y>514 && event.button.y<566)
                    {
                        return 0;
                    }
                    else if (event.button.x>1019 && event.button.x<1164 && event.button.y>514 && event.button.y<566)
                    {
                        return 1;
                    }
                }
                else
                {
                    return 0;
                }
                break;
        }
    }
}

int optionNoTime(SDL_Renderer* render)
{
    SDL_Surface* imageOptionsBG = NULL;
    SDL_Texture* textureOptionsBG = NULL;
    ALLImageAndTransparencyINIT(imageOptionsBG, textureOptionsBG, OptionsBGImageBMP, render)

    SDL_Surface* imageTypepieces1BG = NULL;
    SDL_Texture* textureTypepieces1BG = NULL;
    ALLImageAndTransparencyINIT(imageTypepieces1BG, textureTypepieces1BG, TypePieces1OptionsImageBMP, render)
    SDL_Surface* imageTypepieces2BG = NULL;
    SDL_Texture* textureTypepieces2BG = NULL;
    ALLImageAndTransparencyINIT(imageTypepieces2BG, textureTypepieces2BG, TypePieces2OptionsImageBMP, render)
    SDL_Surface* imageTypepieces3BG = NULL;
    SDL_Texture* textureTypepieces3BG = NULL;
    ALLImageAndTransparencyINIT(imageTypepieces3BG, textureTypepieces3BG, TypePieces3OptionsImageBMP, render)
    SDL_Surface* imageTypepieces4BG = NULL;
    SDL_Texture* textureTypepieces4BG = NULL;
    ALLImageAndTransparencyINIT(imageTypepieces4BG, textureTypepieces4BG, TypePieces4OptionsImageBMP, render)
    SDL_Rect rectTypePieces;
    rectTypePieces.x= 893;
    rectTypePieces.y= 525;
    rectTypePieces.w= 114;
    rectTypePieces.h= 55;

    SDL_Surface* imageSwitchOnBG = NULL;
    SDL_Texture* textureSwitchOnBG = NULL;
    ALLImageAndTransparencyINIT(imageSwitchOnBG, textureSwitchOnBG, OptionsSwitchOnImageBMP, render)

    SDL_Surface* imageSwitchOffBG = NULL;
    SDL_Texture* textureSwitchOffBG = NULL;
    ALLImageAndTransparencyINIT(imageSwitchOffBG, textureSwitchOffBG, OptionsSwitchOffImageBMP, render)

    SDL_Rect rectSwitch1;
    rectSwitch1.x= 893;
    rectSwitch1.y= 525;
    rectSwitch1.w= 114;
    rectSwitch1.h= 55;

    SDL_Rect rectSwitch2;
    rectSwitch2.x= 893;
    rectSwitch2.y= 625;
    rectSwitch2.w= 114;
    rectSwitch2.h= 55;

    SDL_Surface* imageDeconnexionOffBG = NULL;
    SDL_Texture* textureDeconnexionOffBG = NULL;
    ALLImageAndTransparencyINIT(imageDeconnexionOffBG, textureDeconnexionOffBG, DeconnexionOptionsImageBMP, render)

    SDL_Surface* imageHoverDeconnexionOffBG = NULL;
    SDL_Texture* textureHoverDeconnexionOffBG = NULL;
    ALLImageAndTransparencyINIT(imageHoverDeconnexionOffBG, textureHoverDeconnexionOffBG, HoverDeconnexionOptionsImageBMP, render)

    SDL_Rect rectDeconnexion;
    rectDeconnexion.x= 521;
    rectDeconnexion.y= 767;
    rectDeconnexion.w= 268;
    rectDeconnexion.h= 81;

    SDL_Surface* imageRightArrow = NULL;
    SDL_Texture* textureRightArrow = NULL;
    ALLImageAndTransparencyINIT(imageRightArrow, textureRightArrow, RightArrowOptionsImageBMP, render)

    SDL_Surface* imageLeftArrow = NULL;
    SDL_Texture* textureLeftArrow = NULL;
    ALLImageAndTransparencyINIT(imageLeftArrow, textureLeftArrow, LeftArrowOptionsImageBMP, render)

    SDL_Surface* imageHoverRightArrow = NULL;
    SDL_Texture* textureHoverRightArrow = NULL;
    ALLImageAndTransparencyINIT(imageHoverRightArrow, textureHoverRightArrow, HoverRightArrowOptionsImageBMP, render)

    SDL_Surface* imageHoverLeftArrow = NULL;
    SDL_Texture* textureHoverLeftArrow = NULL;
    ALLImageAndTransparencyINIT(imageHoverLeftArrow, textureHoverLeftArrow, HoverLeftArrowOptionsImageBMP, render)

    SDL_Rect rectArrow;
    rectArrow.x= 0;
    rectArrow.y= 792;
    rectArrow.w= 27;
    rectArrow.h= 30;


    int switch1=1;
    int switch2=1;
    int typePieces=1;

    SDL_RenderCopy(render, textureOptionsBG, NULL, NULL);
    SDL_RenderPresent(render);
    SDL_Event event;
    int continuer = 1;
    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_MOUSEMOTION:
                if (event.motion.x>520 && event.motion.x<789 && event.motion.y>766 && event.motion.y<848)
                {
                    SDL_RenderCopy(render, textureHoverDeconnexionOffBG, NULL, &rectDeconnexion);
                    SDL_RenderPresent(render);
                    //déconnexion
                }
                else
                {
                    SDL_RenderCopy(render, textureDeconnexionOffBG, NULL, &rectDeconnexion);
                    SDL_RenderPresent(render);
                    //not déconnexion
                }
                if (event.motion.x>865 && event.motion.x<893 && event.motion.y>791 && event.motion.y<822)
                {
                    rectArrow.x=866;
                    SDL_RenderCopy(render, textureHoverLeftArrow, NULL, &rectArrow);
                    SDL_RenderPresent(render);
                    //leftArrow1
                }
                else
                {
                    rectArrow.x=866;
                    SDL_RenderCopy(render, textureLeftArrow, NULL, &rectArrow);
                    SDL_RenderPresent(render);
                    //not leftArrow1
                }
                if (event.motion.x>1198 && event.motion.x<1226 && event.motion.y>791 && event.motion.y<822)
                {
                    rectArrow.x=1199;
                    SDL_RenderCopy(render, textureHoverLeftArrow, NULL, &rectArrow);
                    SDL_RenderPresent(render);
                    //leftArrow2
                }
                else
                {
                    rectArrow.x=1199;
                    SDL_RenderCopy(render, textureLeftArrow, NULL, &rectArrow);
                    SDL_RenderPresent(render);
                    //not leftArrow2
                }
                if (event.motion.x>1046 && event.motion.x<1074 && event.motion.y>791 && event.motion.y<822)
                {
                    rectArrow.x=1047;
                    SDL_RenderCopy(render, textureHoverRightArrow, NULL, &rectArrow);
                    SDL_RenderPresent(render);
                    //rightArrow1
                }
                else
                {
                    rectArrow.x=1047;
                    SDL_RenderCopy(render, textureRightArrow, NULL, &rectArrow);
                    SDL_RenderPresent(render);
                    //not rightArrow1
                }
                if (event.motion.x>1287 && event.motion.x<1315 && event.motion.y>791 && event.motion.y<822)
                {
                    rectArrow.x=1288;
                    SDL_RenderCopy(render, textureHoverRightArrow, NULL, &rectArrow);
                    SDL_RenderPresent(render);
                    //rightArrow2
                }
                else
                {
                    rectArrow.x=1288;
                    SDL_RenderCopy(render, textureRightArrow, NULL, &rectArrow);
                    SDL_RenderPresent(render);
                    //not rightArrow2
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.x>466 && event.button.x<1455 && event.button.y>331 && event.button.y<886)
                {
                    if (event.button.x>504 && event.button.x<569 && event.button.y>331 && event.button.y<398)
                    {
                        return 0;
                        //Back button
                    }
                    else if (event.button.x>892 && event.button.x<1007 && event.button.y>524 && event.button.y<580)
                    {
                        if (switch1==1)
                        {
                            switch1=0;
                            SDL_RenderCopy(render, textureSwitchOffBG, NULL, &rectSwitch1);
                            SDL_RenderPresent(render);
                        }
                        else
                        {
                            switch1=1;
                            SDL_RenderCopy(render, textureSwitchOnBG, NULL, &rectSwitch1);
                            SDL_RenderPresent(render);
                        }
                        //switch1
                    }
                    else if (event.button.x>892 && event.button.x<1007 && event.button.y>624 && event.button.y<680)
                    {
                        if (switch2==1)
                        {
                            switch2=0;
                            SDL_RenderCopy(render, textureSwitchOffBG, NULL, &rectSwitch2);
                            SDL_RenderPresent(render);
                        }
                        else
                        {
                            switch2=1;
                            SDL_RenderCopy(render, textureSwitchOnBG, NULL, &rectSwitch2);
                            SDL_RenderPresent(render);
                        }
                        //switch2
                    }
                    else if (event.button.x>520 && event.button.x<789 && event.button.y>766 && event.button.y<848)
                    {
                        return 1;
                        //déconnexion
                    }
                    else if (event.button.x>865 && event.button.x<893 && event.button.y>791 && event.button.y<822)
                    {
                        //leftArrow1
                    }
                    else if (event.button.x>1046 && event.button.x<1074 && event.button.y>791 && event.button.y<822)
                    {
                        //rightArrow1
                    }
                    else if (event.button.x>1198 && event.button.x<1226 && event.button.y>791 && event.button.y<822)
                    {
                        //leftArrow2
                    }
                    else if (event.button.x>1287 && event.button.x<1315 && event.button.y>791 && event.button.y<822)
                    {
                        //rightArrow2
                    }
                }
                else
                {
                    continuer=0;
                }
                break;
        }
    }
    return 0;
}

#define openFonts() font = TTF_OpenFont("fonts/arial.ttf", 34);\
    fontBold = TTF_OpenFont("fonts/arialbd.ttf", 28);\
    fontPassword = TTF_OpenFont("fonts/arial.ttf", 62);

#define closeFonts() TTF_CloseFont(font);\
    TTF_CloseFont(fontBold);\
    TTF_CloseFont(fontPassword);
//---------------------------------------------------------------------------------
//------------------------------------All Pages------------------------------------
//---------------------------------------------------------------------------------


void loginPage(SDL_Window* window, SDL_Renderer* render, int* nextPage)
{
    //CreateRenderInNewWindow(window, render)
    TTF_Init();

    TTF_Font * font=NULL;
    TTF_Font *fontBold = TTF_OpenFont("fonts/arialbd.ttf", 28);
    TTF_Font * fontPassword = TTF_OpenFont("fonts/arial.ttf", 62);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

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
    rectButton.y= 739;
    rectButton.h= 81;
    rectButton.w= 323;

    //En dessous
    SDL_Color color = { 0, 0, 0};
    SDL_Color colorIncorrect = {255, 128, 155};
    openFonts()
    SDL_Surface * surfaceConnexion1 = TTF_RenderText_Solid(font,"   ", color);
    closeFonts()
    SDL_Texture * textureConnexion1 = SDL_CreateTextureFromSurface(render, surfaceConnexion1);

    //Au dessus erreur SDL_CreateTextureFromSurface() passed NULL surface
    //En dessous invalid texture

    int texWConnexion1 = 729;
    int texHConnexion1 = 38;
    SDL_QueryTexture(textureConnexion1, NULL, NULL, &texWConnexion1, &texHConnexion1);
    SDL_Rect sdlRectConnexion1 = {597, 529, texWConnexion1, texHConnexion1};
    SDL_RenderCopy(render, textureConnexion1, NULL, &sdlRectConnexion1);

    //Au dessus et en dessous
    openFonts()
    //TTF_CloseFont(font);
    //TTF_Font * fontPassword = TTF_OpenFont("fonts/arial.ttf", 62);
    SDL_Surface * surfaceConnexion2 = TTF_RenderText_Solid(fontPassword,"   ", color);
    SDL_Texture * textureConnexion2 = SDL_CreateTextureFromSurface(render, surfaceConnexion2);
    //TTF_CloseFont(fontPassword);
    int texWConnexion2 = 729;
    int texHConnexion2 = 38;
    SDL_QueryTexture(textureConnexion2, NULL, NULL, &texWConnexion2, &texHConnexion2);
    SDL_Rect sdlRectConnexion2 = {594, 643, texWConnexion2, texHConnexion2};
    SDL_RenderCopy(render, textureConnexion2, NULL, &sdlRectConnexion2);

    //Au dessus

    //TTF_Font *fontBold = TTF_OpenFont("fonts/arialbd.ttf", 28);

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

    closeFonts()

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
                if (event.motion.x >850 && event.motion.x <1078 && event.motion.y >855 && event.motion.y <928)
                {
                    SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
                    SDL_RenderDrawLine(render, 853, 889, 1070, 889);
                    SDL_RenderDrawLine(render, 890, 929, 1037, 929);
                    SDL_RenderPresent(render);
                }
                else
                {
                    SDL_SetRenderDrawColor(render, 29, 137, 228, 255);
                    SDL_RenderDrawLine(render, 853, 889, 1070, 889);
                    SDL_RenderDrawLine(render, 890, 929, 1037, 929);
                    SDL_RenderPresent(render);
                }
                break;
            case SDL_QUIT:
                continuer = 0;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.x >=1875 && event.button.y <=45)
                {
                    if (doYouWantToQuitNoTime(render)==1)
                    {
                        continuer=0;
                        *nextPage=1;
                    }
                    else
                    {
                        SDL_RenderClear(render);
                        SDL_RenderCopy(render, textureConnexionBackground, NULL, NULL);
                        SDL_RenderPresent(render);
                    }
                }
                if (event.button.x >593 && event.button.y > 529 && event.button.x <1324 && event.button.y < 569)
                {
                    openFonts()
                    SDL_RenderClear(render);
                    SDL_RenderCopy(render, textureConnexionBackground, NULL, NULL);
                    focusConnexion1()
                    showTextesConnexion()
                    SDL_RenderPresent(render);
                    closeFonts()
                }
                else if (event.button.x >591 && event.button.y > 644 && event.button.x <1325 && event.button.y < 684)
                {
                    openFonts()
                    SDL_RenderClear(render);
                    SDL_RenderCopy(render, textureConnexionBackground, NULL, NULL);
                    focusConnexion2()
                    showTextesConnexion()
                    SDL_RenderPresent(render);
                    closeFonts()
                }
                else if (event.button.x>797 && event.button.y>740 && event.button.x <1120 && event.button.y<821)
                {
                    if (cptNumberOfValuesConnexion1==0)
                    {
                        openFonts()
                        emptyChamps(714, 484);  
                        SDL_RenderPresent(render);
                        closeFonts()
                    }
                    else if (cptNumberOfValuesConnexion2==0)
                    {
                        openFonts()
                        emptyChamps(840, 603);
                        SDL_RenderPresent(render);
                        closeFonts()
                    }
                    else
                    {   
                        //mysql_real_connect(con, "35.181.56.11", "truc", "Test.123", "pokedex", 3306, NULL, 0)
                        //char chaine[] = "SELECT COUNT(*) FROM USER WHERE"
                        //int requestResult = mysql_query(con, "SELECT COUNT(*) FROM Puzzle");
                        //int testResult = requestResult;
                        //if (testResult==1)
                        //{
                            //Send request if True (need sql)
                            *nextPage=5;
                            continuer=0;
                        //}
                        //else
                        //{
                            //Error if false
                            openFonts()
                            SDL_Surface * surfaceChampsIncorrect = TTF_RenderText_Solid(fontBold,"Email ou Mot de passe incorrect", colorIncorrect);
                            SDL_Texture * textureChampsIncorrect = SDL_CreateTextureFromSurface(render, surfaceChampsIncorrect);    
                            int texWChampsIncorrect = 729;
                            int texHChampsIncorrect = 38;
                            SDL_QueryTexture(textureChampsIncorrect, NULL, NULL, &texWChampsIncorrect, &texHChampsIncorrect);
                            SDL_Rect sdlRectChampsIncorrect = {750, 704, texWChampsIncorrect, texHChampsIncorrect};
                            SDL_RenderCopy(render, textureChampsIncorrect, NULL, &sdlRectChampsIncorrect);
                            SDL_RenderPresent(render);
                            closeFonts()
                        //}
                    }
                }
                else if (event.motion.x >850 && event.motion.x <1078 && event.motion.y >855 && event.motion.y <928)
                {
                    continuer=0;
                    *nextPage=3;
                }
                else
                {
                    openFonts()
                    focus=0;
                    SDL_RenderClear(render);
                    SDL_RenderCopy(render, textureConnexionBackground, NULL, NULL);
                    showTextesConnexion()
                    SDL_RenderPresent(render);
                    closeFonts()
                }
                break;
            case SDL_KEYDOWN: 
                allKeyConnexion()
                openFonts()
                showTextesConnexion()
                SDL_RenderPresent(render);
                closeFonts()
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
        //TTF_CloseFont(fontBold)
    }

    SDL_FreeSurface(imageConnexionBackground);
    SDL_FreeSurface(imageButtonBackground);
    SDL_FreeSurface(imageHoverButtonBackground);
    SDL_FreeSurface(surfaceConnexion1);
    SDL_FreeSurface(surfaceConnexion2);


    SDL_DestroyTexture(textureConnexionBackground);
    SDL_DestroyTexture(textureButtonBackground);
    SDL_DestroyTexture(textureHoverButtonBackground);
    SDL_DestroyTexture(textureConnexion1);
    SDL_DestroyTexture(textureConnexion2);
}




void inscriptionPage(SDL_Window* window, SDL_Renderer* render, int* nextPage)
{
    
    //CreateRenderInNewWindow(window, render)
    SDL_RenderClear(render);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

    SDL_Surface* imageInscriptionBackground = NULL;
    SDL_Texture* textureInscriptionBackground = NULL;
    ALLImageINIT(imageInscriptionBackground, textureInscriptionBackground, InscriptionBGImageBMP, render)

    SDL_RenderCopy(render, textureInscriptionBackground, NULL, NULL);

    SDL_Surface* imageButtonBackground = NULL;
    SDL_Texture* textureButtonBackground = NULL;
    ALLImageAndTransparencyINIT(imageButtonBackground, textureButtonBackground, ButtonInscriptionBMP, render)

    SDL_Surface* imageHoverButtonBackground = NULL;
    SDL_Texture* textureHoverButtonBackground = NULL;
    ALLImageAndTransparencyINIT(imageHoverButtonBackground, textureHoverButtonBackground, HoverButtonInscriptionBMP, render)
    SDL_Rect rectButton;
    rectButton.x= 798;
    rectButton.y= 882;
    rectButton.h= 81;
    rectButton.w= 323;

    TTF_Font * font = NULL;
    TTF_Font * fontBold = NULL;
    TTF_Font * fontPassword = NULL;
    SDL_Color color = { 0, 0, 0};
    SDL_Color colorIncorrect = {255, 128, 155};

    openFonts();
    SDL_Surface * surfaceInscription1 = TTF_RenderText_Solid(font," ", color);
    SDL_Texture * textureInscription1 = SDL_CreateTextureFromSurface(render, surfaceInscription1);    
    int texWInscription1 = 729;
    int texHInscription1 = 38;
    SDL_QueryTexture(textureInscription1, NULL, NULL, &texWInscription1, &texHInscription1);
    SDL_Rect sdlRectInscription1 = {595, 349, texWInscription1, texHInscription1};
    SDL_RenderCopy(render, textureInscription1, NULL, &sdlRectInscription1);
    closeFonts();

    openFonts();
    SDL_Surface * surfaceInscription2 = TTF_RenderText_Solid(font," ", color);
    SDL_Texture * textureInscription2 = SDL_CreateTextureFromSurface(render, surfaceInscription2);    
    int texWInscription2 = 326;
    int texHInscription2 = 38;
    SDL_QueryTexture(textureInscription2, NULL, NULL, &texWInscription2, &texHInscription2);
    SDL_Rect sdlRectInscription2 = {593, 462, texWInscription2, texHInscription2};
    SDL_RenderCopy(render, textureInscription2, NULL, &sdlRectInscription2);
    closeFonts();

    openFonts();
    SDL_Surface * surfaceInscription3 = TTF_RenderText_Solid(font," ", color);
    SDL_Texture * textureInscription3 = SDL_CreateTextureFromSurface(render, surfaceInscription3);    
    int texWInscription3 = 326;
    int texHInscription3 = 38;
    SDL_QueryTexture(textureInscription3, NULL, NULL, &texWInscription3, &texHInscription3);
    SDL_Rect sdlRectInscription3 = {993, 462, texWInscription3, texHInscription3};
    SDL_RenderCopy(render, textureInscription3, NULL, &sdlRectInscription3);
    closeFonts();

    openFonts();
    SDL_Surface * surfaceInscription4 = TTF_RenderText_Solid(font," ", color);
    SDL_Texture * textureInscription4 = SDL_CreateTextureFromSurface(render, surfaceInscription4);    
    int texWInscription4 = 729;
    int texHInscription4 = 38;
    SDL_QueryTexture(textureInscription4, NULL, NULL, &texWInscription4, &texHInscription4);
    SDL_Rect sdlRectInscription4 = {595, 578, texWInscription4, texHInscription4};
    SDL_RenderCopy(render, textureInscription4, NULL, &sdlRectInscription4);
    closeFonts();

    openFonts();
    SDL_Surface * surfaceInscription5 = TTF_RenderText_Solid(fontPassword," ", color);
    SDL_Texture * textureInscription5 = SDL_CreateTextureFromSurface(render, surfaceInscription5);    
    int texWInscription5 = 729;
    int texHInscription5 = 38;
    SDL_QueryTexture(textureInscription5, NULL, NULL, &texWInscription5, &texHInscription5);
    SDL_Rect sdlRectInscription5 = {593, 693, texWInscription5, texHInscription5};
    SDL_RenderCopy(render, textureInscription5, NULL, &sdlRectInscription5);
    closeFonts();

    openFonts();
    SDL_Surface * surfaceInscription6 = TTF_RenderText_Solid(fontPassword," ", color);
    SDL_Texture * textureInscription6 = SDL_CreateTextureFromSurface(render, surfaceInscription6);    
    int texWInscription6 = 729;
    int texHInscription6 = 38;
    SDL_QueryTexture(textureInscription6, NULL, NULL, &texWInscription6, &texHInscription6);
    SDL_Rect sdlRectInscription6 = {593, 804, texWInscription6, texHInscription6};
    SDL_RenderCopy(render, textureInscription6, NULL, &sdlRectInscription6);
    closeFonts();
    
    char strInscription1[38]="                                      ";
    char* strPointeurInscription1 = strInscription1;
    char strInscription2[38]="                                      ";
    char* strPointeurInscription2 = strInscription2;
    char strInscription3[38]="                                      ";
    char* strPointeurInscription3 = strInscription3;
    char strInscription4[38]="                                      ";
    char* strPointeurInscription4 = strInscription4;
    char strInscription5[38]="                                      ";
    char* strPointeurInscription5 = strInscription5;
    char strInscription5Hidder[38]="                                      ";
    char* strPointeurInscription5Hidder = strInscription5Hidder;
    char strInscription6[38]="                                      ";
    char* strPointeurInscription6 = strInscription6;
    char strInscription6Hidder[38]="                                      ";
    char* strPointeurInscription6Hidder = strInscription6Hidder;

    int cptNumberOfValuesInscription1 = 0;
    int cptNumberOfValuesInscription2 = 0;
    int cptNumberOfValuesInscription3 = 0;
    int cptNumberOfValuesInscription4 = 0;
    int cptNumberOfValuesInscription5 = 0;
    int cptNumberOfValuesInscription6 = 0;

    int focus = 0;
    

    int leftShift=0;
    int rightShift=0;

    int leftAlt=0;
    int rightAlt=0;

    int charhidder=42;
    int limitChar=30;
    int limitCharSmall=14;
    int limitCharDate=10;

    SDL_RenderPresent(render);

    SDL_Event event;
    int continuer = 1;
    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_MOUSEMOTION:
                if (event.motion.x >798 && event.motion.x <1121 && event.motion.y >881 && event.motion.y <963)
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
                    if (doYouWantToQuitNoTime(render)==1)
                    {
                        continuer=0;
                        *nextPage=1;
                    }
                }
                else if (event.button.x >598 && event.button.y >225 && event.button.x <640 && event.button.y <269)
                {
                    continuer=0;
                    *nextPage=2;
                }
                else if (event.button.x >593 && event.button.y > 347 && event.button.x <1322 && event.button.y < 385)
                {
                    openFonts();
                    SDL_RenderClear(render);
                    SDL_RenderCopy(render, textureInscriptionBackground, NULL, NULL);
                    focusInscription1()
                    showTextesInscription()
                    SDL_RenderPresent(render);
                    closeFonts();
                }
                else if (event.button.x >593 && event.button.y > 462 && event.button.x <919 && event.button.y < 500)
                {
                    openFonts();
                    SDL_RenderClear(render);
                    SDL_RenderCopy(render, textureInscriptionBackground, NULL, NULL);
                    focusInscription2()
                    showTextesInscription()
                    SDL_RenderPresent(render);
                    closeFonts();
                }
                else if (event.button.x >993 && event.button.y > 462 && event.button.x <1319 && event.button.y < 500)
                {
                    openFonts();
                    SDL_RenderClear(render);
                    SDL_RenderCopy(render, textureInscriptionBackground, NULL, NULL);
                    focusInscription3()
                    showTextesInscription()
                    SDL_RenderPresent(render);
                    closeFonts();
                }
                else if (event.button.x >595 && event.button.y > 578 && event.button.x <1324 && event.button.y < 616)
                {
                    openFonts();
                    SDL_RenderClear(render);
                    SDL_RenderCopy(render, textureInscriptionBackground, NULL, NULL);
                    focusInscription4()
                    showTextesInscription()
                    SDL_RenderPresent(render);
                    closeFonts();
                }
                else if (event.button.x >593 && event.button.y > 693 && event.button.x <1322 && event.button.y < 731)
                {
                    openFonts();
                    SDL_RenderClear(render);
                    SDL_RenderCopy(render, textureInscriptionBackground, NULL, NULL);
                    focusInscription5()
                    showTextesInscription()
                    SDL_RenderPresent(render);
                    closeFonts();
                }
                else if (event.button.x >593 && event.button.y > 803 && event.button.x <1322 && event.button.y < 841)
                {
                    openFonts();
                    SDL_RenderClear(render);
                    SDL_RenderCopy(render, textureInscriptionBackground, NULL, NULL);
                    focusInscription6()
                    showTextesInscription()
                    SDL_RenderPresent(render);
                    closeFonts();
                }
                else if (event.button.x >798 && event.button.x <1121 && event.button.y >881 && event.button.y <963)
                {
                    if (cptNumberOfValuesInscription1==0)
                    {
                        openFonts();
                        emptyChamps(700, 304);
                        SDL_RenderPresent(render);
                        closeFonts();
                    }
                    else if (cptNumberOfValuesInscription2==0)
                    {
                        openFonts();
                        emptyChamps(740, 423);
                        SDL_RenderPresent(render);
                        closeFonts();
                    }
                    else if (cptNumberOfValuesInscription3==0)
                    {
                        openFonts();
                        emptyChamps(995, 464);
                        SDL_RenderPresent(render);
                        closeFonts();
                    }
                    else if (cptNumberOfValuesInscription4==0)
                    {
                        openFonts();
                        emptyChamps(707, 536);
                        SDL_RenderPresent(render);
                        closeFonts();
                    }
                    else if (cptNumberOfValuesInscription5==0)
                    {
                        openFonts();
                        emptyChamps(843, 650);
                        SDL_RenderPresent(render);
                        closeFonts();
                    }
                    else if (cptNumberOfValuesInscription6==0)
                    {
                        openFonts()
                        emptyChamps(842, 764);
                        SDL_RenderPresent(render);
                        closeFonts();
                    }
                    else if (cptNumberOfValuesInscription3==10 && verificationOfTheDayOfBirth(strPointeurInscription3)==0)
                    {
                        openFonts();
                        champsErrorText(995, 514, "Date incorrecte");
                        SDL_RenderPresent(render);
                        closeFonts();
                    }
                    else if (cptNumberOfValuesInscription3!=10)
                    {
                        openFonts();
                        champsErrorText(995, 514, "Date incomplete");
                        SDL_RenderPresent(render);
                        closeFonts();
                    }
                    else if (emailFormatCorrect(strPointeurInscription4, cptNumberOfValuesInscription4)==0)
                    {
                        openFonts()
                        champsErrorText(707, 536, "Format de l'email incorrect");
                        SDL_RenderPresent(render);
                        closeFonts();
                    }
                    else if (passwordSame(strInscription5, strInscription6, cptNumberOfValuesInscription5, cptNumberOfValuesInscription6)==0)
                    {
                        openFonts();
                        champsErrorText(842, 764, "Confirmation differente du mot de passe");
                        SDL_RenderPresent(render);
                        closeFonts();
                    }
                    else
                    {
                        //Send request
                        *nextPage=2;
                        continuer=0;
                    }

                }
                else
                {
                    openFonts();
                    focus=0;
                    SDL_RenderClear(render);
                    SDL_RenderCopy(render, textureInscriptionBackground, NULL, NULL);
                    showTextesInscription()
                    SDL_RenderPresent(render);
                    closeFonts();
                }
                break;
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
                            if (cptNumberOfValuesInscription1>0)
                            {
                                cptNumberOfValuesInscription1-=1;
                                strPointeurInscription1[cptNumberOfValuesInscription1]=32;
                            }
                        }
                        else if (focus==2)
                        {
                            if (cptNumberOfValuesInscription2>0)
                            {
                                cptNumberOfValuesInscription2-=1;
                                strPointeurInscription2[cptNumberOfValuesInscription2]=32;
                            }
                        }
                        else if (focus==3)
                        {
                            if (cptNumberOfValuesInscription3>0)
                            {
                                cptNumberOfValuesInscription3-=1;
                                if (cptNumberOfValuesInscription3==2 || cptNumberOfValuesInscription3==5)
                                {
                                    strPointeurInscription3[cptNumberOfValuesInscription3]=32;
                                    cptNumberOfValuesInscription3-=1;
                                }
                                strPointeurInscription3[cptNumberOfValuesInscription3]=32;
                            }
                        }
                        else if (focus==4)
                        {
                            if (cptNumberOfValuesInscription4>0)
                            {
                                cptNumberOfValuesInscription4-=1;
                                strPointeurInscription4[cptNumberOfValuesInscription4]=32;
                            }
                        }
                        else if (focus==5)
                        {
                            if (cptNumberOfValuesInscription1>0)
                            {
                                cptNumberOfValuesInscription5-=1;
                                strPointeurInscription5[cptNumberOfValuesInscription5]=32;
                                strPointeurInscription5Hidder[cptNumberOfValuesInscription5]=32;
                            }
                        }
                        else if (focus==6)
                        {
                            if (cptNumberOfValuesInscription1>0)
                            {
                                cptNumberOfValuesInscription6-=1;
                                strPointeurInscription6[cptNumberOfValuesInscription6]=32;
                                strPointeurInscription6Hidder[cptNumberOfValuesInscription6]=32;
                            }
                        }
                        break;
                    case SDLK_TAB:
                        if (focus==0)
                        {
                            openFonts();
                            SDL_RenderClear(render);
                            SDL_RenderCopy(render, textureInscriptionBackground, NULL, NULL);
                            showTextesInscription()
                            focusInscription1()
                            int x,y;
                            SDL_GetGlobalMouseState(&x, &y);
                            if (x >798 && x <1121 && y >881 && y <963)
                            {
                                SDL_RenderCopy(render, textureHoverButtonBackground, NULL, &rectButton);
                            }
                            SDL_RenderPresent(render);
                            closeFonts();
                        }
                        else if (focus==1)
                        {
                            openFonts();
                            SDL_RenderClear(render);
                            SDL_RenderCopy(render, textureInscriptionBackground, NULL, NULL);
                            showTextesInscription()
                            focusInscription2()
                            int x,y;
                            SDL_GetGlobalMouseState(&x, &y);
                            if (x >798 && x <1121 && y >881 && y <963)
                            {
                                SDL_RenderCopy(render, textureHoverButtonBackground, NULL, &rectButton);
                            }
                            SDL_RenderPresent(render);
                            closeFonts();
                        }
                        else if (focus==2)
                        {
                            openFonts();
                            SDL_RenderClear(render);
                            SDL_RenderCopy(render, textureInscriptionBackground, NULL, NULL);
                            showTextesInscription()
                            focusInscription3()
                            int x,y;
                            SDL_GetGlobalMouseState(&x, &y);
                            if (x >798 && x <1121 && y >881 && y <963)
                            {
                                SDL_RenderCopy(render, textureHoverButtonBackground, NULL, &rectButton);
                            }
                            SDL_RenderPresent(render);
                            closeFonts();
                        }
                        else if (focus==3)
                        {
                            openFonts();
                            SDL_RenderClear(render);
                            SDL_RenderCopy(render, textureInscriptionBackground, NULL, NULL);
                            showTextesInscription()
                            focusInscription4()
                            int x,y;
                            SDL_GetGlobalMouseState(&x, &y);
                            if (x >798 && x <1121 && y >881 && y <963)
                            {
                                SDL_RenderCopy(render, textureHoverButtonBackground, NULL, &rectButton);
                            }
                            SDL_RenderPresent(render);
                            closeFonts();
                        }
                        else if (focus==4)
                        {
                            openFonts();
                            SDL_RenderClear(render);
                            SDL_RenderCopy(render, textureInscriptionBackground, NULL, NULL);
                            showTextesInscription()
                            focusInscription5()
                            int x,y;
                            SDL_GetGlobalMouseState(&x, &y);
                            if (x >798 && x <1121 && y >881 && y <963)
                            {
                                SDL_RenderCopy(render, textureHoverButtonBackground, NULL, &rectButton);
                            }
                            SDL_RenderPresent(render);
                            closeFonts();
                        }
                        else if (focus==5)
                        {
                            openFonts();
                            SDL_RenderClear(render);
                            SDL_RenderCopy(render, textureInscriptionBackground, NULL, NULL);
                            showTextesInscription()
                            focusInscription6()
                            int x,y;
                            SDL_GetGlobalMouseState(&x, &y);
                            if (x >798 && x <1121 && y >881 && y <963)
                            {
                                SDL_RenderCopy(render, textureHoverButtonBackground, NULL, &rectButton);
                            }
                            SDL_RenderPresent(render);
                            closeFonts();
                        }
                        break;
                    keyPressedInscription(SDLK_a, 97, 65, -1)
                    keyPressedInscription(SDLK_b, 98, 66, -1)
                    keyPressedInscription(SDLK_c, 99, 67, -1)
                    keyPressedInscription(SDLK_d, 100, 68, -1)
                    keyPressedInscription(SDLK_e, 101, 69, -1)
                    keyPressedInscription(SDLK_f, 102, 70, -1)
                    keyPressedInscription(SDLK_g, 103, 71, -1)
                    keyPressedInscription(SDLK_h, 104, 72, -1)
                    keyPressedInscription(SDLK_i, 105, 73, -1)
                    keyPressedInscription(SDLK_j, 106, 74, -1)
                    keyPressedInscription(SDLK_k, 107, 75, -1)
                    keyPressedInscription(SDLK_l, 108, 76, -1)
                    keyPressedInscription(SDLK_m, 109, 77, -1)
                    keyPressedInscription(SDLK_n, 110, 78, -1)
                    keyPressedInscription(SDLK_o, 111, 79, -1)
                    keyPressedInscription(SDLK_p, 112, 80, -1)
                    keyPressedInscription(SDLK_q, 113, 81, -1)
                    keyPressedInscription(SDLK_r, 114, 82, -1)
                    keyPressedInscription(SDLK_s, 115, 83, -1)
                    keyPressedInscription(SDLK_t, 116, 84, -1)
                    keyPressedInscription(SDLK_u, 117, 85, -1)
                    keyPressedInscription(SDLK_v, 118, 86, -1)
                    keyPressedInscription(SDLK_w, 119, 87, -1)
                    keyPressedInscription(SDLK_x, 120, 88, -1)
                    keyPressedInscription(SDLK_y, 121, 89, -1)
                    keyPressedInscription(SDLK_z, 122, 90, -1)
                    keyPressedInscription(SDLK_1, 38, 49, -1)
                    keyPressedInscription(SDLK_2, 233, 50, -1)
                    keyPressedInscription(SDLK_3, -1, 51, 35)
                    keyPressedInscription(SDLK_4, -1, 52, -1)
                    keyPressedInscription(SDLK_5, -1, 53, -1)
                    keyPressedInscription(SDLK_6, 45, 54, -1)
                    keyPressedInscription(SDLK_7, 232, 55, -1)
                    keyPressedInscription(SDLK_8, 95, 56, -1)
                    keyPressedInscription(SDLK_9, 231, 57, -1)
                    keyPressedInscription(SDLK_0, 224, 48, 64)
                    keyPressedInscription(SDLK_COMMA, -1, 63, -1)
                    keyPressedInscription(SDLK_EXCLAIM, 33, -1, -1)
                    keyPressedInscription(SDLK_SEMICOLON, -1, 46, -1)
                }
                openFonts();
                showTextesInscription()
                SDL_RenderPresent(render);
                closeFonts();
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
    }
    SDL_FreeSurface(imageInscriptionBackground);
    SDL_FreeSurface(imageButtonBackground);
    SDL_FreeSurface(imageHoverButtonBackground);
    SDL_FreeSurface(surfaceInscription1);
    SDL_FreeSurface(surfaceInscription2);
    SDL_FreeSurface(surfaceInscription3);
    SDL_FreeSurface(surfaceInscription4);
    SDL_FreeSurface(surfaceInscription5);
    SDL_FreeSurface(surfaceInscription6);

    SDL_DestroyTexture(textureButtonBackground);
    SDL_DestroyTexture(textureInscriptionBackground);
    SDL_DestroyTexture(textureHoverButtonBackground);
    SDL_DestroyTexture(textureInscription1);
    SDL_DestroyTexture(textureInscription2);
    SDL_DestroyTexture(textureInscription3);
    SDL_DestroyTexture(textureInscription4);
    SDL_DestroyTexture(textureInscription5);
    SDL_DestroyTexture(textureInscription6);
}



void modeSelectionPage(SDL_Window* window, SDL_Renderer* render, int* nextPage)
{
    
    //CreateRenderInNewWindow(window, render)
    SDL_RenderClear(render);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

    SDL_Surface* imageModeSelectionBackground = NULL;
    SDL_Texture* textureModeSelectionBackground = NULL;
    ALLImageINIT(imageModeSelectionBackground, textureModeSelectionBackground, ModeSelectionBGImageBMP, render)

    TTF_Font * font = TTF_OpenFont("fonts/arial.ttf", 34);
    SDL_Color color = {255, 255, 255};

    SDL_Surface * surfaceSelectionLocal = TTF_RenderText_Solid(font,"Local", color);
    SDL_Texture * textureSelectionLocal = SDL_CreateTextureFromSurface(render, surfaceSelectionLocal);    
    int texWSelectionLocal = 729;
    int texHSelectionLocal = 38;
    SDL_QueryTexture(textureSelectionLocal, NULL, NULL, &texWSelectionLocal, &texHSelectionLocal);
    SDL_Rect sdlRectSelectionLocal = {596, 772, texWSelectionLocal, texHSelectionLocal};

    SDL_Surface * surfaceMultijoueur = TTF_RenderText_Solid(font,"Multijoueur", color);
    SDL_Texture * textureMultijoueur = SDL_CreateTextureFromSurface(render, surfaceMultijoueur);    
    int texWMultijoueur = 729;
    int texHMultijoueur = 38;
    SDL_QueryTexture(textureMultijoueur, NULL, NULL, &texWMultijoueur, &texHMultijoueur);
    SDL_Rect sdlRectMultijoueur = {878, 772, texWMultijoueur, texHMultijoueur};

    SDL_Surface * surfaceOrdinateur = TTF_RenderText_Solid(font,"Ordinateur", color);
    SDL_Texture * textureOrdinateur = SDL_CreateTextureFromSurface(render, surfaceOrdinateur);    
    int texWOrdinateur = 729;
    int texHOrdinateur = 38;
    SDL_QueryTexture(textureOrdinateur, NULL, NULL, &texWOrdinateur, &texHOrdinateur);
    SDL_Rect sdlRectOrdinateur = {1215, 772, texWOrdinateur, texHOrdinateur};

    SDL_RenderCopy(render, textureModeSelectionBackground, NULL, NULL);
    SDL_RenderPresent(render);

    

    SDL_Event event;
    int continuer = 1;
    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_MOUSEMOTION:
                if (event.motion.x >514 && event.motion.x <766 && event.motion.y >552 && event.motion.y <759)
                {
                    SDL_RenderClear(render);
                    SDL_RenderCopy(render, textureModeSelectionBackground, NULL, NULL);
                    SDL_RenderCopy(render, textureSelectionLocal, NULL, &sdlRectSelectionLocal);
                    SDL_RenderPresent(render);
                }
                else if (event.motion.x >845 && event.motion.x <1082 && event.motion.y >551 && event.motion.y <763)
                {
                    SDL_RenderClear(render);
                    SDL_RenderCopy(render, textureModeSelectionBackground, NULL, NULL);
                    SDL_RenderCopy(render, textureMultijoueur, NULL, &sdlRectMultijoueur);
                    SDL_RenderPresent(render);
                }
                else if (event.motion.x >1187 && event.motion.x <1405 && event.motion.y >545 && event.motion.y <774)
                {
                    SDL_RenderClear(render);
                    SDL_RenderCopy(render, textureModeSelectionBackground, NULL, NULL);
                    SDL_RenderCopy(render, textureOrdinateur, NULL, &sdlRectOrdinateur);
                    SDL_RenderPresent(render);
                }
                else
                {
                    SDL_RenderClear(render);
                    SDL_RenderCopy(render, textureModeSelectionBackground, NULL, NULL);
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
                else if (event.button.x>446 && event.button.y>370 && event.button.x<496 && event.button.y<426 )
                {
                    continuer=0;
                }
                else if (event.button.x >514 && event.button.x <766 && event.button.y >552 && event.button.y <759)
                {
                    //1
                }
                else if (event.button.x >845 && event.button.x <1082 && event.button.y >551 && event.button.y <763)
                {
                    //2
                }
                else if (event.button.x >1187 && event.button.x <1405 && event.button.y >545 && event.button.y <774)
                {
                    //3
                }
                break;
            case SDL_KEYDOWN:
                break;
        }
    }
    SDL_FreeSurface(surfaceOrdinateur);
    SDL_FreeSurface(surfaceSelectionLocal);
    SDL_FreeSurface(surfaceMultijoueur);

    SDL_DestroyTexture(textureOrdinateur);
    SDL_DestroyTexture(textureSelectionLocal);
    SDL_DestroyTexture(textureMultijoueur);
}



void timeSelectionPage(SDL_Window* window, SDL_Renderer* render, int* nextPage)
{
    
    //CreateRenderInNewWindow(window, render)
    SDL_RenderClear(render);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

    SDL_Surface* imageTempsPartieBackground = NULL;
    SDL_Texture* textureTempsPartieBackground = NULL;
    ALLImageINIT(imageTempsPartieBackground, textureTempsPartieBackground, TempsPartieBGImageBMP, render)

    TTF_Font * font = TTF_OpenFont("fonts/arial.ttf", 34);
    SDL_Color color = {255, 255, 255};

    SDL_Surface* imageContourBlancBackground = NULL;
    SDL_Texture* textureContourBlancBackground = NULL;
    ALLImageAndTransparencyINIT(imageContourBlancBackground, textureContourBlancBackground, WhiteBorderBGImageBMP, render)
    
    SDL_Surface* imageHoverRetourBackground = NULL;
    SDL_Texture* textureHoverRetourBackground = NULL;
    ALLImageAndTransparencyINIT(imageHoverRetourBackground, textureHoverRetourBackground, HoverRetourTimeBMP, render)
    SDL_Rect rectButtonRetour;
    rectButtonRetour.x= 640;
    rectButtonRetour.y= 778;
    rectButtonRetour.w= 304;
    rectButtonRetour.h= 92;
    
    SDL_Surface* imageRetourBackground = NULL;
    SDL_Texture* textureRetourBackground = NULL;
    ALLImageAndTransparencyINIT(imageRetourBackground, textureRetourBackground, RetourTimeBMP, render)

    SDL_Surface* imageHoverLancerBackground = NULL;
    SDL_Texture* textureHoverLancerBackground = NULL;
    ALLImageAndTransparencyINIT(imageHoverLancerBackground, textureHoverLancerBackground, HoverLancerTimeBMP, render)
    SDL_Rect rectButtonLancer;
    rectButtonLancer.x= 976;
    rectButtonLancer.y= 778;
    rectButtonLancer.w= 304;
    rectButtonLancer.h= 92;
    
    SDL_Surface* imageLancerBackground = NULL;
    SDL_Texture* textureLancerBackground = NULL;
    ALLImageAndTransparencyINIT(imageLancerBackground, textureLancerBackground, LancerTimeBMP, render)

    SDL_RenderCopy(render, textureTempsPartieBackground, NULL, NULL);
    SDL_RenderPresent(render);

    

    SDL_Event event;
    int continuer=1, focus=0;
    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_MOUSEMOTION:
                if (event.button.x>639 && event.button.x<944 && event.button.y>777 && event.button.y<870)
                {
                    SDL_RenderCopy(render, textureHoverRetourBackground, NULL, &rectButtonRetour);
                    SDL_RenderPresent(render);
                }
                else
                {
                    SDL_RenderCopy(render, textureRetourBackground, NULL, &rectButtonRetour);
                    SDL_RenderPresent(render);
                }
                if (event.button.x>976 && event.button.x<1280 && event.button.y>777 && event.button.y<870)
                {
                    SDL_RenderCopy(render, textureHoverLancerBackground, NULL, &rectButtonLancer);
                    SDL_RenderPresent(render);
                }
                else
                {
                    SDL_RenderCopy(render, textureLancerBackground, NULL, &rectButtonLancer);
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
                else if (event.button.x>688 && event.button.x<834 && event.button.y>403 && event.button.y<475 && focus!=1)
                {
                    focus=1;
                    SDL_RenderClear(render);
                    SDL_RenderCopy(render, textureTempsPartieBackground, NULL, NULL);
                    createRectTimeChoice(688, 403, 146, 71)
                    SDL_RenderCopy(render, textureContourBlancBackground, NULL, &rectTimeChoice);
                    SDL_RenderPresent(render);
                }
                else if (event.button.x>887 && event.button.x<1033 && event.button.y>403 && event.button.y<474 && focus!=2)
                {
                    focus=2;
                    SDL_RenderClear(render);
                    SDL_RenderCopy(render, textureTempsPartieBackground, NULL, NULL);
                    createRectTimeChoice(887, 403, 146, 71)
                    SDL_RenderCopy(render, textureContourBlancBackground, NULL, &rectTimeChoice);
                    SDL_RenderPresent(render);
                }
                else if (event.button.x>1086 && event.button.x<1232 && event.button.y>403 && event.button.y<474 && focus!=3)
                {
                    focus=3;
                    SDL_RenderClear(render);
                    SDL_RenderCopy(render, textureTempsPartieBackground, NULL, NULL);
                    createRectTimeChoice(1086, 403, 146, 71)
                    SDL_RenderCopy(render, textureContourBlancBackground, NULL, &rectTimeChoice);
                    SDL_RenderPresent(render);
                }
                else if (event.button.x>688 && event.button.x<834 && event.button.y>516 && event.button.y<587 && focus!=4)
                {
                    focus=4;
                    SDL_RenderClear(render);
                    SDL_RenderCopy(render, textureTempsPartieBackground, NULL, NULL);
                    createRectTimeChoice(688, 516, 146, 71)
                    SDL_RenderCopy(render, textureContourBlancBackground, NULL, &rectTimeChoice);
                    SDL_RenderPresent(render);
                }
                else if (event.button.x>887 && event.button.x<1033 && event.button.y>516 && event.button.y<587 && focus!=5)
                {
                    focus=5;
                    SDL_RenderClear(render);
                    SDL_RenderCopy(render, textureTempsPartieBackground, NULL, NULL);
                    createRectTimeChoice(887, 516, 146, 71)
                    SDL_RenderCopy(render, textureContourBlancBackground, NULL, &rectTimeChoice);
                    SDL_RenderPresent(render);
                }
                else if (event.button.x>1086 && event.button.x<1232 && event.button.y>516 && event.button.y<587 && focus!=6)
                {
                    focus=6;
                    SDL_RenderClear(render);
                    SDL_RenderCopy(render, textureTempsPartieBackground, NULL, NULL);
                    createRectTimeChoice(1086, 516, 146, 71)
                    SDL_RenderCopy(render, textureContourBlancBackground, NULL, &rectTimeChoice);
                    SDL_RenderPresent(render);
                }
                else if (event.button.x>688 && event.button.x<834 && event.button.y>628 && event.button.y<699 && focus!=7)
                {
                    focus=7;
                    SDL_RenderClear(render);
                    SDL_RenderCopy(render, textureTempsPartieBackground, NULL, NULL);
                    createRectTimeChoice(688, 628, 146, 71)
                    SDL_RenderCopy(render, textureContourBlancBackground, NULL, &rectTimeChoice);
                    SDL_RenderPresent(render);
                }
                else if (event.button.x>887 && event.button.x<1033 && event.button.y>628 && event.button.y<699 && focus!=8)
                {
                    focus=8;
                    SDL_RenderClear(render);
                    SDL_RenderCopy(render, textureTempsPartieBackground, NULL, NULL);
                    createRectTimeChoice(887, 628, 146, 71)
                    SDL_RenderCopy(render, textureContourBlancBackground, NULL, &rectTimeChoice);
                    SDL_RenderPresent(render);
                }
                else if (event.button.x>1086 && event.button.x<1232 && event.button.y>628 && event.button.y<699 && focus!=9)
                {
                    focus=9;
                    SDL_RenderClear(render);
                    SDL_RenderCopy(render, textureTempsPartieBackground, NULL, NULL);
                    createRectTimeChoice(1086, 628, 146, 71)
                    SDL_RenderCopy(render, textureContourBlancBackground, NULL, &rectTimeChoice);
                    SDL_RenderPresent(render);
                }
                else if (event.button.x>639 && event.button.x<944 && event.button.y>777 && event.button.y<870)
                {
                    //Previous Page
                }
                else if (event.button.x>976 && event.button.x<1280 && event.button.y>777 && event.button.y<870)
                {
                    if (focus!=0)
                    {
                        continuer=0;
                    }
                    //Validation
                }
                else
                {
                    focus=0;
                    SDL_RenderClear(render);
                    SDL_RenderCopy(render, textureTempsPartieBackground, NULL, NULL);
                    SDL_RenderPresent(render);
                }
                break;
            case SDL_KEYDOWN:
                break;
        }
    }
}


void mainMenuPage(SDL_Window* window, SDL_Renderer* render, int* nextPage)
{
    
    //CreateRenderInNewWindow(window, render)
    SDL_RenderClear(render);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

    SDL_Surface* imageBackgroundMenu = NULL;
    SDL_Texture* textureBackgroundMenu = NULL;
    ALLImageINIT(imageBackgroundMenu, textureBackgroundMenu, MenuBGImageBMP, render)

    SDL_RenderCopy(render, textureBackgroundMenu, NULL, NULL);
    SDL_RenderPresent(render);

    int slideAmis=0;


    //Puzzle
    SDL_Surface* imageHoverPuzzleBackground = NULL;
    SDL_Texture* textureHoverPuzzleBackground = NULL;
    ALLImageINIT(imageHoverPuzzleBackground, textureHoverPuzzleBackground, HoverPuzzleMainMenuBMP, render)
    SDL_Rect rectButtonPuzzle;
    rectButtonPuzzle.x= 1044;
    rectButtonPuzzle.y= 340;
    rectButtonPuzzle.w= 551;
    rectButtonPuzzle.h= 615;
    
    SDL_Surface* imagePuzzleBackground = NULL;
    SDL_Texture* texturePuzzleBackground = NULL;
    ALLImageINIT(imagePuzzleBackground, texturePuzzleBackground, PuzzleMainMenuBMP, render)


    //Jouer
    SDL_Surface* imageHoverJouerBackground = NULL;
    SDL_Texture* textureHoverJouerBackground = NULL;
    ALLImageAndTransparencyINIT(imageHoverJouerBackground, textureHoverJouerBackground, HoverJouerMainMenuBMP, render)
    SDL_Rect rectButtonJouer;
    rectButtonJouer.x= 452;
    rectButtonJouer.y= 430;
    rectButtonJouer.w= 304;
    rectButtonJouer.h= 92;
    
    SDL_Surface* imageJouerBackground = NULL;
    SDL_Texture* textureJouerBackground = NULL;
    ALLImageAndTransparencyINIT(imageJouerBackground, textureJouerBackground, JouerMainMenuBMP, render)


    //Amis
    SDL_Surface* imageHoverAmisBackground = NULL;
    SDL_Texture* textureHoverAmisBackground = NULL;
    ALLImageAndTransparencyINIT(imageHoverAmisBackground, textureHoverAmisBackground, HoverAmisMainMenuBMP, render)
    SDL_Rect rectButtonAmis;
    rectButtonAmis.x= 452;
    rectButtonAmis.y= 545;
    rectButtonAmis.w= 304;
    rectButtonAmis.h= 92;
    
    SDL_Surface* imageAmisBackground = NULL;
    SDL_Texture* textureAmisBackground = NULL;
    ALLImageAndTransparencyINIT(imageAmisBackground, textureAmisBackground, AmisMainMenuBMP, render)


    //Options
    SDL_Surface* imageHoverOptionsBackground = NULL;
    SDL_Texture* textureHoverOptionsBackground = NULL;
    ALLImageAndTransparencyINIT(imageHoverOptionsBackground, textureHoverOptionsBackground, HoverOptionsMainMenuBMP, render)
    SDL_Rect rectButtonOptions;
    rectButtonOptions.x= 452;
    rectButtonOptions.y= 660;
    rectButtonOptions.w= 304;
    rectButtonOptions.h= 92;
    
    SDL_Surface* imageOptionsBackground = NULL;
    SDL_Texture* textureOptionsBackground = NULL;
    ALLImageAndTransparencyINIT(imageOptionsBackground, textureOptionsBackground, OptionsMainMenuBMP, render)


    //Quitter
    SDL_Surface* imageHoverQuitterBackground = NULL;
    SDL_Texture* textureHoverQuitterBackground = NULL;
    ALLImageAndTransparencyINIT(imageHoverQuitterBackground, textureHoverQuitterBackground, HoverQuitterMainMenuBMP, render)
    SDL_Rect rectButtonQuitter;
    rectButtonQuitter.x= 452;
    rectButtonQuitter.y= 774;
    rectButtonQuitter.w= 304;
    rectButtonQuitter.h= 92;
    
    SDL_Surface* imageQuitterBackground = NULL;
    SDL_Texture* textureQuitterBackground = NULL;
    ALLImageAndTransparencyINIT(imageQuitterBackground, textureQuitterBackground, QuitterMainMenuBMP, render)

    //FriendList
    SDL_Surface* imageMakeFriendListBackground = NULL;
    SDL_Texture* textureMakeFriendListBackground = NULL;
    ALLImageAndTransparencyINIT(imageMakeFriendListBackground, textureMakeFriendListBackground, FriendListMainMenuBMP, render)
    SDL_Rect rectButtonFriendList;
    rectButtonFriendList.x= 0;
    rectButtonFriendList.y= 45;
    rectButtonFriendList.w= 402;
    rectButtonFriendList.h= 1035;
    
    SDL_Surface* imageFriendListHiderBackground = NULL;
    SDL_Texture* textureFriendListHiderBackground = NULL;
    ALLImageAndTransparencyINIT(imageFriendListHiderBackground, textureFriendListHiderBackground, FriendListHiderMainMenuBMP, render)
    SDL_Rect rectButtonFriendListHider;
    rectButtonFriendListHider.x= 0;
    rectButtonFriendListHider.y= 0;
    rectButtonFriendListHider.w= 402;
    rectButtonFriendListHider.h= 1080;
    
    SDL_Event event;
    int continuer = 1;
    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_MOUSEMOTION:
                if(event.motion.x>1043 && event.motion.x<1595 && event.motion.y>340 && event.motion.y<955)
                {
                    SDL_RenderCopy(render, textureHoverPuzzleBackground, NULL, &rectButtonPuzzle);
                    SDL_RenderPresent(render);
                }
                else
                {
                    SDL_RenderCopy(render, texturePuzzleBackground, NULL, &rectButtonPuzzle);
                    SDL_RenderPresent(render);
                }
                if(event.motion.x>451 && event.motion.x<756 && event.motion.y>429 && event.motion.y<522)
                {
                    SDL_RenderCopy(render, textureHoverJouerBackground, NULL, &rectButtonJouer);
                    SDL_RenderPresent(render);
                }
                else
                {
                    SDL_RenderCopy(render, textureJouerBackground, NULL, &rectButtonJouer);
                    SDL_RenderPresent(render);
                }
                if(event.motion.x>451 && event.motion.x<756 && event.motion.y>544 && event.motion.y<637)
                {
                    SDL_RenderCopy(render, textureHoverAmisBackground, NULL, &rectButtonAmis);
                    SDL_RenderPresent(render);
                }
                else
                {
                    SDL_RenderCopy(render, textureAmisBackground, NULL, &rectButtonAmis);
                    SDL_RenderPresent(render);
                }
                if(event.motion.x>451 && event.motion.x<756 && event.motion.y>659 && event.motion.y<752)
                {
                    SDL_RenderCopy(render, textureHoverOptionsBackground, NULL, &rectButtonOptions);
                    SDL_RenderPresent(render);
                }
                else
                {
                    SDL_RenderCopy(render, textureOptionsBackground, NULL, &rectButtonOptions);
                    SDL_RenderPresent(render);
                }
                if(event.motion.x>451 && event.motion.x<756 && event.motion.y>773 && event.motion.y<866)
                {
                    SDL_RenderCopy(render, textureHoverQuitterBackground, NULL, &rectButtonQuitter);
                    SDL_RenderPresent(render);
                }
                else
                {
                    SDL_RenderCopy(render, textureQuitterBackground, NULL, &rectButtonQuitter);
                    SDL_RenderPresent(render);
                }
                break;
            case SDL_QUIT:
                continuer = 0;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (slideAmis==1)
                {
                    if (event.button.x<403 && event.button.y>34)
                    {
                        
                    }
                    else

                    {
                        slideAmis=0;
                        for (int i=16; i>-1; i--)
                        {
                            rectButtonFriendListHider.x= -25*i;
                            SDL_RenderCopy(render, textureFriendListHiderBackground, NULL, &rectButtonFriendListHider);
                            SDL_RenderPresent(render);
                            SDL_Delay(5);
                        }
                    }
                }
                if (event.button.x >=1875 && event.button.y <=45)
                {
                    if (doYouWantToQuitNoTime(render)==1)
                    {
                        *nextPage=1;
                        continuer=0;
                    }
                    else
                    {
                        SDL_RenderClear(render);
                        SDL_RenderCopy(render, textureBackgroundMenu, NULL, NULL);
                        SDL_RenderPresent(render);
                    }
                }
                else if (event.button.x>1043 && event.button.x<1595 && event.button.y>340 && event.button.y<955)
                {
                    //Puzzle
                }
                else if (event.button.x>451 && event.button.x<756 && event.button.y>429 && event.button.y<522)
                {
                    //Jouer
                }
                else if (event.button.x>451 && event.button.x<756 && event.button.y>544 && event.button.y<637)
                {
                    if (slideAmis==0)
                    {
                        slideAmis=1;
                        for (int i=16; i>-1; i--)
                        {
                            rectButtonFriendList.x= -25*i;
                            SDL_RenderCopy(render, textureMakeFriendListBackground, NULL, &rectButtonFriendList);
                            SDL_RenderPresent(render);
                            SDL_Delay(5);
                        }
                    }
                    else
                    {
                        slideAmis=0;
                        for (int i=16; i>-1; i--)
                        {
                            rectButtonFriendListHider.x= -25*i;
                            SDL_RenderCopy(render, textureFriendListHiderBackground, NULL, &rectButtonFriendListHider);
                            SDL_RenderPresent(render);
                            SDL_Delay(5);
                        }
                    }
                    //Amis
                }
                else if (event.button.x>451 && event.button.x<756 && event.button.y>659 && event.button.y<752)
                {
                    if (optionNoTime(render)==1)
                    {
                        //l'utilisateur veut se déconnecter
                        *nextPage=2;
                        continuer=0;
                    }
                    else
                    {
                        SDL_RenderClear(render);
                        SDL_RenderCopy(render, textureBackgroundMenu, NULL, NULL);
                        SDL_RenderPresent(render);
                    }
                    //Options
                }
                else if (event.button.x>451 && event.button.x<756 && event.button.y>773 && event.button.y<866)
                {
                    if (doYouWantToQuitNoTime(render)==1)
                    {
                        *nextPage=1;
                        continuer=0;
                    }
                    else
                    {
                        SDL_RenderClear(render);
                        SDL_RenderCopy(render, textureBackgroundMenu, NULL, NULL);
                        SDL_RenderPresent(render);
                    }
                    //Quitter
                }
                break;
            case SDL_KEYDOWN: 
                break;
        }
    }
}






int mainBoard(SDL_Window* window, SDL_Renderer* render, int* nextPage)
{

    //CreateRenderInNewWindow(window, render)
    SDL_RenderClear(render);
    initAllSurfaces()
    initAllTextures()
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

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
    int inverse=0;
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
            //SDL_Delay(5);
        }
    }

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
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
    SDL_Window* window = NULL;
    SDL_Renderer* render = NULL;
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
    TTF_Init();
    int nextPage=5;
    CreateRenderInNewWindow(window, render)
    while (nextPage!=1)
    {
        int test1 = nextPage;
        if (nextPage==2)
        {
            loginPage(window, render, &nextPage);
        }
        else if (nextPage==3)
        {
            inscriptionPage(window, render, &nextPage);
        }
        else if (nextPage==5)
        {
            mainMenuPage(window, render, &nextPage);
        }
        else if (nextPage==10)
        {
            mainBoard(window, render, &nextPage);
        }
        else if (nextPage==6)
        {
            modeSelectionPage(window, render, &nextPage);
        }
        else if (nextPage==7)
        {
            timeSelectionPage(window, render, &nextPage);
        }
        else
        {
            mainMenuPage(window, render, &nextPage);
        }
    }


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

//#undef NUM
//#define NUM 200