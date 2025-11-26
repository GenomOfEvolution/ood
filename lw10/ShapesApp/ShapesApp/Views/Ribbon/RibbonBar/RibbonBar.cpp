#include "RibbonBar.h"
#include <QPainter>
#include <qfile.h>

UI::RibbonBar::RibbonBar(QWidget* parent) : QTabWidget(parent) 
{
    setObjectName("RibbonBar");
    setMovable(false);
    setTabsClosable(false);
}

void UI::RibbonBar::addRibbonTab(const QString& tabName, QWidget* tabContent) 
{
    addTab(tabContent, tabName);
}

void UI::RibbonBar::paintEvent(QPaintEvent*) 
{
    QPainter painter(this);
    painter.fillRect(rect(), QColor("#f5f5f5")); 
}