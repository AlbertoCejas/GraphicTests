#include <filesystem>

#include "File/File.h"
#include "Util/Assert.h"

namespace potato
{
	File::File(const char* path, const char* fileAccessMode) :
		m_path(path),
		m_fileAccessMode(fileAccessMode)
	{
		m_file = fopen(m_path.c_str(), fileAccessMode);
		POTATO_ASSERT_MSG(m_file != nullptr, ("File '%s' could not be opened.", m_path.c_str()));
	}

	File::~File()
	{
		if (m_file)
		{
			fclose(m_file);
		}
	}

	size_t File::getSize() const
	{
		auto origin = ftell(m_file);
		fseek(m_file, 0, SEEK_END);
		auto end = ftell(m_file);

		fseek(m_file, origin, SEEK_SET);
		return end;
	}


	bool File::read(void* data, size_t size)
	{
		const auto bytesRead = fread(data, 1, size, m_file);
		return bytesRead == size;
	}

	bool File::write(const void* data, size_t size)
	{
		const auto bytesWritten = fwrite(data, 1, size, m_file);
		return bytesWritten == size;
	}
}