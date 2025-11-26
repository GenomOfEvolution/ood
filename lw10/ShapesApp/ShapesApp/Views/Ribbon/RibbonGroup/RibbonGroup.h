#pragma once
#include <QFrame>

namespace UI 
{
    class RibbonGroup : public QFrame 
    {
        Q_OBJECT
    public:
        explicit RibbonGroup(const QString& title, QWidget* parent = nullptr);
    };
}