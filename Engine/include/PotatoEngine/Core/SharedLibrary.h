#pragma once

#include <PotatoEngine/PotatoEngineExport.h>

namespace potato
{
	class SharedLibrary
	{
	public:

        POTATOENGINE_EXPORT static void* load(const char* path);
        POTATOENGINE_EXPORT static void unload(const char* path, void* sharedLibraryHandle);

		POTATOENGINE_EXPORT static void* getFunctionPtr(void* sharedLibraryHandle, const char* functionName);
	};
}