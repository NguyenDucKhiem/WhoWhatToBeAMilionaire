#pragma once


#include "waitting.h"

//Stuct contain information of each thread
//có 2 biến 1 biến ThreadWaitting ThreadPlaying
typedef struct ThreadInfo {
	WSAEVENT *tEvents;
	CRITICAL_SECTION criticle;	
	CRITICAL_SECTION *arrCrt;			//khi khởi tạo = &playCrt or waitCrt.
	Players **listPlayers;
	int indexOfThread;
	int nEvent;
}ThreadInfo, *pThreadInfo;

//Critical Section
CRITICAL_SECTION threadCrt;
CRITICAL_SECTION userCrt;

//To manage threads and number of thread
ThreadInfo threadWaiting[MAX_THREAD];
HANDLE threadWaittingHandle[MAX_THREAD];
int nThreadWaitting;

ThreadInfo threadPlaying;
HANDLE threadPlayingHandle;

SOCKET acceptSocket;


void DeletePlayerInThread(ThreadInfo *threadInfor, int index) {
	EnterCriticalSection(&threadCrt);
	EnterCriticalSection(&threadInfor->criticle);
	//crt waitiing or playing,
	EnterCriticalSection(threadInfor->arrCrt);
	DeletePlayer(threadInfor->listPlayers, index, &threadInfor->nEvent);
	LeaveCriticalSection(threadInfor->arrCrt);
	LeaveCriticalSection(&threadInfor->criticle);
	LeaveCriticalSection(&threadCrt);
}

void CALLBACK WaittingRoutine(DWORD error, DWORD transferredBytes, LPWSAOVERLAPPED overlapped, DWORD inFlags) {
	DWORD sendBytes, recvBytes;
	DWORD flags;

	// Reference the WSAOVERLAPPED structure as a SOCKET_INFORMATION structure
	Players* sockInfo = (Players*)overlapped;
	if (sockInfo == NULL) return;
	if (error != 0)
		printf("I/O operation failed with error %d\n", error);

	if (transferredBytes == 0)
		printf("Closing socket %d\n\n", sockInfo->client);

	if (error != 0 || transferredBytes == 0) {
		//Find and remove socket
		EnterCriticalSection(&waitCrt);

		int index;
		for (index = 0; index < nplayerWaitting; index++)
			if (playersWaitting[index]->client == sockInfo->client)
				break;

		closesocket(playersWaitting[index]->client);
		GlobalFree(playersWaitting[index]);

		playersWaitting[index] = playersWaitting[nplayerWaitting - 1];
		nplayerWaitting--;

		LeaveCriticalSection(&waitCrt);

		return;
	}

	// Check to see if the recvBytes field equals zero. If this is so, then
	// this means a WSARecv call just completed so update the recvBytes field
	// with the transferredBytes value from the completed WSARecv() call	
	/*if (sockInfo->operation == RECV) {
		sockInfo->recvByte = transferredBytes;
		sockInfo->sendByte = 0;
		sockInfo->operation = SEND;
	}
	else {
		sockInfo->sendByte += transferredBytes;
	}*/

	if (sockInfo->operation == RECV) {
		// Post another WSASend() request.
		// Since WSASend() is not guaranteed to send all of the bytes requested,
		// continue posting WSASend() calls until all received bytes are sent
		ZeroMemory(&(sockInfo->overlap), sizeof(WSAOVERLAPPED));
		/*sockInfo->dataBuff.buf = sockInfo->buff + sockInfo->sendByte;
		sockInfo->dataBuff.len = sockInfo->recvByte - sockInfo->sendByte;*/
		int ret = ProcessingWaitting(sockInfo, (Messages*)sockInfo->buff);
		if (ret > 0) {
			sockInfo->operation = SEND;
			if (WSASend(sockInfo->client, &(sockInfo->dataBuff), 1, &sendBytes, 0, &(sockInfo->overlap), WaittingRoutine) == SOCKET_ERROR) {
				if (WSAGetLastError() != WSA_IO_PENDING) {
					printf("WSASend() failed with error %d\n", WSAGetLastError());
					return;
				}
			}
		}
		
		if (ret != 1) {
			EnterCriticalSection(&waitCrt);

			int index;
			for (index = 0; index < nplayerWaitting; index++)
				if (playersWaitting[index]->client == sockInfo->client)
					break;

			closesocket(playersWaitting[index]->client);
			GlobalFree(playersWaitting[index]);

			playersWaitting[index] = playersWaitting[nplayerWaitting - 1];
			nplayerWaitting--;

			LeaveCriticalSection(&waitCrt);
		}
	}
	else {
		// Now that there are no more bytes to send post another WSARecv() request
		sockInfo->recvByte = 0;
		flags = 0;
		ZeroMemory(&(sockInfo->overlap), sizeof(WSAOVERLAPPED));
		/*sockInfo->dataBuff.len = BUFF_SIZE;
		sockInfo->dataBuff.buf = sockInfo->buff;*/
		sockInfo->operation = RECV;
		if (WSARecv(sockInfo->client, &(sockInfo->dataBuff), 1, &recvBytes, &flags, &(sockInfo->overlap), WaittingRoutine) == SOCKET_ERROR) {
			if (WSAGetLastError() != WSA_IO_PENDING) {
				printf("WSARecv() failed with error %d\n", WSAGetLastError());
				return;
			}
		}
	}
}

unsigned __stdcall workerThread(LPVOID lpParameter)
{
	DWORD flags;
	DWORD index;
	DWORD recvBytes;

	// Save the accept event in the event array
	eventsWaitting[0] = (WSAEVENT)lpParameter;
	while (TRUE)
	{
		// Wait for accept() to signal an event and also process workerRoutine() returns
		while (TRUE) {
			index = WSAWaitForMultipleEvents(1, eventsWaitting, FALSE, WSA_INFINITE, TRUE);
			if (index == WSA_WAIT_FAILED) {
				printf("WSAWaitForMultipleEvents() failed with error %d\n", WSAGetLastError());
				return 1;
			}

			if (index != WAIT_IO_COMPLETION) {
				// An accept() call event is ready - break the wait loop
				break;
			}
		}

		WSAResetEvent(eventsWaitting[index - WSA_WAIT_EVENT_0]);

		EnterCriticalSection(&waitCrt);

		if (nplayerWaitting == MAX_CLIENT_WAITTING) {
			printf("Too many clients.\n");
			closesocket(acceptSocket);
			continue;
		}

		// Create a socket information structure to associate with the accepted socket
		playersWaitting[nplayerWaitting] = InitPlayer(acceptSocket);
		flags = 0;

		if (WSARecv(playersWaitting[nplayerWaitting]->client, &(playersWaitting[nplayerWaitting]->dataBuff), 1, &recvBytes,
			&flags, &(playersWaitting[nplayerWaitting]->overlap), WaittingRoutine) == SOCKET_ERROR) {
			if (WSAGetLastError() != WSA_IO_PENDING) {
				printf("WSARecv() failed with error %d\n", WSAGetLastError());
				continue;
			}
		}

		printf("Socket %d got connected...\n", acceptSocket);

		acceptSocket = 0;
		nplayerWaitting++;
		LeaveCriticalSection(&waitCrt);
	}

	return 0;
}