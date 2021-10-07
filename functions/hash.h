#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


unsigned long long int jpow(unsigned long long int x, int n);
int randomNumberBetweenOneAndTwo(void);


unsigned long long int jpow(unsigned long long int x, int n)
{
    unsigned long long int cpt = 1;
    for (int i = 0; i < n; i++)
    {
        cpt *= x;
    }
    return cpt;
}

unsigned long long int main(int argc, char *argv[])
{
    char saisie[128];

    printf("\nEntre ton mot de passe? \n>> ");
    scanf("%s", &saisie);


    int longueurnom = strlen(saisie);
    printf("Le mot de passe %s fait %d caractere de long.\n", saisie, longueurnom);
    unsigned long long int cpt = 0;

    for (int i = 0; i < longueurnom; i++)
    {
        cpt += saisie[i] * jpow(128, i);
    }

    unsigned long long int mdp_ascii = cpt * 256;

    unsigned long long int temp = pow(10, 16);

    printf("Ton mot de passe hashe est :\n%llu", mdp_ascii % temp);

    return (mdp_ascii % temp);
}
