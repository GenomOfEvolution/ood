#pragma once
#include <QScrollArea>

namespace UI
{
    class RibbonTab : public QScrollArea 
    {
        Q_OBJECT
        Q_PROPERTY(bool selected READ isSelected WRITE setSelected)
    public:
        explicit RibbonTab(QWidget* parent = nullptr);
        void addGroup(const QString& groupName, QWidget* groupContent);

        bool isSelected() const { return m_selected; }
        void setSelected(bool selected); 

    private:
        bool m_selected = false; 
    };
}