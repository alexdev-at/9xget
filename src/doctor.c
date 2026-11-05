#include "doctor.h"

#include <stdio.h>
#include <windows.h>

doctor_result_t doctor_result;

bool run_system_check(void) {
    OSVERSIONINFO v;
    v.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&v);

    #ifndef SKIP_SYSTEM_CHECK
    if (v.dwPlatformId != VER_PLATFORM_WIN32_WINDOWS || (v.dwMajorVersion == 4 && v.dwMinorVersion == 90)) {
        fprintf(stderr, "Error: Windows version is not supported\n");
        return false;
    }
    #else
    printf("Skipping system check\n");
    #endif

    if (v.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) {
        if (v.dwMajorVersion == 4 && v.dwMinorVersion == 0) doctor_result.os_version = WINDOWS_95;
        else if (v.dwMajorVersion == 4 && v.dwMinorVersion == 10) doctor_result.os_version = WINDOWS_98;
        else doctor_result.os_version = WINDOWS_UNKNOWN;
    } else {
        doctor_result.os_version = WINDOWS_UNKNOWN;
    }

    return true;
}
