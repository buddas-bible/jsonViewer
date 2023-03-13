#include "JSON_Reader.h"

#include <iostream>
#include <fstream>
// #include <io.h>
// #include <strsafe.h>
// #include <tchar.h>

namespace fs = std::filesystem;

JSON_Reader::JSON_Reader() :
	directory(), jsonName(), jsonList()
{
	std::wcin.imbue(std::locale("korean"));
	std::wcout.imbue(std::locale("korean"));
}

JSON_Reader::~JSON_Reader()
{

}

void JSON_Reader::Run()
{
	std::wcout
		<< L"===========================================" << '\n'
		<< L'\n'
		<< L"             json Reader 실행" << '\n'
		<< L'\n'
		<< L"===========================================" << '\n';

	while (state != JSON_Reader::State::EXIT)
	{
		input.Update();


		switch (state)
		{
			case JSON_Reader::State::READ_DIRECTORY:
			{
				state = ReadDirectory();
			}
			break;

			case JSON_Reader::State::SHOW_DIRECTORY:
			{
				state = ShowDirectory();
			}
			break;

			case State::READ_JSON:
			{
				state = ReadJSON();
			}
			break;

			case State::LOAD_JSON:
			{
				state = LoadJSON();
			}
			break;

			case State::STANDBY:
			{
				state = StandbyMode();
			}
			break;
		}
	}

	std::wcout
		<< L"===========================================" << '\n'
		<< L'\n'
		<< L"             json Reader 종료" << '\n'
		<< L'\n'
		<< L"===========================================" << '\n';
}

void JSON_Reader::SetDirectory(std::wstring str)
{
	if (str.empty())
	{
		return;
	}

	directory = str;
	// directory = directory + L"\\*.json";
}

JSON_Reader::State JSON_Reader::ReadDirectory()
{
	jsonList.clear();

	std::wstring path = directory + L"\\*.json";
	WIN32_FIND_DATAW findfileData;
	HANDLE hFind;

	if ((hFind = FindFirstFile(path.c_str(), &findfileData)) != INVALID_HANDLE_VALUE)
	{
		do
		{
			jsonList.push_back(findfileData.cFileName);
		} while (FindNextFile(hFind, &findfileData) != 0);

		FindClose(hFind);
	}

	return State::SHOW_DIRECTORY;
}

JSON_Reader::State JSON_Reader::ShowDirectory()
{
	std::wcout
		<< L"===========================================" << '\n'
		<< L'\n'
		<< directory << L'\n'
		<< L'\n'
		<< L"해당 디렉토리 안에 있는 .json 목록을 출력합니다." << '\n'
		<< L'\n'
		<< L"===========================================" << '\n';

	/// dir 디렉토리를 열어서 해당 디렉토리의
	/// json 파일의 이름을 json 리스트에 담는다.
	for (auto i = 0; i < jsonList.size(); i++)
	{
			std::wcout << L"1. " << jsonList[i] << std::endl;
	}

	std::wcout << std::endl  
		<< L"오픈할 파일 인덱스를 입력해주세요." << std::endl
		<< L"입력 : ";

	std::wcin >> selectIndex;
	while (std::wcin.fail())
	{
		std::wcout
			<< L"잘못된 입력입니다." << '\n'
			<< L"다시 입력해주세요." << '\n'
			<< L"입력 : ";
		std::wcin.clear();
		std::wcin.ignore(100, '\n');
		std::wcin >> selectIndex;
	}

	if (selectIndex == 0)
	{
		return State::EXIT;
	}
	else if (selectIndex >= jsonList.size())
	{
		selectIndex = jsonList.size() - 1;
	}
	else
	{
		selectIndex -= 1;
	}

	return State::READ_JSON;
}

JSON_Reader::State JSON_Reader::StandbyMode()
{
	std::wstring path{};
	path = directory + L"\\" + jsonList[selectIndex];

	fs::path file_path = path;
	fs::file_time_type new_write_time = fs::last_write_time(file_path);
	if (new_write_time != fileTimeMap[jsonList[selectIndex]])
	{
		std::wcout
			<< L"===========================================" << '\n'
			<< L"로드 중인 파일이 변경 되었습니다." << '\n'
			<< '\n'
			<< L"스페이스바를 눌러 변경된 파일을 로드합니다." << '\n'
			<< L"===========================================" << '\n';
		fileTimeMap[jsonList[selectIndex]] = new_write_time;
		return State::LOAD_JSON;
	}

	if (input.GetPressedKey(VK_BACK))
	{
		return State::READ_DIRECTORY;
	}

	return State::STANDBY;
}

JSON_Reader::State JSON_Reader::ReadJSON()
{
	/// 선택한 json 파일을 출력한다.
	std::wcout
		<< L"===========================================" << '\n'
		<< L'\n'
		<< jsonList[selectIndex] << L'\n'
		<< L'\n'
		<< L"선택한 json 파일을 읽어옵니다." << '\n'
		<< L'\n'
		<< L"===========================================" << '\n';

	std::wstring data;
	std::wstring path{};
	path = directory + L"\\" + jsonList[selectIndex];

	std::wifstream fin;
	fin.open(path);
	if (fin.is_open())
	{
		while (!fin.eof())
		{
			std::getline(fin, data);
			std::wcout << data << std::endl;
		}
	}

	fs::path file_path = path;
	fs::file_time_type last_write_time = fs::last_write_time(file_path);
	fileTimeMap[jsonList[selectIndex]] = last_write_time;

	fin.close();
	return State::STANDBY;
}

JSON_Reader::State JSON_Reader::LoadJSON()
{
	if (input.GetPressedKey(VK_SPACE))
	{
		return State::READ_JSON;
	}

	return State::LOAD_JSON;
}