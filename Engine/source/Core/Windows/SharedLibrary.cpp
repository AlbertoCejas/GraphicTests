#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>
#include <filesystem>

#include "Core/SharedLibrary.h"
#include "Util/Assert.h"

namespace potato
{
    void* SharedLibrary::load(const char* path)
    {
        static constexpr const char* const DLL_EXTENSION = ".dll";

        std::filesystem::path filesystemPath(path);
        if (filesystemPath.has_extension() == false)
        {
            filesystemPath.replace_extension(DLL_EXTENSION);
        }
        filesystemPath = filesystemPath.lexically_normal();

        void* moduleHandle = ::LoadLibraryW(filesystemPath.c_str());
        POTATO_ASSERT_MSG(moduleHandle != nullptr, "DLL '%s' couldn't be loaded", filesystemPath.c_str());
        return moduleHandle;
    }

    void SharedLibrary::unload(const char* path, void* sharedLibraryHandle)
    {
        bool result = ::FreeLibrary(static_cast<HMODULE>(sharedLibraryHandle));
        POTATO_ASSERT_MSG(result == true, "DLL '%s' couldn't be unloaded", path);
    }

    void* SharedLibrary::getFunctionPtr(void* sharedLibraryHandle, const char* functionName)
    {
        FARPROC functionAddress = ::GetProcAddress(static_cast<HMODULE>(sharedLibraryHandle), functionName);
        POTATO_ASSERT_MSG(functionAddress != nullptr, "Could not find function '%s' in DLL", functionName);
        return functionAddress;
    }
}