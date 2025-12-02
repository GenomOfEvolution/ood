#pragma once
#include <QWidget>
#include <QMainWindow>
#include <qtoolbutton.h>

class CustomTitleBar : public QWidget 
{
    Q_OBJECT
public:
    explicit CustomTitleBar(QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override;

private slots:
    void onMaximizeClicked();

private:
    void updateMaximizeButtonIcon();

    QMainWindow* m_mainWindow = nullptr;

    QToolButton* m_undoButton = nullptr;
    QToolButton* m_redoButton = nullptr;
    QToolButton* m_saveButton = nullptr;

    QToolButton* m_maxButton = nullptr;
    QPoint m_dragPosition;
};