#include "RibbonBar.h"
#include <QApplication>
#include <QStyleOption>
#include <QPainter>

using namespace UI;

RibbonBar::RibbonBar(QWidget* parent)
{
}

void RibbonBar::AddTab(const QString& tabName)
{
}

void RibbonBar::AddTab(const QIcon& tabIcon, const QString& tabName)
{
}

void RibbonBar::RemoveTab(const QString& tabName)
{
}

void RibbonBar::AddGroup(const QString& tabName, const QString& groupName)
{
}

void RibbonBar::AddButton(const QString& tabName, const QString& groupName, QToolButton* button)
{
}

void RibbonBar::RemoveButton(const QString& tabName, const QString& groupName, QToolButton* button)
{
}
