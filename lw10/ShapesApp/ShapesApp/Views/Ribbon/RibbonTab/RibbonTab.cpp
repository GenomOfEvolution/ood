#include "RibbonTab.h"
#include <QVBoxLayout>
#include <qvariant.h>
#include <qstyle.h>

UI::RibbonTab::RibbonTab(QWidget* parent)
    : QScrollArea(parent)
{
    setObjectName("RibbonTab");

    setFrameShape(QFrame::NoFrame);
    setWidgetResizable(true);

    QWidget* content = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(content);
    layout->setContentsMargins(10, 5, 10, 10); 
    setWidget(content);
}

void UI::RibbonTab::addGroup(const QString& groupName, QWidget* groupContent)
{
    if (!widget()) return;

    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(widget()->layout());
    if (layout) 
    {
        layout->addWidget(groupContent);
    }
}

void UI::RibbonTab::setSelected(bool selected)
{
    if (m_selected == selected) return;

    m_selected = selected;
    setProperty("selected", selected);
    style()->polish(this);
}