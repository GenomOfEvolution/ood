#include "FileInputStream.h"

FileInputStream::FileInputStream(const std::string& fileName)
{
	m_file.open(fileName, std::ios::binary | std::ios::in);
	m_file.unsetf(std::ios::skipws);

	if (!m_file.is_open())
	{
		throw std::runtime_error("Can`t open file " + fileName);
	}
}

bool FileInputStream::IsEOF() const
{
	if (!m_file.is_open())
	{
		throw std::logic_error("Stream is closed!");
	}

	m_file.peek();
	return m_file.eof();
}

uint8_t FileInputStream::ReadByte()
{
	if (!m_file.is_open())
	{
		throw std::logic_error("Stream is closed!");
	}

	if (IsEOF())
	{
		throw std::ios_base::failure("Error reading from file!");
	}

	uint8_t byte = m_file.get();

	if (m_file.bad())
	{
		throw std::ios_base::failure("Error reading from file!");
	}

	return byte;
}

std::streamsize FileInputStream::ReadBlock(void* dstBuffer, std::streamsize size)
{
	if (!m_file.is_open())
	{
		throw std::logic_error("Stream is closed!");
	}

	m_file.read(static_cast<char*>(dstBuffer), size);

	if (m_file.bad())
	{
		throw std::ios_base::failure("Error reading from file!");
	}

	return m_file.gcount();
}

void FileInputStream::Close()
{
	if (!m_file.is_open())
	{
		throw std::logic_error("File is already closed!");
	}

	m_file.close();
}
