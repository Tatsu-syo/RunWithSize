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
	commandLine = getWordStart(nextStart);
	if (commandLine == NULL) {
		// �R�}���h���C�����Ȃ��ꍇ�͉������Ȃ�
		showErrorMessage();
		return 1;
	}

	return runWithSize(
		&info,
		commandLine);
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
		_T("RunWithSize Version 1.0.1 By Tatsuhiko Syoji 2017\nUsage:\nRunWithSize width height x y CommandLine"),
		_T("How to use"),
		MB_OK | MB_ICONINFORMATION);
	//FreeLibrary(user32Dll);
}

/**
 * �P���؂�o���B
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

// �e�E�C���h�E�ɑ΂���`�F�b�N�������s���R�[���o�b�N
BOOL CALLBACK setWindowSize(HWND hWnd, LPARAM lparam)
{
	DWORD wndPid;
	WINDOWPLACEMENT place;
	place.length = sizeof(WINDOWPLACEMENT);
	struct positionInfo *posInfo;
	
	posInfo = (struct positionInfo *)lparam;

	// �N�������v���O�����̃E�C���h�E��T��
	GetWindowThreadProcessId(hWnd, &wndPid);
	if (wndPid == taqrgetPid) {
		// �����Ȃ��E�C���h�E�̓T�C�Y�𒲐����Ă����傤���Ȃ��̂ŁA
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

				// ���Ƃ��Ƃ̃E�C���h�E�̑傫����0*0�̂��̂̓��T�C�Y�ΏۂƂ��Ȃ��B
				// Delphi���A�v���̃E�C���h�E�N���XTApplication�̃E�C���h�E������ɓ�����B
				// �\�������E�C���h�E�𑊎�ɂ����A�N�����̃E�C���h�E�̗񋓂Ȃ�
				// �I�[�i�[�E�C���h�E�̗L���Ŕ���ł��邪�A����͕\�������E�C���h�E�Ȃ̂�
				// ���̂悤�Ȏ�i���Ƃ�K�v������B
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