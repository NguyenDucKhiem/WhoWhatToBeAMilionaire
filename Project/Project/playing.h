#pragma once

#include "mylibraly.h"
#include "message.h"
#include "player.h"

Players *playersPlaying[MAX_CLIENT_PLAYING];
WSAEVENT eventsPlaying[MAX_CLIENT_PLAYING];

struct InfoGame
{
	int isPlaying;
	int nPlayer;			//number player
	int iPlayer;			//index main player
};

InfoGame inforGame;

int ProcessingPlayingMain(int index, Messages *mess) {
	if (playersPlaying[index]->typePlayer != TYPE_PLAYER_MAIN)
		return -1;

	TYPE_MSG typeMess = TypeMessage(mess);
	int resultMess = ResultMess(mess);

	//type error
	if (typeMess == MESSAGE_ERROR || resultMess == -10)
		return -2;

	switch (playersPlaying[index]->step)
	{
	case STEP_LOGIN:
		/*switch (typeMess)
		{

		
		default:
			SetMessage(mess, MSG_SERVER_ERROR, "-2", (char*) "Wrong step.");
			return 1;
			break;
		}*/
		break;
	
	default:
		//wrong step in server.
		return -3;
		break;
	}
}