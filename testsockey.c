#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib, "ws2_32.lib")
int main()
{
    WSADATA WSAData;
    SOCKET sock;
    SOCKET csock;
    SOCKADDR_IN sin;
    SOCKADDR_IN csin;
    WSAStartup(MAKEWORD(2, 0), &WSAData);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(80);
    bind(sock, (SOCKADDR *)&sin, sizeof(sin));
    listen(sock, 0);
    printf("Niveau du While\n");
    while (1) /* Boucle infinie. Exercice : améliorez ce code. */
    {
        printf("Avant le sinsize\n");
        int sinsize = sizeof(csin);
        printf("Avant le if\n");
        if ((csock = accept(sock, (SOCKADDR *)&csin, &sinsize)) != INVALID_SOCKET)
        {
            printf("Avant le send\n");
            char test[] = "0A";
            send(csock, test, 3, 0);
            closesocket(csock);
            printf("Socket valide\n");
        }
        else {
            printf("Erreur socket invalide\n");
        }
    }
    printf("Après le while\n");
    /*; On devrait faire closesocket(sock); puis WSACleanup(); mais puisqu'on a entré une boucle infinie ... */
    return 0;
}