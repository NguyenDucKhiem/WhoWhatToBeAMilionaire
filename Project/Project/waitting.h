﻿#pragma once

#include "message.h"
#include "player.h"
#include "playing.h"

Players *playersWaitting[MAX_CLIENT_WAITTING];
WSAEVENT eventsWaitting[MAX_CLIENT_WAITTING];


int ProcessingWaitting(int index, Messages *mess) {
	if (playersWaitting[index]->typePlayer != TYPE_PLAYER_WAITTING)
		return -1;

	TYPE_MSG typeMess = TypeMessage(mess);
	int resultMess = ResultMess(mess);

	//type error
	if (typeMess == TYPE_MSG::MESSAGE_ERROR || resultMess == -10)
		return -2;
	Users *user;

	switch (playersWaitting[index]->step)
	{
	case STEP_LOGIN:
		switch (typeMess)
		{
		case TYPE_MSG::CLIENT_REQUEST_LOGIN:
			user = FindUser(listUsers, MAX_USER, mess->data);

			if (user == NULL) {
				SetMessage(mess, MSG_SERVER_REPLY_LOGIN, "-3", (char*) "User or Password is wrong.");
				return 1;
			} else if (user->isLogin == 1) {
				SetMessage(mess, MSG_SERVER_REPLY_LOGIN, "-4", (char*) "User is Login");
				//tiếp tục
				return 1;
			} else if (user->statusAccount != USER_ACTIVE) {
				SetMessage(mess, MSG_SERVER_REPLY_LOGIN, "-2", (char*) "User is Block.");
				//tiếp tục
				return 1;
			}
			else {
				SetMessage(mess, MSG_SERVER_REPLY_LOGIN, "+1", (char*) "Accept User.");
				playersWaitting[index]->userPlayer = user;
				playersWaitting[index]->step = STEP_WAIT;
				return 1;
			}
			break;
		case TYPE_MSG::CLIENT_WANT_QUIT:
			//close client
			return 0;
			break;
		default:
			SetMessage(mess, MSG_SERVER_ERROR, "-2", (char*) "Wrong step.");
			return 1;
		}
		break;
	case STEP_WAIT:
		switch (typeMess)
		{
		case TYPE_MSG::CLIENT_REQUEST_PLAY:
			//do some thing

			break;

		default:
			SetMessage(mess, MSG_SERVER_ERROR, "-2", (char*) "Wrong step.");
			return 1;
		}
		break;
	default:
		//wrong step in server.
		return -3;
	}
	return -1;
}