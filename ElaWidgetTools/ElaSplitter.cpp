#include "ElaSplitter.h"

#include "DeveloperComponents/ElaSplitterHandle.h"
#include "private/ElaSplitterPrivate.h"

Q_PROPERTY_CREATE_Q_CPP(ElaSplitter, int, HandleWidth)
Q_PROPERTY_CREATE_Q_CPP(ElaSplitter, int, GripLength)

ElaSplitter::ElaSplitter(QWidget* parent)
	: QSplitter(parent), d_ptr(new ElaSplitterPrivate())
{
	Q_D(ElaSplitter);
	d->q_ptr = this;
	d->_pHandleWidth = 6;
	d->_pGripLength = 36;
	setHandleWidth(d->_pHandleWidth);
	setChildrenCollapsible(false);
	setStyleSheet("QSplitter{background-color:transparent;}");
}

ElaSplitter::ElaSplitter(Qt::Orientation orientation, QWidget* parent)
	: ElaSplitter(parent)
{
	setOrientation(orientation);
}

ElaSplitter::~ElaSplitter()
{
}

QSplitterHandle* ElaSplitter::createHandle()
{
	Q_D(ElaSplitter);
	ElaSplitterHandle* handle = new ElaSplitterHandle(orientation(), this);
	handle->setGripLength(d->_pGripLength);
	return handle;
}
