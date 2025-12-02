#pragma once
#include <string>

class IImage
{
public:
	virtual std::string GetPath() const = 0;
	virtual void SetPath(const std::string& path) = 0;

	virtual ~IImage() = default;
};