#pragma once

#include <iostream>
#include <vector>

#include <Windows.h>

using namespace std;

typedef struct KEYBOARD_HISTORY {
	DWORD recordingTime;
	WPARAM keyType;
	DWORD vkCode;
}KeyHistory;

typedef class RECORDER {
private:
	enum STATUS {
		REC_ERROR = -1,
		REC_INIT,
		REC_RUNNING,
		REC_STOP,
		REC_END
	};

	char status = REC_INIT;

	//�ش� ������� ������ �ϳ��� ��Ʈ�� �Ҳ���, ���� ���·� ��������
	HANDLE threadHandle = NULL;

	static DWORD WINAPI RecordingThread(void* arg);
	static LRESULT CALLBACK KeyHookProc(int nCode, WPARAM wParam, LPARAM lParam);

public:
	RECORDER();
	~RECORDER();

	bool Recording();
	char GetStatus();
	bool Stop();
	void End();

}Recorder;

