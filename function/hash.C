#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

unsigned long long int jpow(unsigned long long int x, int n)
{
    unsigned long long int cpt = 1;
    for (int i = 0; i < n; i++)
    {
        cpt *= x;
        printf("test")
    }
    return cpt;
}

unsigned long long int main(int argc, char *argv[])
{
    char saisie[32];

    printf("\nC'est quoi ton nom maxi bg ?\n");
    printf(">> ");
    scanf("%s", saisie);
    printf("Salut %s, tu es vraiment un gros bg !\n", saisie);

    int longueurnom = strlen(saisie);
    printf("Ton nom %s fait %d caractere de long (tu es tres beau).\n\n", saisie, longueurnom);

    printf("\n");
    unsigned long long int cpt = 0;

    for (int i = 0; i < longueurnom; i++)
    {
        cpt += saisie[i] * jpow(128, i);
    }

    unsigned long long int mdp_ascii = cpt * 256;

    unsigned long long int temp = pow(10, 16);

    printf("%llu", mdp_ascii % temp);

    return (mdp_ascii % temp);
}
