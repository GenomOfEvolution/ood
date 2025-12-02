#pragma once
#include <QMainWindow>
#include <QVBoxLayout>
#include <memory>
#include "../Ribbon/RibbonBar/RibbonBar.h"
#include "../../Views/CustomTitleBar/CustomTitleBar.h"
#include "../../Views/Workspace/WorkspaceWidget.h"
#include "../../Controllers/DocumentController/DocumentController.h"

namespace UI
{
	class MainView : public QMainWindow
	{
		Q_OBJECT
	public:
		explicit MainView(DocumentController* controller, QWidget* parent = nullptr);
		void UpdateUndoRedoState(bool undoAvailable, bool redoAvailable);

		~MainView() override;

	private:
		void SetupUI();
		void SetupCustomTitleBar();
		void SetupRibbonBar();
		void SetupWorkspace();

		void LoadStyles();

		RibbonBar* m_ribbonBar = nullptr;
		QVBoxLayout* m_mainLayout = nullptr;
		CustomTitleBar* m_titleBar = nullptr;
		WorkspaceWidget* m_workspaceWidget;

		DocumentController* m_controller = nullptr;
	};
} // namespace UI