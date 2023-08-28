#pragma once

#include <PotatoEngine/PotatoEngineExport.h>
#include "Util/Assert.h"
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>

namespace potato
{
	class SharedLibrary
	{
		public:

			POTATOENGINE_EXPORT static void* load(const char* path);
			POTATOENGINE_EXPORT static void unload(const char* path, void* sharedLibraryHandle);

			template <typename T>
			POTATOENGINE_EXPORT static T getFunctionPtr(void* sharedLibraryHandle, const char* functionName);
	};

	template <typename T>
	T SharedLibrary::getFunctionPtr(void* sharedLibraryHandle, const char* functionName)
	{
		T functionAddress = (T)::GetProcAddress(static_cast<HMODULE>(sharedLibraryHandle), functionName);
		POTATO_ASSERT_MSG(functionAddress != nullptr, "Could not find function '%s' in DLL", functionName);
		return functionAddress;
	}
}