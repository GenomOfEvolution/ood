#pragma once
#include <QTabWidget>
#include <QToolButton>

namespace UI 
{
    class RibbonBar : public QTabWidget 
    {
        Q_OBJECT
    public:
        explicit RibbonBar(QWidget* parent = nullptr);
        void addRibbonTab(const QString& tabName, QWidget* tabContent);

    protected:
        void paintEvent(QPaintEvent* event) override;
    };
} // namespace UI