#pragma once

#include "mylibraly.h"
#include "player.h"
#include "message.h"

//
Players *clientWatting[MAX_CLIENT_WATTING];
WSAEVENT eventsWatting[MAX_CLIENT_WATTING];

Players *clientPlaying[MAX_CLIENT_PLAYING];
WSAEVENT eventsPlaying[MAX_CLIENT_PLAYING];
int nWatting = 0;
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