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
	byte macroStatus;

	HANDLE threadHandle;

	//�Լ�
	void WarningMessage(const string msg);

	bool SetMacroTime(int tickTime);
	unsigned int GetMacroTime();

	bool CreateSlaveThread();
	static DWORD WINAPI MacroThread(LPVOID args);

public:
	ACTIVATE_MACRO(int tickTime);
	~ACTIVATE_MACRO();

	byte GetMacroStatus();
	bool MacroStart();
	bool MacroStop();

protected:
	bool MacroUpdate();

public:
	bool RegisterMacroKey(const byte key);
	vector<INPUT> GetRegisterInputs();
	
}ActivateMacro;