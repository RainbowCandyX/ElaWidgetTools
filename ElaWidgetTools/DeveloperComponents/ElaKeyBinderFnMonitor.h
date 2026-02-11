#ifndef ELAWIDGETTOOLS_SRC_DEVELOPERCOMPONENTS_ELAKEYBINDERFNMONITOR_H_
#define ELAWIDGETTOOLS_SRC_DEVELOPERCOMPONENTS_ELAKEYBINDERFNMONITOR_H_

#include <functional>
#include <qglobal.h>

#ifdef Q_OS_MACOS
void elaStartFnKeyMonitor(std::function<void()> onFnKeyPressed);
void elaStopFnKeyMonitor();
#else
inline void elaStartFnKeyMonitor(std::function<void()>) {}
inline void elaStopFnKeyMonitor() {}
#endif

#endif // ELAWIDGETTOOLS_SRC_DEVELOPERCOMPONENTS_ELAKEYBINDERFNMONITOR_H_
