#include "FileOutputStream.h"

FileOutputStream::FileOutputStream(const std::string& fileName)
	: m_file(fileName, std::ios::binary | std::ios::out)
{
	if (!m_file.is_open())
	{
		throw std::runtime_error("Can't open file for writing!");
	}
}

void FileOutputStream::WriteByte(uint8_t data)
{
	if (!m_file.is_open())
	{
		throw std::logic_error("Stream is closed!");
	}

	m_file.put(static_cast<char>(data));

	if (m_file.bad())
	{
		throw std::ios_base::failure("Can't write to file!");
	}

	m_file.flush();
}

void FileOutputStream::WriteBlock(const void* srcData, std::streamsize size)
{
	if (!m_file.is_open())
	{
		throw std::logic_error("Stream is closed!");
	}

	m_file.write(static_cast<const char*>(srcData), size);

	if (m_file.bad())
	{
		throw std::ios_base::failure("Failed to write block to file");
	}
	m_file.flush();
}

void FileOutputStream::Close()
{
	if (!m_file.is_open())
	{
		throw std::logic_error("Stream is already closed!");
	}

	m_file.close();
}


