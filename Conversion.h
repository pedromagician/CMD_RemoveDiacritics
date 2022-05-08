#pragma once

#include "stdafx.h"

class Conversion {
public:
	static int ToInt(const wstring& _buff, int _base = 10);

	static wstring LeftTrimString(wstring _str, const wstring& _val);
	static wstring RightTrimString(wstring _str, const wstring& _val);
	static wstring TrimString(wstring _str, const wstring& _val);
	static wstring TrimWhiteChar(const wstring& _val);

	static wstring ToLower(wstring _val);

	static void StringReplaceAll(wstring& _mess, const wstring& _oldStr, const wstring& _newStr);

	static wstring ConvertString2WString(const string& _inputString);
	static string ConvertWString2String(const wstring& _inputWString, UINT _codePage = CP_ACP);
	static wstring RemoveDiacritics(const wstring& _text);
};
