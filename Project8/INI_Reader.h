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
		READ = 1,			// 읽어오는 상태
		FAILED = 2,			// 읽어오는 상태
		CREATE = 3,			// 새로 만드는 상태
		SET_PATH = 4,		// INI이 있는 경로를 입력하는 상태
	};

private:
	std::wstring iniPath;	// INI가 있는 디렉토리 정보
	std::wstring iniData;	// INI에 있던 디렉토리 정보
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
INI를 찾아봄

 1. 없을 경우
	1) 생성할지 안 할지
	2) INI 경로 입력창

 2. 있을 경우
	INI 안에 있는 디렉토리 정보를 저장함.
*/
