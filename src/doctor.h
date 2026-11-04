#pragma once

#include <stdbool.h>

typedef enum doctor_os_version {
    WINDOWS_95,
    WINDOWS_98,
    WINDOWS_UNKNOWN
} doctor_os_version_e;

typedef struct doctor_result {
    doctor_os_version_e os_version;
} doctor_result_t;

extern doctor_result_t doctor_result;

bool run_system_check(void);
