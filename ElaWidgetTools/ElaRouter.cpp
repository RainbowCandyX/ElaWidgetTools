#include "ElaRouter.h"

#include "ElaNavigationRouter.h"
#include "ElaWindow.h"
#include "private/ElaRouterPrivate.h"

Q_SINGLETON_CREATE_CPP(ElaRouter)

ElaRouter::ElaRouter(QObject *parent) : QObject{parent}, d_ptr(new ElaRouterPrivate())
{
	Q_D(ElaRouter);
	d->q_ptr = this;
}

ElaRouter::~ElaRouter()
{
}

void ElaRouter::bindWindow(ElaWindow *window)
{
	Q_D(ElaRouter);
	d->_window = window;
}

ElaWindow *ElaRouter::getBoundWindow() const
{
	return d_ptr->_window;
}

ElaRouterType::NavigationResult ElaRouter::addRoute(const ElaRouteConfig &config)
{
	Q_D(ElaRouter);
	ElaRouterType::NavigationResult result = d->addRouteToTree(config);
	if (result == ElaRouterType::Success)
	{
		Q_EMIT routeTableChanged();
	}
	return result;
}

ElaRouterType::NavigationResult ElaRouter::addRoutes(const QVector<ElaRouteConfig> &configs)
{
	Q_D(ElaRouter);
	for (const auto &config: configs)
	{
		ElaRouterType::NavigationResult result = d->addRouteToTree(config);
		if (result != ElaRouterType::Success)
		{
			return result;
		}
	}
	Q_EMIT routeTableChanged();
	return ElaRouterType::Success;
}

ElaRouterType::NavigationResult ElaRouter::addDynamicRoute(const QString &parentPath, const ElaRouteConfig &config)
{
	Q_D(ElaRouter);
	QString normalizedParent = ElaRouterPrivate::normalizePath(parentPath);
	if (!d->_pathIndex.contains(normalizedParent))
	{
		return ElaRouterType::ParentRouteNotFound;
	}
	ElaRouterType::NavigationResult result = d->addRouteToTree(config, normalizedParent);
	if (result == ElaRouterType::Success)
	{
		ElaRouteNode *parentNode = d->_pathIndex[normalizedParent];
		if (d->_window && parentNode->isRegisteredInWindow)
		{
			QString fullPath = ElaRouterPrivate::joinPath(normalizedParent, config.path);
			ElaRouteNode *newNode = d->_pathIndex.value(fullPath);
			if (newNode)
			{
				d->installNodeTree(newNode, parentNode->expanderKey);
			}
		}
		Q_EMIT routeTableChanged();
	}
	return result;
}

ElaRouterType::NavigationResult ElaRouter::removeRoute(const QString &path)
{
	Q_D(ElaRouter);
	QString normalizedPath = ElaRouterPrivate::normalizePath(path);
	if (!d->_pathIndex.contains(normalizedPath))
	{
		return ElaRouterType::RouteNotFound;
	}
	ElaRouteNode *node = d->_pathIndex[normalizedPath];

	QVector<QString> pathsToRemove;
	std::function<void(ElaRouteNode *)> collectPaths = [&](ElaRouteNode *n)
	{
		pathsToRemove.append(n->path);
		for (auto *child: n->children)
		{
			collectPaths(child);
		}
	};
	collectPaths(node);

	for (const auto &p: pathsToRemove)
	{
		d->_pathIndex.remove(p);
	}

	if (node->isRegisteredInWindow && d->_window)
	{
		d->_window->removeNavigationNode(node->pageKey);
	}

	if (node->parent)
	{
		node->parent->children.removeOne(node);
	}
	else
	{
		d->_rootNodes.removeOne(node);
	}
	delete node;

	Q_EMIT routeTableChanged();
	return ElaRouterType::Success;
}

bool ElaRouter::hasRoute(const QString &path) const
{
	return d_ptr->_pathIndex.contains(ElaRouterPrivate::normalizePath(path));
}

QStringList ElaRouter::getRoutePaths() const
{
	return d_ptr->_pathIndex.keys();
}

QVariantMap ElaRouter::getRouteMeta(const QString &path) const
{
	QString normalizedPath = ElaRouterPrivate::normalizePath(path);
	if (d_ptr->_pathIndex.contains(normalizedPath))
	{
		return d_ptr->_pathIndex[normalizedPath]->meta;
	}
	return {};
}

ElaRouterType::NavigationResult ElaRouter::push(const QString &path, const QVariantMap &params)
{
	Q_D(ElaRouter);
	return d->doNavigation(path, params, true);
}

ElaRouterType::NavigationResult ElaRouter::replace(const QString &path, const QVariantMap &params)
{
	Q_D(ElaRouter);
	return d->doNavigation(path, params, false);
}

void ElaRouter::back()
{
	Q_D(ElaRouter);
	if (d->_history.isEmpty())
	{
		return;
	}
	bool isOffRoute = !d->_isOnRouterPage();
	int targetIndex = isOffRoute ? d->_historyIndex : d->_historyIndex - 1;
	if (targetIndex < 0)
	{
		return;
	}
	d->_historyIndex = targetIndex;
	const auto& entry = d->_history[d->_historyIndex];
	d->_isNavigatingHistory = true;
	d->doNavigation(entry.path, entry.params, false);
	d->_isNavigatingHistory = false;
}

void ElaRouter::forward()
{
	Q_D(ElaRouter);
	if (d->_history.isEmpty())
	{
		return;
	}
	bool isOffRoute = !d->_isOnRouterPage();
	int targetIndex = isOffRoute ? d->_historyIndex : d->_historyIndex + 1;
	if (targetIndex >= d->_history.size())
	{
		return;
	}
	d->_historyIndex = targetIndex;
	const auto& entry = d->_history[d->_historyIndex];
	d->_isNavigatingHistory = true;
	d->doNavigation(entry.path, entry.params, false);
	d->_isNavigatingHistory = false;
}

QString ElaRouter::getCurrentPath() const
{
	return d_ptr->_currentPath;
}

QVariantMap ElaRouter::getCurrentParams() const
{
	return d_ptr->_currentParams;
}

int ElaRouter::beforeEach(const ElaRouteGuard &guard)
{
	Q_D(ElaRouter);
	int id = d->_nextGuardId++;
	d->_beforeGuards.append({id, guard});
	return id;
}

int ElaRouter::afterEach(const ElaRouteAfterHook &hook)
{
	Q_D(ElaRouter);
	int id = d->_nextGuardId++;
	d->_afterHooks.append({id, hook});
	return id;
}

void ElaRouter::removeBeforeGuard(int guardId)
{
	Q_D(ElaRouter);
	for (int i = 0; i < d->_beforeGuards.size(); ++i)
	{
		if (d->_beforeGuards[i].id == guardId)
		{
			d->_beforeGuards.removeAt(i);
			return;
		}
	}
}

void ElaRouter::removeAfterHook(int hookId)
{
	Q_D(ElaRouter);
	for (int i = 0; i < d->_afterHooks.size(); ++i)
	{
		if (d->_afterHooks[i].id == hookId)
		{
			d->_afterHooks.removeAt(i);
			return;
		}
	}
}

void ElaRouter::setRouteBeforeEnter(const QString &path, const ElaRouteGuard &guard)
{
	Q_D(ElaRouter);
	QString normalizedPath = ElaRouterPrivate::normalizePath(path);
	if (d->_pathIndex.contains(normalizedPath))
	{
		d->_pathIndex[normalizedPath]->beforeEnter = guard;
	}
}

void ElaRouter::installRoutes()
{
	Q_D(ElaRouter);
	if (!d->_window)
	{
		return;
	}
	for (auto *node: d->_rootNodes)
	{
		d->installNodeTree(node);
	}
}

void ElaRouter::resetRouter()
{
	Q_D(ElaRouter);
	qDeleteAll(d->_rootNodes);
	d->_rootNodes.clear();
	d->_pathIndex.clear();
	d->_currentPath.clear();
	d->_currentParams.clear();
	d->_beforeGuards.clear();
	d->_afterHooks.clear();
	Q_EMIT routeTableChanged();
}