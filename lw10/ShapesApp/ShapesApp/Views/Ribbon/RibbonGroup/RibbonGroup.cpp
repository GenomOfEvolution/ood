#include "RibbonGroup.h"
#include <QLabel>

RibbonGroup::RibbonGroup(const QString& title, QWidget* parent)
    : QWidget(parent)
{
    setObjectName("RibbonGroup");
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 4);
    mainLayout->setSpacing(2);

    // Контейнер для кнопок
    auto buttonContainer = new QWidget(this);
    buttonContainer->setObjectName("RibbonGroupContent");
    m_buttonLayout = new QHBoxLayout(buttonContainer);
    m_buttonLayout->setContentsMargins(4, 0, 4, 0);
    m_buttonLayout->setAlignment(Qt::AlignCenter);

    // Заголовок группы
    auto titleLabel = new QLabel(title, this);
    titleLabel->setObjectName("RibbonGroupTitle");
    titleLabel->setAlignment(Qt::AlignHCenter);

    mainLayout->addWidget(buttonContainer);
    mainLayout->addWidget(titleLabel);
}

void RibbonGroup::addButton(const QString& text, const QString& iconPath, std::function<void()> callback)
{
    auto button = new QToolButton(this);
    button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    button->setText(text);
    button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    button->setIconSize(QSize(32, 32));
    button->setAutoRaise(true);

    if (!iconPath.isEmpty())
    {
        QIcon icon(iconPath);
        button->setIcon(icon);
    }

    if (callback)
    {
        connect(button, &QToolButton::clicked, this, callback);
    }

    m_buttonLayout->addWidget(button);
}