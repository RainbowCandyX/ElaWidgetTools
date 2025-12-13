#include "T_ElaScreen.h"

#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
#include "ElaComboBox.h"
#include "ElaScrollPageArea.h"
#include "ElaText.h"
#include "ElaToggleButton.h"
#include <QThread>
#include <QTimer>
#include <QVBoxLayout>

#ifdef Q_OS_WIN
#include "ElaDxgiManager.h"
#include "ElaLineEdit.h"
#include "T_ElaPacketIO.h"
#include "T_RecvScreen.h"
#endif

#ifdef Q_OS_MAC
#include "ElaScreenCaptureManager.h"
#endif

T_ElaScreen::T_ElaScreen(QWidget* parent)
    : T_BasePage(parent)
{
    setWindowTitle("ElaScreen");

#ifdef Q_OS_WIN
    createCustomWidget("DXGI录制组件被放置于此，可在此界面预览录制效果");

    ElaDxgiManager* dxgiManager = ElaDxgiManager::getInstance();
    dxgiManager->setGrabArea(1920, 1080);

    ElaScrollPageArea* dxgiScreenArea = new ElaScrollPageArea(this);
    dxgiScreenArea->setFixedHeight(700);
    QHBoxLayout* dxgiScreenLayout = new QHBoxLayout(dxgiScreenArea);
    _dxgiScreen = new ElaDxgiScreen(this);
    _dxgiScreen->setFixedSize(1200, 678);
    dxgiScreenLayout->addWidget(_dxgiScreen);

    ElaText* dxText = new ElaText("显卡选择", this);
    dxText->setTextPixelSize(15);
    _dxComboBox = new ElaComboBox(this);
    _dxComboBox->addItems(dxgiManager->getDxDeviceList());
    _dxComboBox->setCurrentIndex(dxgiManager->getDxDeviceID());

    ElaText* outputText = new ElaText("屏幕选择", this);
    outputText->setTextPixelSize(15);
    _outputComboBox = new ElaComboBox(this);
    _outputComboBox->addItems(dxgiManager->getOutputDeviceList());
    _outputComboBox->setCurrentIndex(dxgiManager->getOutputDeviceID());

    connect(_dxComboBox, QOverload<int>::of(&ElaComboBox::currentIndexChanged), this, [=](int index) {
        dxgiManager->setDxDeviceID(index);
        _outputComboBox->blockSignals(true);
        _outputComboBox->clear();
        _outputComboBox->addItems(dxgiManager->getOutputDeviceList());
        _outputComboBox->setCurrentIndex(dxgiManager->getOutputDeviceID());
        _outputComboBox->blockSignals(false);
        _dxgiScreen->update();
    });
    connect(_outputComboBox, QOverload<int>::of(&ElaComboBox::currentIndexChanged), this, [=](int index) {
        dxgiManager->setOutputDeviceID(index);
        _dxgiScreen->update();
    });

    ElaToggleButton* startButton = new ElaToggleButton("捕获", this);
    connect(startButton, &ElaToggleButton::toggled, this, [=](bool isToggled) {
        if (isToggled)
        {
            dxgiManager->startGrabScreen();
        }
        else
        {
            dxgiManager->stopGrabScreen();
            _dxgiScreen->update();
        }
    });

    QHBoxLayout* comboBoxLayout = new QHBoxLayout();
    comboBoxLayout->addWidget(dxText);
    comboBoxLayout->addWidget(_dxComboBox);
    comboBoxLayout->addSpacing(10);
    comboBoxLayout->addWidget(outputText);
    comboBoxLayout->addWidget(_outputComboBox);
    comboBoxLayout->addWidget(startButton);
    comboBoxLayout->addStretch();

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("ElaScreen");
    QVBoxLayout* centerLayout = new QVBoxLayout(centralWidget);
    centerLayout->setContentsMargins(0, 0, 0, 0);
    centerLayout->addLayout(comboBoxLayout);
    centerLayout->addWidget(dxgiScreenArea);

#if defined(BUILD_WITH_ELAPACKETIO)
    QHBoxLayout* packetLayout = new QHBoxLayout();
    ElaText* packetIOText = new ElaText("网络视图 (需要先进行屏幕捕获 若接口IP不正确或不可用 程序可能会崩溃)", this);
    packetIOText->setTextPixelSize(17);

    ElaText* interfaceIPText = new ElaText("接口IP", this);
    interfaceIPText->setTextPixelSize(15);
    ElaLineEdit* interfaceIPEdit = new ElaLineEdit(this);
    interfaceIPEdit->setMaximumWidth(140);
    interfaceIPEdit->setFixedHeight(33);
    interfaceIPEdit->setPlaceholderText("接口IP");
    interfaceIPEdit->setText("192.168.1");

    ElaToggleButton* sendButton2 = new ElaToggleButton("初始发送", this);
    connect(sendButton2, &ElaToggleButton::toggled, this, [=](bool isToggled) {
        if (isToggled)
        {
            _initSendThread(interfaceIPEdit->text().trimmed());
        }
        else
        {
            _unInitThread(true);
        }
    });
    ElaToggleButton* recvButton = new ElaToggleButton("初始接收", this);
    connect(recvButton, &ElaToggleButton::toggled, this, [=](bool isToggled) {
        if (isToggled)
        {
            _initRecvThread(interfaceIPEdit->text().trimmed());
        }
        else
        {
            _unInitThread(false);
        }
    });
    packetLayout->addWidget(interfaceIPText);
    packetLayout->addWidget(interfaceIPEdit);
    packetLayout->addWidget(sendButton2);
    packetLayout->addWidget(recvButton);
    packetLayout->addStretch();
    _recvScreen = new T_RecvScreen(this);
    ElaScrollPageArea* recvScreenArea = new ElaScrollPageArea(this);
    recvScreenArea->setFixedHeight(700);
    QHBoxLayout* recvScreenLayout = new QHBoxLayout(recvScreenArea);
    recvScreenLayout->addWidget(_recvScreen);
    centerLayout->addSpacing(30);
    centerLayout->addWidget(packetIOText);
    centerLayout->addLayout(packetLayout);
    centerLayout->addWidget(recvScreenArea);
#endif

    addCentralWidget(centralWidget, false, true);
#endif

#ifdef Q_OS_MAC
    createCustomWidget("屏幕录制组件被放置于此，可在此界面预览录制效果");

    ElaScreenCaptureManager* captureManager = ElaScreenCaptureManager::getInstance();
    captureManager->setGrabArea(1920, 1080);

    ElaScrollPageArea* captureScreenArea = new ElaScrollPageArea(this);
    captureScreenArea->setFixedHeight(700);
    QHBoxLayout* captureScreenLayout = new QHBoxLayout(captureScreenArea);
    _captureScreen = new ElaScreenCaptureScreen(this);
    _captureScreen->setFixedSize(1200, 678);
    captureScreenLayout->addWidget(_captureScreen);

    ElaText* displayText = new ElaText("显示器选择", this);
    displayText->setTextPixelSize(15);
    _displayComboBox = new ElaComboBox(this);
    _displayComboBox->addItems(captureManager->getDisplayList());
    _displayComboBox->setCurrentIndex(captureManager->getDisplayID());

    connect(_displayComboBox, QOverload<int>::of(&ElaComboBox::currentIndexChanged), this, [=](int index) {
        captureManager->setDisplayID(index);
        _captureScreen->update();
    });

    ElaToggleButton* startButton = new ElaToggleButton("捕获", this);
    connect(startButton, &ElaToggleButton::toggled, this, [=](bool isToggled) {
        if (isToggled)
        {
            captureManager->startGrabScreen();
        }
        else
        {
            captureManager->stopGrabScreen();
            _captureScreen->update();
        }
    });

    QHBoxLayout* comboBoxLayout = new QHBoxLayout();
    comboBoxLayout->addWidget(displayText);
    comboBoxLayout->addWidget(_displayComboBox);
    comboBoxLayout->addWidget(startButton);
    comboBoxLayout->addStretch();

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("ElaScreen");
    QVBoxLayout* centerLayout = new QVBoxLayout(centralWidget);
    centerLayout->setContentsMargins(0, 0, 0, 0);
    centerLayout->addLayout(comboBoxLayout);
    centerLayout->addWidget(captureScreenArea);

    addCentralWidget(centralWidget, false, true);
#endif
}

T_ElaScreen::~T_ElaScreen()
{
#if defined(Q_OS_WIN) && defined(BUILD_WITH_ELAPACKETIO)
    _unInitThread(true);
    _unInitThread(false);
#endif
}

#if defined(Q_OS_WIN) && defined(BUILD_WITH_ELAPACKETIO)
void T_ElaScreen::_initSendThread(QString interfaceIP)
{
    _packetIOSendThread = new QThread(this);
    _packetSendIO = new T_ElaPacketIO();
    _packetSendIO->setInterfaceIP(interfaceIP);
    _packetSendIO->moveToThread(_packetIOSendThread);
    connect(_packetIOSendThread, &QThread::started, _packetSendIO, &T_ElaPacketIO::handleGrabImage);
    _packetIOSendThread->start();
}

void T_ElaScreen::_initRecvThread(QString interfaceIP)
{
    // 原则上一个程序最好只初始化一个XIO 但如果需要多个XIO 则控制初始化间隔 不能同时进行初始化
    QTimer::singleShot(1000, this, [=]() {
        _packetIORecvThread = new QThread(this);
        _packetRecvIO = new T_ElaPacketIO();
        _packetRecvIO->setInterfaceIP(interfaceIP);
        _packetRecvIO->moveToThread(_packetIORecvThread);
        connect(_packetIORecvThread, &QThread::started, _packetRecvIO, &T_ElaPacketIO::handleImagePacket);
        _packetIORecvThread->start();
        connect(_packetRecvIO, &T_ElaPacketIO::sendHandleResult, _recvScreen, &T_RecvScreen::onSendHandleResult);
    });
}

void T_ElaScreen::_unInitThread(bool isSend)
{
    if (isSend)
    {
        if (!_packetSendIO || !_packetIOSendThread)
        {
            return;
        }
        _packetSendIO->setIsActive(false);
        _packetIOSendThread->quit();
        _packetIOSendThread->wait();
        delete _packetSendIO;
        delete _packetIOSendThread;
        _packetSendIO = nullptr;
        _packetIOSendThread = nullptr;
    }
    else
    {
        if (!_packetRecvIO || !_packetIORecvThread)
        {
            return;
        }
        _packetRecvIO->setIsActive(false);
        _packetIORecvThread->quit();
        _packetIORecvThread->wait();
        delete _packetRecvIO;
        delete _packetIORecvThread;
        _packetRecvIO = nullptr;
        _packetIORecvThread = nullptr;
    }
}
#endif
#endif
