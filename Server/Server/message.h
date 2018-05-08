#pragma once
#include "constant.h"

typedef struct Messages {
	char messtype[MSG_LENGTH_TYPE];
	char messrec[MSG_LENGTH_RESULT];
	char data[MSG_LENGTH_DATA];
} Messages;

int TypeMessage(Messages mess) {
	if (strcmp(mess.messtype, MSG_LOGIN))
		return MSG_TYPE_LOGIN;
	if (strcmp(mess.messtype, MSG_BEGIN_GAME))
		return MSG_TYPE_BEGIN_GAME;
	if (strcmp(mess.messtype, MSG_LOGOUT))
		return MSG_TYPE_LOGOUT;
	if (strcmp(mess.messtype, MSG_QUESTION))
		return MSG_TYPE_QUESTION;
	if (strcmp(mess.messtype, MSG_CHOOSE_PLAYER))
		return MSG_TYPE_CHOOSE_PLAYER;
	if (strcmp(mess.messtype, MSG_ANSWER))
		return MSG_TYPE_ANSWER;
	if (strcmp(mess.messtype, MSG_HELP))
		return MSG_TYPE_HELP;
	if (strcmp(mess.messtype, MSG_ERROR))
		return MSG_TYPE_ERROR;
	if (strcmp(mess.messtype, MSG_END_GAME))
		return MSG_TYPE_END_GAME;
	if (strcmp(mess.messtype, MSG_END_CONNECT_SERVER))
		return MSG_TYPE_END_CONNECT_SERVER;
	if (strcmp(mess.messtype, MSG_END_CONNECT_CLIENT))
		return MSG_TYPE_END_CONNECT_CLIENT;
	if (strcmp(mess.messtype, MSG_NEWS))
		return MSG_TYPE_NEWS;
	return MSG_TYPE_ERROR;
}

void SetMessage(Messages *mess, const char messtype[], char data[]) {
	strcpy(mess->messtype, messtype);
	strcmp(mess->data, data);
}