// UI/QtDialogService.h
#pragma once
#include "../IDialogService.h"
#include <QWidget>

class QtDialogService : public IDialogService
{
public:
    explicit QtDialogService(QWidget* parent = nullptr);

    std::optional<std::string> GetSaveFilePath() override;
    std::optional<std::string> GetOpenFilePath() override;

private:
    QWidget* m_parent;
};