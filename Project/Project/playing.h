#pragma once

#include "mylibraly.h"
#include "message.h"
#include "player.h"
#include "question.h"

Players *playersPlaying[MAX_CLIENT_PLAYING];
WSAEVENT eventsPlaying[MAX_CLIENT_PLAYING];
CRITICAL_SECTION playCrt;

struct InfoGame
{
	int isPlaying = 0;
	int nPlayer = 0;			//number player
	int indexPlayer = -1;			//index main player
	STEP_GAME stepGame = STEP_GAME::WAIT_PLAY;
	int isSend[10];
	Questions questions;
};

InfoGame inforGame;

int SendAllPlayer(Players **listPlayer, int nPlayer, Messages *mess) {
	for (int i = 0; i < nPlayer; i++) {
		listPlayer[i]->operation = SEND;
		if (WSASend(listPlayer[i]->client, &(listPlayer[i]->dataBuff), 1, &listPlayer[i]->sendByte, 0, &(listPlayer[i]->overlap), NULL) == SOCKET_ERROR) {
			if (WSAGetLastError() != WSA_IO_PENDING) {
				printf("WSASend() failed with error %d\n", WSAGetLastError());
				return -1;
			}
		}
	}
	return 1;
}

void ClosePlayingPlayer(int index) {
	EnterCriticalSection(&playCrt);

	WSACloseEvent(eventsPlaying[index]);
	eventsPlaying[index] = eventsPlaying[inforGame.nPlayer - 1];
	eventsPlaying[inforGame.nPlayer - 1] = 0;
	closesocket(playersPlaying[index]->client);
	GlobalFree(playersPlaying[index]);

	playersPlaying[index] = playersPlaying[inforGame.nPlayer - 1];
	inforGame.nPlayer--;

	LeaveCriticalSection(&playCrt);
}

int ProcessingPlayingMain(Players* sockInfo) {
	Messages *mess = (Messages*)sockInfo->buff;
	if (sockInfo->typePlayer != TYPE_PLAYER_WAITTING)
		return -1;

	TYPE_MSG typeMess = TypeMessage(mess);
	int resultMess = ResultMess(mess);

	//type error
	if (typeMess == TYPE_MSG::MESSAGE_ERROR || resultMess == -10)
		return -2;
	//step game
	switch (sockInfo->step)
	{
	case STEP_GAME::QUESTION:
		break;
	case STEP_GAME::HELP:
		break;
	case STEP_GAME::RESULT:
		break;
	default:
		break;
	}

	return SendAllPlayer(playersPlaying, inforGame.nPlayer, mess);
}

int  ProcessingPlayingSub(Players* sockInfo) {
	Messages *mess = (Messages*) sockInfo->buff;
	if (sockInfo->typePlayer != TYPE_PLAYER_WAITTING)
		return -1;

	TYPE_MSG typeMess = TypeMessage(mess);
	int resultMess = ResultMess(mess);

	//type error
	if (typeMess == TYPE_MSG::MESSAGE_ERROR || resultMess == -10)
		return -2;
	//step game
	switch (inforGame.stepGame)
	{
	case STEP_GAME::QUESTION_BEGIN:
		if (typeMess == TYPE_MSG::CLIENT_ANSWER_QUESTION) {
			if (resultMess == inforGame.questions.answer);
		}
		break;
	case STEP_GAME::STREAM:
		break;
	default:
		break;
	}

	return 1;
}

void CALLBACK PlayingRoutine(DWORD error, DWORD transferredBytes, LPWSAOVERLAPPED overlapped, DWORD inFlags) {
	DWORD sendBytes, recvBytes;
	DWORD flags;
	Players* sockInfo = (Players*)overlapped;

	//find socket
	int i;
	for (i = 0; i < inforGame.nPlayer; i++) {
		if (playersPlaying[i]->client == sockInfo->client)
			break;
	}

	
	// Reference the WSAOVERLAPPED structure as a SOCKET_INFORMATION structure
	
	if (sockInfo == NULL) return;
	if (error != 0)
		printf("I/O operation failed with error %d\n", error);

	if (transferredBytes == 0)
		printf("Closing socket %d\n\n", sockInfo->client);

	if (error != 0 || transferredBytes == 0) {
		//remove socket
		
		ClosePlayingPlayer(i);

		return;
	}
	inforGame.isSend[i] = 1;
	int ret;
	if (sockInfo->typePlayer == TYPE_PLAYER_MAIN) ret = ProcessingPlayingMain(sockInfo);
	else ret = ProcessingPlayingSub(sockInfo);

	if (ret < 0) {
		ClosePlayingPlayer(i);
	}
}


//thread in Game Play
unsigned __stdcall PlayingThread(LPVOID lpParameter) {
	for (int i = 0; i < 10; i++) {
		inforGame.isSend[i] = 0;
	}
	while (true) {
		if (inforGame.isPlaying == 0 && inforGame.nPlayer == 10) {
			inforGame.stepGame = STEP_GAME::QUESTION_BEGIN;
			//send question begin


		}
		else {
			//sleep
			Sleep(100);
			continue;
		}
		DWORD index;
		// Step 5: Wait for the overlapped I/O call to complete
		index = WSAWaitForMultipleEvents(inforGame.nPlayer, eventsPlaying, FALSE, 10, FALSE);
		index = index - WSA_WAIT_EVENT_0;

		/*********************************ADDITIONAL CODE***************************************/

		// Step 7: Reset the signaled event		
		WSAResetEvent(eventsPlaying[index]);

		// First check to see whether the peer has closed
		// the connection, and if so, close the socket
		if (playersPlaying[index]->transferredBytes == 0) {
			printf("Closing socket %d\n", playersPlaying[index]->client);
			closesocket(playersPlaying[index]->client);
			ClosePlayingPlayer(index);
			return 1;
		}


		// Step 8: Post another I/O operation
		// Since WSASend() is not guaranteed to send all of the bytes requested,
		// continue posting WSASend() calls until all received bytes are sent.
		//if (playersPlaying[index]->operation == RECV) {
			playersPlaying[index]->operation = RECV;
			if (WSARecv(playersPlaying[index]->client, &(playersPlaying[index]->dataBuff), 1,
				&playersPlaying[index]->sendByte, 0, &(playersPlaying[index]->overlap), PlayingRoutine) == SOCKET_ERROR) {
				if (WSAGetLastError() != WSA_IO_PENDING) {
					printf("WSASend() failed with error %d\n", WSAGetLastError());
					ClosePlayingPlayer(index);
					return 1;
				}
			}

		//}
		//else {
		//	// No more bytes to send post another WSARecv() request
		//	ZeroMemory(&acceptOverlapped, sizeof(WSAOVERLAPPED));
		//	acceptOverlapped.hEvent = events[index];
		//	recvBytes = 0;
		//	operation = RECEIVE;
		//	dataBuf.len = DATA_BUFSIZE;
		//	dataBuf.buf = buffer;
		//	flags = 0;

		//	if (WSARecv(connSocket, &dataBuf, 1, &transferredBytes, &flags, &acceptOverlapped, NULL) == SOCKET_ERROR) {
		//		if (WSAGetLastError() != WSA_IO_PENDING) {
		//			printf("WSARecv() failed with error %d\n", WSAGetLastError());
		//			closesocket(connSocket);
		//			return 1;
		//		}
		//	}
		}
}