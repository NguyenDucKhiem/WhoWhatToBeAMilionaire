#pragma once

#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include "constant.h"

// Define User
typedef struct Users
{
	char userID[SIZE_USERID];
	char password[SIZE_PASSWORD];
	int statusAccount;
	int isLogin;
} Users;

Users *listUsers[MAX_USER];

/*
[IN] userID
[IN] password
[IN] status

init users and return it
*/
Users *InitUser(char *userID, char *password, int statusAccount) {
	Users *user = (Users*)malloc(sizeof(Users));

	strcpy(user->userID, userID);
	strcpy(user->password, password);
	user->statusAccount = statusAccount;
	user->isLogin = 0;
	return user;
}

/*
[IN] file
[OUT] listUser where Users save
[IN] maxUser is number Users can save

Read file, save in listUser and return reading numver Users
*/

int ReadUserFromFile(FILE *file, Users **listUser, int maxUser) {
	char userID[SIZE_USERID];
	char password[SIZE_PASSWORD];
	int statusAccount;
	int i = 0;

	while (feof(file) == 0 && i < maxUser) {
		if (fscanf(file, "%s %s %d", userID, password, &statusAccount)) {
			listUser[i] = InitUser(userID, password, statusAccount);
			i++;
		}
	}
	return i;
}

/*
[IN] file
[IN] listUser which will save in file
[IN] numberUsers is number Users

Write listUser in file
*/
void WriteUserInFile(FILE *file, Users **listUser, int numberUsers) {
	for (int i = 0; i < numberUsers; i++) {
		fprintf_s(file, "%s %s %d\n", listUser[i]->userID, listUser[i]->password, listUser[i]->statusAccount);
	}
}

/*
[IN] file
[OUT] listUser where Users save
[IN] maxUser is number Users can save

Read file, save in listUser and return reading numver Users
*/

Users *FindUser(Users **listUsers, int numberUsers, char *userID) {
	for (int i = 0; i < numberUsers; i++) {
		if (listUsers[i] == NULL) return NULL;
		if (strcmp(listUsers[i]->userID, userID) == 0)
			return listUsers[i];
	}
	return NULL;
}

/*
Free user

[IN/OUT] user. Which is free
*/
void FreeUser(Users *user) {
	free(user->userID);
	free(user->password);
	free(user);
}

/*
Delete User and diminished user

[IN/OUT] lissUsers
[IN/OUT] numberUsers
[IN] userID which is delete
*/
int DeleteUser(Users **listUsers, int *numberUsers, char *userID) {
	for (int i = 0; i < *numberUsers; i++) {
		if (strcmp(listUsers[i]->userID, userID) == 0) {
			FreeUser(listUsers[i]);
			listUsers[i] = listUsers[*numberUsers - 1];
			(*numberUsers)--;
			return 1;
		}
	}
	return 0;
}