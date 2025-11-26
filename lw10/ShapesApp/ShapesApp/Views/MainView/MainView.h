#pragma once
#include <QMainWindow>
#include <QVBoxLayout>
#include <memory>
#include "../Ribbon/RibbonBar/RibbonBar.h"
#include "../../Views/CustomTitleBar/CustomTitleBar.h"

namespace UI
{
	class MainView : public QMainWindow
	{
		Q_OBJECT
	public:
		explicit MainView(QWidget* parent = nullptr);
		void UpdateUndoRedoState(bool undoAvailable, bool redoAvailable);

		~MainView() override;

	private:
		void SetupUI();
		void SetupCustomTitleBar();
		void SetupRibbonBar();
		void LoadStyles();

		RibbonBar* m_ribbonBar = nullptr;
		QVBoxLayout* m_mainLayout = nullptr;
		CustomTitleBar* m_titleBar = nullptr;
	};
} // namespace UI