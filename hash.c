#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "hash.h"


int main(int argc, char *argv[])
{
    char saisie[32];

    printf("\nC'est quoi ton nom maxi bg ?\n>> ");
    scanf("%s", saisie);
    int longueurnom = strlen(saisie);
    printf("Ton nom %s fait %d caractere de long (tu es tres beau).\n\n", saisie, longueurnom);

    unsigned long long int cpt = 0;
    for (int i = 0; i < longueurnom; i++)
    {
        cpt += saisie[i] * ULLIpow(128, i);
    }

    unsigned long long int mdp_ascii = cpt * 256;
    unsigned long long int temp = pow(10, 16);

    printf("%llu", mdp_ascii % temp);

    return (mdp_ascii % temp);
}

int randomNumberBetweenOneAndTwo()
{
    srand(time(0));
    return rand()%2;
}


unsigned long long int ULLIpow(unsigned long long int x, int n)
{
    /* Function power that elevate the numnber x to power n 
    It uses unsigned long long int variable which has a maximum value of 18,446,744,073,709,551,615*/
    unsigned long long int cpt = 1;
    for (int i = 0; i < n; i++)
    {
        cpt *= x;
    }
    return cpt;
}