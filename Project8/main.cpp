#include <iostream>
#include <windows.h>
#include "JSON_Reader.h"
#include "INI_Reader.h"

/*
------------------------
C++17 이상으로 쓰여짐
------------------------
*/

void CursorView()
{
	CONSOLE_CURSOR_INFO cursorInfo{ 0, };
	// screen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	// screen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

int main()
{
	CursorView();

	INI_Reader iniReader;
	iniReader.Run();

	JSON_Reader jsonReader;
	jsonReader.SetDirectory(iniReader.GetINIData());
	jsonReader.Run();
	
	return 0;
}