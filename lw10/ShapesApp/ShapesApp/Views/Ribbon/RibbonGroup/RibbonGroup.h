#pragma once
#include <QVBoxLayout>
#include <QScrollArea>
#include <QToolButton>
#include <functional>

class RibbonGroup : public QWidget 
{
    Q_OBJECT
public:
    explicit RibbonGroup(const QString& title, QWidget* parent = nullptr);
    void addButton(const QString& text, const QString& iconPath = "", std::function<void()> callback = nullptr);

private:
    QHBoxLayout* m_buttonLayout;
};