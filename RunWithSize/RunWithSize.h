/*
RunWithSIze (C) 2017 Tatsuhiko Syoji
The sources for RunWithSIze are distributed under the MIT open source license
*/
#pragma once

#include "targetver.h"

#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <memory.h>

struct positionInfo {
	int x;
	int y;
	int width;
	int height;
	bool noWidth;
	bool noHeight;
	bool noXpos;
	bool noYpos;
	bool fromRight;
	bool fromBottom;
	DWORD taqrgetPid;
	bool resized;
};

void showErrorMessage(void);
TCHAR *getWord(LPTSTR targetStr, TCHAR **nextPtr);
TCHAR *getWordStart(LPTSTR targetStr);
int runWithSize(
	struct positionInfo *posInfo,TCHAR *commandLine
);
