#include "RibbonBar.h"
#include <QPainter>
#include <qfile.h>

RibbonBar::RibbonBar(QWidget* parent) : QTabWidget(parent) 
{
    setObjectName("RibbonBar");
    setMovable(false);
    setTabsClosable(false);
}

void RibbonBar::addRibbonTab(const QString& tabName, QWidget* tabContent) 
{
    addTab(tabContent, tabName);
}

void RibbonBar::paintEvent(QPaintEvent*) 
{
    QPainter painter(this);
    painter.fillRect(rect(), QColor("#f3f2f1")); 
}