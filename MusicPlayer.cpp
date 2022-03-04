#include "MusicPlayer.h"
#include<Windows.h>
#include<mmsystem.h>
#pragma comment(lib, "Winmm.lib")
LPCWSTR string2LPCWSTR(std::string orig)
{
	size_t origsize = orig.length() + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t* wcstring = (wchar_t*)malloc(sizeof(wchar_t) * (orig.length() - 1));
	mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);
	return wcstring;
}

bool MusicPlayer::play() {
	string opencode = "OPEN ";
	opencode += bgm;
	if (loop) {
		opencode += " type MPEGVideo";
		opencode += " ALIAS MUSIC1";
		if (mciSendString(string2LPCWSTR(opencode), NULL, 0, 0) != 0)
			return false;
		mciSendString(TEXT("PLAY MUSIC1 REPEAT"), NULL, 0, NULL);
	}
	else {
		string closecode = "CLOSE ";
		closecode += alias;
		mciSendString(string2LPCWSTR(closecode), NULL, 0, NULL);
		opencode += " ALIAS ";
		opencode += alias;
		if (mciSendString(string2LPCWSTR(opencode), NULL, 0, 0) != 0)
			return false;
		opencode = "PLAY ";
		opencode += alias;
		mciSendString(string2LPCWSTR(opencode), NULL, 0, NULL);
	}
	return true;
}

void MusicPlayer::stop() {
	mciSendString(TEXT("STOP MUSIC1"), NULL, 0, NULL);
}

void MusicPlayer::close() {
	mciSendString(TEXT("CLOSE MUSIC1"), NULL, 0, NULL);
}