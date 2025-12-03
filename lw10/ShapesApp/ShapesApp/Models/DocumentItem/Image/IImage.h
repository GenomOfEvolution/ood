#pragma once
#include "../Shape/IShape.h"
#include <string>

class IImage : public IShape
{
public:
	virtual std::string GetPath() const = 0;
	virtual void SetPath(const std::string& path) = 0;

	virtual ~IImage() = default;
};