#pragma once
#include <string>
#include <vector>

class INI_Reader
{
public:
	INI_Reader();
	~INI_Reader();

	enum class State
	{
		EXIT = 0,
		READ = 1,			// �о���� ����
		FAILED = 2,			// �о���� ����
		CREATE = 3,			// ���� ����� ����
		SET_PATH = 4,		// INI�� �ִ� ��θ� �Է��ϴ� ����
	};

private:
	std::wstring iniPath;	// INI�� �ִ� ���丮 ����
	std::wstring iniData;	// INI�� �ִ� ���丮 ����
	State state = State::READ;

public:
	void Run();

	std::wstring GetINIData();

private:
	State Read();
	State ReadDirectory();
	State Failed();
	State Create();
	State SetPath();
};


/*
INI�� ã�ƺ�

 1. ���� ���
	1) �������� �� ����
	2) INI ��� �Է�â

 2. ���� ���
	INI �ȿ� �ִ� ���丮 ������ ������.
*/
