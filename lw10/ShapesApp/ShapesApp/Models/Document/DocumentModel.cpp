#include "DocumentModel.h"
#include "../DocumentSerializer/XmlSerializer.h"
#include <iostream>

DocumentModel::DocumentModel(
	std::shared_ptr<IHistory>&& history,
	std::shared_ptr<ISerializer>&& saver,
	const Rect& fieldArea)
	: m_history(std::move(history))
	, m_saver(std::move(saver))
	, m_fieldArea(fieldArea)
{
}

void DocumentModel::Save()
{
    if (m_savePath.empty())
    {
        throw std::runtime_error("No save path set. Use SaveAs first.");
    }

    m_saver->Serialize(m_savePath);
}

void DocumentModel::SaveAs(const std::string& path)
{
    if (path.empty())
    {
        throw std::invalid_argument("Save path cannot be empty");
    }

    m_saver->Serialize(path);
    m_savePath = path;
}

void DocumentModel::Load(const std::string& path)
{
    if (path.empty())
    {
        throw std::invalid_argument("Load path cannot be empty");
    }

	try
	{
		m_items.clear();
		m_saver->Deserialize(path);
		m_savePath = path;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

std::string DocumentModel::GetSavePath() const
{
	std::filesystem::path path = m_savePath;
	return path.parent_path().string();
}

Rect DocumentModel::GetBounds() const
{
	return m_fieldArea;
}

Point DocumentModel::MoveItemsBy(const std::vector<size_t> indexes, const Point& delta)
{
    if (indexes.empty()) {
        return { 0.0, 0.0 };
    }

    // 1. Вычисляем общую bounding box для всех перемещаемых объектов
    Rect groupBBox = CalculateGroupBoundingBox(indexes);
    if (groupBBox.width <= 0 || groupBBox.height <= 0) {
        return { 0.0, 0.0 };
    }

    // 2. Вычисляем границы документа
    double docLeft = m_fieldArea.x;
    double docRight = m_fieldArea.x + m_fieldArea.width;
    double docTop = m_fieldArea.y;
    double docBottom = m_fieldArea.y + m_fieldArea.height;

    // 3. Рассчитываем потенциальную новую позицию
    double proposedX = groupBBox.x + delta.x;
    double proposedY = groupBBox.y + delta.y;

    // 4. Ограничиваем позицию так, чтобы вся группа помещалась в документ
    double constrainedX = proposedX;
    double constrainedY = proposedY;

    // Проверка левой границы
    if (constrainedX < docLeft) {
        constrainedX = docLeft;
    }
    // Проверка правой границы
    else if (constrainedX + groupBBox.width > docRight) {
        constrainedX = docRight - groupBBox.width;
    }

    // Проверка верхней границы
    if (constrainedY < docTop) {
        constrainedY = docTop;
    }
    // Проверка нижней границы
    else if (constrainedY + groupBBox.height > docBottom) {
        constrainedY = docBottom - groupBBox.height;
    }

    // 5. Вычисляем фактическое смещение
    double actualDeltaX = constrainedX - groupBBox.x;
    double actualDeltaY = constrainedY - groupBBox.y;

    // 6. Применяем смещение ко всем объектам
    Point actualDelta(actualDeltaX, actualDeltaY);
    if (actualDelta.x != 0 || actualDelta.y != 0) {
        for (size_t index : indexes) {
            if (index < m_items.size()) {
                m_items[index]->MoveBy(actualDelta);
            }
        }
    }

    return actualDelta;
}

Point DocumentModel::ConstrainPoint(const Point& p) const
{
	double minX = m_fieldArea.x;
	double maxX = m_fieldArea.x + m_fieldArea.width;
	double minY = m_fieldArea.y;
	double maxY = m_fieldArea.y + m_fieldArea.height;

	return 
	{
		std::clamp(p.x, minX, maxX),
		std::clamp(p.y, minY, maxY)
	};
}

Rect DocumentModel::CalculateGroupBoundingBox(const std::vector<size_t>& indexes) const
{
    if (indexes.empty())
    {
        return {};
    }

    double minX = std::numeric_limits<double>::max();
    double minY = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();
    double maxY = std::numeric_limits<double>::lowest();

    for (size_t index : indexes) 
    {
        if (index >= m_items.size()) 
            continue;

        Rect bbox = m_items[index]->GetBoundingBox();
        minX = std::min(minX, bbox.x);
        minY = std::min(minY, bbox.y);
        maxX = std::max(maxX, bbox.x + bbox.width);
        maxY = std::max(maxY, bbox.y + bbox.height);
    }

    return {
        minX,
        minY,
        maxX - minX,
        maxY - minY
    };
}

void DocumentModel::AddItem(std::unique_ptr<DocumentItem>&& item)
{
	m_items.push_back(std::move(item));
}

std::shared_ptr<DocumentItem> DocumentModel::GetItemAtIndex(size_t index)
{
	return m_items.at(index);
}

std::shared_ptr<const DocumentItem> DocumentModel::GetItemAtIndex(size_t index) const
{
	return m_items.at(index);
}

void DocumentModel::RemoveItemAtIndex(size_t index)
{
	if (index < m_items.size())
	{
		m_items.erase(m_items.begin() + index);
	}
}

size_t DocumentModel::GetItemsCount() const
{
	return m_items.size();
}

bool DocumentModel::CanUndo() const
{
	return m_history->CanUndo();
}

void DocumentModel::Undo()
{
	if (CanUndo())
		m_history->Undo();
}

bool DocumentModel::CanRedo() const
{
	return m_history->CanRedo();
}

void DocumentModel::Redo()
{
	if (CanRedo())
		m_history->Redo();
}
