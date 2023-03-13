#pragma once
#include <string>
#include <vector>
#include <windows.h>
#include "Input.h"
#include <map>
#include <filesystem>

namespace fs = std::filesystem;

class JSON_Reader
{
public:
	JSON_Reader();
	~JSON_Reader();

	enum class State
	{
		EXIT = 0,
		READ_DIRECTORY,		// ���丮 �б�
		SHOW_DIRECTORY,
		STANDBY,
		READ_JSON,			// �о���� ����
		LOAD_JSON,			// ���� �ε�?
	};

private:
	std::wstring directory;
	std::vector<std::wstring> jsonList;
	std::wstring jsonName;
	std::map<std::wstring, fs::file_time_type> fileTimeMap;
	State state = State::READ_DIRECTORY;

public:
	void Run();

	void SetDirectory(std::wstring str);

private:
	int selectIndex;
	Input input;

	State ReadDirectory();
	State ShowDirectory();
	State StandbyMode();
	State ReadJSON();
	State LoadJSON();
};


/*
* 1. setting.ini�� �о ���丮 �̸��� ������
* 
* 2. �ش� ���丮�� JSON ������ ��� ǥ��
* 3. Ư�� JSON ������ ����
* 4. JSON ������ �о�ͼ� ������ ǥ��
* 5. �ε��� JSON ������ �����Ǹ� �ڵ����� ����, �ȳ�
* 6. ������ �ֽ� �������� �ٽ� �ε�
* 7. ���� ����� �����Ǹ� ���� ������ ȭ�鿡 ǥ��
*/