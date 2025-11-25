#include "MainView.h"
#include <QVBoxLayout>
#include <QFile>
#include <QScreen>

UI::MainView::MainView(QWidget* parent)
	: QMainWindow(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    setAttribute(Qt::WA_TranslucentBackground);

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setObjectName("centralWidget");
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

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
    // m_titleBar = new CustomTitleBar(this);
}

void UI::MainView::SetupRibbonBar()
{
    m_ribbonBar = new RibbonBar(this);

    m_ribbonBar->AddTab("File");
    m_ribbonBar->AddTab("Home");
    m_ribbonBar->AddTab("Insert");

    QToolButton* openFileButton = new QToolButton;
    openFileButton->setText("Open");
    openFileButton->setToolTip("Open existing file");
    // openProjectButton->setIcon(QIcon(""));
    openFileButton->setEnabled(true);
    m_ribbonBar->AddButton("Project", "Project", openFileButton);

    static_cast<QVBoxLayout*>(centralWidget()->layout())->addWidget(m_ribbonBar, 1);
}

void UI::MainView::LoadStyles()
{
    
}
