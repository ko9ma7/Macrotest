#include "Recorder.h"

RECORDER::RECORDER() {
	status = REC_INIT;

	


}

RECORDER::~RECORDER() {
	End();
	//������ ����
	WaitForSingleObject(threadHandle, INFINITE);
	CloseHandle(threadHandle);
}

DWORD WINAPI RECORDER::RecordingThread(void* arg) {
	Recorder* rc = (Recorder*)arg;
	
	if (rc == nullptr || rc == NULL) {
		//msgbox �̿� ��� ����Ͽ�����, �� Ŭ������ ���� �޼ҵ� Ȱ�� ���ɼ� ����
		MessageBoxA(NULL, "Failed to create recroding thread", "ERROR", NULL);
		return -1;
	}

	static vector<KeyHistory> kbHistory{};
	HHOOK hookHandle = NULL;

	hookHandle = SetWindowsHookEx(WH_KEYBOARD_LL, rc->KeyHookProc, NULL, NULL);
	if (hookHandle == NULL) {
		MessageBoxA(NULL, "Failed to SetWindowsHookEx", "ERROR", NULL);
		return -1;
	}

	MSG msg;
	char status;

	while (status = rc->GetStatus()) {
		if (status != REC_RUNNING) {
			if (status == REC_STOP) {
				// ���Ѵ��
				while (status != REC_RUNNING) {
					status = rc->GetStatus();
				}
			}
			else if (status == REC_END) {
				//������ ����
				break;
			}
		}

		else {
			if (GetMessage(&msg, 0, NULL, NULL) != 0) {
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}

		}
	}

	UnhookWindowsHookEx(hookHandle);

	return 0;
}

LRESULT CALLBACK RECORDER::KeyHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	//wParam ���� Ű �ڵ�
	//cout << "wParam : " << wParam << endl;

	if (nCode < 0) {
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}

	static vector<KeyHistory> kbHistory;
	
	KBDLLHOOKSTRUCT* kls = (KBDLLHOOKSTRUCT*)lParam;

	//GetMessageTime() �Լ� ����
	static LONG time = kls->time;
	unsigned int recordingTime = kls->time - time;

	//cout << "Recording Time " << (kls->time - time) / 1000 << "." << (kls->time - time) % 1000 << endl;

	//cout << "kls->vkCode : " << kls->vkCode << endl;
	
	KeyHistory kh{};

	kh.keyType = wParam;
	kh.recordingTime = recordingTime;
	kh.vkCode = kls->vkCode;
	
	kbHistory.push_back(kh);

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

bool RECORDER::Recording() {
	if (threadHandle == NULL) {
		threadHandle = CreateThread(NULL, 0, RecordingThread, this, NULL, NULL);
		if (threadHandle == NULL) {
			cerr << "Failed to create thread" << endl;
			return false;
		}
	}

	status = REC_RUNNING;

	return true;
}

char RECORDER::GetStatus() {
	return this->status;
}

bool RECORDER::Stop() {
	status = REC_STOP;

	if (threadHandle != NULL) {
		PostThreadMessage(GetThreadId(threadHandle), WM_QUIT, 1, 0);
	}
	
	return true;
}

void RECORDER::End() {
	status = REC_END;

	if (threadHandle != NULL) {
		PostThreadMessage(GetThreadId(threadHandle), WM_QUIT, 1, 0);
	}
}