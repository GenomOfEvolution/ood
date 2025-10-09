#pragma once
#include <string>
#include <vector>
#include <map>
#include <functional>

struct Args
{
	std::string inputFileName;
	std::string outputFileName;

	std::vector<int> cryptKeys;
	std::vector<int> decryptKeys;
	
	bool compress = false;
	bool decompress = false;
};

class ArgsParser
{
public:
	ArgsParser(int argc, char* argv[]);
	Args GetArgs() const;

private:
	using Handler = std::function<void(const std::string&, Args&)>;
	using ActionMap = std::map<std::string, Handler>;
	const ActionMap m_actionMap;

	void ApplyKey(const std::string& param, std::vector<int>& keys, const std::string& optionName);
	void ApplyCompress(const std::string& param, Args& args);
	void ApplyDecompress(const std::string& param, Args& args);

	Args m_args;
	std::vector<std::string> m_argv;
};
