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
CRITICAL_SECTION playCrt;
CRITICAL_SECTION waitCrt;

//To manage threads and number of thread
ThreadInfo threadWaiting[MAX_THREAD];
HANDLE threadWaittingHandle[MAX_THREAD];
int nThreadWaitting;

ThreadInfo threadPlaying;
HANDLE threadPlayingHandle;


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

unsigned __stdcall WaittingThread(LPVOID parma) {
	ThreadInfo *pThInfo = (ThreadInfo*)parma;

	int index;
	int idx;
	int idxSect;//Index in section array
	DWORD transByte;
	DWORD flags = 0;

	while (TRUE) {
		//Wait for event signed	
		index = WSAWaitForMultipleEvents(pThInfo->nEvent, pThInfo->tEvents, FALSE, 200, FALSE);
		if (index == WSA_WAIT_TIMEOUT) {
			continue;
		}
		if (index == WSA_WAIT_FAILED) {
			//Check if nEvent == 0
			if (pThInfo->nEvent == 0) {
				SleepEx(1000, FALSE);
				//Set thread to sleep
				//Must fix
				continue;
			}
		}
		else {
			//Have event in client
			index -= WSA_WAIT_EVENT_0;
			//Check event from index to nEvent of thread
			for (int i = index; i < pThInfo->nEvent; i++) {
				idx = WSAWaitForMultipleEvents(1, pThInfo->tEvents + i, FALSE, 10, FALSE);
				if (idx != WSA_WAIT_FAILED && idx != WSA_WAIT_TIMEOUT) {
					idx -= WSA_WAIT_EVENT_0;
					//Define index of  client in the section array
					idxSect = i + idx + pThInfo->indexOfThread * WSA_MAXIMUM_WAIT_EVENTS;

					WSAGetOverlappedResult(playersWaitting[idxSect]->client, &playersWaitting[idxSect]->overlap, &transByte, FALSE, &flags);

					if (transByte == 0) {
						//
						DeletePlayerInThread(pThInfo, i + idx);
						continue;
					}
					else {
						//Tranfer success
						if (playersWaitting[idxSect]->operation == RECV) {
							//Process msg from client

							//action watting
							//processRecvMsg(idxSect, playersWaitting[idxSect]->buff, BUFF_SIZE, &playersWaitting[idxSect]->sendByte);
							if (playersWaitting[idxSect]->isSend == 1)
								ProcessingWaitting(idxSect, (Messages*)playersWaitting[idxSect]->buff);

							//Send msg to client, usse WSASend()
							playersWaitting[idxSect]->operation = SEND;
							ResetEvent(eventsWaitting[idxSect]);
							if (WSASend(playersWaitting[idxSect]->client, &playersWaitting[idxSect]->dataBuff, 1, &playersWaitting[idxSect]->sendByte,
								flags, &playersWaitting[idxSect]->overlap, NULL) == SOCKET_ERROR) {
								if (WSAGetLastError() != WSA_IO_PENDING) {
									////Hve error in transfer
									DeletePlayerInThread(pThInfo, i + idx);
								}
								playersWaitting[idxSect]->isSend = 0;
							}
							else {
								playersWaitting[idxSect]->isSend = 1;
							}
						}
						else {
							//Send 1 byte msg success request to receive msg from client
							// reset thì các giá trị về 0 -> WSA lỗi ??
							//resetSection(idxSect);
							flags = 0;
							//Post Recv request
			
							if (WSARecv(playersWaitting[idxSect]->client, &playersWaitting[idxSect]->dataBuff, 1, &transByte, &flags, &playersWaitting[idxSect]->overlap, NULL) == SOCKET_ERROR) {
								if (WSAGetLastError() != WSA_IO_PENDING) {
									//Have error in the socket index
									printf("WSARecv() failed with error %d\n", WSAGetLastError());
									DeletePlayerInThread(pThInfo, i + idx);
								}
								continue;
							}
							else {
								playersWaitting[idxSect]->operation = RECV;
							}
						}
					}
				}
			}
		}
	}
	return 0;
}