#pragma once
#include "../IShape.h"
#include "IGroup.h"
#include <vector>

class ShapeGroup : public IGroup, public IShape
{
public:
	ShapeGroup() = default;

	bool ContainsPoint(const QPointF& point) const override;
	QRectF GetBoundingBox() const override;
	void MoveBy(const QPointF& delta) override;
	void ApplyTransform(const QTransform& transform) override;

	size_t GetShapeCount() const override;

	std::shared_ptr<IShape> GetShapeAtIndex(size_t index) override;
	std::shared_ptr<const IShape> GetShapeAtIndex(size_t index) const override;

	void InsertShape(std::shared_ptr<IShape> shape, size_t index) override;
	void RemoveShapeAtIndex(size_t index) override;
	void ClearGroup() override;

	bool ContainsShape(const IShape* shape) const;
	size_t FindShapeIndex(const IShape* shape) const;

private:
	std::vector<std::shared_ptr<IShape>> m_shapes;
};