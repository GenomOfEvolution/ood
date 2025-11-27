#include "MainView.h"
#include "../../Views/Ribbon/RibbonTab/RibbonTab.h"
#include "../../Views/Ribbon/RibbonGroup/RibbonGroup.h"

#include <QFile>
#include <QScreen>

namespace 
{
    QString LoadResource(const QString& path)
    {
        QFile file(path);
        if (file.open(QFile::ReadOnly)) 
        {
            return file.readAll();
        }
        return QString();
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

    // Создание вкладок
    auto fileTab = new RibbonTab();
    auto homeTab = new RibbonTab();
    auto insertTab = new RibbonTab();

    m_ribbonBar->addRibbonTab("File", fileTab);
    m_ribbonBar->addRibbonTab("Home", homeTab);
    m_ribbonBar->addRibbonTab("Insert", insertTab);

    // === File Tab ===
    auto fileGroup = fileTab->addGroup("File");
    fileGroup->addButton("Open", ":/icons/open-file-icon.svg", [this]() { });
    fileGroup->addButton("Save", ":/icons/save-icon.svg", [this]() { });
    fileGroup->addButton("Save As", ":/icons/save-as-icon.svg", [this]() { });

    // === Home Tab ===
    auto shapesGroup = homeTab->addGroup("Shapes");
    shapesGroup->addButton("Rectangle", ":/icons/rectangle-icon.svg", [this]() { });
    shapesGroup->addButton("Triangle", ":/icons/triangle-icon.svg", [this]() {  });
    shapesGroup->addButton("Ellipse", ":/icons/ellipse-icon.svg", [this]() {  });

    // === Insert Tab ===
    auto mediaGroup = insertTab->addGroup("Media");
    mediaGroup->addButton("Image", ":/icons/image-icon.svg", [this]() {  });

    m_mainLayout->addWidget(m_ribbonBar, 0);
}

void UI::MainView::LoadStyles()
{
    QString baseStyle = LoadResource(":/styles/BaseStyles.qss");
    QString titleStyle = LoadResource(":/styles/TitleBarStyles.qss");
    QString ribbonStyle = LoadResource(":/styles/RibbonStyles.qss");

    setStyleSheet(baseStyle + ribbonStyle);

    m_titleBar->setStyleSheet(titleStyle);
}
