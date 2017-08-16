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

	// ���̎擾
	commandWord = getWord(lpCmdLine, &nextStart);
	if (commandWord == NULL) {
		// �P�ꂪ�Ȃ����Ƃɂ������s���̏ꍇ�͉������Ȃ�
		showErrorMessage();
		return 1;
	}
	if (nextStart == NULL) {
		// �P���؂�o�������_�ŕ�����I���ɓ��B�����ꍇ(�����s���̏ꍇ)�͉������Ȃ�
		showErrorMessage();
		return 1;
	}
	if (!_tcscmp(_T("?"), commandWord)) {
		info.noWidth = true;
	} else {
		info.width = _tstoi(commandWord);
		info.noWidth = false;
	}

	// �����̎擾
	commandWord = getWord(nextStart, &nextStart);
	if (commandWord == NULL) {
		// �P�ꂪ�Ȃ����Ƃɂ������s���̏ꍇ�͉������Ȃ�
		showErrorMessage();
		return 1;
	}
	if (nextStart == NULL) {
		// �P���؂�o�������_�ŕ�����I���ɓ��B�����ꍇ(�����s���̏ꍇ)�͉������Ȃ�
		showErrorMessage();
		return 1;
	}
	if (!_tcscmp(_T("?"), commandWord)) {
		info.noHeight = true;
	} else {
		info.height = _tstoi(commandWord);
		info.noHeight = false;
	}

	// ���ʒu�̎擾
	commandWord = getWord(nextStart, &nextStart);
	if (commandWord == NULL) {
		// �P�ꂪ�Ȃ����Ƃɂ������s���̏ꍇ�͉������Ȃ�
		showErrorMessage();
		return 1;
	}
	if (nextStart == NULL) {
		// �P���؂�o�������_�ŕ�����I���ɓ��B�����ꍇ(�����s���̏ꍇ)�͉������Ȃ�
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

	// �c�ʒu�̎擾
	commandWord = getWord(nextStart, &nextStart);
	if (commandWord == NULL) {
		// �P�ꂪ�Ȃ����Ƃɂ������s���̏ꍇ�͉������Ȃ�
		showErrorMessage();
		return 1;
	}
	if (nextStart == NULL) {
		// �P���؂�o�������_�ŕ�����I���ɓ��B�����ꍇ(�����s���̏ꍇ)�͉������Ȃ�
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

	// �R�}���h���C���̎擾
	// �܂��A���̌��̂Ƃ���܂ōs���B
	commandLine = getWordStart(nextStart);

	if (commandLine != NULL) {
		commandLine = getOptions(commandLine);
	}

	if (commandLine == NULL) {
		// �R�}���h���C�����Ȃ��ꍇ�͉������Ȃ�
		showErrorMessage();
		return 1;
	}

	return runWithSize(
		&info,
		commandLine);
}

/**
 * �I�v�V�������擾����B
 *
 * @param start �擾�J�n�ʒu
 * @return �I�v�V�������������R�}���h���C���J�n�ʒu
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
 * �G���[���b�Z�[�W��\������
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
 * �P���؂�o���B<br>
 * �P�ꂪ���������ꍇ�ANULL�^�[�~�l�[�g���s���B
 *
 * @param targetStr �P��؂�o���Ώە�����
 * @param nextPtr ���̒P��؂�o���J�n�ʒu
 * @return NULL:�P�ꂪ������Ȃ� ��NULL:�P�ꂪ��������
 */
TCHAR *getWord(LPTSTR targetStr, TCHAR **nextPtr)
{
	TCHAR *p;

	p = targetStr;
	*nextPtr = NULL;

	// �ŏ��̃X�y�[�X�ȊO�̕�����T��
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
 * �X�y�[�X�ȊO�̎���̎n�܂����������B
 *
 * @param targetStr �P��؂�o���Ώە�����
 * @return NULL:����ȏ�̎���͑��݂��Ȃ� ��NULL:���傪���݂���
 */
TCHAR *getWordStart(LPTSTR targetStr)
{
	TCHAR *p;

	p = targetStr;

	// �ŏ��̃X�y�[�X�ȊO�̕�����T��
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
 * �X�y�[�X�ŋ�؂�ꂽ�P����r����
 *
 * @param target �����Ώە�����
 * @param toSearch ��r���镶����
 * @return ��NULL:�����Ώە�����͔�r���镶���񂾂��� NULL:�����Ώە�����͔�r���镶����ł͂Ȃ��B
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
			// �P��̐؂�ڂŃX�y�[�X�������猩���������ƂƂ���B
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
		// �ǂ������I����Ă���
		return target;
	}
	return NULL;
}

// �e�E�C���h�E�ɑ΂���`�F�b�N�������s���R�[���o�b�N
BOOL CALLBACK setWindowSize(HWND hWnd, LPARAM lparam)
{
	DWORD wndPid;
	struct positionInfo *posInfo;

	DWORD version = GetVersion();
	DWORD versionHigh = (DWORD)(LOBYTE(LOWORD(version)));

	posInfo = (struct positionInfo *)lparam;

	// �N�������v���O�����̃E�C���h�E��T��
	GetWindowThreadProcessId(hWnd, &wndPid);
	if (wndPid == taqrgetPid) {
		// �����Ȃ��E�C���h�E�̓T�C�Y�𒲐����Ă����傤���Ȃ��̂ŁA
		// �ΏۂƂ��Ȃ�
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

				// ���̌v�Z
				if (!posInfo->noXpos) {
					// �w�肵���_�����Ă���
					place.left = posInfo->x;
					place.right = posInfo->x;
					if ((versionHigh == 10) && (!noWin10)) {
						// Windows 10�͌����ڂ̃E�C���h�E�̊O�ɓ����̘g���Ƌ󔒂�����̂�
						// �ʒu�������ڂ̈ʒu�ɍ��킹��
						if (posInfo->fromRight) {
							// �E����̈ʒu�v�Z�̏ꍇ
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
						// Windows 10�͌����ڂ̃E�C���h�E�̊O�ɓ����̘g���Ƌ󔒂�����̂�
						// �ʒu�������ڂ̈ʒu�ɍ��킹��
						// �������A��͌����ڂƃV�X�e���̏�[�������B
						if (posInfo->fromBottom) {
							place.bottom = posInfo->y +
								4 +
								(metrics.iBorderWidth - 1) +
								(metrics.iPaddedBorderWidth - 1);
						}
					}

				}

				// ���E�����̌v�Z
				RECT r;
				GetWindowRect(hWnd, &r);
				int orgWidth = r.right - r.left;
				int orgHeight = r.bottom - r.top;

				// ���̌v�Z
				int width;
				if (posInfo->noWidth) {
					width = orgWidth + 1;
				} else {
					width = posInfo->width;
					if ((versionHigh == 10) && (!noWin10)) {
						// Windows 10�͌����ڂ̕��̊O���ɓ����̘g���Ƌ󔒂�����̂ŁA
						// �����ڂ̕����w�肵�����ɍ��킹��
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

				// ���Ƃ��Ƃ̃E�C���h�E�̑傫����0*0�̂��̂̓��T�C�Y�ΏۂƂ��Ȃ��B
				// Delphi���A�v���̃E�C���h�E�N���XTApplication�̃E�C���h�E������ɓ�����B
				// �\�������E�C���h�E�𑊎�ɂ����A�N�����̃E�C���h�E�̗񋓂Ȃ�
				// �I�[�i�[�E�C���h�E�̗L���Ŕ���ł��邪�A����͕\�������E�C���h�E�Ȃ̂�
				// ���̂悤�Ȏ�i���Ƃ�K�v������B
				if ((orgWidth != 0) || (orgHeight != 0)) {
					if (posInfo->fromRight) {
						// �E����̎w��̏ꍇ�͍����̎w��𒼂��B
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
 * ���A�����A(���ʒu)�A(�c�ʒu)���w�肵�ăA�v���P�[�V�������N������
 *
 * @param  posInfo �ʒu���
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

	// �N�������v���O�����̃E�C���h�E��5�b�ԒT��
	resized = false;
	for (int i = 0; resized == false && i < 5; i++) {
		// �A�v�������T�C�Y������A�A�v���̋N����҂����肷��̂�
		// ���̕��҂�
		Sleep(1000);
		EnumWindows(setWindowSize, (LPARAM)posInfo);
	}


	CloseHandle(procInfo.hThread);
	CloseHandle(procInfo.hProcess);


	return 0;
}