#pragma once

class IImageStorage
{
public:
	virtual ~IImageStorage() = default;

	virtual void SaveImage();
};