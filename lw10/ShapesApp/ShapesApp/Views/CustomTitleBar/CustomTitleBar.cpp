#include "CustomTitleBar.h"
#include <QHBoxLayout>
#include <QToolButton>
#include <QMouseEvent>

CustomTitleBar::CustomTitleBar(DocumentController* controller, QWidget* parent) 
    : QWidget(parent) 
    , m_controller(controller)
{
    setObjectName("titleBar");

    QToolButton* minButton = new QToolButton();
    minButton->setObjectName("minButton"); 
    minButton->setIcon(QIcon(":/icons/window-minimize-icon.svg"));
    minButton->setToolTip("Minimize");

    QToolButton* maxButton = new QToolButton();
    maxButton->setObjectName("maxButton"); 
    maxButton->setIcon(QIcon(":/icons/window-maximize-icon.svg"));
    maxButton->setToolTip("Maximize");

    QToolButton* closeButton = new QToolButton();
    closeButton->setObjectName("closeButton");
    closeButton->setIcon(QIcon(":/icons/window-close-icon.svg"));
    closeButton->setToolTip("Close");

    // Кнопки Undo/Redo
    m_undoButton = new QToolButton();
    m_undoButton->setIcon(QIcon(":/icons/undo-icon.svg"));
    m_undoButton->setToolTip("Undo (Ctrl+Z)");

    m_redoButton = new QToolButton();
    m_redoButton->setIcon(QIcon(":/icons/redo-icon.svg"));
    m_redoButton->setToolTip("Redo (Ctrl+Y)");

    m_saveButton = new QToolButton();
    m_saveButton->setIcon(QIcon(":/icons/save-icon-white.svg"));
    m_saveButton->setToolTip("Save (Ctrl+S)");
    m_saveButton->setObjectName("saveButton");

    // Сборка макета
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    layout->addWidget(m_saveButton);
    layout->addWidget(m_undoButton);
    layout->addWidget(m_redoButton);
    layout->addStretch();
    layout->addWidget(minButton);
    layout->addWidget(maxButton);
    layout->addWidget(closeButton);

    m_maxButton = maxButton;

    m_mainWindow = qobject_cast<QMainWindow*>(window());
    if (m_mainWindow) 
    {
        m_mainWindow->installEventFilter(this);
        updateMaximizeButtonIcon();
    }

    // Подключение сигналов
    connect(minButton, &QToolButton::clicked, parent, &QWidget::showMinimized);
    connect(maxButton, &QToolButton::clicked, this, &CustomTitleBar::onMaximizeClicked);
    connect(closeButton, &QToolButton::clicked, parent, &QWidget::close);

    connect(m_undoButton, &QToolButton::clicked, m_controller, &DocumentController::Undo);
    connect(m_redoButton, &QToolButton::clicked, m_controller, &DocumentController::Redo);
    connect(m_saveButton, &QToolButton::clicked, m_controller, &DocumentController::Save);
}

void CustomTitleBar::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && m_mainWindow && !m_mainWindow->isMaximized())
    {
        m_dragPosition = event->globalPosition().toPoint() - m_mainWindow->frameGeometry().topLeft();
        event->accept();
    }
    else
    {
        QWidget::mousePressEvent(event);
    }
}

void CustomTitleBar::mouseMoveEvent(QMouseEvent* event)
{
    if ((event->buttons() & Qt::LeftButton) && m_mainWindow && !m_mainWindow->isMaximized())
    {
        m_mainWindow->move(event->globalPosition().toPoint() - m_dragPosition);
        event->accept();
    }
    else
    {
        QWidget::mouseMoveEvent(event);
    }
}

bool CustomTitleBar::eventFilter(QObject* obj, QEvent* event) 
{
    if (m_mainWindow && obj == m_mainWindow && event->type() == QEvent::WindowStateChange)
    {
        updateMaximizeButtonIcon();
    }

    return QWidget::eventFilter(obj, event);
}

void CustomTitleBar::onMaximizeClicked()
{
    if (!m_mainWindow) return; 

    if (m_mainWindow->isMaximized()) 
    {
        m_mainWindow->showNormal();
    }
    else
    {
        m_mainWindow->showMaximized();
    }
}

void CustomTitleBar::updateMaximizeButtonIcon()
{
    if (!m_mainWindow || !m_maxButton) return;

    if (m_mainWindow->isMaximized()) {
        m_maxButton->setIcon(QIcon(":/icons/window-restore-icon.svg"));
        m_maxButton->setToolTip("Restore");
    }
    else {
        m_maxButton->setIcon(QIcon(":/icons/window-maximize-icon.svg"));
        m_maxButton->setToolTip("Maximize");
    }
}