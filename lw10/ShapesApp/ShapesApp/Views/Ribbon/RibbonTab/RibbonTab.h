#pragma once
#include "../RibbonGroup/RibbonGroup.h"
#include <QScrollArea>

class RibbonTab : public QScrollArea 
{
    Q_OBJECT
    Q_PROPERTY(bool selected READ isSelected WRITE setSelected)
public:
    explicit RibbonTab(QWidget* parent = nullptr);
    RibbonGroup* addGroup(const QString& groupName);

    bool isSelected() const { return m_selected; }
    void setSelected(bool selected);

private:
    bool m_selected = false;
    QVBoxLayout* m_mainLayout;
};