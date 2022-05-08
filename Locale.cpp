#include "Locale.h"

bool Locale::EnableLocale()
{
	char* local = setlocale(LC_ALL, "en-US");//english
	if (local) {
		if (strcmp(local, "en-US") != 0) return false;
	}
	wchar_t* wlocal = _wsetlocale(LC_ALL, _T("en-US"));
	if (wlocal) {
		if (wcscmp(wlocal, _T("en-US")) != 0) return false;
	}

	local = setlocale(LC_CTYPE, "sk-SK");//case sensitivity vs diacritics
	if (local) {
		if (strcmp(local, "sk-SK") != 0) return false;
	}
	wlocal = _wsetlocale(LC_CTYPE, _T("sk-SK"));
	if (wlocal) {
		if (wcscmp(wlocal, _T("sk-SK")) != 0) return false;
	}

	local = setlocale(LC_COLLATE, "sk-SK");//sorting (c,h,ch)
	if (local) {
		if (strcmp(local, "sk-SK") != 0) return false;
	}
	wlocal = _wsetlocale(LC_COLLATE, _T("sk-SK"));
	if (wlocal) {
		if (wcscmp(wlocal, _T("sk-SK")) != 0) return false;
	}

	local = setlocale(LC_NUMERIC, "en-US");//separator . (decimal point)
	if (local) {
		if (strcmp(local, "en-US") != 0) return false;
	}
	wlocal = _wsetlocale(LC_NUMERIC, _T("en-US"));
	if (wlocal) {
		if (wcscmp(wlocal, _T("en-US")) != 0) return false;
	}

	return true;
}
