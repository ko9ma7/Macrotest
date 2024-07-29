#pragma once

#include <iostream>
#include <vector>
#include <string>

#include <Windows.h>

using namespace std;

typedef class ACTIVATE_MACRO {
private:
	//enum
	enum STATUS {
		MACRO_ERROR = -1,
		MACRO_INIT,
		MACRO_START,
		MACRO_STOP,
		MACRO_UPDATE
	};

	//����
	vector<INPUT> inputs{};
	unsigned int tickTime;
	signed char macroStatus = MACRO_INIT;

	//������ ���� ����
	//�ٸ� ������Ʈ�� �����Ǿ��� ��� ������ ���� ����ȭ�� ���Ͽ� �̸� ����
	HANDLE  threadHandle  = NULL,
				 macroMtx = NULL;

	CRITICAL_SECTION	cs{};
	CONDITION_VARIABLE	updateConditionVar{};

	//�Լ�
	void WarningMessage(const string msg);

	bool SetMacroTime(int tickTime);
	unsigned int GetMacroTime();
	
	//������ ���� �Լ�
	HANDLE GetMutex();
	CONDITION_VARIABLE GetConditionVar();
	CRITICAL_SECTION GetCriticalSection();

	bool CreateSlaveThread();
	static DWORD WINAPI MacroThread(LPVOID args);

public:
	ACTIVATE_MACRO(int tickTime = 0);
	~ACTIVATE_MACRO();

	byte GetMacroStatus();
	bool MacroStart();
	bool MacroStop();
	//������ ����ȭ�� ���� �Լ�
	bool MacroRun();

protected:
	bool MacroUpdate();

public:
	//bool RegisterMacroKey(const byte key);
	bool RegisterMacroKey(const byte key, const bool up = false);
	vector<INPUT> GetRegisterInputs();
	
}ActivateMacro;