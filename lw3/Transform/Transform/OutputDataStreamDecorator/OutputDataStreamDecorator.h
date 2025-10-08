#pragma once
#include "../OutputDataStream/IOutputDataStream.h"
#include <memory>

class OutputDataStreamDecorator : public IOutputDataStream
{
public:
	OutputDataStreamDecorator(std::unique_ptr<IOutputDataStream>&& stream);

protected:
	std::unique_ptr<IOutputDataStream> m_stream;
};
