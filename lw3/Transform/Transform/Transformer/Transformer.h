#pragma once
#include "../ArgsParser/ArgsParser.h"
#include "../InputDataStream/IInputDataStream.h"
#include "../OutputDataStream/IOutputDataStream.h"

class Transformer
{
public:
	Transformer(const Args& args);
	void Transform();

private:
	void ApplyDecorators(const Args& args);

	std::unique_ptr<IInputDataStream> m_input;
	std::unique_ptr<IOutputDataStream> m_output;
};
