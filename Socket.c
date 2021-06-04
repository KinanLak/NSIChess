#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")

int main()
{
    WSADATA WSAData;
    SOCKET sock;
    SOCKADDR_IN sin;
    char buffer[255];
    WSAStartup(MAKEWORD(2, 0), &WSAData);
    /* Tout est configuré pour se connecter sur IRC, haarlem, Undernet. */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    sin.sin_addr.s_addr = inet_addr("85.14.144.155");
    sin.sin_family = AF_INET;
    sin.sin_port = htons(6667);
    connect(sock, (SOCKADDR *)&sin, sizeof(sin));
    int caca = 0;
    while (caca = 0){
        printf("%d",recv(sock, buffer, sizeof(buffer), 0));
        };
    closesocket(sock);
    WSACleanup();
    return 0;
}