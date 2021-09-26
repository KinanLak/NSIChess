#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib, "ws2_32.lib")

int envoie(char paquet[])
{
    WSADATA WSAData;
    SOCKET sock;
    SOCKET csock;
    SOCKADDR_IN sin;
    SOCKADDR_IN csin;

    int sinsize = sizeof(csin);
    WSAStartup(MAKEWORD(2, 0), &WSAData);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(80);

    bind(sock, (SOCKADDR *)&sin, sizeof(sin));
    listen(sock, 0);

    csock = accept(sock, (SOCKADDR *)&csin, &sinsize);
    send(csock, paquet, 16, 0);

    WSACleanup();
    closesocket(csock);
    return 0;
}

int reception(char IP[])
{
    WSADATA WSAData;
    SOCKET sock;
    SOCKADDR_IN sin;

    char buffer[255];
    WSAStartup(MAKEWORD(2, 0), &WSAData);
    sock = socket(AF_INET, SOCK_STREAM, 0);

    sin.sin_addr.s_addr = inet_addr(IP);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(80);
    connect(sock, (SOCKADDR *)&sin, sizeof(sin));
    recv(sock, buffer, sizeof(buffer), 0);
    printf("%s", buffer);

    closesocket(sock);
    WSACleanup();
    return 0;
}
int main()
{
    char paquet[] = "gros chien";
    envoie(paquet);
    
    char IP[] = "86.195.96.56";
    reception(IP);
    return 0;
}
