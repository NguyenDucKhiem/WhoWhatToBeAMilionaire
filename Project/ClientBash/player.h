#pragma once
#include "user.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <process.h>

#pragma comment(lib, "Ws2_32.lib")

// Define States
typedef struct
{
	WSAOVERLAPPED overlap;
	SOCKET client;
	DWORD recvByte;
	DWORD sendByte;
	char buff[BUFF_SIZE];
	WSABUF dataBuff;
	int operation;			// trạng thái SEND hoặc RECV

	Users *userPlayer;
	int indexInListAcc;		// vị trí user trong mảng tài khoản
	int helpCall;
	int typePlayer;
	int step;				//đang ở bước nào người chơi phụ hay người chơi chính
	int orderquestion;		//câu hỏi 1,2,… (nếu flag == người chơi)
	int isSend;				//gói tin đã được gửi hay chưa
} Players;



/*
[IN] userID
[IN] clientAddr
[IN] status

init State and return it
*/
Players *InitPlayer(SOCKET client) {
	Players *player = (Players*)malloc(sizeof(Players));

	player->client = client;
	ZeroMemory(&(player->overlap), sizeof(WSAOVERLAPPED));

	player->operation = RECV;
	player->orderquestion = -1;
	player->userPlayer = NULL;
	player->recvByte = 0;
	player->sendByte = 0;
	player->dataBuff.buf = player->buff;
	player->dataBuff.len = strlen(player->buff);
	player->helpCall = 15;
	player->typePlayer = TYPE_PLAYER_WAITTING;
	player->step = STEP_GAME::CONNECT;
	player->orderquestion = 0;
	player->isSend = 1;
	return player;
}

/*
[IN] numberUsers: number User initialization

initialization list states and return it
*/
Players **InitListPlayer(int numberUsers) {
	Players **listStates = (Players**)malloc(numberUsers * sizeof(Players*));

	for (int i = 0; i < numberUsers; i++) {
		listStates[i] = NULL;
	}
	return listStates;
}

