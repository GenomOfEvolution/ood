#pragma once
#include <memory>
#include <vector>
#include "../Shapes/Shape.h"


class PictureDraft
{
public:
	using Storage = std::vector<std::unique_ptr<Shape>>;

	size_t GetShapeCount() const;
	void AddShape(std::unique_ptr<Shape>&& shape);

	Storage::iterator begin() noexcept { return m_storage.begin(); }
	Storage::iterator end() noexcept { return m_storage.end(); }

	Storage::const_iterator begin() const noexcept { return m_storage.begin(); }
	Storage::const_iterator end() const noexcept { return m_storage.end(); }
	Storage::const_iterator cbegin() const noexcept { return m_storage.cbegin(); }
	Storage::const_iterator cend() const noexcept { return m_storage.cend(); }

private:
	Storage m_storage;
};
