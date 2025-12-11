#pragma once
#include "../Shared/Geometry.h"
#include "../Document/IDocument.h"
#include <vector>

class ISelection
{
public:
	virtual ~ISelection() = default;

	virtual void SelectItem(const Point& point, bool additive = false) = 0;
	virtual void AddIndex(size_t index) = 0;
	virtual void ClearSelection() = 0;
	virtual void SetDocument(std::shared_ptr<IDocument> doc) = 0;
	virtual std::vector<size_t> GetSelectedIndexes() const = 0;
};