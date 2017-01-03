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

void showErrorMessage(void);
TCHAR *getWord(LPTSTR targetStr, TCHAR **nextPtr);
TCHAR *getWordStart(LPTSTR targetStr);
int runWithSize(
	int width,
	int height,
	int x,
	int y,
	bool noXpos,
	bool noYpos,
	TCHAR *commandLine
);
