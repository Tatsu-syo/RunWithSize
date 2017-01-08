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
	width = _tstoi(commandWord);

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
	height = _tstoi(commandWord);

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
		noXpos = true;
	} else {
		x = _tstoi(commandWord);
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
		noYpos = true;
	} else {
		y = _tstoi(commandWord);
	}

	// �R�}���h���C���̎擾
	commandLine = getWordStart(nextStart);
	if (commandLine == NULL) {
		// �R�}���h���C�����Ȃ��ꍇ�͉������Ȃ�
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
	HWND parent;
	HWND now;
	WINDOWPLACEMENT place;
	place.length = sizeof(WINDOWPLACEMENT);

	

	// �N�������v���O�����̃E�C���h�E��T��
	GetWindowThreadProcessId(hWnd, &wndPid);
	if (wndPid == taqrgetPid) {
		// �����Ȃ��E�C���h�E�̓T�C�Y�𒲐����Ă����傤���Ȃ��̂ŁA
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
 * ���A�����A(���ʒu)�A(�c�ʒu)���w�肵�ăA�v���P�[�V�������N������
 *
 * @param  width ��
 * @param  height ����
 * @param  x ���ʒu
 * @param  y �c�ʒu
 * @param  noXpos ���ʒu���w�肷�邩�ǂ���
 * @param  noYpos �c�ʒu���w�肷�邩�ǂ���
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

	// �N�������v���O�����̃E�C���h�E��5�b�ԒT��
	resized = false;
	for (int i = 0; resized == false && i < 5; i++) {
		// �A�v�������T�C�Y������A�A�v���̋N����҂����肷��̂�
		// ���̕��҂�
		Sleep(1000);
		EnumWindows(setWindowSize, 0);
	}


	CloseHandle(procInfo.hThread);
	CloseHandle(procInfo.hProcess);


	return 0;
}