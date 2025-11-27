#pragma once
#include <QTabWidget>
#include <QToolButton>

class RibbonBar : public QTabWidget 
{
	Q_OBJECT
public:
	explicit RibbonBar(QWidget* parent = nullptr);
	void addRibbonTab(const QString& tabName, QWidget* tabContent);

protected:
	void paintEvent(QPaintEvent* event) override;
};