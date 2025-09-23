#include "Picture.h"
#include <stdexcept>

namespace shapes 
{
	Shape* Picture::GetShape(const std::string& id)
	{
		auto it = m_shapes.find(id);
		return (it != m_shapes.end()) ? it->second.get() : nullptr;
	}

	const Shape* Picture::GetShape(const std::string& id) const
	{
		auto it = m_shapes.find(id);
		return (it != m_shapes.end()) ? it->second.get() : nullptr;
	}

	void Picture::AddShape(const std::string& id, std::unique_ptr<Shape> shape)
	{
		m_shapes[id] = std::move(shape);
		m_order.push_back(id);
	}

	void Picture::DeleteShape(const std::string& id)
	{
		if (auto it = m_shapes.find(id); it != m_shapes.end()) 
		{
			m_shapes.erase(it);
			m_order.erase(std::remove(m_order.begin(), m_order.end(), id), m_order.end());
		}
	}

	void Picture::MovePicture(double dx, double dy)
	{
		for (const auto& id : m_order) 
		{
			auto it = m_shapes.find(id);
			if (it != m_shapes.end() && it->second) 
			{
				it->second->Move(dx, dy);
			}
		}
	}

	void Picture::DrawPicture(gfx::ICanvas* canvas) const
	{
		if (!canvas) 
		{
			throw std::invalid_argument("Canvas cannot be null");
		}

		for (const auto& id : m_order) 
		{
			auto it = m_shapes.find(id);
			if (it != m_shapes.end() && it->second) 
			{
				it->second->Draw(canvas);
			}
		}
	}
	std::string Picture::GetAllInfo() const
	{
		std::string result;
		int i = 1;

		for (const auto& id : m_order)
		{
			auto it = m_shapes.find(id);
			if (it != m_shapes.end() && it->second)
			{
				result += std::to_string(i) 
					+ " " + it->second->GetType() 
					+ " " + it->first
					+ " " + it->second->GetColor().m_data
					+ " " + it->second->GetInfo() + "\n";
				i++;
			}
		}

		return result;
	}
} // namespace shapes