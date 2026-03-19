#include "T_Router.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "ElaLineEdit.h"
#include "ElaPushButton.h"
#include "ElaRouter.h"
#include "ElaScrollPageArea.h"
#include "ElaPlainTextEdit.h"
#include "ElaText.h"
#include "ElaToggleSwitch.h"
#include "ElaToast.h"
#include "ElaWindow.h"

T_Router::T_Router(QWidget *parent) : T_BasePage(parent)
{
	setWindowTitle("ElaRouter");
	createCustomWidget("ElaRouter 声明式路由演示 - 路由守卫、动态路由、懒加载、导航拦截");

	QWidget *centralWidget = new QWidget(this);
	centralWidget->setWindowTitle("ElaRouter");
	QVBoxLayout *c = new QVBoxLayout(centralWidget);
	c->setContentsMargins(0, 0, 0, 0);

	// ========== 路由注册示例 ==========
	ElaScrollPageArea *registerArea = new ElaScrollPageArea(this);
	registerArea->setMinimumHeight(100);
	QVBoxLayout *registerLayout = new QVBoxLayout(registerArea);
	registerLayout->addWidget(new ElaText("路由注册 (addRoutes + installRoutes)", 15, this));

	ElaPushButton *registerBtn = new ElaPushButton("注册示例路由", this);
	registerBtn->setFixedSize(130, 38);

	ElaPushButton *installBtn = new ElaPushButton("安装到导航栏", this);
	installBtn->setFixedSize(130, 38);

	ElaText *registerStatus = new ElaText("", this);
	registerStatus->setTextPixelSize(13);
	registerStatus->setMinimumWidth(300);

	connect(registerBtn, &ElaPushButton::clicked, this, [=]()
	{
		ElaWindow *w = dynamic_cast<ElaWindow *>(window());
		if (!w)
		{
			return;
		}
		eRouter->bindWindow(w);

		ElaRouterType::NavigationResult result = eRouter->addRoutes(
			{
				{
					"/router-demo", "路由演示", ElaIconType::Route, {}, nullptr,
					{
						{
							"/page-a", "页面 A", ElaIconType::FileLines,
							{{"keepAlive", true}},
							[=]() -> QWidget *
							{
								QWidget *page = new QWidget();
								QVBoxLayout *lay = new QVBoxLayout(page);
								lay->addWidget(new ElaText("这是页面 A (keepAlive: true)", 16, page));
								lay->addWidget(new ElaText("通过 ElaRouter 懒加载创建，首次导航时才实例化", 13, page));
								lay->addStretch();
								return page;
							}
						},
						{
							"/page-b", "页面 B", ElaIconType::FilePen,
							{{"role", "user"}},
							[=]() -> QWidget *
							{
								QWidget *page = new QWidget();
								QVBoxLayout *lay = new QVBoxLayout(page);
								lay->addWidget(new ElaText("这是页面 B (role: user)", 16, page));
								lay->addWidget(new ElaText("需要 user 角色才能访问（守卫可拦截）", 13, page));
								lay->addStretch();
								return page;
							}
						},
						{
							"/page-c", "页面 C", ElaIconType::FileShield,
							{{"role", "admin"}},
							[=]() -> QWidget *
							{
								QWidget *page = new QWidget();
								QVBoxLayout *lay = new QVBoxLayout(page);
								lay->addWidget(new ElaText("这是页面 C (role: admin)", 16, page));
								lay->addWidget(new ElaText("需要 admin 角色才能访问", 13, page));
								lay->addStretch();
								return page;
							}
						},
					}
				},
			});
		if (result == ElaRouterType::Success)
		{
			registerStatus->setText("路由注册成功! 共 " + QString::number(eRouter->getRoutePaths().size()) + " 条路由");
			appendLog("addRoutes: 注册了 /router-demo 及 3 个子路由");
		}
		else if (result == ElaRouterType::RouteAlreadyExists)
		{
			registerStatus->setText("路由已存在，无需重复注册");
		}
	});

	connect(installBtn, &ElaPushButton::clicked, this, [=]()
	{
		if (!eRouter->getBoundWindow())
		{
			registerStatus->setText("请先注册路由");
			return;
		}
		eRouter->installRoutes();
		registerStatus->setText("已安装到导航栏，请查看左侧 \"路由演示\" 分组");
		appendLog("installRoutes: 路由已安装到 ElaWindow 导航栏");
	});

	QHBoxLayout *registerBtnLayout = new QHBoxLayout();
	registerBtnLayout->addWidget(registerBtn);
	registerBtnLayout->addWidget(installBtn);
	registerBtnLayout->addWidget(registerStatus);
	registerBtnLayout->addStretch();
	registerLayout->addLayout(registerBtnLayout);

	// ========== 路由导航示例 ==========
	ElaScrollPageArea *navArea = new ElaScrollPageArea(this);
	navArea->setMinimumHeight(100);
	QVBoxLayout *navLayout = new QVBoxLayout(navArea);
	navLayout->addWidget(new ElaText("路由导航 (push / replace / back / forward)", 15, this));

	ElaPushButton *pushABtn = new ElaPushButton("push 页面A", this);
	pushABtn->setFixedSize(110, 32);
	ElaPushButton *pushBBtn = new ElaPushButton("push 页面B", this);
	pushBBtn->setFixedSize(110, 32);
	ElaPushButton *pushCBtn = new ElaPushButton("push 页面C", this);
	pushCBtn->setFixedSize(110, 32);
	ElaPushButton *backBtn = new ElaPushButton("back", this);
	backBtn->setFixedSize(70, 32);
	ElaPushButton *forwardBtn = new ElaPushButton("forward", this);
	forwardBtn->setFixedSize(70, 32);

	connect(pushABtn, &ElaPushButton::clicked, this, [=]()
	{
		auto r = eRouter->push("/router-demo/page-a", {{"from", "demo"}});
		appendLog("push(/router-demo/page-a) → " + QString(r == ElaRouterType::Success ? "Success" : "Failed"));
	});
	connect(pushBBtn, &ElaPushButton::clicked, this, [=]()
	{
		auto r = eRouter->push("/router-demo/page-b");
		appendLog("push(/router-demo/page-b) → " + QString(r == ElaRouterType::Success ? "Success" : "Failed"));
	});
	connect(pushCBtn, &ElaPushButton::clicked, this, [=]()
	{
		auto r = eRouter->push("/router-demo/page-c");
		if (r == ElaRouterType::GuardRejected)
		{
			appendLog("push(/router-demo/page-c) → GuardRejected (需要 admin 角色)");
		}
		else
		{
			appendLog("push(/router-demo/page-c) → " + QString(r == ElaRouterType::Success ? "Success" : "Failed"));
		}
	});
	connect(backBtn, &ElaPushButton::clicked, this, [=]()
	{
		eRouter->back();
		appendLog("back()");
	});
	connect(forwardBtn, &ElaPushButton::clicked, this, [=]()
	{
		eRouter->forward();
		appendLog("forward()");
	});

	QHBoxLayout *navBtnLayout = new QHBoxLayout();
	navBtnLayout->addWidget(pushABtn);
	navBtnLayout->addWidget(pushBBtn);
	navBtnLayout->addWidget(pushCBtn);
	navBtnLayout->addSpacing(10);
	navBtnLayout->addWidget(backBtn);
	navBtnLayout->addWidget(forwardBtn);
	navBtnLayout->addStretch();
	navLayout->addLayout(navBtnLayout);

	// ========== 路由守卫示例 ==========
	ElaScrollPageArea *guardArea = new ElaScrollPageArea(this);
	guardArea->setMinimumHeight(100);
	QVBoxLayout *guardLayout = new QVBoxLayout(guardArea);
	guardLayout->addWidget(new ElaText("路由守卫 (beforeEach)", 15, this));

	ElaToggleSwitch *guardSwitch = new ElaToggleSwitch(this);
	ElaText *guardDesc = new ElaText("关闭时：拦截 role=admin 的路由 (页面C)", this);
	guardDesc->setTextPixelSize(13);
	guardDesc->setMinimumWidth(300);

	int *guardId = new int(-1);

	auto updateGuard = [=](bool isAdmin)
	{
		if (*guardId >= 0)
		{
			eRouter->removeBeforeGuard(*guardId);
			*guardId = -1;
		}
		if (!isAdmin)
		{
			*guardId = eRouter->beforeEach([=](const QString &to, const QString &, const QVariantMap &) -> bool
			{
				QVariantMap meta = eRouter->getRouteMeta(to);
				if (meta.value("role").toString() == "admin")
				{
					ElaToast::warning("权限不足: 需要 admin 角色", 2000, window());
					return false;
				}
				return true;
			});
			appendLog("beforeEach: 已启用角色守卫 (拦截 admin 路由)");
		}
		else
		{
			appendLog("beforeEach: 已移除角色守卫 (允许所有路由)");
		}
		guardDesc->setText(isAdmin ? "开启时：允许所有路由" : "关闭时：拦截 role=admin 的路由 (页面C)");
	};

	connect(guardSwitch, &ElaToggleSwitch::toggled, this, updateGuard);
	updateGuard(false);

	QHBoxLayout *guardBtnLayout = new QHBoxLayout();
	ElaText *adminLabel = new ElaText("模拟 Admin 身份", 13, this);
	adminLabel->setMinimumWidth(120);
	guardBtnLayout->addWidget(adminLabel);
	guardBtnLayout->addWidget(guardSwitch);
	guardBtnLayout->addSpacing(10);
	guardBtnLayout->addWidget(guardDesc);
	guardBtnLayout->addStretch();
	guardLayout->addLayout(guardBtnLayout);

	// ========== 动态路由示例 ==========
	ElaScrollPageArea *dynamicArea = new ElaScrollPageArea(this);
	dynamicArea->setMinimumHeight(100);
	QVBoxLayout *dynamicLayout = new QVBoxLayout(dynamicArea);
	ElaText *dynamicTitle = new ElaText("动态路由 (addDynamicRoute / removeRoute)", 15, this);
	dynamicTitle->setMinimumWidth(350);
	dynamicLayout->addWidget(dynamicTitle);

	ElaLineEdit *dynamicNameEdit = new ElaLineEdit(this);
	dynamicNameEdit->setPlaceholderText("输入路由名称 (如: page-d)");
	dynamicNameEdit->setFixedSize(200, 35);

	ElaPushButton *addDynamicBtn = new ElaPushButton("添加动态路由", this);
	addDynamicBtn->setFixedSize(120, 32);
	ElaPushButton *removeDynamicBtn = new ElaPushButton("移除动态路由", this);
	removeDynamicBtn->setFixedSize(120, 32);

	ElaText *dynamicStatus = new ElaText("", this);
	dynamicStatus->setTextPixelSize(13);
	dynamicStatus->setMinimumWidth(300);

	connect(addDynamicBtn, &ElaPushButton::clicked, this, [=]()
	{
		QString name = dynamicNameEdit->text().trimmed();
		if (name.isEmpty())
		{
			name = "page-d";
		}
		QString path = "/" + name;
		auto r = eRouter->addDynamicRoute("/router-demo",
		                                  {
			                                  path, name, ElaIconType::FileDoc, {}, [=]() -> QWidget *
			                                  {
				                                  QWidget *page = new QWidget();
				                                  QVBoxLayout *lay = new QVBoxLayout(page);
				                                  lay->addWidget(new ElaText("动态页面: " + name, 16, page));
				                                  lay->addWidget(new ElaText("通过 addDynamicRoute 在运行时添加", 13, page));
				                                  lay->addStretch();
				                                  return page;
			                                  }
		                                  });
		if (r == ElaRouterType::Success)
		{
			dynamicStatus->setText("已添加: /router-demo" + path);
			appendLog("addDynamicRoute: /router-demo" + path);
		}
		else
		{
			dynamicStatus->setText("添加失败 (可能已存在或父路由不存在)");
		}
	});

	connect(removeDynamicBtn, &ElaPushButton::clicked, this, [=]()
	{
		QString name = dynamicNameEdit->text().trimmed();
		if (name.isEmpty())
		{
			name = "page-d";
		}
		QString fullPath = "/router-demo/" + name;
		auto r = eRouter->removeRoute(fullPath);
		if (r == ElaRouterType::Success)
		{
			dynamicStatus->setText("已移除: " + fullPath);
			appendLog("removeRoute: " + fullPath);
		}
		else
		{
			dynamicStatus->setText("移除失败 (路由不存在)");
		}
	});

	QHBoxLayout *dynamicBtnLayout = new QHBoxLayout();
	dynamicBtnLayout->addWidget(dynamicNameEdit);
	dynamicBtnLayout->addWidget(addDynamicBtn);
	dynamicBtnLayout->addWidget(removeDynamicBtn);
	dynamicBtnLayout->addWidget(dynamicStatus);
	dynamicBtnLayout->addStretch();
	dynamicLayout->addLayout(dynamicBtnLayout);

	// ========== 路由状态 & 日志 ==========
	ElaScrollPageArea *logArea = new ElaScrollPageArea(this);
	logArea->setFixedHeight(200);
	QVBoxLayout *logLayout = new QVBoxLayout(logArea);

	QHBoxLayout *logHeader = new QHBoxLayout();
	logHeader->addWidget(new ElaText("路由日志", 15, this));

	ElaText *currentPathText = new ElaText("当前路由: (无)", this);
	currentPathText->setTextPixelSize(13);
	currentPathText->setMinimumWidth(400);
	logHeader->addSpacing(20);
	logHeader->addWidget(currentPathText);
	logHeader->addStretch();

	ElaPushButton *clearLogBtn = new ElaPushButton("清除", this);
	clearLogBtn->setFixedSize(60, 28);
	logHeader->addWidget(clearLogBtn);

	logLayout->addLayout(logHeader);

	_logEdit = new ElaPlainTextEdit(this);
	_logEdit->setReadOnly(true);
	logLayout->addWidget(_logEdit);

	connect(clearLogBtn, &ElaPushButton::clicked, this, [=]()
	{
		_logEdit->clear();
	});

	connect(eRouter, &ElaRouter::routeChanged, this, [=](const QString &path, const QVariantMap &params)
	{
		QString paramStr;
		if (!params.isEmpty())
		{
			QStringList parts;
			for (auto it = params.begin(); it != params.end(); ++it)
			{
				parts.append(it.key() + "=" + it.value().toString());
			}
			paramStr = " {" + parts.join(", ") + "}";
		}
		currentPathText->setText("当前路由: " + path + paramStr);
	});

	connect(eRouter, &ElaRouter::navigationBlocked, this, [=](const QString &path)
	{
		appendLog("BLOCKED: " + path);
	});

	// ========== 中心布局 ==========
	c->addWidget(registerArea);
	c->addWidget(navArea);
	c->addWidget(guardArea);
	c->addWidget(dynamicArea);
	c->addWidget(logArea);
	c->addStretch();
	addCentralWidget(centralWidget, true, false, 0);
}

T_Router::~T_Router()
{
}

void T_Router::appendLog(const QString &text)
{
	if (!_logEdit)
	{
		return;
	}
	_logEdit->appendPlainText("> " + text);
}