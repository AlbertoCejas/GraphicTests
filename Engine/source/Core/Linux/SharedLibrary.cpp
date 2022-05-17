#include <dlfcn.h>

#include <PotatoEngine/Core/SharedLibrary.h>

#include <filesystem>
#include <PotatoEngine/Util/Assert.h>
#include <string>

namespace potato
{
    void* SharedLibrary::load(const char* path)
    {
    	static constexpr const char* const LIB_PREFIX = "lib";
        static constexpr const char* const SO_EXTENSION = ".so";

        std::filesystem::path filesystemPath(path);
        if (filesystemPath.has_extension() == false)
        {
            filesystemPath.replace_extension(SO_EXTENSION);
        }
        filesystemPath.replace_filename(LIB_PREFIX + filesystemPath.filename().string());
        filesystemPath = filesystemPath.lexically_normal();

        void* moduleHandle = ::dlopen(filesystemPath.c_str(), RTLD_NOW);
        POTATO_ASSERT_MSG(moduleHandle != nullptr, ".so lib '%s' couldn't be loaded", filesystemPath.c_str());
        return moduleHandle;
    }

    void SharedLibrary::unload(const char* path, void* sharedLibraryHandle)
    {
        int result = ::dlclose(sharedLibraryHandle);
        POTATO_ASSERT_MSG(result == 0, ".so lib '%s' couldn't be unloaded", path);
    }

    void* SharedLibrary::getFunctionPtr(void* sharedLibraryHandle, const char* functionName)
    {
        void* functionAddress = ::dlsym(sharedLibraryHandle, functionName);
        POTATO_ASSERT_MSG(functionAddress != nullptr, "Could not find function '%s' in .so lib", functionName);
        return functionAddress;
    }
}