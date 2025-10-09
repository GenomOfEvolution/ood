#include <iostream>
#include "ArgsParser/ArgsParser.h"
#include "Transformer/Transformer.h"

int main(int argc, char* argv[])
{
	try
	{
		ArgsParser parser(argc, argv);
		auto args = parser.GetArgs();

		Transformer trasnformer(args);
		trasnformer.Transform();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
