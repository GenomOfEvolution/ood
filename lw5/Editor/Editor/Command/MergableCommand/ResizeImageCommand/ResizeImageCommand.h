#pragma once
#include "../IMergableCommand.h"
#include "../../../DocumentItem/DocumentItem.h"
#include <vector>


class ResizeImageCommand : public IMergableCommand
{
public:
	ResizeImageCommand(std::shared_ptr<IImage> image, int width, int height);

	void DoExecute() override;
	void DoUnexecute() override;

	bool CanMergeWith(const ICommand& other) const override;
	void MergeWith(std::unique_ptr<ICommand> other) override;

private:
	std::shared_ptr<IImage> m_image;
	int m_newWidth, m_newHeight;
	int m_oldWidth = 0, m_oldHeight = 0;
};