#pragma once

constexpr unsigned int INPUT_INDEX_MAX = 0xFF;

class Input
{
public:
	void Update();

	bool GetPressedKey(char key);
	bool GetPressedNowKey(char key);
	bool GetReleaseKey(char key);
	bool GetReleaseNowKey(char key);

private:
	char data[INPUT_INDEX_MAX]{};
};

