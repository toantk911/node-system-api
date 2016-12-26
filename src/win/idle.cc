#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION
#endif

#include <stdint.h>
#include <stdio.h>
#include <windows.h>

uint32_t SystemIdleTime(void)
{
  LASTINPUTINFO lif;
  lif.cbSize = sizeof(lif);
  if (!GetLastInputInfo(&lif))
    return -1;
  uint64_t tickCount = GetTickCount64();
  uint32_t IdleTime = (uint32_t)((tickCount - (uint64_t)lif.dwTime));
  return IdleTime;
}

char *SystemActiveWindow(void)
{
  // Get active app name
  DWORD maxPath = 1024;
  TCHAR buffer[1024];

  HWND fg = GetForegroundWindow();
  if (fg)
  {
    DWORD pid;
    GetWindowThreadProcessId(fg, &pid);
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
    if (hProcess)
    {
      BOOL ret = QueryFullProcessImageName(hProcess, 0, buffer, &maxPath);
      CloseHandle(hProcess);
    }
  }
  return buffer;
}
