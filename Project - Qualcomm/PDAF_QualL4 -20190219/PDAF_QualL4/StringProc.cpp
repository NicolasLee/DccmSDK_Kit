#include "stdafx.h"
#include "StringProc.h"

wchar_t* AnsiToUnicode(char *str)
{
	int cnt = MultiByteToWideChar (CP_ACP, 0, str, -1, NULL, 0);
	wchar_t *wStr;
	wStr = new wchar_t[cnt];

	MultiByteToWideChar (CP_ACP, 0, str, -1, wStr, cnt);
	return wStr;
}

int AnsiToUnicode(char *str, wchar_t* wStr, int size)
{
	int cnt = MultiByteToWideChar (CP_ACP, 0, str, -1, NULL, 0);

	if (cnt > size) return -1;

	MultiByteToWideChar (CP_ACP, 0, str, -1, wStr, size);

	return 0;
}

//Unicode转ansi
int UnicodeToAnsi(wchar_t *wStr, char* str, int size)
{
	int cnt = WideCharToMultiByte(CP_OEMCP, NULL, wStr, -1, NULL, 0, NULL, FALSE);

	if (cnt > size) return -1;

	WideCharToMultiByte (CP_OEMCP, NULL, wStr, -1, str, size, NULL, FALSE);
	return 0;
}

//Unicode转ansi
char* UnicodeToAnsi(wchar_t *wStr)
{
	int Cnt = WideCharToMultiByte(CP_OEMCP, NULL, wStr, -1, NULL, 0, NULL, FALSE);
	char *str;
	str = new char[Cnt];

	WideCharToMultiByte (CP_OEMCP, NULL, wStr, -1, str, Cnt, NULL, FALSE);
	return str;
}
