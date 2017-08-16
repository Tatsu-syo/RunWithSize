/*
RunWithSIze (C) 2017 Tatsuhiko Syoji
The sources for RunWithSIze are distributed under the MIT open source license
*/
#pragma once

#ifndef WINVER                  // 最低限必要なプラットフォームが Windows Vista であることを指定します。
#define WINVER 0x0600           // これを Windows の他のバージョン向けに適切な値に変更してください。
#endif

#ifndef _WIN32_WINNT            // 最低限必要なプラットフォームが Windows Vista であることを指定します。
#define _WIN32_WINNT 0x0600     // これを Windows の他のバージョン向けに適切な値に変更してください。
#endif

#ifndef _WIN32_WINDOWS          // 最低限必要なプラットフォームが Windows Vista であることを指定します。
#define _WIN32_WINDOWS 0x0600	// これを Windows Me またはそれ以降のバージョン向けに適切な値に変更してください。
#endif

#ifndef _WIN32_IE               // 最低限必要なプラットフォームが Internet Explorer 7.0 であることを指定します。
#define _WIN32_IE 0x0700        // これを IE の他のバージョン向けに適切な値に変更してください。
#endif

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
TCHAR *strcmpspc(TCHAR *target, TCHAR *toSearch);
TCHAR *getOptions(TCHAR *start);
int runWithSize(
	struct positionInfo *posInfo,TCHAR *commandLine
);
