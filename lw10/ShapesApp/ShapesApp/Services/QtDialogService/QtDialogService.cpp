#include "QtDialogService.h"
#include <QFileDialog>

QtDialogService::QtDialogService(QWidget* parent)
    : m_parent(parent)
{
}

std::optional<std::string> QtDialogService::GetSaveFilePath()
{
    QString path = QFileDialog::getSaveFileName(
        m_parent, "Save Document As", "", "Documents (*.xml)"
    );
    return path.isEmpty() ? std::nullopt : std::optional<std::string>(path.toStdString());
}

std::optional<std::string> QtDialogService::GetOpenFilePath()
{
    QString path = QFileDialog::getOpenFileName(
        m_parent, "Open Document", "", "Documents (*.xml)"
    );
    return path.isEmpty() ? std::nullopt : std::optional<std::string>(path.toStdString());
}