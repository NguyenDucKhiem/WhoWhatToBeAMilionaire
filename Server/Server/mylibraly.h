#include <WinSock2.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <WS2tcpip.h>
#include "constant.h"

#pragma comment(lib, "Ws2_32.lib")

// Inittiate WinSock
// if successful return 0, otherwrise it return error code
int InitWinSock()
{
	WSADATA wsaData;
	WORD wVerson = MAKEWORD(2, 2);
	if (WSAStartup(wVerson, &wsaData))
	{
		printf_s("Version is not supported");
		_getch();
		return WSAGetLastError();
	}
	return 0;
}
//Parse String to Interger
int ParseInt(char *number, int length) {
	int ret = 0;
	for (int i = 0; i < length; i++) {
		if (number[i] < '0' || number[i] > '9') return 0;
		ret += (number[i] - '0') * (int)pow(10, length - i - 1);
	}
	return ret;
}
/*
[OUT] *addr
[IN] sin_family
[IN] *sin_addr
[IN] sin_port

set addr
*/
void setSockAddrIn(sockaddr_in *addr, int sin_family, const char *sin_addr, int sin_port) {
	addr->sin_family = sin_family;
	addr->sin_addr.s_addr = inet_addr(sin_addr);
	addr->sin_port = htons(sin_port);
}


/*
[IN] Sock is Socket use
[IN] addr is sockaddr_in send
[OUT] buff
[IN] size buff

send messenge from addr and show display
*/
int SendMess(SOCKET Sock, sockaddr_in addr, char *buff, int size) {
	int ret;
	//receive message from client
	ret = send(Sock, buff, size, 0);
	if (ret == SOCKET_ERROR)
	{
		printf_s("Error: %d\n", WSAGetLastError());
		return -1;
	}
	return 0;
}

/*
[IN] Sock is Socket use
[IN] addr is sockaddr_in send
[OUT] buff
[IN] size buff
[IN] visible can show display

recive messenge from addr and show display
*/

int ReceiveMess(SOCKET connSock, char *buff, int size, int visible) {
	int ret;
	//receive message from client
	ret = recv(connSock, buff, size, 0);
	if (ret == SOCKET_ERROR)
	{
		if (visible != 0) printf_s("Error: %d", WSAGetLastError());
		return -1;
	}
	else
	{
		if (visible != 0) {
			buff[ret] = 0;
			printf_s("Receive: %s\n", buff);
		}
		return 0;
	}
}