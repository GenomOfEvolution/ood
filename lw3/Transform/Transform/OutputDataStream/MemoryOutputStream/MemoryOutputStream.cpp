#include "MemoryOutputStream.h"

MemoryOutputStream::MemoryOutputStream(std::vector<uint8_t>& data)
	: m_data(data)
	, m_closed(false)
{
}

void MemoryOutputStream::WriteByte(uint8_t data)
{
	if (m_closed)
	{
		throw std::logic_error("Can't write to closed stream!");
	}

	try
	{
		m_data.push_back(data);
	}
	catch(const std::exception& e)
	{
		throw std::ios_base::failure(e.what());
	}
}

void MemoryOutputStream::WriteBlock(const void* srcData, std::streamsize size)
{
	if (m_closed)
	{
		throw std::logic_error("Can't write to closed stream!");
	}

	try
	{
		const auto* bytes = static_cast<const uint8_t*>(srcData);
		m_data.insert(m_data.end(), bytes, bytes + size);
	}
	catch (const std::exception& e)
	{
		throw std::ios_base::failure(e.what());
	}
}

void MemoryOutputStream::Close()
{
	if (m_closed)
	{
		throw std::logic_error("Stream already closed!");
	}

	m_closed = true;
}
