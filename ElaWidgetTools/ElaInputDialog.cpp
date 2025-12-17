#include "ElaInputDialog.h"

#include "ElaDoubleSpinBox.h"
#include "ElaLineEdit.h"
#include "ElaMaskWidget.h"
#include "ElaPushButton.h"
#include "ElaSpinBox.h"
#include "ElaText.h"
#include "ElaTheme.h"
#include "ElaWinShadowHelper.h"
#include "private/ElaInputDialogPrivate.h"
#include <QApplication>
#include <QDoubleValidator>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QIntValidator>
#include <QKeyEvent>
#include <QPainter>
#include <QScreen>
#include <QTextEdit>
#include <QTimer>
#include <QVBoxLayout>

Q_TAKEOVER_NATIVEEVENT_CPP(ElaInputDialog, d_func()->_appBar);
Q_PROPERTY_CREATE_Q_CPP(ElaInputDialog, QString, TitleText)
Q_PROPERTY_CREATE_Q_CPP(ElaInputDialog, QString, SubTitleText)
Q_PROPERTY_CREATE_Q_CPP(ElaInputDialog, QString, LabelText)
Q_PROPERTY_CREATE_Q_CPP(ElaInputDialog, QString, TextValue)
Q_PROPERTY_CREATE_Q_CPP(ElaInputDialog, int, IntValue)
Q_PROPERTY_CREATE_Q_CPP(ElaInputDialog, double, DoubleValue)
Q_PROPERTY_CREATE_Q_CPP(ElaInputDialog, QString, OkButtonText)
Q_PROPERTY_CREATE_Q_CPP(ElaInputDialog, QString, CancelButtonText)
Q_PROPERTY_CREATE_Q_CPP(ElaInputDialog, QString, PlaceholderText)
Q_PROPERTY_CREATE_Q_CPP(ElaInputDialog, int, InputMinimumWidth)
Q_PROPERTY_CREATE_Q_CPP(ElaInputDialog, int, InputMaximumWidth)

ElaInputDialog::ElaInputDialog(QWidget* parent)
    : QDialog{parent}, d_ptr(new ElaInputDialogPrivate())
{
    Q_D(ElaInputDialog);
    d->q_ptr = this;

    d->_maskWidget = new ElaMaskWidget(parent);
    d->_maskWidget->move(0, 0);
    d->_maskWidget->setFixedSize(parent->size());
    d->_maskWidget->setVisible(false);

    resize(400, 250);
    setWindowModality(Qt::ApplicationModal);

    d->_appBar = new ElaAppBar(this);
    d->_appBar->setWindowButtonFlags(ElaAppBarType::NoneButtonHint);
    d->_appBar->setIsFixedSize(true);
    d->_appBar->setAppBarHeight(0);
#ifdef Q_OS_WIN
    createWinId();
#endif

    d->_pTitleText = "";
    d->_pSubTitleText = "";
    d->_pLabelText = "";
    d->_pTextValue = "";
    d->_pIntValue = 0;
    d->_pDoubleValue = 0.0;
    d->_pOkButtonText = "确定";
    d->_pCancelButtonText = "取消";
    d->_pPlaceholderText = "";
    d->_pInputMinimumWidth = 100;
    d->_pInputMaximumWidth = QWIDGETSIZE_MAX;

    d->_titleLabel = new ElaText(d->_pTitleText, this);
    d->_titleLabel->setTextStyle(ElaTextType::Title);
    d->_subTitleLabel = new ElaText(d->_pSubTitleText, this);
    d->_subTitleLabel->setTextStyle(ElaTextType::Body);

    d->_inputLabel = new ElaText(d->_pLabelText, this);
    d->_inputLabel->setTextStyle(ElaTextType::Body);
    d->_lineEdit = new ElaLineEdit(this);
    d->_lineEdit->setText(d->_pTextValue);
    d->_lineEdit->setPlaceholderText(d->_pPlaceholderText);
    d->_lineEdit->setIsClearButtonEnable(true);
    connect(d->_lineEdit, &ElaLineEdit::textChanged, this, [=](const QString& text) {
        d->_pTextValue = text;
        Q_EMIT textValueChanged(text);
        bool okInt;
        int intValue = text.toInt(&okInt);
        if (okInt) {
            d->_pIntValue = intValue;
            Q_EMIT intValueChanged(intValue);
        }
        bool okDouble;
        double doubleValue = text.toDouble(&okDouble);
        if (okDouble) {
            d->_pDoubleValue = doubleValue;
            Q_EMIT doubleValueChanged(doubleValue);
        }
    });

    d->_cancelButton = new ElaPushButton(d->_pCancelButtonText, this);
    connect(d->_cancelButton, &ElaPushButton::clicked, this, &ElaInputDialog::onCancelButtonClicked);
    d->_cancelButton->setMinimumSize(0, 0);
    d->_cancelButton->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    d->_cancelButton->setFixedHeight(38);
    d->_cancelButton->setBorderRadius(6);

    d->_okButton = new ElaPushButton(d->_pOkButtonText, this);
    connect(d->_okButton, &ElaPushButton::clicked, this, &ElaInputDialog::onOkButtonClicked);
    d->_okButton->setLightDefaultColor(ElaThemeColor(ElaThemeType::Light, PrimaryNormal));
    d->_okButton->setLightHoverColor(ElaThemeColor(ElaThemeType::Light, PrimaryHover));
    d->_okButton->setLightPressColor(ElaThemeColor(ElaThemeType::Light, PrimaryPress));
    d->_okButton->setLightTextColor(Qt::white);
    d->_okButton->setDarkDefaultColor(ElaThemeColor(ElaThemeType::Dark, PrimaryNormal));
    d->_okButton->setDarkHoverColor(ElaThemeColor(ElaThemeType::Dark, PrimaryHover));
    d->_okButton->setDarkPressColor(ElaThemeColor(ElaThemeType::Dark, PrimaryPress));
    d->_okButton->setDarkTextColor(Qt::black);
    d->_okButton->setMinimumSize(0, 0);
    d->_okButton->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    d->_okButton->setFixedHeight(38);
    d->_okButton->setBorderRadius(6);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QWidget* contentWidget = new QWidget(this);
    d->_contentLayout = new QVBoxLayout(contentWidget);
    d->_contentLayout->setContentsMargins(15, 25, 15, 15);
    d->_contentLayout->addWidget(d->_titleLabel);
    d->_contentLayout->addSpacing(2);
    d->_contentLayout->addWidget(d->_subTitleLabel);
    d->_contentLayout->addSpacing(15);
    d->_contentLayout->addWidget(d->_inputLabel);
    d->_contentLayout->addSpacing(5);
    d->_contentLayout->addWidget(d->_lineEdit);
    d->_contentLayout->addStretch();

    d->_buttonWidget = new QWidget(this);
    d->_buttonWidget->setFixedHeight(60);
    QHBoxLayout* buttonLayout = new QHBoxLayout(d->_buttonWidget);
    buttonLayout->setContentsMargins(15, 11, 15, 11);
    buttonLayout->addWidget(d->_cancelButton);
    buttonLayout->addWidget(d->_okButton);

    mainLayout->addWidget(contentWidget);
    mainLayout->addWidget(d->_buttonWidget);

    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
    });

    QTimer::singleShot(0, d->_lineEdit, [=]() {
        d->_lineEdit->setFocus();
    });
}

ElaInputDialog::~ElaInputDialog()
{
    Q_D(ElaInputDialog);
    d->_maskWidget->deleteLater();
}

QString ElaInputDialog::getText(QWidget* parent, const QString& title,
                                const QString& subtitle, const QString& label,
                                const QString& text,
                                bool* ok, const QString& okButtonText,
                                const QString& cancelButtonText,
                                int inputMinWidth, int inputMaxWidth)
{
    ElaInputDialog dialog(parent);
    dialog.setTitleText(title);
    dialog.setSubTitleText(subtitle);
    dialog.setLabelText(label);
    dialog.setTextValue(text);
    dialog.setOkButtonText(okButtonText);
    dialog.setCancelButtonText(cancelButtonText);
    dialog.setInputMinimumWidth(inputMinWidth);
    dialog.setInputMaximumWidth(inputMaxWidth);
    dialog.updateLabels();

    bool accepted = (dialog.exec() == QDialog::Accepted);
    if (ok)
        *ok = accepted;

    return accepted ? dialog.getTextValue() : QString();
}

int ElaInputDialog::getInt(QWidget* parent, const QString& title,
                           const QString& subtitle, const QString& label,
                           int value,
                           int minValue, int maxValue, int step,
                           bool* ok, const QString& okButtonText,
                           const QString& cancelButtonText,
                           int inputMinWidth, int inputMaxWidth)
{
    ElaInputDialog dialog(parent);
    dialog.setTitleText(title);
    dialog.setSubTitleText(subtitle);
    dialog.setLabelText(label);
    dialog.setIntValue(value);
    dialog.setTextValue(QString::number(value));
    dialog.setOkButtonText(okButtonText);
    dialog.setCancelButtonText(cancelButtonText);
    dialog.setInputMinimumWidth(inputMinWidth);
    dialog.setInputMaximumWidth(inputMaxWidth);
    dialog.updateLabels();
    dialog.setIntRange(minValue, maxValue, step);

    bool accepted = (dialog.exec() == QDialog::Accepted);
    if (ok)
        *ok = accepted;

    return accepted ? dialog.getIntValue() : value;
}

double ElaInputDialog::getDouble(QWidget* parent, const QString& title,
                                 const QString& subtitle, const QString& label,
                                 double value,
                                 double minValue, double maxValue, int decimals,
                                 bool* ok, const QString& okButtonText,
                                 const QString& cancelButtonText,
                                 int inputMinWidth, int inputMaxWidth)
{
    ElaInputDialog dialog(parent);
    dialog.setTitleText(title);
    dialog.setSubTitleText(subtitle);
    dialog.setLabelText(label);
    dialog.setDoubleValue(value);
    dialog.setTextValue(QString::number(value, 'f', decimals));
    dialog.setOkButtonText(okButtonText);
    dialog.setCancelButtonText(cancelButtonText);
    dialog.setInputMinimumWidth(inputMinWidth);
    dialog.setInputMaximumWidth(inputMaxWidth);
    dialog.updateLabels();
    dialog.setDoubleRange(minValue, maxValue, decimals);

    bool accepted = (dialog.exec() == QDialog::Accepted);
    if (ok)
        *ok = accepted;

    return accepted ? dialog.getDoubleValue() : value;
}

QString ElaInputDialog::getMultiLineText(QWidget* parent, const QString& title,
                                         const QString& subtitle, const QString& label,
                                         const QString& text,
                                         bool* ok, const QString& okButtonText,
                                         const QString& cancelButtonText,
                                         int inputMinWidth, int inputMaxWidth)
{
    ElaInputDialog dialog(parent);
    dialog.setTitleText(title);
    dialog.setSubTitleText(subtitle);
    dialog.setLabelText(label);
    dialog.setTextValue(text);
    dialog.setOkButtonText(okButtonText);
    dialog.setCancelButtonText(cancelButtonText);
    dialog.setInputMinimumWidth(inputMinWidth);
    dialog.setInputMaximumWidth(inputMaxWidth);
    dialog.updateLabels();
    dialog.setMultiLine(true);

    bool accepted = (dialog.exec() == QDialog::Accepted);
    if (ok)
        *ok = accepted;

    return accepted ? dialog.getTextValue() : QString();
}

void ElaInputDialog::setTextEchoMode(QLineEdit::EchoMode mode)
{
    Q_D(ElaInputDialog);
    d->_lineEdit->setEchoMode(mode);
}

QLineEdit::EchoMode ElaInputDialog::textEchoMode() const
{
    Q_D(const ElaInputDialog);
    return d->_lineEdit->echoMode();
}

void ElaInputDialog::setIntRange(int minValue, int maxValue, int step)
{
    Q_D(ElaInputDialog);

    if (d->_isIntMode)
        return;

    d->_isIntMode = true;

    if (!d->_spinBox)
    {
        d->_spinBox = new ElaSpinBox(this);
        d->_spinBox->setButtonMode(ElaSpinBoxType::PMSide);
        d->_spinBox->setMinimumWidth(d->_pInputMinimumWidth);
        d->_spinBox->setMaximumWidth(d->_pInputMaximumWidth);
        d->_spinBox->setMinimum(minValue);
        d->_spinBox->setMaximum(maxValue);
        d->_spinBox->setSingleStep(step);
        d->_spinBox->setValue(d->_pIntValue);

        connect(d->_spinBox, QOverload<int>::of(&ElaSpinBox::valueChanged), this, [=](int value) {
            d->_pIntValue = value;
            d->_pTextValue = QString::number(value);
            Q_EMIT intValueChanged(value);
            Q_EMIT textValueChanged(d->_pTextValue);
        });
    }

    d->_lineEdit->setVisible(false);
    d->_spinBox->setVisible(true);
    d->_spinBox->setMinimum(minValue);
    d->_spinBox->setMaximum(maxValue);
    d->_spinBox->setSingleStep(step);
    d->_spinBox->setValue(d->_pIntValue);

    if (d->_contentLayout)
    {
        d->_contentLayout->removeWidget(d->_lineEdit);
        d->_contentLayout->insertWidget(6, d->_spinBox);
    }
}

void ElaInputDialog::setDoubleRange(double minValue, double maxValue, int decimals)
{
    Q_D(ElaInputDialog);

    if (d->_isDoubleMode)
        return;

    d->_isDoubleMode = true;

    if (!d->_doubleSpinBox)
    {
        d->_doubleSpinBox = new ElaDoubleSpinBox(this);
        d->_doubleSpinBox->setButtonMode(ElaSpinBoxType::PMSide);
        d->_doubleSpinBox->setMinimumWidth(d->_pInputMinimumWidth);
        d->_doubleSpinBox->setMaximumWidth(d->_pInputMaximumWidth);
        d->_doubleSpinBox->setMinimum(minValue);
        d->_doubleSpinBox->setMaximum(maxValue);
        d->_doubleSpinBox->setDecimals(decimals);
        d->_doubleSpinBox->setValue(d->_pDoubleValue);

        connect(d->_doubleSpinBox, QOverload<double>::of(&ElaDoubleSpinBox::valueChanged), this, [=](double value) {
            d->_pDoubleValue = value;
            d->_pTextValue = QString::number(value, 'f', decimals);
            Q_EMIT doubleValueChanged(value);
            Q_EMIT textValueChanged(d->_pTextValue);
        });
    }

    d->_lineEdit->setVisible(false);
    d->_doubleSpinBox->setVisible(true);
    d->_doubleSpinBox->setMinimum(minValue);
    d->_doubleSpinBox->setMaximum(maxValue);
    d->_doubleSpinBox->setDecimals(decimals);
    d->_doubleSpinBox->setValue(d->_pDoubleValue);

    if (d->_contentLayout)
    {
        d->_contentLayout->removeWidget(d->_lineEdit);
        d->_contentLayout->insertWidget(6, d->_doubleSpinBox);
    }
}

void ElaInputDialog::setMultiLine(bool multiLine)
{
    Q_D(ElaInputDialog);
    if (d->_isMultiLine == multiLine)
        return;

    d->_isMultiLine = multiLine;

    if (multiLine)
    {
        if (!d->_textEdit)
        {
            d->_textEdit = new QTextEdit(this);
            d->_textEdit->setPlainText(d->_pTextValue);
            d->_textEdit->setMinimumHeight(150);
            connect(d->_textEdit, &QTextEdit::textChanged, this, [=]() {
                d->_pTextValue = d->_textEdit->toPlainText();
                Q_EMIT textValueChanged(d->_pTextValue);
            });
        }
        d->_lineEdit->setVisible(false);
        d->_textEdit->setVisible(true);
        d->_textEdit->setPlainText(d->_pTextValue);

        if (d->_contentLayout)
        {
            d->_contentLayout->removeWidget(d->_lineEdit);
            d->_contentLayout->insertWidget(6, d->_textEdit);
        }
        resize(400, 350);
    }
    else
    {
        if (d->_textEdit)
        {
            d->_textEdit->setVisible(false);
        }
        d->_lineEdit->setVisible(true);
        d->_lineEdit->setText(d->_pTextValue);

        if (d->_contentLayout && d->_textEdit)
        {
            d->_contentLayout->removeWidget(d->_textEdit);
            d->_contentLayout->insertWidget(4, d->_lineEdit);
        }
        resize(400, 250);
    }
}

void ElaInputDialog::showEvent(QShowEvent* event)
{
    Q_D(ElaInputDialog);
    if (parentWidget())
    {
        d->_maskWidget->setVisible(true);
        d->_maskWidget->raise();
        d->_maskWidget->setFixedSize(parentWidget()->size());
        d->_maskWidget->doMaskAnimation(90);
    }
    QDialog::showEvent(event);
}

void ElaInputDialog::paintEvent(QPaintEvent* event)
{
    Q_D(ElaInputDialog);
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

void ElaInputDialog::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        onOkButtonClicked();
        event->accept();
        return;
    }
    else if (event->key() == Qt::Key_Escape)
    {
        onCancelButtonClicked();
        event->accept();
        return;
    }
    QDialog::keyPressEvent(event);
}

void ElaInputDialog::onOkButtonClicked()
{
    Q_D(ElaInputDialog);
    d->_doCloseAnimation(true);
}

void ElaInputDialog::onCancelButtonClicked()
{
    Q_D(ElaInputDialog);
    d->_doCloseAnimation(false);
}

void ElaInputDialog::updateLabels()
{
    Q_D(ElaInputDialog);
    d->_titleLabel->setText(d->_pTitleText);
    d->_subTitleLabel->setText(d->_pSubTitleText);
    d->_inputLabel->setText(d->_pLabelText);
    d->_cancelButton->setText(d->_pCancelButtonText);
    d->_okButton->setText(d->_pOkButtonText);
}
