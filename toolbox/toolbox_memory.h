#ifndef TOOLBOX_MEMORY_INCLUDED
#define TOOLBOX_MEMORY_INCLUDED

#include "toolbox_defines.h"
#include "toolbox_types.h"

/* HEADER_ONLY_ASSEMBLER_START */

#ifdef __cplusplus
extern "C" {
#endif

/// @brief custom malloc call, overrides default malloc with an optional custom function provided by the user
TOOLBOX_API void* toolbox_custom_malloc(const toolbox_memfuncs* fun, unsigned long long size);

/// @brief custom calloc call, overrides default calloc with an optional custom function provided by the user
TOOLBOX_API void* toolbox_custom_calloc(const toolbox_memfuncs* fun, unsigned long long num, unsigned long long size);

/// @brief custom free call, overrides default free with an optional custom function provided by the user
TOOLBOX_API void toolbox_custom_free(const toolbox_memfuncs* fun, void* ptr);

/// @brief custom realloc call, overrides default realloc with an optional custom function provided by the user
TOOLBOX_API void* toolbox_custom_realloc(const toolbox_memfuncs* fun, void* ptr, unsigned long long newSize);

#ifdef __cplusplus
}
#endif

/* HEADER_ONLY_ASSEMBLER_STOP */

#endif // TOOLBOX_MEMORY_INCLUDED