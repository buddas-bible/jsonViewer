#include "INI_Reader.h"

#include <windows.h>
#include <fstream>
#include <iostream>
#include <locale>

constexpr unsigned int BUFFER_MAX_SIZE = 10000;

INI_Reader::INI_Reader() : 
	iniPath(L".\\setting.ini"),
	iniData()
{
	std::wcin.imbue(std::locale("korean"));
	std::wcout.imbue(std::locale("korean"));
}

INI_Reader::~INI_Reader()
{
	iniPath.clear();
}

void INI_Reader::Run()
{
	std::wcout
		<< L"===========================================" << '\n'
		<< L'\n'
		<< L"             ini Reader 실행" << '\n'
		<< L'\n'
		<< L"===========================================" << '\n';

	while (state != INI_Reader::State::EXIT)
	{

		switch (state)
		{
			case INI_Reader::State::READ:
			{
				// system("cls");
				state = Read();
			}
			break;

			case INI_Reader::State::FAILED:
			{
				// system("cls");
				state = Failed();
			}
			break;

			case INI_Reader::State::CREATE:
			{
				// system("cls");
				state = Create();
			}
			break;
		
			case INI_Reader::State::SET_PATH:
			{
				// system("cls");
				state = SetPath();
			}
			break;

			default:
				throw(L"적합하지 않은 값입니다. INI_Reader를 확인해주세요.");
				break;
		}
	}

	std::wcout
		<< L"===========================================" << '\n'
		<< L'\n'
		<< L"             ini Reader 종료" << '\n'
		<< L'\n'
		<< L"===========================================" << '\n';
}

std::wstring INI_Reader::GetINIData()
{
	return iniData;
}

/// <summary>
/// INI을 읽어옴
/// </summary>
INI_Reader::State INI_Reader::Read()
{
	std::cout
		<< "===========================================" << '\n'
		<< '\n'
		<< "              ini  파일 읽기                " << '\n'
		<< '\n'
		<< "===========================================" << '\n';

	// 변수에 데이터가 있다면 루프를 탈출한다.
	if (!iniData.empty())
	{
		std::wcout
			<< L"===========================================" << '\n'
			<< L'\n'
			<< L"        ini에 저장된 데이터를 불러옴" << '\n'
			<< L'\n'
			<< L"     Data : " << iniData << '\n'
			<< L'\n'
			<< L"===========================================" << '\n';
		return State::EXIT;
	}
	
	// 맴버 변수에 데이터가 없다면 파일을 읽어온다.
	// ini 안에 있는 디렉토리 정보를 저장할 버퍼

	WCHAR* buffer2 = new WCHAR[BUFFER_MAX_SIZE]{};
	GetCurrentDirectoryW(BUFFER_MAX_SIZE, buffer2);
	std::wstring temp = buffer2;
	if (iniPath == L"setting.ini")
	{
		iniPath = temp + L"\\" + iniPath;
	}
	else if (iniPath == L"\\setting.ini")
	{
		iniPath = temp + L"\\setting.ini";
	}

	// INI 파일 안에 있는 디렉토리 정보를 읽어서 변수에 저장한다.
	// INI 파일 경로의 기본값은 L"\\setting.ini"
	WCHAR* buffer = new WCHAR[BUFFER_MAX_SIZE]{};
	DWORD r = GetPrivateProfileStringW(L"Directory", L"dir", NULL, buffer, BUFFER_MAX_SIZE, iniPath.c_str());

	if (r == 0)
	{
		// INI이 없다면 경로 설정 또는 생성.
		std::cout
			<< "===========================================" << '\n'
			<< '\n'
			<< "              ini  읽기 실패                " << '\n'
			<< '\n'
			<< "===========================================" << '\n';

		delete[] buffer;
		buffer = nullptr;
		delete[] buffer2;
		buffer2 = nullptr;
		return State::FAILED;
	}

	// 읽어온 데이터를 변수에 저장하고 루프를 탈출함.
	iniData = buffer;

	std::wcout
		<< L"===========================================" << '\n'
		<< L'\n'
		<< L"        ini에 저장된 데이터를 불러옴" << '\n'
		<< L'\n'
		<< L"     Data : " << iniData << '\n'
		<< L'\n'
		<< L"===========================================" << '\n';

	delete[] buffer;
	buffer = nullptr;
	delete[] buffer2;
	buffer2 = nullptr;
	return State::EXIT;
}

/// <summary>
/// INI의 경로를 설정할지 새로 만들지 정하는 함수
/// </summary>
/// <returns></returns>
INI_Reader::State INI_Reader::Failed()
{

	std::wcout
		<< L"===========================================" << '\n'
		<< L"  다음 중 하나를 선택해주세요." << '\n'
		<< L'\n'
		<< L"     1. ini 파일 생성" << '\n'
		<< L"     2. ini 경로 설정" << '\n'
		<< L"===========================================" << '\n'
		<< L'\n'
		<< L"입력 : ";

	// 정하는 변수
	int buffer{};
	std::wcin.clear();
	std::wcin >> buffer;
	while (std::wcin.fail())
	{
		std::wcout
			<< L"잘못된 입력입니다." << '\n'
			<< L"다시 입력해주세요." << '\n'
			<< L"입력 : ";
		std::wcin.clear();
		std::wcin.ignore(100, '\n');
		std::wcin >> buffer;
	}

	switch (buffer)
	{
		case 1:
			return State::CREATE;
			break;

		case 2:
			return State::SET_PATH;
			break;

		default:
			return State::FAILED;
			break;
	}
}

/// <summary>
/// 새 INI을 생성
/// </summary>
INI_Reader::State INI_Reader::Create()
{
	std::cout
		<< "===========================================" << '\n'
		<< '\n'
		<< "              ini  파일 생성                " << '\n'
		<< '\n'
		<< "===========================================" << '\n';

	// ini를 생성할 위치를 담을 버퍼
	WCHAR* buffer = new WCHAR[BUFFER_MAX_SIZE]{};

	if (!GetCurrentDirectoryW(BUFFER_MAX_SIZE, buffer))
	{
		// 현재 경로 저장 실패
		throw(L"현재 경로를 저장할 수 없습니다. 경로를 확인 바랍니다.");
		delete[] buffer;
		return State::EXIT;
	}
	
	// 현재 경로를 맴버 변수에 저장하고 할당을 해제한다.
	std::wstring temp = buffer;
	if (iniPath == L"setting.ini")
	{
		temp = temp + L"\\" + iniPath;
	}
	else
	{
		temp = temp + L"\\setting.ini";
	}
	iniPath = temp;
	delete[] buffer;
	buffer = nullptr;

	std::wcout
		<< iniPath << '\n'
		<< L"  해당 경로에 생성될 ini 파일에 저장할 데이터를 입력해주세요." << '\n'
		<< L'\n'
		<< L"  입력 : ";

	// ini에 저장할 데이터를 입력받는다.
	// buffer = new WCHAR[BUFFER_MAX_SIZE]{};
	// std::wcin.clear();
	// std::wcin >> buffer;
	// while (std::wcin.fail())
	// {
	// 	std::wcin.clear();
	// 	std::wcin >> buffer;
	// }

	std::wstring buffer1{};
	std::wcin.clear();
	std::wcin >> iniData;
	std::getline(std::wcin, buffer1);
	while (std::wcin.fail())
	{
		std::wcin.clear();
		std::wcin >> iniData;
		std::getline(std::wcin, buffer1);
	}

	// 입력한 경로를 맴버 변수에 저장하고 읽어옴.
	iniData = iniData + buffer1;

	/*
	iniData = buffer;
	delete[] buffer;
	buffer = nullptr;
	*/

	// INI를 생성
	if (WritePrivateProfileStringW(L"Directory", L"dir", iniData.c_str(), iniPath.c_str()))
	{
		std::wcout
			<< "===========================================" << '\n'
			<< '\n'
			<< L"   ini 파일이 생성되었습니다." << '\n'
			<< '\n'
			<< L"   파일 읽기를 실시합니다." << '\n'
			<< '\n'
			<< "===========================================" << '\n';
		
		return State::READ;
	}

	return State::CREATE;
}

/// <summary>
/// INI 경로 설정하는 함수
/// </summary>
/// <returns></returns>
INI_Reader::State INI_Reader::SetPath()
{
	std::wcout
		<< L"===========================================" << '\n'
		<< '\n'
		<< L"               .ini 경로 설정" << '\n'
		<< '\n'
		<< L"===========================================" << '\n';
	
	std::wcout
		<< L"ini 경로를 입력해주세요." << '\n';

	std::wstring buffer{};
	std::wcin.clear();
	std::wcin >> iniPath;
	std::getline(std::wcin, buffer);
	while (std::wcin.fail())
	{
		std::wcin.clear();
		std::wcin >> iniPath;
		std::getline(std::wcin, buffer);
	}

	// 입력한 경로를 맴버 변수에 저장하고 읽어옴.
	iniPath = iniPath + buffer;

	std::wcout
		<< L"===========================================" << '\n'
		<< '\n'
		<< L"     입력한 경로 : " << iniPath << '\n'
		<< '\n'
		<< L"     해당 경로에 있는 ini 파일 읽기를 시도합니다." << '\n'
		<< '\n'
		<< L"===========================================" << '\n';

	return State::READ;
}