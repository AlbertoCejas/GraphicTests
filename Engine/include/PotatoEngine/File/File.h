#pragma once

#include <string>

#include <PotatoEngine/PotatoEngineExport.h>

namespace potato
{
	class POTATOENGINE_EXPORT File
	{
	public:

		~File();

		const char* getPath() const { return m_path.c_str(); }
		size_t getSize() const;
		FILE* getCFile() const { return m_file; }

		bool read(void* data, size_t size);
		bool write(const void* data, size_t size);

	private:

		friend class FileSystem;

		File(const char* path, const char* fileAccessMode);

		FILE* m_file;

		const std::string m_path;
		const std::string m_fileAccessMode;
	};
}