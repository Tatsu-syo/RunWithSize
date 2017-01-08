/*
RunWithSIze (C) 2017 Tatsuhiko Syoji
The sources for RunWithSIze are distributed under the MIT open source license
*/
#include "RunWithSize.h"

int x;
int y;
int width;
int height;
bool noXpos;
bool noYpos;
DWORD taqrgetPid;
bool resized;

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	TCHAR *commandWord;
	TCHAR *commandLine;
	TCHAR *nextStart;

	// 幅の取得
	commandWord = getWord(lpCmdLine, &nextStart);
	if (commandWord == NULL) {
		// 単語がないことによる引数不足の場合は何もしない
		showErrorMessage();
		return 1;
	}
	if (nextStart == NULL) {
		// 単語を切り出した時点で文字列終了に到達した場合(引数不足の場合)は何もしない
		showErrorMessage();
		return 1;
	}
	width = _tstoi(commandWord);

	// 高さの取得
	commandWord = getWord(nextStart, &nextStart);
	if (commandWord == NULL) {
		// 単語がないことによる引数不足の場合は何もしない
		showErrorMessage();
		return 1;
	}
	if (nextStart == NULL) {
		// 単語を切り出した時点で文字列終了に到達した場合(引数不足の場合)は何もしない
		showErrorMessage();
		return 1;
	}
	height = _tstoi(commandWord);

	// 横位置の取得
	commandWord = getWord(nextStart, &nextStart);
	if (commandWord == NULL) {
		// 単語がないことによる引数不足の場合は何もしない
		showErrorMessage();
		return 1;
	}
	if (nextStart == NULL) {
		// 単語を切り出した時点で文字列終了に到達した場合(引数不足の場合)は何もしない
		showErrorMessage();
		return 1;
	}
	if (!_tcscmp(_T("?"), commandWord)) {
		noXpos = true;
	} else {
		x = _tstoi(commandWord);
	}

	// 縦位置の取得
	commandWord = getWord(nextStart, &nextStart);
	if (commandWord == NULL) {
		// 単語がないことによる引数不足の場合は何もしない
		showErrorMessage();
		return 1;
	}
	if (nextStart == NULL) {
		// 単語を切り出した時点で文字列終了に到達した場合(引数不足の場合)は何もしない
		showErrorMessage();
		return 1;
	}
	if (!_tcscmp(_T("?"), commandWord)) {
		noYpos = true;
	} else {
		y = _tstoi(commandWord);
	}

	// コマンドラインの取得
	commandLine = getWordStart(nextStart);
	if (commandLine == NULL) {
		// コマンドラインがない場合は何もしない
		showErrorMessage();
		return 1;
	}

	return runWithSize(
		width,
		height,
		x,
		y,
		noXpos,
		noYpos,
		commandLine);
}

//typedef int (WINAPI *pMessageBox)(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType);

/**
 * エラーメッセージを表示する
 */
void showErrorMessage(void)
{
	//HMODULE user32Dll = LoadLibrary(_T("user32.dll"));
	//pMessageBox PMessageBox = 
	//	(pMessageBox)GetProcAddress(user32Dll, "MessageBoxW");
	MessageBox(
		NULL,
		_T("RunWithSize Version 1.0.1 By Tatsuhiko Syoji 2017\nUsage:\nRunWithSize width height x y CommandLine"),
		_T("How to use"),
		MB_OK | MB_ICONINFORMATION);
	//FreeLibrary(user32Dll);
}

/**
 * 単語を切り出す。
 *
 * @param targetStr 単語切り出し対象文字列
 * @param nextPtr 次の単語切り出し開始位置
 * @return NULL:単語が見つからない 非NULL:単語が見つかった
 */
TCHAR *getWord(LPTSTR targetStr, TCHAR **nextPtr)
{
	TCHAR *p;

	p = targetStr;
	*nextPtr = NULL;

	// 最初のスペース以外の文字を探す
	while (*p) {
		if (!_istspace(*p)) {
			break;
		}
		p++;
	}
	if (*p == _T('\0')) {
		return NULL;
	}
	TCHAR *wordStart = p;

	while (*p) {
		if (_istspace(*p)) {
			*p = _T('\0');
			*nextPtr = p + 1;
			break;
		}
		p++;
	}
	return wordStart;
}

/**
 * スペース以外の字句の始まりを検索する。
 *
 * @param targetStr 単語切り出し対象文字列
 * @return NULL:これ以上の字句は存在しない 非NULL:字句が存在する
 */
TCHAR *getWordStart(LPTSTR targetStr)
{
	TCHAR *p;

	p = targetStr;

	// 最初のスペース以外の文字を探す
	while (*p) {
		if (!_istspace(*p)) {
			break;
		}
		p++;
	}
	if (*p == _T('\0')) {
		return NULL;
	}
	return p;
}

// 各ウインドウに対するチェック処理を行うコールバック
BOOL CALLBACK setWindowSize(HWND hWnd, LPARAM lparam)
{
	DWORD wndPid;
	HWND parent;
	HWND now;
	WINDOWPLACEMENT place;
	place.length = sizeof(WINDOWPLACEMENT);

	

	// 起動したプログラムのウインドウを探す
	GetWindowThreadProcessId(hWnd, &wndPid);
	if (wndPid == taqrgetPid) {
		// 見えないウインドウはサイズを調整してもしょうがないので、
		// 
		if (IsWindowVisible(hWnd)) {
			now = hWnd;
			do {
				parent = GetWindow(now, GW_OWNER);
				if (parent != NULL) {
					now = parent;
				}
			} while (parent != NULL);

			if (IsWindow(now)) {
				GetWindowPlacement(now, &place);
				if (!noXpos) {
					place.rcNormalPosition.left = x;
				}
				if (!noYpos) {
					place.rcNormalPosition.top = y;
				}
				place.rcNormalPosition.right = place.rcNormalPosition.left + width - 1;
				place.rcNormalPosition.bottom = place.rcNormalPosition.top + height - 1;

				if (IsWindow(now)) {
					SetWindowPlacement(now, &place);
				}

			}

			resized = true;
			return FALSE;
		}
	}
	return TRUE;
}

/**
 * 幅、高さ、(横位置)、(縦位置)を指定してアプリケーションを起動する
 *
 * @param  width 幅
 * @param  height 高さ
 * @param  x 横位置
 * @param  y 縦位置
 * @param  noXpos 横位置を指定するかどうか
 * @param  noYpos 縦位置を指定するかどうか
 * @param commandLine
 * 
 */
int runWithSize(
	int width,
	int height,
	int x,
	int y,
	bool noXpos,
	bool noYpos,
	TCHAR *commandLine
)
{
	STARTUPINFO info;
	PROCESS_INFORMATION procInfo;
	BOOL startResult;

	memset(&info, 0, sizeof(STARTUPINFO));
	memset(&procInfo, 0, sizeof(PROCESS_INFORMATION));

	if (!noXpos) {
		info.dwX = x;
		info.dwFlags = STARTF_USEPOSITION;
	}
	if (!noYpos) {
		info.dwY = y;
		info.dwFlags = STARTF_USEPOSITION;
	}
	info.dwXSize = width;
	info.dwYSize = height;
	info.dwFlags |= STARTF_USESIZE;

	startResult = CreateProcess(
		NULL,
		commandLine,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&info,
		&procInfo
	);

	if (startResult == FALSE) {
		return 1;
	}

	taqrgetPid = procInfo.dwProcessId;

	// 起動したプログラムのウインドウを5秒間探す
	resized = false;
	for (int i = 0; resized == false && i < 5; i++) {
		// アプリがリサイズしたり、アプリの起動を待ったりするので
		// その分待つ
		Sleep(1000);
		EnumWindows(setWindowSize, 0);
	}


	CloseHandle(procInfo.hThread);
	CloseHandle(procInfo.hProcess);


	return 0;
}