#ifndef BRIG_EVENTDISPATCHER_MAC_H
#define BRIG_EVENTDISPATCHER_MAC_H

#ifdef __MACOSX_CORE__
void prepareMacWindowSystem();
void respondMacWindowSystem();
bool isPowerSaveMode();
#endif

#endif
