#pragma once
#include "constant.h"

typedef struct DataQuestions {
	char question[MAX_QUES_LENGTH];		// câu hỏi có cả đáp án \t (câu hỏi ngắn < 512)
	char choose[4][MAX_ANS_LENGTH];		// 128
} DataQuestions;

typedef struct Questions {
	char answer;						//đáp án
	DataQuestions data;
};