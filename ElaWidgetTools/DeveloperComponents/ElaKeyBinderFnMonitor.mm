#include "ElaKeyBinderFnMonitor.h"
#ifdef Q_OS_MACOS
#import <Cocoa/Cocoa.h>

static id s_fnKeyMonitor = nil;
static bool s_fnKeyWasDown = false;

void elaStartFnKeyMonitor(std::function<void()> onFnKeyPressed)
{
    elaStopFnKeyMonitor();
    s_fnKeyWasDown = false;
    s_fnKeyMonitor = [NSEvent addLocalMonitorForEventsMatchingMask:NSEventMaskFlagsChanged
                                                          handler:^NSEvent*(NSEvent* event) {
        if (event.keyCode == 0x3F)
        {
            bool fnDown = (event.modifierFlags & NSEventModifierFlagFunction) != 0;
            if (fnDown && !s_fnKeyWasDown)
            {
                onFnKeyPressed();
            }
            s_fnKeyWasDown = fnDown;
        }
        return event;
    }];
}

void elaStopFnKeyMonitor()
{
    if (s_fnKeyMonitor)
    {
        [NSEvent removeMonitor:s_fnKeyMonitor];
        s_fnKeyMonitor = nil;
    }
    s_fnKeyWasDown = false;
}
#endif
