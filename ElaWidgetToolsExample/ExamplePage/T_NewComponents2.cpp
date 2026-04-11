#include "T_NewComponents2.h"

#include "ElaChatBubble.h"
#include "ElaDashboardGauge.h"
#include "ElaScrollPageArea.h"
#include "ElaSlider.h"
#include "ElaTerminalWidget.h"
#include "ElaText.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

T_NewComponents2::T_NewComponents2(QWidget *parent)
	: T_BasePage(parent)
{
	setWindowTitle("ElaNewComponents2");
	createCustomWidget("新增组件演示页面 II - ChatBubble / DashboardGauge / TerminalWidget");

	QWidget *centralWidget = new QWidget(this);
	centralWidget->setWindowTitle("ElaNewComponents2");

	// ========== ElaChatBubble 示例 ==========
	ElaText *chatTitle = new ElaText("ElaChatBubble", this);
	chatTitle->setTextPixelSize(18);

	_chatBubbleLeft = new ElaChatBubble(this);
	_chatBubbleLeft->setDirection(ElaChatBubble::Left);
	_chatBubbleLeft->setSenderName("Alice");
	_chatBubbleLeft->setMessageText("你好！最近在忙什么项目？听说你在做一个新的组件库？");
	_chatBubbleLeft->setTimestamp("10:30");
	_chatBubbleLeft->setMinimumWidth(500);

	_chatBubbleRight = new ElaChatBubble(this);
	_chatBubbleRight->setDirection(ElaChatBubble::Right);
	_chatBubbleRight->setSenderName("Bob");
	_chatBubbleRight->setMessageText("是的，我在开发 ElaWidgetTools 的新组件，包括聊天气泡、仪表盘和终端模拟器！");
	_chatBubbleRight->setTimestamp("10:31");
	_chatBubbleRight->setStatus(ElaChatBubble::Read);
	_chatBubbleRight->setMinimumWidth(500);

	_chatBubbleSystem = new ElaChatBubble(this);
	_chatBubbleSystem->setDirection(ElaChatBubble::Left);
	_chatBubbleSystem->setMessageText("这个组件支持左右对齐、头像、时间戳、发送状态等功能。");
	_chatBubbleSystem->setTimestamp("10:32");
	_chatBubbleSystem->setStatus(ElaChatBubble::Sending);
	_chatBubbleSystem->setMinimumWidth(500);

	QVBoxLayout *chatLayout = new QVBoxLayout();
	chatLayout->setSpacing(4);
	chatLayout->addWidget(_chatBubbleLeft);
	chatLayout->addWidget(_chatBubbleRight);
	chatLayout->addWidget(_chatBubbleSystem);

	ElaScrollPageArea *chatArea = new ElaScrollPageArea(this);
	chatArea->setFixedHeight(380);
	QVBoxLayout *chatAreaLayout = new QVBoxLayout(chatArea);
	chatAreaLayout->addWidget(chatTitle);
	chatAreaLayout->addLayout(chatLayout);

	// ========== ElaDashboardGauge 示例 ==========
	ElaText *gaugeTitle = new ElaText("ElaDashboardGauge", this);
	gaugeTitle->setTextPixelSize(18);

	_gauge = new ElaDashboardGauge(this);
	_gauge->setMinimum(0);
	_gauge->setMaximum(200);
	_gauge->setValue(80);
	_gauge->setTitle("速度");
	_gauge->setUnit("km/h");
	_gauge->setValuePixelSize(16);
	_gauge->setTickWarningPercent(0.7);
	_gauge->setDecimals(2);

	_gaugeSmall = new ElaDashboardGauge(this);
	_gaugeSmall->setFixedSize(180, 180);
	_gaugeSmall->setMinimum(0);
	_gaugeSmall->setMaximum(100);
	_gaugeSmall->setValue(50);
	_gaugeSmall->setTitle("CPU");
	_gaugeSmall->setUnit("%");
	_gaugeSmall->setValuePixelSize(24);
	_gaugeSmall->setTickWarningPercent(0.6);
	_gaugeSmall->setDecimals(2);

	ElaSlider *gaugeSlider = new ElaSlider(this);
	gaugeSlider->setRange(static_cast<int>(_gauge->getMinimum()), static_cast<int>(_gauge->getMaximum()));
	gaugeSlider->setValue(72);
	gaugeSlider->setFixedWidth(260);
	ElaText *gaugeValueLabel = new ElaText("80", this);
	gaugeValueLabel->setTextPixelSize(12);
	connect(gaugeSlider, &ElaSlider::valueChanged, this, [=](int v)
	{
		_gauge->setValue(v);
		gaugeValueLabel->setText(QString::number(v));
	});

	QHBoxLayout *gaugeWidgetLayout = new QHBoxLayout();
	gaugeWidgetLayout->addWidget(_gauge);
	gaugeWidgetLayout->addSpacing(20);
	gaugeWidgetLayout->addWidget(_gaugeSmall);
	gaugeWidgetLayout->addStretch();

	QHBoxLayout *gaugeControlLayout = new QHBoxLayout();
	gaugeControlLayout->addWidget(new ElaText("值控制:", 13, this));
	gaugeControlLayout->addWidget(gaugeSlider);
	gaugeControlLayout->addWidget(gaugeValueLabel);
	gaugeControlLayout->addStretch();

	ElaScrollPageArea *gaugeArea = new ElaScrollPageArea(this);
	gaugeArea->setFixedHeight(340);
	QVBoxLayout *gaugeAreaLayout = new QVBoxLayout(gaugeArea);
	gaugeAreaLayout->addWidget(gaugeTitle);
	gaugeAreaLayout->addLayout(gaugeWidgetLayout);
	gaugeAreaLayout->addLayout(gaugeControlLayout);

	// ========== ElaTerminalWidget 示例 ==========
	ElaText *terminalTitle = new ElaText("ElaTerminalWidget", this);
	terminalTitle->setTextPixelSize(18);

	_terminal = new ElaTerminalWidget(this);
	_terminal->setFixedHeight(300);
	_terminal->setPrompt("ela> ");
	_terminal->appendSuccess("ElaTerminalWidget 已启动");
	_terminal->appendOutput("输入 help 查看可用命令，输入 clear 清屏。");

	connect(_terminal, &ElaTerminalWidget::commandSubmitted, this, [=](const QString &cmd)
	{
		if (cmd == "help")
		{
			_terminal->appendOutput("可用命令: help, clear, version, echo <text>, gauge <value>");
		}
		else if (cmd == "clear")
		{
			_terminal->clear();
		}
		else if (cmd == "version")
		{
			_terminal->appendOutput(QString("ElaWidgetTools %1").arg(ElaDefVersion));
		}
		else if (cmd.startsWith("echo "))
		{
			_terminal->appendOutput(cmd.mid(5));
		}
		else if (cmd.startsWith("gauge "))
		{
			bool ok;
			double val = cmd.mid(6).toDouble(&ok);
			if (ok)
			{
				qreal clamped = qBound(_gauge->getMinimum(), val, _gauge->getMaximum());
				_gauge->setValue(clamped);
				gaugeSlider->setValue(static_cast<int>(clamped));
				if (val < _gauge->getMinimum())
				{
					_terminal->appendOutput(QString("值 %1 低于最小值，已设置为 %2").arg(val).arg(clamped), QColor(0xF7, 0x94, 0x0B));
				}
				else if (val > _gauge->getMaximum())
				{
					_terminal->appendOutput(QString("值 %1 超过最大值，已设置为 %2").arg(val).arg(clamped), QColor(0xF7, 0x94, 0x0B));
				}
				else
				{
					_terminal->appendSuccess(QString("仪表盘值已设置为 %1").arg(clamped));
				}
			}
			else
			{
				_terminal->appendError("无效数值");
			}
		}
		else
		{
			_terminal->appendError(QString("未知命令: %1").arg(cmd));
		}
	});

	ElaScrollPageArea *terminalArea = new ElaScrollPageArea(this);
	terminalArea->setFixedHeight(360);
	QVBoxLayout *terminalAreaLayout = new QVBoxLayout(terminalArea);
	terminalAreaLayout->addWidget(terminalTitle);
	terminalAreaLayout->addWidget(_terminal);

	// ========== 中心布局 ==========
	QVBoxLayout *c = new QVBoxLayout(centralWidget);
	c->setContentsMargins(0, 0, 0, 0);
	c->addWidget(chatArea);
	c->addWidget(gaugeArea);
	c->addWidget(terminalArea);
	addCentralWidget(centralWidget, true, false, 0);
}

T_NewComponents2::~T_NewComponents2()
{
}