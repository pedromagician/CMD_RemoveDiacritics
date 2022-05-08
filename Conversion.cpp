#include "Conversion.h"

#define WHITE_SPACE_CHARACTERS _T(" \t\n\v\f\r ")
#define CODE_PAGE_US_ASCII_7_BIT 20127

int Conversion::ToInt(const wstring& _buff, int _base)
{
	return _tcstol(_buff.c_str(), nullptr, _base);
}

wstring Conversion::LeftTrimString(wstring _str, const wstring& _val)
{
	wstring::size_type pos = _str.find_first_not_of(_val);
	if (pos != wstring::npos)
		_str.erase(0, pos);
	else
		_str.erase(_str.begin(), _str.end());

	return _str;
}

wstring Conversion::RightTrimString(wstring _str, const wstring& _val)
{
	wstring::size_type pos = _str.find_last_not_of(_val);
	if (pos != wstring::npos)
		_str.erase(pos + 1);
	else
		_str.erase(_str.begin(), _str.end());

	return _str;
}

wstring Conversion::TrimString(wstring _str, const wstring& _val)
{
	wstring::size_type pos = _str.find_last_not_of(_val);
	if (pos != wstring::npos)
	{
		_str.erase(pos + 1);
		pos = _str.find_first_not_of(_val);
		if (pos != wstring::npos)
			_str.erase(0, pos);
	}
	else
		_str.erase(_str.begin(), _str.end());

	return _str;
}

wstring Conversion::TrimWhiteChar(const wstring& _val)
{
	return TrimString(_val, WHITE_SPACE_CHARACTERS);
}

wstring Conversion::ToLower(wstring _val)
{
	transform(_val.begin(), _val.end(), _val.begin(), [](wchar_t c) { return towlower(c); });
	return _val;
}

void Conversion::StringReplaceAll(wstring& _mess, const wstring& _oldStr, const wstring& _newStr)
{
	const size_t oldLen = _oldStr.length();
	const size_t newLen = _newStr.length();
	size_t position = 0;
	while ((position = _mess.find(_oldStr, position)) != string::npos)
	{
		_mess.replace(position, oldLen, _newStr);
		position += newLen;
	}
}

wstring Conversion::ConvertString2WString(const string& _inputString)
{
	int len = MultiByteToWideChar(CP_ACP, 0, _inputString.c_str(), (int)_inputString.length(), nullptr, 0);

	wstring wstrTo(len, 0);
	MultiByteToWideChar(CP_ACP, 0, _inputString.c_str(), (int)_inputString.length(), &wstrTo[0], len);
	return wstrTo;
}

string Conversion::ConvertWString2String(const wstring& _inputWString, UINT _codePage /*= CP_ACP*/)
{
	int len = WideCharToMultiByte(_codePage, 0, _inputWString.c_str(), (int)_inputWString.length(), nullptr, 0, 0, 0);

	string strTo(len, 0);
	WideCharToMultiByte(_codePage, 0, _inputWString.c_str(), (int)_inputWString.length(), &strTo[0], len, 0, 0);
	return strTo;
}

wstring Conversion::RemoveDiacritics(const wstring& _text)
{
	string newText = ConvertWString2String(_text, CODE_PAGE_US_ASCII_7_BIT);//US-ASCII (7-bit) = 20127
	return ConvertString2WString(newText);
}
