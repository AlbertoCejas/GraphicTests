#pragma once

#include <cstdint>
#include <memory>
#include <PotatoEngine/PotatoEngineExport.h>

namespace potato
{
	class File;

	enum POTATOENGINE_EXPORT class FileAccessMode : uint8_t { Read, Write, Append, ReadUpdate, WriteUpdate, AppendUpdate };

	class POTATOENGINE_EXPORT FileSystem
	{
		public:

			static std::unique_ptr<File> openFile(const char* path, FileAccessMode fileAccessMode);
			static bool existsFile(const char* path);

		private:

			static const char* getFileAccessCStr(FileAccessMode fileAccessMode);
	};
}