#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <random>

#include <Windows.h>

using namespace std;

#define DURATION_MIN	5
#define DURATION_MAX	10

typedef struct EXTENDED_INPUT {
	DWORD recordingTime;
	INPUT input;
}ExtendedInput;

typedef class ACTIVATE_MACRO {
private:
	//enum
	enum STATUS {
		MACRO_ERROR = -1,
		MACRO_INIT,
		MACRO_START,
		MACRO_STOP,
		MACRO_PLAY,
		MACRO_UPDATE,
		MACRO_RANDOM,
	};

	//����
	vector<vector<ExtendedInput>> macroAlgorithms;

	//vector<INPUT> inputs{};
	vector<ExtendedInput> inputs{};

	unsigned int tickTime;
	//2024-08-19  uchar -> int
	//signed char macroStatus = MACRO_INIT;
	int macroStatus = MACRO_INIT;

	//������ ���� ����
	//�ٸ� ������Ʈ�� �����Ǿ��� ��� ������ ���� ����ȭ�� ���Ͽ� �̸� ����
	HANDLE  threadHandle  = NULL,
				 macroMtx = NULL;

	CRITICAL_SECTION	cs{};
	CONDITION_VARIABLE	updateConditionVar{};

	//�Լ�
	void WarningMessage(const string msg);

	//2024-08-02 public ���� �̵�
	/*bool SetMacroTime(int tickTime);
	unsigned int GetMacroTime();*/
	
	//������ ���� �Լ�
	HANDLE GetMutex();
	//CONDITION_VARIABLE GetConditionVar();
	//CONDITION_VARIABLE& GetConditionVar();
	CONDITION_VARIABLE* GetConditionVar();
	CRITICAL_SECTION GetCriticalSection();

	bool CreateSlaveThread();
	static DWORD WINAPI MacroThread(LPVOID args);

public:
	ACTIVATE_MACRO(int tickTime = 0);
	~ACTIVATE_MACRO();

	bool Reset();

	//���� ���� unsigned char x, int ������ ����
	//byte GetMacroStatus();
	int GetMacroStatus();
	bool MacroStart();
	bool MacroStop();
	//2024-08-19 ��ũ�� ���� ������ �� �ٽý����� �ƴ� �ش� �������� �ٽ� ���
	bool MacroPlay();
	
//protected:
	bool MacroUpdate();
	bool MacroRandom();

public:
	bool SetMacroTime(int tickTime);
	unsigned int GetMacroTime();

	//ŰŸ��  byte -> WORD ������ ����
	//bool RegisterMacroKey(const byte key);
	//bool RegisterMacroKey(const byte key, const bool up = false);
	bool RegisterMacroKey(const WORD key, const bool up = false);
	//bool RegisterMacroKey(const DWORD time, const WPARAM keyType, const byte key,const DWORD scanCode);
	bool RegisterMacroKey(const DWORD time, const WPARAM keyType, const WORD key,const DWORD scanCode);
	bool RegisterCurrentAlgorithm();

	byte GetAlgorithmCount();
	bool SelectAlgorithm(int idx);
	

	vector<EXTENDED_INPUT> GetRegisterInputs();
	
}ActivateMacro;