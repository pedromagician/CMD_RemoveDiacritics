#include "Debug.h"

#if defined(_DEBUG)
void Debug::MemoryLeakDetection()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
}
#endif
