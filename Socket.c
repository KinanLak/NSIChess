#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
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
    sin.sin_addr.s_addr = inet_addr("90.90.34.8");
    sin.sin_family = AF_INET;
    sin.sin_port = htons(6667);
    connect(sock, (SOCKADDR *)&sin, sizeof(sin));
    int caca = 0;
    while (caca == 0) {
        recv(sock, buffer, 1, 0);
        printf("%c", buffer);
        }
    closesocket(sock);
    WSACleanup();
    return 0;   
}