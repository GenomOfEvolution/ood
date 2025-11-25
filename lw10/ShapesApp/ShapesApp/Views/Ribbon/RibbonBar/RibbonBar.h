#pragma once
#include <QTabWidget>
#include <QToolButton>

namespace UI 
{
	class RibbonBar : public QTabWidget
	{
		Q_OBJECT
	public:
		RibbonBar(QWidget* parent = nullptr);

		void AddTab(const QString& tabName);
		void AddTab(const QIcon& tabIcon, const QString& tabName);
		void RemoveTab(const QString& tabName);

		void AddGroup(const QString& tabName, const QString& groupName);

		void AddButton(const QString& tabName, const QString& groupName, QToolButton* button);
		void RemoveButton(const QString& tabName, const QString& groupName, QToolButton* button);
	};
} // namespace UI