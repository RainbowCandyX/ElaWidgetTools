#include "ElaRouterPrivate.h"

#include <QWidget>

#include "ElaWindow.h"

ElaRouterPrivate::ElaRouterPrivate(QObject *parent)
	: QObject{parent}
{
}

ElaRouterPrivate::~ElaRouterPrivate()
{
	qDeleteAll(_rootNodes);
}

QString ElaRouterPrivate::normalizePath(const QString &path)
{
	QString p = path;
	if (!p.startsWith('/'))
	{
		p.prepend('/');
	}
	while (p.length() > 1 && p.endsWith('/'))
	{
		p.chop(1);
	}
	return p;
}

QString ElaRouterPrivate::joinPath(const QString &parent, const QString &child)
{
	QString c = child;
	while (c.startsWith('/'))
	{
		c = c.mid(1);
	}
	if (parent == "/")
	{
		return normalizePath("/" + c);
	}
	return normalizePath(parent + "/" + c);
}

bool ElaRouterPrivate::_isOnRouterPage() const
{
	if (!_window || _history.isEmpty() || _historyIndex < 0)
	{
		return false;
	}
	QString currentPageKey = _window->getCurrentNavigationPageKey();
	const auto &entry = _history[_historyIndex];
	if (_pathIndex.contains(entry.path))
	{
		ElaRouteNode *node = _pathIndex[entry.path];
		return node->pageKey == currentPageKey;
	}
	return false;
}

ElaRouterType::NavigationResult ElaRouterPrivate::addRouteToTree(const ElaRouteConfig &config, const QString &parentPathPrefix)
{
	QString fullPath;
	if (parentPathPrefix.isEmpty())
	{
		fullPath = normalizePath(config.path);
	}
	else
	{
		fullPath = joinPath(parentPathPrefix, config.path);
	}

	if (_pathIndex.contains(fullPath))
	{
		return ElaRouterType::RouteAlreadyExists;
	}

	ElaRouteNode *node = new ElaRouteNode();
	node->path = fullPath;
	node->title = config.title;
	node->icon = config.icon;
	node->meta = config.meta;
	node->factory = config.factory;
	node->keyPoints = config.keyPoints;

	if (!parentPathPrefix.isEmpty() && _pathIndex.contains(parentPathPrefix))
	{
		ElaRouteNode *parentNode = _pathIndex[parentPathPrefix];
		node->parent = parentNode;
		parentNode->children.append(node);
	}
	else
	{
		_rootNodes.append(node);
	}

	_pathIndex.insert(fullPath, node);

	for (const auto &child: config.children)
	{
		ElaRouterType::NavigationResult result = addRouteToTree(child, fullPath);
		if (result != ElaRouterType::Success)
		{
			return result;
		}
	}
	return ElaRouterType::Success;
}

bool ElaRouterPrivate::runBeforeGuards(const QString &to, const QString &from, const QVariantMap &params, ElaRouteNode *toNode)
{
	for (const auto &entry: _beforeGuards)
	{
		if (!entry.guard(to, from, params))
		{
			return false;
		}
	}
	if (toNode && toNode->beforeEnter)
	{
		if (!toNode->beforeEnter(to, from, params))
		{
			return false;
		}
	}
	return true;
}

void ElaRouterPrivate::runAfterHooks(const QString &to, const QString &from, const QVariantMap &params)
{
	for (const auto &entry: _afterHooks)
	{
		entry.hook(to, from, params);
	}
}

QWidget *ElaRouterPrivate::ensureWidget(ElaRouteNode *node)
{
	if (node->widget)
	{
		bool keepAlive = node->meta.value("keepAlive", true).toBool();
		if (keepAlive)
		{
			return node->widget;
		}
		node->widget->deleteLater();
		node->widget = nullptr;
		node->isRegisteredInWindow = false;
	}
	if (node->factory)
	{
		node->widget = node->factory();
	}
	return node->widget;
}

bool ElaRouterPrivate::registerNodeInWindow(ElaRouteNode *node)
{
	if (!_window || node->isRegisteredInWindow)
	{
		return node->isRegisteredInWindow;
	}
	if (!node->widget)
	{
		return false;
	}

	ElaNavigationType::NodeResult result;
	if (node->parent && !node->parent->expanderKey.isEmpty())
	{
		result = _window->addPageNode(node->title, node->widget, node->parent->expanderKey, node->keyPoints, node->icon);
	}
	else
	{
		result = _window->addPageNode(node->title, node->widget, node->keyPoints, node->icon);
	}

	if (result == ElaNavigationType::Success)
	{
		node->pageKey = node->widget->property("ElaPageKey").toString();
		node->isRegisteredInWindow = true;
		return true;
	}
	return false;
}

void ElaRouterPrivate::installNodeTree(ElaRouteNode *node, const QString &parentExpanderKey)
{
	if (!_window)
	{
		return;
	}

	bool hasChildren = !node->children.isEmpty();

	if (hasChildren)
	{
		if (node->expanderKey.isEmpty())
		{
			QString expanderKey;
			if (parentExpanderKey.isEmpty())
			{
				_window->addExpanderNode(node->title, expanderKey, node->icon);
			}
			else
			{
				_window->addExpanderNode(node->title, expanderKey, parentExpanderKey, node->icon);
			}
			node->expanderKey = expanderKey;
		}

		for (auto *child: node->children)
		{
			installNodeTree(child, node->expanderKey);
		}
	}
	else if (!node->isRegisteredInWindow)
	{
		QWidget *widget = ensureWidget(node);
		if (widget)
		{
			ElaNavigationType::NodeResult result;
			if (parentExpanderKey.isEmpty())
			{
				result = _window->addPageNode(node->title,  widget, node->keyPoints, node->icon);
			}
			else
			{
				result = _window->addPageNode(node->title, widget, parentExpanderKey, node->keyPoints, node->icon);
			}
			if (result == ElaNavigationType::Success)
			{
				node->pageKey = widget->property("ElaPageKey").toString();
				node->isRegisteredInWindow = true;
			}
		}
	}
}

ElaRouterType::NavigationResult ElaRouterPrivate::doNavigation(const QString &path, const QVariantMap &params, bool pushToHistory)
{
	Q_Q(ElaRouter);
	if (!_window)
	{
		return ElaRouterType::WindowNotBound;
	}

	QString normalizedPath = normalizePath(path);
	if (!_pathIndex.contains(normalizedPath))
	{
		return ElaRouterType::RouteNotFound;
	}

	ElaRouteNode *node = _pathIndex[normalizedPath];

	if (!node->children.isEmpty())
	{
		return ElaRouterType::RouteNotFound;
	}

	QString fromPath = _currentPath;

	if (!runBeforeGuards(normalizedPath, fromPath, params, node))
	{
		Q_EMIT q->navigationBlocked(normalizedPath);
		return ElaRouterType::GuardRejected;
	}

	QWidget *widget = ensureWidget(node);
	if (!widget)
	{
		return ElaRouterType::FactoryFailed;
	}

	if (!node->isRegisteredInWindow)
	{
		if (!registerNodeInWindow(node))
		{
			return ElaRouterType::FactoryFailed;
		}
	}

	_window->navigation(node->pageKey);

	_currentPath = normalizedPath;
	_currentParams = params;

	if (pushToHistory && !_isNavigatingHistory)
	{
		if (_historyIndex < _history.size() - 1)
		{
			_history.resize(_historyIndex + 1);
		}
		_history.append({normalizedPath, params});
		_historyIndex = _history.size() - 1;
	}

	runAfterHooks(normalizedPath, fromPath, params);
	Q_EMIT q->routeChanged(normalizedPath, params);

	return ElaRouterType::Success;
}