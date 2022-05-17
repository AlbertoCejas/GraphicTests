#include <filesystem>

#include "File/FileSystem.h"
#include "File/File.h"

namespace potato
{
	std::unique_ptr<File> FileSystem::openFile(const char* path, FileAccessMode fileAccessMode)
	{
		if (existsFile(path))
		{
			return std::unique_ptr<File>(new File(path, FileSystem::getFileAccessCStr(fileAccessMode)));
		}
		else
		{
			return nullptr;
		}
	}

	bool FileSystem::existsFile(const char* path)
	{
		std::filesystem::path filePath(path);
		return std::filesystem::exists(filePath);
	}

	const char* FileSystem::getFileAccessCStr(FileAccessMode fileAccessMode)
	{
		switch (fileAccessMode)
		{
			case FileAccessMode::Read: return "rb";
			case FileAccessMode::Write: return "wb";
			case FileAccessMode::Append: return "ab";
			case FileAccessMode::ReadUpdate: return "r+b";
			case FileAccessMode::WriteUpdate: return "w+b";
			case FileAccessMode::AppendUpdate: return "a+b";
			default: return "";
		}
	}
}