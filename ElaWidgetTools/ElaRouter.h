#ifndef ELAROUTER_H
#define ELAROUTER_H

#include <QObject>
#include <QVariantMap>
#include <functional>

#include "ElaDef.h"
#include "ElaProperty.h"
#include "ElaSingleton.h"

class ElaWindow;
class ElaRouterPrivate;

struct ELA_EXPORT ElaRouteConfig
{
	QString path;
	QString title;
	ElaIconType::IconName icon = ElaIconType::None;
	QVariantMap meta;
	std::function<QWidget*()> factory;
	QVector<ElaRouteConfig> children;
	int keyPoints = 0;
};

using ElaRouteGuard = std::function<bool(const QString& to, const QString& from, const QVariantMap& params)>;
using ElaRouteAfterHook = std::function<void(const QString& to, const QString& from, const QVariantMap& params)>;

class ELA_EXPORT ElaRouter : public QObject
{
	Q_OBJECT
	Q_SINGLETON_CREATE_H(ElaRouter)

private:
	explicit ElaRouter(QObject* parent = nullptr);
	~ElaRouter() override;

public:
	void bindWindow(ElaWindow* window);
	ElaWindow* getBoundWindow() const;

	ElaRouterType::NavigationResult addRoute(const ElaRouteConfig& config);
	ElaRouterType::NavigationResult addRoutes(const QVector<ElaRouteConfig>& configs);
	ElaRouterType::NavigationResult addDynamicRoute(const QString& parentPath, const ElaRouteConfig& config);
	ElaRouterType::NavigationResult removeRoute(const QString& path);

	bool hasRoute(const QString& path) const;
	QStringList getRoutePaths() const;
	QVariantMap getRouteMeta(const QString& path) const;

	ElaRouterType::NavigationResult push(const QString& path, const QVariantMap& params = {});
	ElaRouterType::NavigationResult replace(const QString& path, const QVariantMap& params = {});
	void back();
	void forward();

	QString getCurrentPath() const;
	QVariantMap getCurrentParams() const;

	int beforeEach(const ElaRouteGuard& guard);
	int afterEach(const ElaRouteAfterHook& hook);
	void removeBeforeGuard(int guardId);
	void removeAfterHook(int hookId);

	void setRouteBeforeEnter(const QString& path, const ElaRouteGuard& guard);

	void installRoutes();
	void resetRouter();

Q_SIGNALS:
	Q_SIGNAL void routeChanged(const QString& path, const QVariantMap& params);
	Q_SIGNAL void navigationBlocked(const QString& path);
	Q_SIGNAL void routeTableChanged();

private:
	QScopedPointer<ElaRouterPrivate> d_ptr;
	Q_DISABLE_COPY(ElaRouter)
	Q_DECLARE_PRIVATE(ElaRouter)
};

#define eRouter ElaRouter::getInstance()

#endif // ELAROUTER_H
