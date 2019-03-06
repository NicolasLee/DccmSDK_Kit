#pragma once

//Ansi转Unicode
int AnsiToUnicode(char *str, wchar_t* wStr, int size);
wchar_t* AnsiToUnicode(char *str);

//Unicode转Ansi
int UnicodeToAnsi(wchar_t *wStr, char* str, int size);
char* UnicodeToAnsi(wchar_t *wStr);
