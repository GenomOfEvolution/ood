#include "MainView.h"
#include "../../Views/Ribbon/RibbonTab/RibbonTab.h"
#include "../../Views/Ribbon/RibbonGroup/RibbonGroup.h"

#include <QFile>
#include <QScreen>

namespace 
{
    void LoadStyleSheet(QWidget* widget, const QString& resourcePath) 
    {
        QFile file(resourcePath);
        if (file.open(QFile::ReadOnly)) 
        {
            QString styleSheet = QLatin1String(file.readAll());
            widget->setStyleSheet(styleSheet);
            file.close();
        }
    }
}


UI::MainView::MainView(QWidget* parent)
	: QMainWindow(parent)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_OpaquePaintEvent);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget); 
    centralWidget->setObjectName("centralWidget");

    m_mainLayout = new QVBoxLayout(centralWidget);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);
    setCentralWidget(centralWidget);

    SetupUI();
    LoadStyles();
}

void UI::MainView::UpdateUndoRedoState(bool undoAvailable, bool redoAvailable)
{
}

UI::MainView::~MainView() = default;

void UI::MainView::SetupUI()
{
    SetupCustomTitleBar();
    SetupRibbonBar();
}

void UI::MainView::SetupCustomTitleBar()
{
    m_titleBar = new CustomTitleBar(this);
    m_mainLayout->addWidget(m_titleBar);
}

void UI::MainView::SetupRibbonBar()
{
    m_ribbonBar = new RibbonBar(this);

    // Вкладки
    RibbonTab* fileTab = new RibbonTab();
    RibbonTab* homeTab = new RibbonTab();
    RibbonTab* insertTab = new RibbonTab();

    m_ribbonBar->addRibbonTab("File", fileTab);
    m_ribbonBar->addRibbonTab("Home", homeTab);
    m_ribbonBar->addRibbonTab("Insert", insertTab);

    m_mainLayout->addWidget(m_ribbonBar, 0);
}

void UI::MainView::LoadStyles()
{
    LoadStyleSheet(this, ":/styles/BaseStyles.qss");
    LoadStyleSheet(m_titleBar, ":/styles/TitleBarStyles.qss");
    LoadStyleSheet(m_ribbonBar, ":/styles/RibbonStyles.qss");
}
