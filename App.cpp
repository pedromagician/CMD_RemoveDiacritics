#include "stdafx.h"

Debug theDebug;

#include "File.h"
#include "Conversion.h"
#include "Locale.h"
#include "CommandLine.h"

int _tmain(int _argc, _TCHAR* _argv[])
{
	if (Locale::EnableLocale() == false)
		_tprintf(_T("Error - Locale initialization failed\n"));

	int correctParameters = 0;
	bool help = false;
	wstring inutFileName = _T("");
	wstring outputFileName = _T("");
	bool utf8 = false;

	CommandLine cmd;
	cmd.Add(CommandLine::_TRUE,		3, _T("-help"),		_T("-h"),	_T("-?"),	_T("To view help."),																												&help);
	cmd.Add(CommandLine::_STRING,	2, _T("-input"),	_T("-i"),				_T("The 'xxx' argument specifies the name of the source file."),																	&inutFileName);
	cmd.Add(CommandLine::_STRING,	2, _T("-output"),	_T("-o"),				_T("The 'xxx' argument specifies the name of the destination file. The file name is generated in the form 'inputFileName.out'."),	&outputFileName);
	cmd.Add(CommandLine::_TRUE,		1, _T("-utf8"),								_T("The resulting file will be saved in utf8 format."),																				&utf8);

	if (cmd.ParseCommandLine(_argc, _argv, correctParameters) != 0) {
		cmd.Help();
		return 0;
	}

	if (help || correctParameters == 0) {
		cmd.Help();
		return 0;
	}

	if (inutFileName.empty()) {
		_tprintf(_T("Error - unknown input file\n"));
		return 1;
	}

	if (outputFileName.empty())
		outputFileName = inutFileName + _T(".out");

	_tprintf((_T("Read: ") + inutFileName + _T("\n")).c_str());
	wstring inputBuffer;
	if (!File::ReadTextFile(inutFileName, inputBuffer)) {
		_tprintf(_T("Error - input file\n"));
		return 1;
	}

	wstring outputBuffer = Conversion::RemoveDiacritics(inputBuffer);

	_tprintf((_T("Create: ") + outputFileName + _T("\n")).c_str());
	if (utf8) {
		if (!File::WriteUTF8File(outputFileName, outputBuffer)) {
			_tprintf(_T("Error - output file\n"));
			return 1;
		}
	}
	else {
		if (!File::WriteANSIFile(outputFileName, outputBuffer)) {
			_tprintf(_T("Error - output file\n"));
			return 1;
		}
	}

	return 0;
}
