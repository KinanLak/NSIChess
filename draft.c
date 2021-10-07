#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
//#include <time.h>
//#include <string.h>
//#include <mysql.h>
//#include "sqlite3.h"

int main(int argc, char *argv[]) {



}


/*

//Creation of the structure for each move
typedef struct MoveStructure MoveStructure;
struct MoveStructure
{
    int departureCase;
    int arrivalCase;
    int bonus;
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
void addMoveFile(FileMoveStructure *file, int newDepartureCase, int newArrivalCase, int bonus);
void storeAllMovesSQL(FileMoveStructure *file);


FileMoveStructure *initialise()
{
    FileMoveStructure *file = malloc(sizeof(*file));
    file->firstMove = NULL;

    return file;
}


void addMoveFile(FileMoveStructure *file, int newDepartureCase, int newArrivalCase, int bonus)
{
    MoveStructure *new = malloc(sizeof(*new));
    if (file == NULL || new == NULL)
    {
        exit(EXIT_FAILURE);
    }

    new->departureCase = newDepartureCase;
    new->arrivalCase = newArrivalCase;
    new->bonus = bonus;
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

int deleteMoveFile(FileMoveStructure *file)
{
    if (file->firstMove!=NULL)
    {
        file->firstMove = (file->firstMove)->nextMove;
        return 0;
    }
    else
    {
        return 1;
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
        printf("%d -> %d (%d)\n", MoveStructure->departureCase, MoveStructure->arrivalCase, MoveStructure->bonus);
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
        cpt+=2;
        if (listMoves[cpt]==32)
        {
            cpt+=1;
            addMoveFile(file, departure, arrival, 0);
        }
        else
        {
            int BonusNB=0;
            if (listMoves[cpt]==113)
            {
                BonusNB=6;
            }
            else if (listMoves[cpt]==114)
            {
                BonusNB=4;
            }
            else if (listMoves[cpt]==110)
            {
                BonusNB=3;
            }
            else if (listMoves[cpt]==98)
            {
                BonusNB=2;
            }
            addMoveFile(file, departure, arrival, BonusNB);
            cpt+=2;
        }
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


void FENToList(char* fen, unsigned int* chessBoard, int* rock, int* teamToPlay, int* enPassant)
{
    int cptStr=0;
    int cptChessBoard=0;
    while (fen[cptStr] != 32)
    {
        if (fen[cptStr] != 47)
        {
            if ((fen[cptStr] > 48) && (fen[cptStr] <58))
            {
                for (int x=0; x<(fen[cptStr]-48); x++)
                {
                    chessBoard[cptChessBoard]=0;
                    cptChessBoard+=1;
                }
            }
            else if (fen[cptStr]<91)
            {
                if (fen[cptStr] == 80)
                {
                    chessBoard[cptChessBoard]=9;
                    cptChessBoard+=1;
                }
                else if (fen[cptStr] == 78)
                {
                    chessBoard[cptChessBoard]=10;
                    cptChessBoard+=1;
                }
                else if (fen[cptStr] == 66)
                {
                    chessBoard[cptChessBoard]=11;
                    cptChessBoard+=1;
                }
                else if (fen[cptStr] == 82)
                {
                    chessBoard[cptChessBoard]=12;
                    cptChessBoard+=1;
                }
                else if (fen[cptStr] == 81)
                {
                    chessBoard[cptChessBoard]=14;
                    cptChessBoard+=1;
                }
                else if (fen[cptStr] == 75)
                {
                    chessBoard[cptChessBoard]=15;
                    cptChessBoard+=1;
                }
            }
            else
            {
                if (fen[cptStr] == 112)
                {
                    chessBoard[cptChessBoard]=1;
                    cptChessBoard+=1;
                }
                else if (fen[cptStr] == 110)
                {
                    chessBoard[cptChessBoard]=2;
                    cptChessBoard+=1;
                }
                else if (fen[cptStr] == 98)
                {
                    chessBoard[cptChessBoard]=3;
                    cptChessBoard+=1;
                }
                else if (fen[cptStr] == 114)
                {
                    chessBoard[cptChessBoard]=4;
                    cptChessBoard+=1;
                }
                else if (fen[cptStr] == 113)
                {
                    chessBoard[cptChessBoard]=6;
                    cptChessBoard+=1;
                }
                else if (fen[cptStr] == 107)
                {
                    chessBoard[cptChessBoard]=7;
                    cptChessBoard+=1;
                }
            }
        }
        cptStr+=1;
    }
    cptStr+=1;
    if (fen[cptStr]==119)
    {
        *teamToPlay=1;
    }
    else
    {
        *teamToPlay=0;
    }
    cptStr+=2;
    if (fen[cptStr]==45)
    {
        cptStr+=1;
    }
    else
    {
        while (fen[cptStr] != 32)
        {
            if (fen[cptStr]==113)
            {
                *rock+=1;
            }
            else if (fen[cptStr]==107)
            {
                *rock+=2;
            }
            else if (fen[cptStr]==81)
            {
                *rock+=4;
            }
            else if (fen[cptStr]==75)
            {
                *rock+=8;
            }
            cptStr+=1;
        }
    }
    cptStr+=1;
    if (fen[cptStr]==45)
    {
        cptStr+=1;
    }
    else
    {
        *enPassant = fen[cptStr]-97 + (56-fen[cptStr+1])*8;
        if (*teamToPlay==1)
        {
            *enPassant+=8;
        }
        else
        {
            *enPassant-=8;
        }
        cptStr+=2;
    }
    printf("\nwho to play -> %d \nrock -> %d \nenPassant -> %d\n\n", *teamToPlay, *rock, *enPassant);
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


void shrinkChar(char* longChar, int sizeNewChar)
{
    char newChar[sizeNewChar];
    for (int i=0; i<sizeNewChar; i++)
    {
        newChar[i] = longChar[i];
    }
    printf("%s", newChar);
}

int maxValueList(int* charValues, int lengthChar)
{
    int max=0; //all values of int >0
    for (int i=0; i<lengthChar; i++)
    {
        if (max<charValues[i])
        {
            max=charValues[i];
        }
    }
    return max;
}

int minValueList(int* charValues, int lengthChar)
{
    int min=10000; //all values of int <10 000
    for (int i=0; i<lengthChar; i++)
    {
        if (min>charValues[i])
        {
            min=charValues[i];
        }
    }
    return min;
}



int charHelp(char* ptChar)
{
    for (int i=0; i<6; i++)
    {
        if (ptChar[i]!=48)
        {
            return 0;
        }
    }
    return 1;
}
typedef struct DataPlayerRank DataPlayerRank;
struct DataPlayerRank
{
    int rank;
    char FirstName[30];
    int score;
    int numberPuzzlePlayed;
};

int main(int argc, char* argv[])
{

    char request[]="SELECT prenom, puzzle_score, (Select count(*) from Puzzle_done where user_id=User.user_id) From User order by puzzle_score desc;";

    MYSQL *con = mysql_init(NULL);
    if (mysql_real_connect(con, "logames.fr", "truc", "Test.123", "pokedex", 3306, NULL, 0)==NULL)
    {
        return 13;
    }
    if (mysql_query(con, request))
    {
        return 12;
    }
    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL)
    {
        return 11;
    }

    int num_fields = mysql_num_fields(result);

    int numberValues = mysql_affected_rows(con);

    DataPlayerRank list[numberValues];
    MYSQL_ROW row = mysql_fetch_row(result);

    for (int i=0; i<numberValues; i++)
    {
        list[i].rank = i+1;
        list[i].numberPuzzlePlayed = atoi(row[2]);
        list[i].score = atoi(row[1]);
        strcpy( list[i].FirstName, row[0]);
        
        row = mysql_fetch_row(result);
    }

    for (int i=0; i<numberValues; i++)
    {
        printf("%d, %d, %d, %s\n", list[i].rank, list[i].numberPuzzlePlayed, list[i].score, list[i].FirstName);
    }

    mysql_free_result(result);
    mysql_close(con);

    return 1;
}


int main(int argc, char* argv[])
{
    sqlite3 *db;
    sqlite3_open("database.db", &db);
    sqlite3_exec(db, "INSERT INTO Puzzle_done VALUES (0, 0, 1, '2021-06-08      ', 2000);", NULL, NULL, NULL);
    sqlite3_close(db);

    return 1;
}

*/