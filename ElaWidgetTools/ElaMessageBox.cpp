#include "ElaMessageBox.h"

#include "ElaMaskWidget.h"
#include "ElaText.h"
#include "ElaTheme.h"
#include "ElaWinShadowHelper.h"
#include "private/ElaMessageBoxPrivate.h"
#include <QApplication>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QPainter>
#include <QScreen>
#include <QVBoxLayout>

Q_TAKEOVER_NATIVEEVENT_CPP(ElaMessageBox, d_func()->_appBar);

ElaMessageBox::ElaMessageBox(QWidget* parent)
    : QDialog{parent}, d_ptr(new ElaMessageBoxPrivate())
{
    Q_D(ElaMessageBox);
    d->q_ptr = this;

    d->_maskWidget = new ElaMaskWidget(parent);
    if (parent)
    {
        d->_maskWidget->move(0, 0);
        d->_maskWidget->setFixedSize(parent->size());
    }
    d->_maskWidget->setVisible(false);

    resize(400, 180);
    setFixedWidth(400);
    setMinimumHeight(180);
    setWindowModality(Qt::ApplicationModal);

    d->_appBar = new ElaAppBar(this);
    d->_appBar->setWindowButtonFlags(ElaAppBarType::NoneButtonHint);
    d->_appBar->setIsFixedSize(true);
    d->_appBar->setAppBarHeight(10);
#ifdef Q_OS_WIN
    createWinId();
#endif

    d->_pTitle = "";
    d->_pText = "";
    d->_pInformationText = "";
    d->_pIconColor = QColor();
    d->_icon = ElaIconType::None;

    d->_iconLabel = new ElaText(this);
    d->_iconLabel->setTextPixelSize(40);
    d->_iconLabel->setFixedWidth(45);
    d->_iconLabel->setMinimumHeight(40);
    d->_iconLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    d->_iconLabel->hide();

    d->_titleLabel = new ElaText(this);
    d->_titleLabel->setTextStyle(ElaTextType::Title);
    d->_textLabel = new ElaText(this);
    d->_textLabel->setTextStyle(ElaTextType::Body);
    d->_informationLabel = new ElaText(this);
    d->_informationLabel->setTextStyle(ElaTextType::Body);

    QVBoxLayout* textLayout = new QVBoxLayout();
    textLayout->setContentsMargins(0, 0, 0, 0);
    textLayout->setSpacing(6);
    textLayout->addWidget(d->_titleLabel);
    textLayout->addWidget(d->_textLabel);
    textLayout->addWidget(d->_informationLabel);

    QHBoxLayout* contentLayout = new QHBoxLayout();
    contentLayout->setContentsMargins(15, 20, 15, 10);
    contentLayout->setSpacing(12);
    contentLayout->addWidget(d->_iconLabel, 0);
    contentLayout->addLayout(textLayout, 1);

    d->_buttonWidget = new QWidget(this);
    d->_buttonWidget->setFixedHeight(60);
    d->_buttonLayout = new QHBoxLayout(d->_buttonWidget);
    d->_buttonLayout->setContentsMargins(15, 11, 15, 11);
    d->_buttonLayout->setSpacing(10);
    d->_buttonLayout->addStretch();

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addLayout(contentLayout);
    mainLayout->addStretch();
    mainLayout->addWidget(d->_buttonWidget);

    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
        d->_updateContent();
        update();
    });

    d->_rebuildButtons();
    d->_updateContent();
}

ElaMessageBox::~ElaMessageBox()
{
    Q_D(ElaMessageBox);
    if (d->_maskWidget)
    {
        d->_maskWidget->deleteLater();
    }
}

void ElaMessageBox::setTitle(QString Title)
{
    Q_D(ElaMessageBox);
    d->_pTitle = Title;
    Q_EMIT pTitleChanged();
    d->_updateContent();
}

QString ElaMessageBox::getTitle() const
{
    return d_ptr->_pTitle;
}

void ElaMessageBox::setText(QString Text)
{
    Q_D(ElaMessageBox);
    d->_pText = Text;
    Q_EMIT pTextChanged();
    d->_updateContent();
}

QString ElaMessageBox::getText() const
{
    return d_ptr->_pText;
}

void ElaMessageBox::setInformationText(QString InformationText)
{
    Q_D(ElaMessageBox);
    d->_pInformationText = InformationText;
    Q_EMIT pInformationTextChanged();
    d->_updateContent();
}

QString ElaMessageBox::getInformationText() const
{
    return d_ptr->_pInformationText;
}

void ElaMessageBox::setIconColor(QColor IconColor)
{
    Q_D(ElaMessageBox);
    d->_pIconColor = IconColor;
    Q_EMIT pIconColorChanged();
    d->_iconLabel->setIconColor(IconColor);
    update();
}

QColor ElaMessageBox::getIconColor() const
{
    return d_ptr->_pIconColor;
}

void ElaMessageBox::setIcon(ElaIconType::IconName icon)
{
    Q_D(ElaMessageBox);
    if (d->_icon == icon)
    {
        return;
    }
    d->_icon = icon;
    d->_updateContent();
}

ElaIconType::IconName ElaMessageBox::getIcon() const
{
    return d_ptr->_icon;
}

void ElaMessageBox::setButtons(const QStringList& buttons)
{
    Q_D(ElaMessageBox);
    d->_buttonTexts = buttons;
    d->_rebuildButtons();
    d->_updateContent();
}

QStringList ElaMessageBox::getButtons() const
{
    return d_ptr->_buttonTexts;
}

void ElaMessageBox::setDefaultButtonIndex(int index)
{
    Q_D(ElaMessageBox);
    d->_defaultButtonIndex = index;
    d->_isDefaultButtonIndexSet = true;
    d->_rebuildButtons();
}

int ElaMessageBox::getDefaultButtonIndex() const
{
    return d_ptr->_effectiveDefaultButtonIndex();
}

int ElaMessageBox::getClickedButtonIndex() const
{
    return d_ptr->_clickedButtonIndex;
}

int ElaMessageBox::information(QWidget* parent, const QString& title, const QString& text,
                               const QStringList& buttons, int defaultButtonIndex)
{
    ElaMessageBox box(parent);
    box.setTitle(title);
    box.setText(text);
    box.setIcon(ElaIconType::CircleInfo);
    box.setIconColor(ElaThemeColor(eTheme->getThemeMode(), PrimaryNormal));
    box.setButtons(buttons);
    if (defaultButtonIndex >= 0)
    {
        box.setDefaultButtonIndex(defaultButtonIndex);
    }
    return box.exec();
}

int ElaMessageBox::warning(QWidget* parent, const QString& title, const QString& text,
                           const QStringList& buttons, int defaultButtonIndex)
{
    ElaMessageBox box(parent);
    box.setTitle(title);
    box.setText(text);
    box.setIcon(ElaIconType::TriangleExclamation);
    box.setIconColor(QColor(0xF7, 0x93, 0x0E));
    box.setButtons(buttons);
    if (defaultButtonIndex >= 0)
    {
        box.setDefaultButtonIndex(defaultButtonIndex);
    }
    return box.exec();
}

int ElaMessageBox::critical(QWidget* parent, const QString& title, const QString& text,
                            const QStringList& buttons, int defaultButtonIndex)
{
    ElaMessageBox box(parent);
    box.setTitle(title);
    box.setText(text);
    box.setIcon(ElaIconType::CircleExclamation);
    box.setIconColor(ElaThemeColor(eTheme->getThemeMode(), StatusDanger));
    box.setButtons(buttons);
    if (defaultButtonIndex >= 0)
    {
        box.setDefaultButtonIndex(defaultButtonIndex);
    }
    return box.exec();
}

int ElaMessageBox::question(QWidget* parent, const QString& title, const QString& text,
                            const QStringList& buttons, int defaultButtonIndex)
{
    ElaMessageBox box(parent);
    box.setTitle(title);
    box.setText(text);
    box.setIcon(ElaIconType::CircleQuestion);
    box.setIconColor(ElaThemeColor(eTheme->getThemeMode(), PrimaryNormal));
    box.setButtons(buttons);
    if (defaultButtonIndex >= 0)
    {
        box.setDefaultButtonIndex(defaultButtonIndex);
    }
    return box.exec();
}

void ElaMessageBox::about(QWidget* parent, const QString& title, const QString& text)
{
    ElaMessageBox box(parent);
    box.setTitle(title);
    box.setText(text);
    box.setIcon(ElaIconType::CircleInfo);
    box.setIconColor(ElaThemeColor(eTheme->getThemeMode(), PrimaryNormal));
    box.setButtons({QStringLiteral("确定")});
    box.exec();
}

void ElaMessageBox::reject()
{
    Q_D(ElaMessageBox);
    d->_clickedButtonIndex = -1;
    QDialog::done(-1);
}

void ElaMessageBox::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    Q_D(ElaMessageBox);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(ElaThemeColor(d->_themeMode, DialogBase));
    painter.drawRect(rect());
    painter.setBrush(ElaThemeColor(d->_themeMode, DialogLayoutArea));
    painter.drawRoundedRect(QRectF(0, height() - 60, width(), 60), 8, 8);
    painter.restore();
}

void ElaMessageBox::showEvent(QShowEvent* event)
{
    Q_D(ElaMessageBox);
    if (parentWidget() && d->_maskWidget)
    {
        d->_maskWidget->setVisible(true);
        d->_maskWidget->raise();
        d->_maskWidget->setFixedSize(parentWidget()->size());
        d->_maskWidget->doMaskAnimation(90);
    }
    d->_moveToCenter();
    QDialog::showEvent(event);
}

void ElaMessageBox::keyPressEvent(QKeyEvent* event)
{
    Q_D(ElaMessageBox);
    if (event->key() == Qt::Key_Escape)
    {
        d->_doCloseAnimation(-1);
        event->accept();
        return;
    }
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        const int defaultIndex = d->_effectiveDefaultButtonIndex();
        if (defaultIndex >= 0 && defaultIndex < d->_buttons.count())
        {
            d->_doCloseAnimation(defaultIndex);
            event->accept();
            return;
        }
    }
    QDialog::keyPressEvent(event);
}
