#include "doctor.h"

#include <stdio.h>
#include <windows.h>

bool run_system_check(void) {
    OSVERSIONINFO v;
    v.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&v);

    if (v.dwPlatformId != VER_PLATFORM_WIN32_WINDOWS || (v.dwMajorVersion == 4 && v.dwMinorVersion == 90)) {
        fprintf(stderr, "Error: Windows version is not supported");
        return false;
    }

    return true;
}
