#pragma once

#include "mylibraly.h"
#include "player.h"
#include "message.h"

//
Players *clientWaitting[MAX_CLIENT_WAITTING];
WSAEVENT eventsWaitting[MAX_CLIENT_WAITTING];
int nWaitting = 0;

Players *clientPlaying[MAX_CLIENT_PLAYING];
WSAEVENT eventsPlaying[MAX_CLIENT_PLAYING];
int nPlaying = 0;

//Stuct contain information of each thread
typedef struct ThreadInfo {
	WSAEVENT *tEvents;
	CRITICAL_SECTION criticle;
	int indexOfThread;
	int nEvent;
}ThreadInfo, *pThreadInfo;

//To manage threads and number of thread
ThreadInfo threads[MAX_THREAD];
HANDLE threadHandle[MAX_THREAD];
int nThread;

CRITICAL_SECTION threadCrt;
CRITICAL_SECTION userCrt;
CRITICAL_SECTION playCrt;
CRITICAL_SECTION waitCrt;
