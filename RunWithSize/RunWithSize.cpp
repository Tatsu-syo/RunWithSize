/*
RunWithSIze (C) 2017 Tatsuhiko Syoji
The sources for RunWithSIze are distributed under the MIT open source license
*/
#include "RunWithSize.h"

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
	struct positionInfo info;

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
	if (!_tcscmp(_T("?"), commandWord)) {
		info.noWidth = true;
	} else {
		info.width = _tstoi(commandWord);
		info.noWidth = false;
	}

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
	if (!_tcscmp(_T("?"), commandWord)) {
		info.noHeight = true;
	} else {
		info.height = _tstoi(commandWord);
		info.noHeight = false;
	}

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
		info.noXpos = true;
		info.fromRight = false;
	} else {
		if (_totupper(*commandWord) == _T('R')) {
			info.x = _tstoi(commandWord + 1);
			info.fromRight = true;
		} else {
			info.x = _tstoi(commandWord);
			info.fromRight = false;
		}
		info.noXpos = false;
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
		info.noYpos = true;
		info.fromBottom = false;
	} else {
		if (_totupper(*commandWord) == _T('B')) {
			info.y = _tstoi(commandWord + 1);
			info.fromBottom = true;
		} else {
			info.y = _tstoi(commandWord);
			info.fromBottom = false;
		}
		info.noYpos = false;
	}

	// コマンドラインの取得
	commandLine = getWordStart(nextStart);
	if (commandLine == NULL) {
		// コマンドラインがない場合は何もしない
		showErrorMessage();
		return 1;
	}

	return runWithSize(
		&info,
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
	WINDOWPLACEMENT place;
	place.length = sizeof(WINDOWPLACEMENT);
	struct positionInfo *posInfo;
	
	posInfo = (struct positionInfo *)lparam;

	// 起動したプログラムのウインドウを探す
	GetWindowThreadProcessId(hWnd, &wndPid);
	if (wndPid == taqrgetPid) {
		// 見えないウインドウはサイズを調整してもしょうがないので、
		// 
		if (IsWindowVisible(hWnd)) {

			if (IsWindow(hWnd)) {
				GetWindowPlacement(hWnd, &place);
				if (!posInfo->noXpos) {
					place.rcNormalPosition.left = posInfo->x;
				}
				if (!posInfo->noYpos) {
					place.rcNormalPosition.top = posInfo->y;
				}

				RECT r;
				GetWindowRect(hWnd, &r);
				int orgWidth = r.right - r.left;
				int orgHeight = r.bottom - r.top;

				int width;
				if (posInfo->noWidth) {
					width = orgWidth + 1;
				} else {
					width = posInfo->width;
				}

				int height;
				if (posInfo->noHeight) {
					height = orgHeight + 1;
				} else {
					height = posInfo->height;
				}

				// もともとのウインドウの大きさが0*0のものはリサイズ対象としない。
				// Delphi製アプリのウインドウクラスTApplicationのウインドウがこれに当たる。
				// 表示されるウインドウを相手にせず、起動中のウインドウの列挙なら
				// オーナーウインドウの有無で判定できるが、相手は表示されるウインドウなので
				// このような手段をとる必要がある。
				if ((orgWidth != 0) || (orgHeight != 0)) {
					if (posInfo->fromRight) {
						place.rcNormalPosition.right = posInfo->x;
						place.rcNormalPosition.left = place.rcNormalPosition.right - width + 1;
					} else {
						place.rcNormalPosition.right = place.rcNormalPosition.left + width - 1;
					}
					if (posInfo->fromBottom) {
						place.rcNormalPosition.bottom = posInfo->y;
						place.rcNormalPosition.top = place.rcNormalPosition.bottom - height + 1;
					} else {
						place.rcNormalPosition.bottom = place.rcNormalPosition.top + height - 1;
					}

					if (IsWindow(hWnd)) {
						SetWindowPlacement(hWnd, &place);
					}
				}
			}

			resized = true;
			return TRUE;
		}
	}
	return TRUE;
}

/**
 * 幅、高さ、(横位置)、(縦位置)を指定してアプリケーションを起動する
 *
 * @param  posInfo 位置情報
 * @param commandLine
 * 
 */
int runWithSize(
	struct positionInfo *posInfo,
	TCHAR *commandLine
)
{
	STARTUPINFO info;
	PROCESS_INFORMATION procInfo;
	BOOL startResult;

	memset(&info, 0, sizeof(STARTUPINFO));
	memset(&procInfo, 0, sizeof(PROCESS_INFORMATION));

	if (!posInfo->noXpos) {
		info.dwX = posInfo->x;
		info.dwFlags = STARTF_USEPOSITION;
	}
	if (!posInfo->noYpos) {
		info.dwY = posInfo->y;
		info.dwFlags = STARTF_USEPOSITION;
	}
	info.dwXSize = posInfo->width;
	info.dwYSize = posInfo->height;
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
		EnumWindows(setWindowSize, (LPARAM)posInfo);
	}


	CloseHandle(procInfo.hThread);
	CloseHandle(procInfo.hProcess);


	return 0;
}