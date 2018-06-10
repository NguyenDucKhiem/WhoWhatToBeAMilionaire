#pragma once
#include "constant.h"
#include "enum.h"
#include <string.h>

#pragma warning(disable: 4996)

typedef struct Messages {
	char messType[MSG_LENGTH_TYPE];
	char messResult[MSG_LENGTH_RESULT];
	char data[MSG_LENGTH_DATA];
} Messages;

#define BUFF_SIZE sizeof(Messages)

TYPE_MSG TypeMessage(Messages *mess) {
	if (strcmp(mess->messType, MSG_CLIENT_REQUEST_LOGIN))
		return CLIENT_REQUEST_LOGIN;
	if (strcmp(mess->messType, MSG_SERVER_REPLY_LOGIN))
		return SERVER_REPLY_LOGIN;
	if (strcmp(mess->messType, MSG_CLIENT_REQUEST_PLAY))
		return CLIENT_REQUEST_PLAY;
	if (strcmp(mess->messType, MSG_SERVER_REPLY_PLAY))
		return SERVER_REPLY_PLAY;
	if (strcmp(mess->messType, MSG_CLIENT_REQUEST_LOGOUT))
		return CLIENT_REQUEST_LOGOUT;
	if (strcmp(mess->messType, MSG_SERVER_REPLY_LOGOUT))
		return SERVER_REPLY_LOGOUT;
	if (strcmp(mess->messType, MSG_SERVER_SEND_QUESTION_BEGIN))
		return  SERVER_SEND_QUESTION_BEGIN;
	if (strcmp(mess->messType, MSG_CLIENT_ANSWER_QUESTION_BEGIN))
		return CLIENT_ANSWER_QUESTION_BEGIN;
	if (strcmp(mess->messType, MSG_SERVER_ANSWER_QUESTION_BEGIN))
		return SERVER_ANSWER_QUESTION_BEGIN;
	if (strcmp(mess->messType, MSG_SERVER_SEND_QUESTION))
		return SERVER_SEND_QUESTION;
	if (strcmp(mess->messType, MSG_CLIENT_ANSWER_QUESTION))
		return CLIENT_ANSWER_QUESTION;
	if (strcmp(mess->messType, MSG_CLIENT_USE_HELP_50_50))
		return CLIENT_USE_HELP_50_50;
	if (strcmp(mess->messType, MSG_SERVER_ANSWER_HELP_50_50))
		return SERVER_ANSWER_HELP_50_50;
	if (strcmp(mess->messType, MSG_CLIENT_USE_HELP_CALL))
		return CLIENT_USE_HELP_CALL;
	if (strcmp(mess->messType, MSG_SERVER_ANSWER_HELP_CALL))
		return SERVER_ANSWER_HELP_CALL;
	if (strcmp(mess->messType, MSG_CLIENT_USE_HELP_ASK_AUDIENCE))
		return CLIENT_USE_HELP_ASK_AUDIENCE;
	if (strcmp(mess->messType, MSG_SERVER_ANSWER_HELP_AUDIENCE))
		return SERVER_ANSWER_HELP_AUDIENCE;
	if (strcmp(mess->messType, MSG_CLIENT_USE_HELP_ADVISORY))
		return CLIENT_USE_HELP_ADVISORY;
	if (strcmp(mess->messType, MSG_SERVER_ANSWER_HELP_ADVISORY))
		return SERVER_ANSWER_HELP_ADVISORY;
	if (strcmp(mess->messType, MSG_SERVER_NEWS))
		return SERVER_NEWS;
	if (strcmp(mess->messType, MSG_CLIENT_WANT_END_GAME))
		return CLIENT_WANT_END_GAME;
	if (strcmp(mess->messType, MSG_SERVER_ACCEPT_END_GAME))
		return SERVER_ACCEPT_END_GAME;
	if (strcmp(mess->messType, MSG_CLIENT_WANT_QUIT))
		return CLIENT_WANT_QUIT;
	if (strcmp(mess->messType, MSG_SERVER_ERROR))
		return SERVER_ERROR;
	if (strcmp(mess->messType, MSG_CLIENT_ERROR))
		return CLIENT_ERROR;
	return MESSAGE_ERROR;
}

int ResultMess(Messages *mess) {
	int opcode = mess->messResult[1] - '0';
	if (opcode > 9 || opcode < 0) return -10;
	switch (mess->messResult[0])
	{
	case '+':
		return opcode;
	case '-':
		return -opcode;
	default:
		return -10;			//error
	}
}

void SetMessage(Messages *mess,const char messtype[],const char messResult[], char data[]) {
	strcpy(mess->messType, messtype);
	strcpy(mess->messResult, messResult);
	strcpy(mess->data, data);
}