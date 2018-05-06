#pragma once
#include "constant.h"

typedef struct Message {
	char messtype[MSG_LENGTH_TYPE];
	char data[MSG_LENGTH_DATA];
} Messages;