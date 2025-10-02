#pragma once
#include <fstream>
#include "../IInputDataStream.h"

class FileInputStream : public IInputDataStream
{
public:
	FileInputStream(const std::string& fileName);

	bool IsEOF() const override;
	uint8_t ReadByte() override;
	std::streamsize ReadBlock(void* dstBuffer, std::streamsize size) override;
	void Close() override;

private:
	mutable std::ifstream m_file;
};