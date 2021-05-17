#include <stdio.h>
#include <stdlib.h>


typedef struct MoveStructure MoveStructure;
struct MoveStructure
{
    int departureCase;
    int arrivalCase;
    MoveStructure *nextMove;
};

typedef struct FileMoveStructure FileMoveStructure;
struct FileMoveStructure
{
    MoveStructure *firstMove;
};

FileMoveStructure *initialiser();
void enfiler(FileMoveStructure *file, int newDepartureCase, int newArrivalCase);
int defiler(FileMoveStructure *file);
void afficherFile(FileMoveStructure *file);




int main()
{
    FileMoveStructure *maFile = initialiser();

    enfiler(maFile, 4, 5);
    enfiler(maFile, 8, 9);
    enfiler(maFile, 15, 16);
    enfiler(maFile, 16, 17);
    enfiler(maFile, 23, 24);
    enfiler(maFile, 42, 43);

    printf("\nEtat de la file :\n");
    afficherFile(maFile);

    printf("\nEtat de la file :\n");
    afficherFile(maFile);

    return 0;
}

FileMoveStructure *initialiser()
{
    FileMoveStructure *file = malloc(sizeof(*file));
    file->firstMove = NULL;

    return file;
}


void enfiler(FileMoveStructure *file, int newDepartureCase, int newArrivalCase)
{
    MoveStructure *new = malloc(sizeof(*new));
    if (file == NULL || new == NULL)
    {
        exit(EXIT_FAILURE);
    }

    new->departureCase = newDepartureCase;
    new->arrivalCase = newArrivalCase;
    new->nextMove = NULL;

    if (file->firstMove != NULL) /* La file n'est pas vide */
    {
        /* On se positionne à la fin de la file */
        MoveStructure *MoveStructureActuel = file->firstMove;
        while (MoveStructureActuel->nextMove != NULL)
        {
            MoveStructureActuel = MoveStructureActuel->nextMove;
        }
        MoveStructureActuel->nextMove = new;
    }
    else /* La file est vide, notre élément est le firstMove */
    {
        file->firstMove = new;
    }
}


void afficherFile(FileMoveStructure *file)
{
    if (file == NULL)
    {
        exit(EXIT_FAILURE);
    }

    MoveStructure *MoveStructure = file->firstMove;

    while (MoveStructure != NULL)
    {
        printf("%d -> %d     ", MoveStructure->departureCase, MoveStructure->arrivalCase);
        MoveStructure = MoveStructure->nextMove;
    }

    printf("\n");
}


/* To change for sql
int defiler(FileMoveStructure *file)
{
    if (file == NULL)
    {
        exit(EXIT_FAILURE);
    }

    int nombreDefile = 0;

    // On vérifie s'il y a quelque chose à défiler
    if (file->firstMove != NULL)
    {
        MoveStructure *elementDefile = file->firstMove;

        nombreDefile = elementDefile->departureCase;
        file->firstMove = elementDefile->nextMove;
        free(elementDefile);
    }

    return nombreDefile;
}
*/