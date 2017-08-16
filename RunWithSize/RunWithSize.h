/*
RunWithSIze (C) 2017 Tatsuhiko Syoji
The sources for RunWithSIze are distributed under the MIT open source license
*/
#pragma once

#ifndef WINVER                  // �Œ���K�v�ȃv���b�g�t�H�[���� Windows Vista �ł��邱�Ƃ��w�肵�܂��B
#define WINVER 0x0600           // ����� Windows �̑��̃o�[�W���������ɓK�؂Ȓl�ɕύX���Ă��������B
#endif

#ifndef _WIN32_WINNT            // �Œ���K�v�ȃv���b�g�t�H�[���� Windows Vista �ł��邱�Ƃ��w�肵�܂��B
#define _WIN32_WINNT 0x0600     // ����� Windows �̑��̃o�[�W���������ɓK�؂Ȓl�ɕύX���Ă��������B
#endif

#ifndef _WIN32_WINDOWS          // �Œ���K�v�ȃv���b�g�t�H�[���� Windows Vista �ł��邱�Ƃ��w�肵�܂��B
#define _WIN32_WINDOWS 0x0600	// ����� Windows Me �܂��͂���ȍ~�̃o�[�W���������ɓK�؂Ȓl�ɕύX���Ă��������B
#endif

#ifndef _WIN32_IE               // �Œ���K�v�ȃv���b�g�t�H�[���� Internet Explorer 7.0 �ł��邱�Ƃ��w�肵�܂��B
#define _WIN32_IE 0x0700        // ����� IE �̑��̃o�[�W���������ɓK�؂Ȓl�ɕύX���Ă��������B
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
