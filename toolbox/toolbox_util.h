#ifndef TOOLBOX_UTIL_INCLUDED
#define TOOLBOX_UTIL_INCLUDED

#include "toolbox_defines.h"
#include "toolbox_types.h"

/* HEADER_ONLY_ASSEMBLER_START */

#ifdef __cplusplus
extern "C" {
#endif

/// @brief returns the filename.extension of a given path
TOOLBOX_API bool filename_from_path(const char* path, char* outBuffer, unsigned long long bufferSize);

/// @brief returns the file or path's name extension (including the dot)
TOOLBOX_API bool extension_from_path(const char* path, char* outBuffer, unsigned long long bufferSize);

/// @brief returns just the filename without extension
TOOLBOX_API bool basename_from_path(const char* path, char* outBuffer, unsigned long long bufferSize);

/// @brief returns the directory path (everything before the last slash)
TOOLBOX_API bool dirname_from_path(const char* path, char* outBuffer, unsigned long long bufferSize);

#ifdef __cplusplus
}
#endif

/* HEADER_ONLY_ASSEMBLER_STOP */

#endif // TOOLBOX_UTIL_INCLUDED