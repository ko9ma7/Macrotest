#pragma once

#include <iostream>
#include <string>
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

protected:
	static vector<KeyHistory> kbHistory;

public:
	RECORDER();
	~RECORDER();

	bool Recording();
	char GetStatus();
	bool Stop();
	void End();

	vector<KeyHistory> GetRecordData();
	bool ResetRecordData();
	bool SaveRecordData(const std::string fileName);

}Recorder;

