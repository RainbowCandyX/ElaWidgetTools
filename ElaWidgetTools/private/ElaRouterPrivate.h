#ifndef ELAROUTERPRIVATE_H
#define ELAROUTERPRIVATE_H

#include <QMap>
#include <QObject>
#include <QVector>
#include <functional>

#include "ElaDef.h"
#include "ElaRouter.h"

class ElaWindow;

struct ElaRouteNode
{
	QString path;
	QString title;
	ElaIconType::IconName icon = ElaIconType::None;
	QVariantMap meta;
	std::function<QWidget*()> factory;
	int keyPoints = 0;

	ElaRouteNode* parent = nullptr;
	QVector<ElaRouteNode*> children;

	QWidget* widget = nullptr;
	QString pageKey;
	bool isRegisteredInWindow = false;
	QString expanderKey;

	ElaRouteGuard beforeEnter;

	~ElaRouteNode()
	{
		qDeleteAll(children);
	}
};

struct ElaGuardEntry
{
	int id;
	ElaRouteGuard guard;
};

struct ElaAfterHookEntry
{
	int id;
	ElaRouteAfterHook hook;
};

class ElaRouterPrivate : public QObject
{
	Q_OBJECT
	Q_D_CREATE(ElaRouter)
public:
	explicit ElaRouterPrivate(QObject* parent = nullptr);
	~ElaRouterPrivate() override;

private:
	friend class ElaRouter;

	ElaWindow* _window = nullptr;

	QVector<ElaRouteNode*> _rootNodes;
	QMap<QString, ElaRouteNode*> _pathIndex;

	QString _currentPath;
	QVariantMap _currentParams;

	struct HistoryEntry
	{
		QString path;
		QVariantMap params;
	};
	QVector<HistoryEntry> _history;
	int _historyIndex = -1;
	bool _isNavigatingHistory = false;

	QVector<ElaGuardEntry> _beforeGuards;
	QVector<ElaAfterHookEntry> _afterHooks;
	int _nextGuardId = 1;

	static QString normalizePath(const QString& path);
	static QString joinPath(const QString& parent, const QString& child);

	bool _isOnRouterPage() const;
	ElaRouterType::NavigationResult addRouteToTree(const ElaRouteConfig& config, const QString& parentPathPrefix = QString());
	bool runBeforeGuards(const QString& to, const QString& from, const QVariantMap& params, ElaRouteNode* toNode);
	void runAfterHooks(const QString& to, const QString& from, const QVariantMap& params);
	QWidget* ensureWidget(ElaRouteNode* node);
	bool registerNodeInWindow(ElaRouteNode* node);
	void installNodeTree(ElaRouteNode* node, const QString& parentExpanderKey = QString());
	ElaRouterType::NavigationResult doNavigation(const QString& path, const QVariantMap& params, bool pushToHistory);
};

#endif // ELAROUTERPRIVATE_H
