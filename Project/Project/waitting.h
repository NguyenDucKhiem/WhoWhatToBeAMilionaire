#pragma once

#include "message.h"
#include "player.h"
#include "playing.h"

Players *playersWaitting[MAX_CLIENT_WAITTING];
WSAEVENT eventsWaitting[MAX_CLIENT_WAITTING];
int nplayerWaitting = 0;
CRITICAL_SECTION waitCrt;


int ProcessingWaitting(Players* sockInfo, Messages *mess) {
	if (sockInfo->typePlayer != TYPE_PLAYER_WAITTING)
		return -1;

	TYPE_MSG typeMess = TypeMessage(mess);
	int resultMess = ResultMess(mess);

	//type error
	if (typeMess == TYPE_MSG::MESSAGE_ERROR || resultMess == -10)
		return -2;
	Users *user;

	switch (sockInfo->step)
	{
	case STEP_GAME::LOGIN:
		switch (typeMess)
		{
		case TYPE_MSG::CLIENT_REQUEST_LOGIN:
			user = FindUser(listUsers, MAX_USER, mess->data);

			if (user == NULL) {
				SetMessage(mess, MSG_SERVER_REPLY_LOGIN, "-3", (char*) "User or Password is wrong.");
				printf("Send socket %d: %s\n", sockInfo->client, mess->data);
				return 1;
			} else if (user->isLogin == 1) {
				SetMessage(mess, MSG_SERVER_REPLY_LOGIN, "-4", (char*) "User is Login");
				//tiếp tục
				printf("Send socket %d: %s\n", sockInfo->client, mess->data);
				return 1;
			} else if (user->statusAccount != USER_ACTIVE) {
				SetMessage(mess, MSG_SERVER_REPLY_LOGIN, "-2", (char*) "User is Block.");
				printf("Send socket %d: %s\n", sockInfo->client, mess->data);
				//tiếp tục
				return 1;
			}
			else {
				SetMessage(mess, MSG_SERVER_REPLY_LOGIN, "+1", (char*) "Accept User.");
				printf("Send socket %d: %s\n", sockInfo->client, mess->data);
				sockInfo->userPlayer = user;
				sockInfo->step = STEP_WAIT;
				return 1;
			}
			break;
		case TYPE_MSG::CLIENT_WANT_QUIT:
			//close client
			return 0;
			break;
		default:
			SetMessage(mess, MSG_SERVER_ERROR, "-2", (char*) "Wrong step.");
			printf("Send socket %d: %s\n", sockInfo->client, mess->data);
			return 1;
		}
		break;
	case STEP_GAME::WAIT:
		switch (typeMess)
		{
		case TYPE_MSG::CLIENT_REQUEST_PLAY:
			//do some thing
			if (!inforGame.isPlaying && inforGame.nPlayer < 10) {
				//move waitting to playing
				EnterCriticalSection(&playCrt);
				playersPlaying[inforGame.nPlayer] = sockInfo;
				inforGame.nPlayer++;
				LeaveCriticalSection(&playCrt);
				
				SetMessage(mess, MSG_SERVER_REPLY_PLAY, "+1", (char*) "Accept play.");
				printf("Send socket %d: %s\n", sockInfo->client, mess->data);
				return 2;
			}
			else if (!inforGame.isPlaying && inforGame.nPlayer >= 10) {
				SetMessage(mess, MSG_SERVER_REPLY_PLAY, "-2", (char*) "Enough players.");
				printf("Send socket %d: %s\n", sockInfo->client, mess->data);
				return 1;
			} else {
				SetMessage(mess, MSG_SERVER_REPLY_PLAY, "-3", (char*) "Game starts.");
				printf("Send socket %d: %s\n", sockInfo->client, mess->data);
				return 1;
			}
			break;

		default:
			SetMessage(mess, MSG_SERVER_ERROR, "-2", (char*) "Wrong step.");
			printf("Send socket %d: %s\n", sockInfo->client, mess->data);
			return 1;
		}
		break;
	default:
		//wrong step in server.
		return -3;
	}
	return -1;
}

