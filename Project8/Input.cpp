#include "Input.h"

#include <windows.h>

void Input::Update()
{
	for (int i = 0; i < INPUT_INDEX_MAX; i++)
	{
		data[i] = GetAsyncKeyState(i);
	}
}

bool Input::GetPressedKey(char key)
{
	return data[key] & 0x8001;
}

bool Input::GetPressedNowKey(char key)
{
	return data[key] & 0x8000;
}

bool Input::GetReleaseKey(char key)
{
	return data[key] & 0x0000;
}

bool Input::GetReleaseNowKey(char key)
{
	return data[key] & 0x0001;
}
