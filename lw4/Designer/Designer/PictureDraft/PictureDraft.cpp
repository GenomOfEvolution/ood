#include "PictureDraft.h"

size_t PictureDraft::GetShapeCount() const
{
    return m_storage.size();
}

void PictureDraft::AddShape(std::unique_ptr<Shape>&& shape)
{
    m_storage.push_back(std::move(shape));
}
