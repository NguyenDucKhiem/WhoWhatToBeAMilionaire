// ClientBash.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "working.h"

Players *player;

int main()
{
	WSADATA wsaData;
	WORD wVerson = MAKEWORD(2, 2);
	if (WSAStartup(wVerson, &wsaData))
		printf_s("Version is not supported\n");
	printf_s("Client started.\n");

	//Step2: Construct socket
	SOCKET client;
	client = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	//Optional Set time-out for receiving
	int tv = 10000; //Time-out interval: 10000ms
	setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, (const char*)(&tv), sizeof(int));

	gamePlay.error = 0;

	player = InitPlayer(client);

	do {
		if (player->step == STEP_GAME::CONNECT) {
			if (ProcessingConnect(player) == 1) {
				ProcessingLogin(player);
				player->operation = SEND;
				if (WSASend(player->client, &(player->dataBuff), 1, &player->sendByte, 0, &(player->overlap), WorkingRoutine) == SOCKET_ERROR) {
					if (WSAGetLastError() != WSA_IO_PENDING) {
						printf("WSASend() failed with error %d\n", WSAGetLastError());
						gamePlay.error = -2;
						player->step = STEP_GAME::CONNECT;
						continue;
					}
				}
			}
		}
	} while (1);

	//Step 5: Close socket
	closesocket(client);

	//Step 6: Terminate Winsock
	WSACleanup();

	return 0;

}

