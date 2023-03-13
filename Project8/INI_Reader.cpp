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
		<< L"             ini Reader ����" << '\n'
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
				throw(L"�������� ���� ���Դϴ�. INI_Reader�� Ȯ�����ּ���.");
				break;
		}
	}

	std::wcout
		<< L"===========================================" << '\n'
		<< L'\n'
		<< L"             ini Reader ����" << '\n'
		<< L'\n'
		<< L"===========================================" << '\n';
}

std::wstring INI_Reader::GetINIData()
{
	return iniData;
}

/// <summary>
/// INI�� �о��
/// </summary>
INI_Reader::State INI_Reader::Read()
{
	std::cout
		<< "===========================================" << '\n'
		<< '\n'
		<< "              ini  ���� �б�                " << '\n'
		<< '\n'
		<< "===========================================" << '\n';

	// ������ �����Ͱ� �ִٸ� ������ Ż���Ѵ�.
	if (!iniData.empty())
	{
		std::wcout
			<< L"===========================================" << '\n'
			<< L'\n'
			<< L"        ini�� ����� �����͸� �ҷ���" << '\n'
			<< L'\n'
			<< L"     Data : " << iniData << '\n'
			<< L'\n'
			<< L"===========================================" << '\n';
		return State::EXIT;
	}
	
	// �ɹ� ������ �����Ͱ� ���ٸ� ������ �о�´�.
	// ini �ȿ� �ִ� ���丮 ������ ������ ����

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

	// INI ���� �ȿ� �ִ� ���丮 ������ �о ������ �����Ѵ�.
	// INI ���� ����� �⺻���� L"\\setting.ini"
	WCHAR* buffer = new WCHAR[BUFFER_MAX_SIZE]{};
	DWORD r = GetPrivateProfileStringW(L"Directory", L"dir", NULL, buffer, BUFFER_MAX_SIZE, iniPath.c_str());

	if (r == 0)
	{
		// INI�� ���ٸ� ��� ���� �Ǵ� ����.
		std::cout
			<< "===========================================" << '\n'
			<< '\n'
			<< "              ini  �б� ����                " << '\n'
			<< '\n'
			<< "===========================================" << '\n';

		delete[] buffer;
		buffer = nullptr;
		delete[] buffer2;
		buffer2 = nullptr;
		return State::FAILED;
	}

	// �о�� �����͸� ������ �����ϰ� ������ Ż����.
	iniData = buffer;

	std::wcout
		<< L"===========================================" << '\n'
		<< L'\n'
		<< L"        ini�� ����� �����͸� �ҷ���" << '\n'
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
/// INI�� ��θ� �������� ���� ������ ���ϴ� �Լ�
/// </summary>
/// <returns></returns>
INI_Reader::State INI_Reader::Failed()
{

	std::wcout
		<< L"===========================================" << '\n'
		<< L"  ���� �� �ϳ��� �������ּ���." << '\n'
		<< L'\n'
		<< L"     1. ini ���� ����" << '\n'
		<< L"     2. ini ��� ����" << '\n'
		<< L"===========================================" << '\n'
		<< L'\n'
		<< L"�Է� : ";

	// ���ϴ� ����
	int buffer{};
	std::wcin.clear();
	std::wcin >> buffer;
	while (std::wcin.fail())
	{
		std::wcout
			<< L"�߸��� �Է��Դϴ�." << '\n'
			<< L"�ٽ� �Է����ּ���." << '\n'
			<< L"�Է� : ";
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
/// �� INI�� ����
/// </summary>
INI_Reader::State INI_Reader::Create()
{
	std::cout
		<< "===========================================" << '\n'
		<< '\n'
		<< "              ini  ���� ����                " << '\n'
		<< '\n'
		<< "===========================================" << '\n';

	// ini�� ������ ��ġ�� ���� ����
	WCHAR* buffer = new WCHAR[BUFFER_MAX_SIZE]{};

	if (!GetCurrentDirectoryW(BUFFER_MAX_SIZE, buffer))
	{
		// ���� ��� ���� ����
		throw(L"���� ��θ� ������ �� �����ϴ�. ��θ� Ȯ�� �ٶ��ϴ�.");
		delete[] buffer;
		return State::EXIT;
	}
	
	// ���� ��θ� �ɹ� ������ �����ϰ� �Ҵ��� �����Ѵ�.
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
		<< L"  �ش� ��ο� ������ ini ���Ͽ� ������ �����͸� �Է����ּ���." << '\n'
		<< L'\n'
		<< L"  �Է� : ";

	// ini�� ������ �����͸� �Է¹޴´�.
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

	// �Է��� ��θ� �ɹ� ������ �����ϰ� �о��.
	iniData = iniData + buffer1;

	/*
	iniData = buffer;
	delete[] buffer;
	buffer = nullptr;
	*/

	// INI�� ����
	if (WritePrivateProfileStringW(L"Directory", L"dir", iniData.c_str(), iniPath.c_str()))
	{
		std::wcout
			<< "===========================================" << '\n'
			<< '\n'
			<< L"   ini ������ �����Ǿ����ϴ�." << '\n'
			<< '\n'
			<< L"   ���� �б⸦ �ǽ��մϴ�." << '\n'
			<< '\n'
			<< "===========================================" << '\n';
		
		return State::READ;
	}

	return State::CREATE;
}

/// <summary>
/// INI ��� �����ϴ� �Լ�
/// </summary>
/// <returns></returns>
INI_Reader::State INI_Reader::SetPath()
{
	std::wcout
		<< L"===========================================" << '\n'
		<< '\n'
		<< L"               .ini ��� ����" << '\n'
		<< '\n'
		<< L"===========================================" << '\n';
	
	std::wcout
		<< L"ini ��θ� �Է����ּ���." << '\n';

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

	// �Է��� ��θ� �ɹ� ������ �����ϰ� �о��.
	iniPath = iniPath + buffer;

	std::wcout
		<< L"===========================================" << '\n'
		<< '\n'
		<< L"     �Է��� ��� : " << iniPath << '\n'
		<< '\n'
		<< L"     �ش� ��ο� �ִ� ini ���� �б⸦ �õ��մϴ�." << '\n'
		<< '\n'
		<< L"===========================================" << '\n';

	return State::READ;
}