#include "RibbonTab.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QIcon>
#include <QStyle>

RibbonTab::RibbonTab(QWidget* parent)
    : QScrollArea(parent)
{
    setObjectName("RibbonTab");
    setAttribute(Qt::WA_Hover);
    setFrameShape(QFrame::NoFrame);
    setWidgetResizable(true);

    auto contentWidget = new QWidget(this);
    m_mainLayout = new QVBoxLayout(contentWidget);
    m_mainLayout->setContentsMargins(10, 5, 10, 10);
    m_mainLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    setWidget(contentWidget);
}

RibbonGroup* RibbonTab::addGroup(const QString& groupName)
{
    auto group = new RibbonGroup(groupName, this);
    m_mainLayout->addWidget(group);
    return group;
}

void RibbonTab::setSelected(bool selected)
{
    if (m_selected == selected) return;
    m_selected = selected;
    setProperty("selected", selected);
    style()->polish(this);
}