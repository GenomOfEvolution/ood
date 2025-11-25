#pragma once
#include <QMainWindow>
#include <memory>
#include "../Ribbon/RibbonBar/RibbonBar.h"

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
	};
} // namespace UI