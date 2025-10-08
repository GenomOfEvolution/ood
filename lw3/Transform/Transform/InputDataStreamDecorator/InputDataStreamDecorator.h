#pragma once
#include "../InputDataStream/IInputDataStream.h"
#include <memory>

class InputDataStreamDecorator : public IInputDataStream
{
public:
	bool IsEOF() const override;
	void Close() override;

protected:
	InputDataStreamDecorator(std::unique_ptr<IInputDataStream>&& stream);

	std::unique_ptr<IInputDataStream> m_stream;
};