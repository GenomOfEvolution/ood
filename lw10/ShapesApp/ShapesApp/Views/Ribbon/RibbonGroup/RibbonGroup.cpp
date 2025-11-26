#include "RibbonGroup.h"
#include <QVBoxLayout>
#include <QLabel>

UI::RibbonGroup::RibbonGroup(const QString& title, QWidget* parent) : QFrame(parent) 
{
    setObjectName("RibbonGroup");

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(3);

    QLabel* titleLabel = new QLabel(title);
    layout->addWidget(titleLabel);
    layout->addStretch();
}