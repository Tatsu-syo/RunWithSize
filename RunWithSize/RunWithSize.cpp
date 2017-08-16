/*
RunWithSIze (C) 2017 Tatsuhiko Syoji
The sources for RunWithSIze are distributed under the MIT open source license
*/
#include "RunWithSize.h"

DWORD taqrgetPid;
bool resized;
bool noWin10;
bool topMost;

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	TCHAR *commandWord;
	TCHAR *commandLine;
	TCHAR *nextStart;
	struct positionInfo info;

	noWin10 = false;
	topMost = false;

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
	// まず、次の語句のところまで行く。
	commandLine = getWordStart(nextStart);

	if (commandLine != NULL) {
		commandLine = getOptions(commandLine);
	}

	if (commandLine == NULL) {
		// コマンドラインがない場合は何もしない
		showErrorMessage();
		return 1;
	}

	return runWithSize(
		&info,
		commandLine);
}

/**
 * オプションを取得する。
 *
 * @param start 取得開始位置
 * @return オプションを除いたコマンドライン開始位置
 */
TCHAR *getOptions(TCHAR *start)
{
	TCHAR *p;
	bool found = true;

	p = start;

	while (found) {
		if (strcmpspc(p, _T("/no10")) != NULL) {
			noWin10 = true;
			p = p + 5;
			p = getWordStart(p);
			if (p == NULL) {
				return NULL;
			}
			found = true;
			continue;
		}

		if (strcmpspc(p, _T("/top")) != NULL) {
			topMost = true;
			p = p + 4;
			p = getWordStart(p);
			if (p == NULL) {
				return NULL;
			}
			found = true;
			continue;
		}
		found = false;
	}

	return p;
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
		_T("RunWithSize Version 1.0.4 By Tatsuhiko Syoji 2017\nUsage:\nRunWithSize width height x y (Options...) CommandLine\n"
			"Options:\n"
			"/top Show window above other windows\n"
			"/no10 Don't adjust window size and position on Windows 10.\n"),
		_T("How to use"),
		MB_OK | MB_ICONINFORMATION);
	//FreeLibrary(user32Dll);
}

/**
 * 単語を切り出す。<br>
 * 単語が見つかった場合、NULLターミネートを行う。
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

/**
 * スペースで区切られた単語を比較する
 *
 * @param target 検索対象文字列
 * @param toSearch 比較する文字列
 * @return 非NULL:検索対象文字列は比較する文字列だった NULL:検索対象文字列は比較する文字列ではない。
 */
TCHAR *strcmpspc(TCHAR *target, TCHAR *toSearch)
{
	TCHAR *p;
	TCHAR *s;

	p = target;
	s = toSearch;


	p = target;
	while (*p) {

		if (_istspace(*p)) {
			// 単語の切れ目でスペースが来たら見つかったこととする。
			if (*s == _T('\0')) {
				return target;
			}
		}
		if (*p != *s) {
			return NULL;
		}
		p++;
		s++;
	}
	if (*s == _T('\0')) {
		// どっちも終わっていた
		return target;
	}
	return NULL;
}

// 各ウインドウに対するチェック処理を行うコールバック
BOOL CALLBACK setWindowSize(HWND hWnd, LPARAM lparam)
{
	DWORD wndPid;
	struct positionInfo *posInfo;

	DWORD version = GetVersion();
	DWORD versionHigh = (DWORD)(LOBYTE(LOWORD(version)));

	posInfo = (struct positionInfo *)lparam;

	// 起動したプログラムのウインドウを探す
	GetWindowThreadProcessId(hWnd, &wndPid);
	if (wndPid == taqrgetPid) {
		// 見えないウインドウはサイズを調整してもしょうがないので、
		// 対象としない
		if (IsWindowVisible(hWnd)) {
			RECT place;

			if (IsWindow(hWnd)) {
				NONCLIENTMETRICS metrics;
				FillMemory(&metrics, sizeof(NONCLIENTMETRICS), 0x00);
				metrics.cbSize = sizeof(NONCLIENTMETRICS);
				SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
					metrics.cbSize,
					&metrics,
					0);

				GetWindowRect(hWnd, &place);

				// 左の計算
				if (!posInfo->noXpos) {
					// 指定した点を入れておく
					place.left = posInfo->x;
					place.right = posInfo->x;
					if ((versionHigh == 10) && (!noWin10)) {
						// Windows 10は見た目のウインドウの外に透明の枠線と空白があるので
						// 位置を見た目の位置に合わせる
						if (posInfo->fromRight) {
							// 右からの位置計算の場合
							place.right =
								posInfo->x +
								4 +
								(metrics.iPaddedBorderWidth - 1) +
								(metrics.iBorderWidth - 1);
						} else {
							place.left =
								place.left -
								4 -
								(metrics.iPaddedBorderWidth - 1) -
								(metrics.iBorderWidth - 1);
						}
					}
				}
				if (!posInfo->noYpos) {
					place.top = posInfo->y;
					place.bottom = posInfo->y;
					if ((versionHigh == 10) && (!noWin10)) {
						// Windows 10は見た目のウインドウの外に透明の枠線と空白があるので
						// 位置を見た目の位置に合わせる
						// ただし、上は見た目とシステムの上端が合う。
						if (posInfo->fromBottom) {
							place.bottom = posInfo->y +
								4 +
								(metrics.iBorderWidth - 1) +
								(metrics.iPaddedBorderWidth - 1);
						}
					}

				}

				// 幅・高さの計算
				RECT r;
				GetWindowRect(hWnd, &r);
				int orgWidth = r.right - r.left;
				int orgHeight = r.bottom - r.top;

				// 幅の計算
				int width;
				if (posInfo->noWidth) {
					width = orgWidth + 1;
				} else {
					width = posInfo->width;
					if ((versionHigh == 10) && (!noWin10)) {
						// Windows 10は見た目の幅の外側に透明の枠線と空白があるので、
						// 見た目の幅を指定した幅に合わせる
						width =
							width +
							8 +
							((metrics.iBorderWidth - 1) + (metrics.iPaddedBorderWidth - 1)) * 2;
					}
				}

				int height;
				if (posInfo->noHeight) {
					height = orgHeight + 1;
				} else {
					height = posInfo->height;
					if ((versionHigh == 10) && (!noWin10)) {
						height =
							height +
							(metrics.iBorderWidth - 1) +
							(metrics.iPaddedBorderWidth - 1) +
							4;

					}
				}

				// もともとのウインドウの大きさが0*0のものはリサイズ対象としない。
				// Delphi製アプリのウインドウクラスTApplicationのウインドウがこれに当たる。
				// 表示されるウインドウを相手にせず、起動中のウインドウの列挙なら
				// オーナーウインドウの有無で判定できるが、相手は表示されるウインドウなので
				// このような手段をとる必要がある。
				if ((orgWidth != 0) || (orgHeight != 0)) {
					if (posInfo->fromRight) {
						// 右からの指定の場合は左側の指定を直す。
						place.left = place.right - width + 1;
					} else {
						place.right = place.left + width - 1;
					}
					if (posInfo->fromBottom) {
						place.top = place.bottom - height + 1;
					} else {
						place.bottom = place.top + height - 1;
					}

					if (IsWindow(hWnd)) {
						HWND insertAfter = NULL;
						UINT flags = SWP_NOZORDER;
						if (topMost) {
							insertAfter = HWND_TOPMOST;
							flags = SWP_SHOWWINDOW;
						}
						SetWindowPos(
							hWnd,
							insertAfter,
							place.left,
							place.top,
							width,
							height,
							flags);
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