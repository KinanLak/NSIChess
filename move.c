
#include "move.h"


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

