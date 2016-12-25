#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION
#endif

#include <stdint.h>
#include <stdio.h>
#include <windows.h>

uint32_t SystemIdleTime(void) {
	LASTINPUTINFO lif;
	lif.cbSize = sizeof(lif);
	if (!GetLastInputInfo(&lif)) return -1;
	uint64_t tickCount = GetTickCount64();
	uint32_t IdleTime = (uint32_t)((tickCount - (uint64_t)lif.dwTime));
	return IdleTime;
}

char* SystemActiveWindow(void)  {
    HWND foreground;
    char wnd_title[256];
    foreground = GetForegroundWindow();
    if (foreground)
    {
      GetWindowText(foreground, wnd_title, 256);
    }
    return wnd_title;
  }
