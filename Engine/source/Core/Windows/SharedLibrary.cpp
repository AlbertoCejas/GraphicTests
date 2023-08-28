#include <filesystem>

#include "Core/SharedLibrary.h"

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
}