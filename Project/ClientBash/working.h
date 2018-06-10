#pragma once

#include "mylibraly.h"
#include "message.h"
#include "player.h"

typedef struct GamePlay {
	int error;
	//int isLoading;		//wait revc from server
} GamePlay;

GamePlay gamePlay;

void AnalysisMess(Players* sockInfo) {
	Messages *mess = (Messages*)sockInfo->buff;

	TYPE_MSG typeMess = TypeMessage(mess);
	int resultMess = ResultMess(mess);

	//type error
	if (typeMess == TYPE_MSG::MESSAGE_ERROR || resultMess == -10) {
		sockInfo->step = STEP_GAME::ERROR_SERVER;
	}

	switch (sockInfo->step)
	{
	case STEP_GAME::CONNECT:
		break;
	case STEP_GAME::LOGIN:
		if (typeMess == TYPE_MSG::SERVER_REPLY_LOGIN) {
			if (resultMess == 1) {
				//accept login
				printf("%s.\n", mess->data);

				sockInfo->step = STEP_GAME::WAIT;
				break;
			}
			else if (resultMess == -2) {
				// block user
				printf("%s.\n", mess->data);

				break;
			} else if (resultMess == -3) {
				// wrong password or user
				printf("%s.\n", mess->data);

				break;
			}
			else if (resultMess == -4) {
				//user is login
				printf("%s.\n", mess->data);

				break;
			}
			else {
				// error result Mess
				printf("error result Mess.\n");

				break;
			}

		}
		else if (typeMess == TYPE_MSG::SERVER_ERROR) {
			//server error
			printf("%s.\n", mess->data);

			break;
		}
		else {
			//wrong type error
			gamePlay.error = -1;
			printf("Wrong type error.\n");

			break;
		}
	case STEP_GAME::WAIT:
		break;
	case STEP_GAME::WAIT_PLAY:
		break;
	case STEP_GAME::QUESTION_BEGIN:
		break;
	case STEP_GAME::QUESTION:
		break;
	case STEP_GAME::RESULT:
		break;
	case STEP_GAME::STREAM:
		break;
	case STEP_GAME::HELP:
		break;
	default:
		break;
	}
}

int ProcessingConnect(Players* sockInfo) {
	if (!sockInfo->client)
		closesocket(sockInfo->client);
	char addr[20];
	int port = 0;
	printf_s("Addr: ");
	scanf("%s", addr);
	printf_s("port: ");
	scanf("%d", &port);
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr(addr);

	//Step4: Request to connect server
	if (connect(sockInfo->client, (sockaddr*)&serverAddr, sizeof(serverAddr)))
	{
		printf_s("Error! Cannot connet server. %d", WSAGetLastError());
		_getch();
		//gamePlay.error = -4;
		return -1;
	}

	printf_s("Connet to server!\n");
	sockInfo->step = STEP_GAME::LOGIN;
	return 1;
}

int ProcessingLogin(Players* sockInfo) {
	DWORD sendByte;
	char user[SIZE_USERID], pass[SIZE_PASSWORD];
	//fflush(stdin);
	printf_s("User: ");
	scanf("%s", user);
	//fflush(stdin);
	printf_s("Pass: ");
	scanf("%s", pass);

	char data[MSG_LENGTH_DATA];
	sprintf(data, "%s %s", user, pass);
	SetMessage((Messages*) sockInfo->buff, MSG_CLIENT_REQUEST_LOGIN, "+0", data);

	return 1;
}

int Processing(Players* sockInfo) {
	if (gamePlay.error) return gamePlay.error;
	switch (sockInfo->step)
	{
	case STEP_GAME::CONNECT:
		return -1;
	case STEP_GAME::LOGIN:
		return ProcessingLogin(sockInfo);
	case STEP_GAME::WAIT:
		break;
	case STEP_GAME::WAIT_PLAY:
		break;
	case STEP_GAME::QUESTION_BEGIN:
		break;
	case STEP_GAME::QUESTION:
		break;
	case STEP_GAME::RESULT:
		break;
	case STEP_GAME::STREAM:
		break;
	case STEP_GAME::HELP:
		break;
	default:
		break;
	}
	return 1;
}



void CALLBACK WorkingRoutine(DWORD error, DWORD transferredBytes, LPWSAOVERLAPPED overlapped, DWORD inFlags) {
	DWORD sendByte, recvBytes;
	DWORD flags;
	Players* sockInfo = (Players*)overlapped;

	flags = 0;
	ZeroMemory(&(sockInfo->overlap), sizeof(WSAOVERLAPPED));

	if (sockInfo->operation == SEND) {
		sockInfo->operation = RECV;

		if (WSARecv(sockInfo->client, &(sockInfo->dataBuff), 1, &recvBytes, &flags, &(sockInfo->overlap), WorkingRoutine) == SOCKET_ERROR) {
			if (WSAGetLastError() != WSA_IO_PENDING) {
				printf("WSARecv() failed with error %d\n", WSAGetLastError());
				gamePlay.error = -3;
				return;
			}
		}
	}
	else {
		AnalysisMess(sockInfo);
		if (Processing(sockInfo) > 0) {
			sockInfo->operation = SEND;
			if (WSASend(sockInfo->client, &(sockInfo->dataBuff), 1, &sendByte, 0, &(sockInfo->overlap), WorkingRoutine) == SOCKET_ERROR) {
				if (WSAGetLastError() != WSA_IO_PENDING) {
					printf("WSASend() failed with error %d\n", WSAGetLastError());
					gamePlay.error = -2;
					return;
				}
			}
		}
	}

}