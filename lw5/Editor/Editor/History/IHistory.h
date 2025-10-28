#pragma once



// TODO: какие средства ооп позвол€ют ограничить доступ к опред объекту
// TODO: разделить интерфейс истории на 2 версии 
// - только с методами Undo/Redo/CanUndo/CanRedo 
// - с AddAndExecute + 4 метода выше - принцип тонких интерфейсов
class IHistory
{
public:
	virtual bool CanUndo() const = 0;
	virtual void Undo() = 0;

	virtual bool CanRedo() const = 0;
	virtual void Redo() = 0;
};