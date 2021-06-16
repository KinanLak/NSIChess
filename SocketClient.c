#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib, "ws2_32.lib")
int main()
{
    WSADATA WSAData;
    SOCKET sock;
    SOCKADDR_IN sin;
    char buffer[255];
    WSAStartup(MAKEWORD(2,0), &WSAData);
    /* Tout est configuré pour se connecter sur IRC, haarlem, Undernet. */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    sin.sin_addr.s_addr = inet_addr("86.195.96.56");
    sin.sin_family = AF_INET;
    sin.sin_port = htons(80);
    connect(sock, (SOCKADDR *)&sin, sizeof(sin));
    recv(sock, buffer, sizeof(buffer), 0);
    printf("%d", atoi(buffer));
    closesocket(sock);
    WSACleanup();
    return 0;
}
