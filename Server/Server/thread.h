#pragma once

#include "message.h"
#include "player.h"
#include "mylibraly.h"

//
Players *playersWaitting[MAX_CLIENT_WAITTING];
WSAEVENT eventsWaitting[MAX_CLIENT_WAITTING];

Players *playersPlaying[MAX_CLIENT_PLAYING];
WSAEVENT eventsPlaying[MAX_CLIENT_PLAYING];

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

					WSAGetOverlappedResult(playersWaitting[idxSect]->sock, &sections[idxSect]->overlap, &transByte, FALSE, &flags);

					if (transByte == 0) {
						//
						closeSectionEx(i + idx, pThInfo);
						continue;
					}
					else {
						//Tranfer success
						if (sections[idxSect]->operation == RECV) {
							//Process msg from client
							sections[idxSect]->buff[transByte] = '\0';
							processRecvMsg(idxSect, sections[idxSect]->buff, BUFF_SIZE, &sections[idxSect]->sendByte);
							//Send msg to client, usse WSASend()
							sections[idxSect]->operation = SEND;
							ResetEvent(events[idxSect]);
							if (WSASend(sections[idxSect]->sock, &sections[idxSect]->dataBuff, 1, &sections[idxSect]->sendByte,
								flags, &sections[idxSect]->overlap, NULL) == SOCKET_ERROR) {
								if (WSAGetLastError() != WSA_IO_PENDING) {
									//Hve error in transfer
									closeSectionEx(i + idx, pThInfo);
									continue;
								}
							}
						}
						else {
							//Send 1 byte msg success request to receive msg from client
							resetSection(idxSect);
							flags = 0;
							//Post Recv request
							if (WSARecv(sections[idxSect]->sock, &sections[idxSect]->dataBuff, 1, &transByte, &flags, &sections[idxSect]->overlap, NULL) == SOCKET_ERROR) {
								if (WSAGetLastError() != WSA_IO_PENDING) {
									//Have error in the socket index
									printf("WSARecv() failed with error %d\n", WSAGetLastError());
									closeSectionEx(i + idx, pThInfo);
								}
							}
						}
					}
				}
			}
		}
	}
	return 0;
}