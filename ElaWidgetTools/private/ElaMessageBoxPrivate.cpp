#include "ElaMessageBoxPrivate.h"

#include "ElaMaskWidget.h"
#include "ElaMessageBox.h"
#include "ElaPushButton.h"
#include "ElaText.h"
#include "ElaTheme.h"
#include <QGuiApplication>
#include <QLayout>
#include <QScreen>

ElaMessageBoxPrivate::ElaMessageBoxPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaMessageBoxPrivate::~ElaMessageBoxPrivate()
{
}

int ElaMessageBoxPrivate::_effectiveDefaultButtonIndex() const
{
    if (_isDefaultButtonIndexSet)
    {
        return _defaultButtonIndex;
    }
    return _buttonTexts.count() - 1;
}

void ElaMessageBoxPrivate::_rebuildButtons()
{
    Q_Q(ElaMessageBox);
    for (auto button: _buttons)
    {
        _buttonLayout->removeWidget(button);
        // 先隐藏再延迟删除，否则旧按钮会在事件循环处理前继续显示
        button->hide();
        button->deleteLater();
    }
    _buttons.clear();

    if (_buttonTexts.isEmpty())
    {
        _buttonTexts.append(QStringLiteral("确定"));
    }

    const int effectiveDefault = _effectiveDefaultButtonIndex();
    for (int i = 0; i < _buttonTexts.count(); ++i)
    {
        ElaPushButton* button = new ElaPushButton(_buttonTexts[i], _buttonWidget);
        button->setMinimumSize(0, 0);
        button->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
        button->setFixedHeight(38);
        button->setMinimumWidth(80);
        button->setBorderRadius(6);
        if (i == effectiveDefault)
        {
            button->setLightDefaultColor(ElaThemeColor(ElaThemeType::Light, PrimaryNormal));
            button->setLightHoverColor(ElaThemeColor(ElaThemeType::Light, PrimaryHover));
            button->setLightPressColor(ElaThemeColor(ElaThemeType::Light, PrimaryPress));
            button->setLightTextColor(Qt::white);
            button->setDarkDefaultColor(ElaThemeColor(ElaThemeType::Dark, PrimaryNormal));
            button->setDarkHoverColor(ElaThemeColor(ElaThemeType::Dark, PrimaryHover));
            button->setDarkPressColor(ElaThemeColor(ElaThemeType::Dark, PrimaryPress));
            button->setDarkTextColor(Qt::black);
        }
        connect(button, &ElaPushButton::clicked, q, [this, i]() {
            _doCloseAnimation(i);
        });
        _buttonLayout->addWidget(button);
        _buttons.append(button);
    }
}

void ElaMessageBoxPrivate::_updateContent()
{
    Q_Q(ElaMessageBox);
    _titleLabel->setText(_pTitle);
    _titleLabel->setVisible(!_pTitle.isEmpty());
    _textLabel->setText(_pText);
    _textLabel->setVisible(!_pText.isEmpty());
    _informationLabel->setText(_pInformationText);
    _informationLabel->setVisible(!_pInformationText.isEmpty());

    _iconLabel->setElaIcon(_icon);
    _iconLabel->setVisible(_icon != ElaIconType::None);

    QLayout* mainLayout = q->layout();
    if (mainLayout)
    {
        mainLayout->activate();
    }
    q->adjustSize();
    q->resize(q->width(), qMax(180, q->height()));
}

void ElaMessageBoxPrivate::_doCloseAnimation(int buttonIndex)
{
    Q_Q(ElaMessageBox);
    _clickedButtonIndex = buttonIndex;
    if (_maskWidget)
    {
        _maskWidget->doMaskAnimation(0);
    }
    if (buttonIndex >= 0)
    {
        Q_EMIT q->buttonClicked(buttonIndex);
    }
    q->done(buttonIndex);
}

void ElaMessageBoxPrivate::_moveToCenter()
{
    Q_Q(ElaMessageBox);
    // 优先相对父窗口居中
    QWidget* parent = q->parentWidget();
    if (parent && parent->isVisible())
    {
        const QPoint parentCenter = parent->mapToGlobal(parent->rect().center());
        q->move(parentCenter.x() - q->width() / 2, parentCenter.y() - q->height() / 2);
        return;
    }
    // 父窗口不可用时退回到屏幕居中
    QScreen* targetScreen = q->screen();
    if (!targetScreen)
    {
        targetScreen = QGuiApplication::primaryScreen();
    }
    if (!targetScreen)
    {
        return;
    }
    const QRect availableRect = targetScreen->availableGeometry();
    q->move(availableRect.left() + (availableRect.width() - q->width()) / 2,
            availableRect.top() + (availableRect.height() - q->height()) / 2);
}
