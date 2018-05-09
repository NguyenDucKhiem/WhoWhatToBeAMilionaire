#pragma once
#include "constant.h"

typedef struct DataQuestions {
	char question[MAX_QUES_LENGTH];		// 
	char choose[4][MAX_ANS_LENGTH];		// 128
	
} DataQuestions;

typedef struct Questions {
	char answer;
	DataQuestions data;
};

/*
[IN] question
[IN] choose1
[IN] choose2
[IN] choose3
[IN] choose4
[IN] answer

init question , return a point to it
*/

Questions *InitQuestion(char *question, char *choose1, char *choose2, char *choose3, char *choose4, int answer) {
	Questions *pQuestion = (Questions*)malloc(sizeof(Questions));

	strcpy(pQuestion->data.question, question);
	strcpy(pQuestion->data.choose[0], choose1);
	strcpy(pQuestion->data.choose[1], choose2);
	strcpy(pQuestion->data.choose[2], choose3);
	strcpy(pQuestion->data.choose[3], choose4);
	pQuestion->answer = answer;

	return pQuestion;

}

/*
[IN] nameFile
[IN] mode :mode open file
*/

FILE* Fopen(char *nameFile, char* mode) {
	FILE * pFile;

	pFile = fopen(nameFile, mode);
	if (pFile == NULL) printf("Error!Can't open %s ", nameFile);

	return pFile;
}


/*
[IN] file
[OUT] listQues where Questions save
[IN] maxNumberQues is number of questions can save

Read file, save in listQues and return reading number of questions
*/

int ReadQuesFromFile(char * nameFile, Questions **listQues, int maxNumberQues) {
	FILE * pFile;
	char question[MAX_QUES_LENGTH];
	char choose[4][MAX_ANS_LENGTH];
	int answer;
	int i = 0;
	int j = 0;
	//open file
	pFile = Fopen(nameFile, "r");
	if (!pFile) return 0;
	
	while (fgets(question, MAX_QUES_LENGTH, pFile) && i < maxNumberQues) {

		for (j = 0;j < 4;j++) 
			fgets(choose[j], MAX_ANS_LENGTH, pFile);

		fscanf(pFile, "%d", &answer);

		listQues[i] = InitQuestion(question, choose[0], choose[1], choose[2], choose[3], answer);
		fgets(question, MAX_QUES_LENGTH, pFile);

		i++;
	}

	return i;
}


