#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
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
void storeAllMovesSQL(FileMoveStructure *file);


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

void storeAllMovesSQL(FileMoveStructure *file)
{
    if (file == NULL)
    {
        exit(EXIT_FAILURE);
    }

    MoveStructure *MoveStructure = file->firstMove;

    //récuperer le plus grand moveId qui existe pour le moment
    // et le rentrer dans la variable en dessous

    while (MoveStructure != NULL)
    {
        printf("%d -> %d \n", MoveStructure->departureCase, MoveStructure->arrivalCase);
        //Manque la requête qui ajoute le mouvement
        // INSERT INTO table_name
        // VALUES (moveId, MoveStructure->departureCase, MoveStructure->arrivalCase, gameId);
        MoveStructure = MoveStructure->nextMove;
        //We have to free -> elementDepile (check how to do that)
    }

}



void listMovesToFile(FileMoveStructure* file, char* listMoves)
{
    int cpt=0;
    int departure=0;
    int arrival=0;
    int lenListMoves = (strlen(listMoves)+1)/5;
    for (int x=0; x<lenListMoves; x++)
    {
        departure = ((8-(listMoves[cpt+1]-48))*8) + (listMoves[cpt]-97);
        cpt+=2;

        arrival = ((8-(listMoves[cpt+1]-48))*8) + (listMoves[cpt]-97);
        cpt+=3;

        addMoveFile(file, departure, arrival);
    }
}

void fileToListMoves(FileMoveStructure* file, int numberMoves)
{
    //
    //
    //
    // Don't work
    //
    //
    char* listMoves[(numberMoves*5)-1];
    MoveStructure *MoveStructure = file->firstMove;

    *listMoves[0] = 97 + ((MoveStructure->departureCase)%8);
    *listMoves[1] = 56 - ((MoveStructure->departureCase)/8);
    *listMoves[2] = 97 + ((MoveStructure->arrivalCase)%8);
    *listMoves[3] = 56 - ((MoveStructure->arrivalCase)/8);
    printf("\n%d", strlen(listMoves));

    MoveStructure = MoveStructure->nextMove;
    for (int i=1; i<numberMoves; i++)
    {
        listMoves[(i*5)-1] = 32;
        printf("\n%d", strlen(listMoves));
        listMoves[(i*5)] = 97 + ((MoveStructure->departureCase)%8);
        printf("\n%d", strlen(listMoves));
        listMoves[(i*5)+1] = 56 - ((MoveStructure->departureCase)/8);
        printf("\n%d", strlen(listMoves));
        listMoves[(i*5)+2] = 97 + ((MoveStructure->arrivalCase)%8);
        printf("\n%d", strlen(listMoves));
        listMoves[(i*5)+3] = 56 - ((MoveStructure->arrivalCase)/8);
        printf("\n%d", strlen(listMoves));
        MoveStructure = MoveStructure->nextMove;
    }
    
    printf("\n%d ---- %s", strlen(listMoves), listMoves);
}

int main(int argc, char* argv[])
{
    char listMoves[] = "g6h4 g6h4";
    FileMoveStructure* file=initialise();
    listMovesToFile(file, listMoves);
    storeAllMovesSQL(file);
    printf("\n\n");
    fileToListMoves(file, 2);


    return 1;
}


void FENToList(char* fen, unsigned int* chessBoard)
{
    int cptStr=0;
    int cptChessBoard=0;
    int valueStr = fen[cptStr];
    while (valueStr != 32)
    {
        if (valueStr != 47)
        {
            if ((valueStr > 48) && (valueStr <58))
            {
                for (int x=0; x<(valueStr-48); x++)
                {
                    chessBoard[cptChessBoard]=0;
                    cptChessBoard+=1;
                }
            }
            else if (valueStr<91)
            {
                if (valueStr == 80)
                {
                    chessBoard[cptChessBoard]=9;
                    cptChessBoard+=1;
                }
                else if (valueStr == 78)
                {
                    chessBoard[cptChessBoard]=10;
                    cptChessBoard+=1;
                }
                else if (valueStr == 66)
                {
                    chessBoard[cptChessBoard]=11;
                    cptChessBoard+=1;
                }
                else if (valueStr == 82)
                {
                    chessBoard[cptChessBoard]=12;
                    cptChessBoard+=1;
                }
                else if (valueStr == 81)
                {
                    chessBoard[cptChessBoard]=14;
                    cptChessBoard+=1;
                }
                else if (valueStr == 75)
                {
                    chessBoard[cptChessBoard]=15;
                    cptChessBoard+=1;
                }
            }
            else
            {
                if (valueStr == 112)
                {
                    chessBoard[cptChessBoard]=1;
                    cptChessBoard+=1;
                }
                else if (valueStr == 110)
                {
                    chessBoard[cptChessBoard]=2;
                    cptChessBoard+=1;
                }
                else if (valueStr == 98)
                {
                    chessBoard[cptChessBoard]=3;
                    cptChessBoard+=1;
                }
                else if (valueStr == 114)
                {
                    chessBoard[cptChessBoard]=4;
                    cptChessBoard+=1;
                }
                else if (valueStr == 113)
                {
                    chessBoard[cptChessBoard]=6;
                    cptChessBoard+=1;
                }
                else if (valueStr == 107)
                {
                    chessBoard[cptChessBoard]=7;
                    cptChessBoard+=1;
                }
            }
        }
        cptStr+=1;
        valueStr = fen[cptStr];
    }
}

void printBoard(unsigned int* chessBoard)
{
    for (int x=0; x<8; x++)
    {
        for (int y=0; y<8; y++)
        {
            printf("%d ", chessBoard[(x*8)+y]);
        }
        printf("\n");
    }
}




/*int main(int argc, char* argv[])
{
    unsigned int chessBoard[64];

    char fen[] = "r2qkb1r/p2b1ppp/4pn2/1B1p4/8/1Q2P3/PP1N1PPP/R1B1K2R w KQkq - 2 11";
    FENToList(fen, chessBoard);
    printf("1");

    printBoard(chessBoard);

    return 1;
}*/


/*int main(int argc, char* argv[])
{
    sqlite3 *db;
    sqlite3_open("database.db", &db);
    sqlite3_exec(db, "INSERT INTO Puzzle_done VALUES (0, 0, 1, '2021-06-08      ', 2000);", NULL, NULL, NULL);
    sqlite3_close(db);

    return 1;
}*/