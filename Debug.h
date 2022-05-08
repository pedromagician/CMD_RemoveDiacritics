#pragma once

#if defined(_WIN32) && defined(_DEBUG) && defined(_MSC_VER)
#define UNSAFETY_NEW ::new

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)
#else
#define UNSAFETY_NEW new
#endif

#if defined(_DEBUG)
class Debug
{
public:
	static void MemoryLeakDetection();
};
#endif
