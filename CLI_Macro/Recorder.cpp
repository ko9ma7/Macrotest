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

vector<KeyHistory> RECORDER::kbHistory;

LRESULT CALLBACK RECORDER::KeyHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	//wParam ���� Ű �ڵ�
	//cout << "wParam : " << wParam << endl;

	if (nCode < 0) {
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}
	
	KBDLLHOOKSTRUCT* kls = (KBDLLHOOKSTRUCT*)lParam;

	//GetMessageTime() �Լ� ����
	static LONG time = kls->time;
	unsigned int recordingTime = kls->time - time;

	//cout << "Recording Time " << (kls->time - time) / 1000 << "." << (kls->time - time) % 1000 << endl;

 	cout << "wParam : " << wParam << endl;
	cout << "lParam : " << lParam<< endl;
	cout << "kls->vkCode : " << kls->vkCode << endl;
	cout << "kls->scanCode : " << kls->scanCode << endl;
	
	KeyHistory kh{};

	kh.keyType = wParam;
	kh.recordingTime = recordingTime;
	kh.vkCode = kls->vkCode;
	kh.scanCode = kls->scanCode;
	
	
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

vector<KeyHistory> RECORDER::GetRecordData() {
	return kbHistory;
}

bool RECORDER::ResetRecordData() {
	if (kbHistory.empty()) {
		cerr << "Aleady empty record data" << endl;
		return false;
	}

	kbHistory.clear();
	kbHistory.resize(0);

	return true;
}

bool RECORDER::SaveRecordData(const std::string fileName) {
	if (kbHistory.empty()) {
		cerr << "Failed to save record data" << endl;
		return false;
	}

	int idx = 0;

	for (auto& kb : kbHistory) {
		auto appName = to_string(++idx);

		WritePrivateProfileStringA(appName.c_str(), "RecordingTime", to_string(kb.recordingTime).c_str(), fileName.c_str());

		if (kb.keyType == WM_KEYDOWN) {
			WritePrivateProfileStringA(appName.c_str(), "KeyType", "KEYDOWN", fileName.c_str());
		}
		else if (kb.keyType == WM_KEYUP) {
			WritePrivateProfileStringA(appName.c_str(), "KeyType", "KEYUP", fileName.c_str());
		}

		WritePrivateProfileStringA(appName.c_str(), "VKCode", to_string(kb.vkCode).c_str(), fileName.c_str());
	}

	return true;
}