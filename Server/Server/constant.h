#pragma once

// Users
#define SIZE_USERID 20
#define SIZE_PASSWORD 15

// trạng thái user
#define BLOCK 0
#define ACTIVE 1

//
#define MAX_USER 10000
#define MAX_CLIENT_WATTING 50000
#define MAX_CLIENT_PLAYING 10
#define MAX_THREAD 500

//step game
#define STEP_CONNECT 0
#define STEP_LOGIN 1
#define STEP_WAIT 2
#define STEP_PLAY 3
#define STEP_PLAY_MAIN 4
#define STEP_LOGOUT 5
#define STEP_QUESTION 6
#define STEP_RESULT 7
#define STEP_CHOOSE_ANSWER 8
#define STEP_HELP 9
#define STEP_END_GAME 10

#define BUFF_SIZE 2048

#define SERVER_PORT 5500
#define SERVER_ADDR "127.0.0.1"

// lệnh
#define LOGIN 1
#define LOGOUT 2
#define END_GAME 3
#define END_SERVER 4
#define END_CLIENT 5

#define CLOSE_CLIENT -1

#define MAX_ERROR 3
#define ACCOUNT_FILE "account.txt"

//length question
#define MAX_QUES_LENGTH 1024
#define MAX_ANS_LENGTH 128 

//message  type
#define MSG_LOGIN "LOGN"
#define MSG_BEGIN_GAME "BEGI";
#define MSG_LOGOUT "LOUT"
#define MSG_QUESTION "QUESTION"
#define MSG_CHOOSE_PLAYER "CHOO"
#define MSG_ANSWER "ANSW"
#define MSG_HELP "HELP"
#define MSG_ERROR "ERRO"
#define MSG_END_GAME "ENDG"
#define MSG_END_CONNECT_SERVER "ENDS"
#define MSG_END_CONNECT_CLIENT "ENDC"
#define MSG_NEWS "NEWS"		//Loại thông báo (dùng cuộc chơi, thoát,…)(đối với người chơi chính)

//length message
#define MSG_LENGTH_TYPE 4
#define MSG_LENGTH_DATA 4096                            

//player operation
#define RECV 1
#define SEND 1
