#pragma once
#include <QWidget>
#include <QMainWindow>
#include <qtoolbutton.h>

class CustomTitleBar : public QWidget {
    Q_OBJECT
public:
    explicit CustomTitleBar(QWidget* parent = nullptr);

    QToolButton* undoButton() const { return m_undoButton; }
    QToolButton* redoButton() const { return m_redoButton; }

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
    QToolButton* m_maxButton = nullptr;
    QPoint m_dragPosition;
};