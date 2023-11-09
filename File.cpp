#include "File.h"
#include "Conversion.h"

#define UNSAFETY_NEW ::new

bool File::CreateDirectoryRecursively(wstring _directory)
{
	_directory = Conversion::RightTrimString(_directory, _T("/"));
	_directory = Conversion::RightTrimString(_directory, _T("\\"));

	static const wstring separators(L"\\/");

	DWORD fileAttributes = ::GetFileAttributesW(_directory.c_str());
	if (fileAttributes == INVALID_FILE_ATTRIBUTES) {
		size_t slashIndex = _directory.find_last_of(separators);
		if (slashIndex != wstring::npos) {
			CreateDirectoryRecursively(_directory.substr(0, slashIndex));
		}

		BOOL result = ::CreateDirectoryW(_directory.c_str(), nullptr);
		if (result == FALSE) {
			return false; // Could not create directory
		}
	}
	else { // Specified directory name already exists as a file or directory
		bool isDirectoryOrJunction =
			((fileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) ||
			((fileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0);

		if (!isDirectoryOrJunction) {
			return false; // Could not create directory because a file with the same name exists
		}
	}
	return true;
}

bool File::GetTextFileType(const wstring & _filename, TEXT_FILE_TYPE & _fileType)
{
	unsigned int length = 0;
	BYTE bufferHeader[4];
	memset(bufferHeader, 0, 4);
	ifstream in(_filename, ios::in | ios::binary);
	if (in.is_open() && in.good()) {
		in.seekg(0, ios::end);
		length = (unsigned int)in.tellg();
		if (length > 4)
			length = 4;

		if (length > 0) {
			in.seekg(0, ios::beg);
			in.read((char *)bufferHeader, length);
		}
		in.close();
	}

	_fileType = UNKNOWN;

	if (length == 0) return true;

	if ((unsigned char)bufferHeader[0] == 0xEF && (unsigned char)bufferHeader[1] == 0xBB && (unsigned char)bufferHeader[2] == 0xBF) {
		_fileType = UTF8;
		if (length == 3) return true;
	}
	if ((unsigned char)bufferHeader[0] == 0xFE && (unsigned char)bufferHeader[1] == 0xFF) {
		_fileType = UTF16_BE;
		if (length == 2) return true;
	}
	if ((unsigned char)bufferHeader[0] == 0xFF && (unsigned char)bufferHeader[1] == 0xFE) {
		_fileType = UTF16_LE;
		if (length == 2) return true;
	}

	return false;
}

//*******************
// Text file
//*******************
bool File::ReadTextFile(const wstring & _filename, wstringstream & _buffer)
{
	TEXT_FILE_TYPE fileType;
	bool empty = GetTextFileType(_filename, fileType);
	if (empty) return false;

	if (fileType == UTF16_LE || fileType == UTF16_BE) {
		streampos fileSize;
		streampos bufferSize;
		return ReadUTF16File(_filename, _buffer, fileSize, bufferSize);
	}

	if (fileType == UTF8 || fileType == UNKNOWN) {
		streampos fileSize;
		streampos bufferSize;
		if (ReadUTF8File(_filename, _buffer, fileSize, bufferSize))
			return true;

		if (fileSize != bufferSize)
			fileType = ANSI;
	}

	if (fileType == ANSI) {
		streampos fileSize;
		streampos bufferSize;
		if (ReadANSIFile(_filename, _buffer, fileSize, bufferSize))
			return true;
	}

	wstring mess = _T("Error - unknown file type\nFile: ") + _filename + _T("\nSupported file type: UTF8, UTF16, ANSI\n");
	_tprintf(mess.c_str());
	return false;
}

bool File::ReadTextFile(const wstring & _filename, wstring & _buffer)
{
	TEXT_FILE_TYPE fileType;
	bool empty = GetTextFileType(_filename, fileType);
	if (empty) return false;

	if (fileType == UTF16_LE || fileType == UTF16_BE) {
		streampos fileSize;
		streampos bufferSize;
		return ReadUTF16File(_filename, _buffer, fileSize, bufferSize);
	}

	if (fileType == UTF8 || fileType == UNKNOWN) {
		streampos fileSize;
		streampos bufferSize;
		if (ReadUTF8File(_filename, _buffer, fileSize, bufferSize))
			return true;

		if (fileSize != bufferSize)
			fileType = ANSI;
	}

	if (fileType == ANSI) {
		streampos fileSize;
		streampos bufferSize;
		if (ReadANSIFile(_filename, _buffer, fileSize, bufferSize))
			return true;
	}

	wstring mess=_T("Error - unknown file type\nFile: ") + _filename + _T("\nSupported file type: UTF8, UTF16, ANSI\n");
	_tprintf(mess.c_str());
	return false;
}

//*******************
// UTF16 text file
//*******************
bool File::ReadUTF16File(const wstring & _filename, wstringstream & _buffer, streampos & _fileSize, streampos & _bufferSize)
{
	wifstream stream;
	stream.open(_filename);
	if (stream.is_open() && stream.good()) {
		stream.imbue(locale(locale::empty(), UNSAFETY_NEW codecvt_utf16<wchar_t, 0x10ffff, consume_header>));

		_buffer << stream.rdbuf();

		_bufferSize = stream.tellg();
		stream.seekg(0, ios::end);
		_fileSize = stream.tellg();
		stream.close();

		if (_buffer.good() && _bufferSize == _fileSize)
			return true;
	}

	return false;
}

bool File::ReadUTF16File(const wstring & _filename, wstring & _buffer, streampos & _fileSize, streampos & _bufferSize)
{
	wifstream stream;
	stream.open(_filename);
	if (stream.is_open() && stream.good()) {
		stream.imbue(locale(locale::empty(), UNSAFETY_NEW codecvt_utf16<wchar_t, 0x10ffff, consume_header>));

		wstringstream file;
		file << stream.rdbuf();

		_bufferSize = stream.tellg();
		stream.seekg(0, ios::end);
		_fileSize = stream.tellg();
		stream.close();

		if (!file.good()) return false;
		if (_bufferSize != _fileSize) return false;

		_buffer = file.str();
		return true;
	}
	return false;
}

//*******************
// UTF8 text file
//*******************
bool File::ReadUTF8File(const wstring & _filename, wstringstream & _buffer, streampos & _fileSize, streampos & _bufferSize)
{
	wifstream stream;
	stream.open(_filename);
	if (stream.is_open() && stream.good()) {
		stream.imbue(locale(locale::empty(), UNSAFETY_NEW codecvt_utf8<wchar_t, 0x10ffff, consume_header>));

		_buffer << stream.rdbuf();

		_bufferSize = stream.tellg();
		stream.seekg(0, ios::end);
		_fileSize = stream.tellg();
		stream.close();

		if (_buffer.good() && _bufferSize == _fileSize)
			return true;
	}

	return false;
}

bool File::ReadUTF8File(const wstring & _filename, wstring & _buffer, streampos & _fileSize, streampos & _bufferSize)
{
	wifstream stream;
	stream.open(_filename);
	if (stream.is_open() && stream.good()) {
		stream.imbue(locale(locale::empty(), UNSAFETY_NEW codecvt_utf8<wchar_t, 0x10ffff, consume_header>));

		wstringstream file;
		file << stream.rdbuf();

		_bufferSize = stream.tellg();
		stream.seekg(0, ios::end);
		_fileSize = stream.tellg();
		stream.close();

		if (!file.good()) return false;
		if (_bufferSize != _fileSize) return false;

		_buffer = file.str();
		return true;
	}
	return false;
}

bool File::WriteUTF8File(const wstring & _filename, const wstring & _buffer)
{
	// create directory
	size_t size = _filename.length();
	wchar_t *dir = DBG_NEW wchar_t[size];
	wchar_t* drive = DBG_NEW wchar_t[size];
	_wsplitpath_s(_filename.c_str(), drive, size, dir, size, NULL, 0, NULL, 0);
	wstring folder(drive + wstring(dir));
	CreateDirectoryRecursively(folder);
	delete[]drive;
	delete[]dir;

	// open stream
	wofstream stream;
	stream.imbue(locale(locale::empty(), UNSAFETY_NEW codecvt_utf8<wchar_t, 0x10ffff, generate_header>));
	stream.open(_filename);

	// write file
	if (stream.is_open()) {
		stream << _buffer;
		stream.close();
		return true;
	}
	return false;
}

//*******************
// ANSI text file
//*******************
bool File::ReadANSIFile(const wstring & _filename, wstringstream & _buffer, streampos & _fileSize, streampos & _bufferSize)
{
	wifstream stream;
	stream.open(_filename);
	stream.imbue(locale("sk-SK"));

	if (stream.is_open() && stream.good()) {
		_buffer << stream.rdbuf();

		_bufferSize = stream.tellg();
		stream.seekg(0, ios::end);
		_fileSize = stream.tellg();
		stream.close();

		if (_buffer.good() && _bufferSize == _fileSize)
			return true;
	}
	return false;
}

bool File::ReadANSIFile(const wstring & _filename, wstring & _buffer, streampos & _fileSize, streampos & _bufferSize)
{
	wifstream stream;
	stream.open(_filename);
	stream.imbue(locale("sk-SK"));

	if (stream.is_open() && stream.good()) {
		wstringstream file;
		file << stream.rdbuf();

		_bufferSize = stream.tellg();
		stream.seekg(0, ios::end);
		_fileSize = stream.tellg();
		stream.close();

		if (!file.good()) return false;
		if (_bufferSize != _fileSize) return false;

		_buffer = file.str();
		return true;
	}
	return false;
}

bool File::WriteANSIFile(const wstring& _filename, const wstring& _buffer)
{
	//create directory
	size_t size = _filename.length();
	wchar_t* dir = DBG_NEW wchar_t[size];
	wchar_t* drive = DBG_NEW wchar_t[size];
	_wsplitpath_s(_filename.c_str(), drive, size, dir, size, NULL, 0, NULL, 0);
	wstring folder(drive + wstring(dir));
	CreateDirectoryRecursively(folder);
	delete[]drive;
	delete[]dir;

	//write file
	wofstream stream;
	stream.open(_filename);
	stream.imbue(locale("sk-SK"));

	if (stream.is_open()) {
		stream << _buffer;
		stream.close();
		return true;
	}
	return false;
}

//*******************
// binary file
//*******************
bool File::ReadFile(const wstring & _filename, BYTE ** _buffer, unsigned int & _length)
{
	ifstream in(_filename, ios::in | ios::binary);
	if (in.is_open() && in.good()) {
		in.seekg(0, ios::end);
		_length = (unsigned int)in.tellg();
		if (_length >0) {
			*_buffer = DBG_NEW BYTE[_length];
			if (*_buffer) {
				in.seekg(0, ios::beg);
				in.read((char *)*_buffer, _length);
				in.close();
				return true;
			}
		}
	}
	_length = 0;
	*_buffer = NULL;
	return false;
}

bool File::WriteFile(const wstring & _filename, const BYTE * _buffer, unsigned int _length)
{
	// create directory
	size_t size = _filename.length();
	wchar_t *dir = DBG_NEW wchar_t[size];
	wchar_t* drive = DBG_NEW wchar_t[size];
	_wsplitpath_s(_filename.c_str(), drive, size, dir, size, NULL, 0, NULL, 0);
	wstring folder(drive + wstring(dir));
	CreateDirectoryRecursively(folder);
	delete[]drive;
	delete[]dir;

	// write file
	ofstream out(_filename, ios::out | ios::binary | ios::trunc);
	if (out) {
		out.write((char *)_buffer, _length);
		out.close();
		return true;
	}
	return false;
}
