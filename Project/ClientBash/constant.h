﻿#pragma once
#include <process.h>

// Users
#define SIZE_USERID 20
#define SIZE_PASSWORD 15

// trạng thái user
#define USER_BLOCK 0
#define USER_ACTIVE 1

//trạng thái player
#define TYPE_PLAYER_WAITTING 0
#define TYPE_PLAYER_SUB 1
#define TYPE_PLAYER_MAIN 2

//
#define MAX_THREAD 500
#define MAX_USER 10000
#define MAX_CLIENT_PLAYING 10
#define MAX_CLIENT_WAITTING (MAX_THREAD * WSA_MAXIMUM_WAIT_EVENTS)

//step game
#define STEP_CONNECT 0			//không cần thiết
#define STEP_LOGIN 1
#define STEP_WAIT 2				//trạng thái sau khi login chờ để chơi.
#define STEP_PLAY 3
#define STEP_PLAY_MAIN 4
#define STEP_LOGOUT 5MS
#define STEP_QUESTION 6
#define STEP_RESULT 7
#define STEP_CHOOSE_ANSWER 8
#define STEP_HELP 9
#define STEP_END_GAME 10

#define SERVER_PORT 5500
#define SERVER_ADDR "127.0.0.1"

#define MAX_ERROR 3
#define ACCOUNT_FILE "account.txt"

//length question
#define MAX_QUES_LENGTH 1024
#define MAX_ANS_LENGTH 128 

//message  type
#define MSG_CLIENT_REQUEST_LOGIN "CLOGI"
#define MSG_SERVER_REPLY_LOGIN "SLOGI"
#define MSG_CLIENT_REQUEST_PLAY "CPLAY"
#define MSG_SERVER_REPLY_PLAY "SPLAY"
#define MSG_CLIENT_REQUEST_LOGOUT "CLOGO"
#define MSG_SERVER_REPLY_LOGOUT "SLOGO"
#define MSG_SERVER_SEND_QUESTION_BEGIN "SQUEB"
#define MSG_CLIENT_ANSWER_QUESTION_BEGIN "CCHOB"
#define MSG_SERVER_ANSWER_QUESTION_BEGIN "SANSB"
#define MSG_SERVER_SEND_QUESTION "SQUES"
#define MSG_CLIENT_ANSWER_QUESTION "CCHOO"
#define MSG_SERVER_ANSWER_QUESTION "SANSQ"
#define MSG_CLIENT_USE_HELP_50_50 "CHLP1"
#define MSG_SERVER_ANSWER_HELP_50_50 "SHLP1"
#define MSG_CLIENT_USE_HELP_CALL "CHLP2"
#define MSG_SERVER_ANSWER_HELP_CALL "SHLP2"
#define MSG_CLIENT_USE_HELP_ASK_AUDIENCE "CHLP3"
#define MSG_SERVER_ANSWER_HELP_AUDIENCE "SHLP3"
#define MSG_CLIENT_USE_HELP_ADVISORY "CHLP4"
#define MSG_SERVER_ANSWER_HELP_ADVISORY "SHLP4"
#define MSG_SERVER_NEWS "SNEWS"
#define MSG_CLIENT_WANT_END_GAME "CENDG"
#define MSG_SERVER_ACCEPT_END_GAME "SENDG"
#define MSG_CLIENT_WANT_QUIT "CQUIT"
#define MSG_SERVER_ERROR "SERRO"
#define MSG_CLIENT_ERROR "CERRO"

//length message
#define MSG_LENGTH_TYPE 5
#define MSG_LENGTH_RESULT 2
#define MSG_LENGTH_DATA 2048                            

//player operation
#define RECV 1
#define SEND 0


