#pragma once

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN

#include <windows.h>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <codecvt>
#include "atlstr.h"

#include "Debug.h"
#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

using namespace std;
