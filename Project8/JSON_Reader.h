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
		READ_DIRECTORY,		// 디렉토리 읽기
		SHOW_DIRECTORY,
		STANDBY,
		READ_JSON,			// 읽어오는 상태
		LOAD_JSON,			// 파일 로드?
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
* 1. setting.ini를 읽어서 디렉토리 이름을 가져옴
* 
* 2. 해당 디렉토리의 JSON 파일을 모두 표시
* 3. 특정 JSON 파일을 선택
* 4. JSON 파일을 읽어와서 내용을 표시
* 5. 로드한 JSON 파일이 수정되면 자동으로 감지, 안내
* 6. 수정된 최신 버전으로 다시 로드
* 7. 파일 목록이 수정되면 변경 사항을 화면에 표시
*/