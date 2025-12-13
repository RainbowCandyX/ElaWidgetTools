#include "ElaInputDialogPrivate.h"

#include "ElaInputDialog.h"
#include "ElaMaskWidget.h"

ElaInputDialogPrivate::ElaInputDialogPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaInputDialogPrivate::~ElaInputDialogPrivate()
{
}

void ElaInputDialogPrivate::_doCloseAnimation(bool isAccept)
{
    Q_Q(ElaInputDialog);
    _maskWidget->doMaskAnimation(0);
    isAccept ? q->accept() : q->reject();
}
