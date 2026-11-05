#pragma once

#include <stdbool.h>
#include <stddef.h>

bool path_get_app_dir(char* out, size_t max_len);
bool path_get_packages_dir(char* out, size_t max_len);
bool path_get_index_path(char* out, size_t max_len);
bool path_get_installed_path(char* out, size_t max_len);

