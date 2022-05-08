#pragma once

#include "stdafx.h"

class File
{
public:
	enum TEXT_FILE_TYPE {
		UNKNOWN = 0,
		ANSI,
		UTF8,
		UTF16_BE,
		UTF16_LE,
	};
	static bool GetTextFileType(const wstring& _filename, TEXT_FILE_TYPE& _fileType);
	static bool CreateDirectoryRecursively(wstring _directory);

	//text file
	static bool ReadTextFile(const wstring& _filename, wstringstream& _buffer);
	static bool ReadTextFile(const wstring& _filename, wstring& _buffer);

	//UTF16 text file
	static bool ReadUTF16File(const wstring& _filename, wstringstream& _buffer, streampos& _fileSize, streampos& _bufferSize);
	static bool ReadUTF16File(const wstring& _filename, wstring& _buffer, streampos& _fileSize, streampos& _bufferSize);

	//UTF8 text file
	static bool ReadUTF8File(const wstring& _filename, wstringstream& _buffer, streampos& _fileSize, streampos& _bufferSize);
	static bool ReadUTF8File(const wstring& _filename, wstring& _buffer, streampos& _fileSize, streampos& _bufferSize);
	static bool WriteUTF8File(const wstring& _filename, const wstring& _buffer);

	//ANSI text file
	static bool ReadANSIFile(const wstring& _filename, wstringstream& _buffer, streampos& _fileSize, streampos& _bufferSize);
	static bool ReadANSIFile(const wstring& _filename, wstring& _buffer, streampos& _fileSize, streampos& _bufferSize);
	static bool WriteANSIFile(const wstring& _filename, const wstring& _buffer);

	//binary file
	static bool ReadFile(const wstring& _filename, BYTE** _buffer, unsigned int& _length);
	static bool WriteFile(const wstring& _filename, const BYTE* _buffer, unsigned int _length);

private:
	File(void) {};
	~File(void) {};
};
