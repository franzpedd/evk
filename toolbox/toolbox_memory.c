#include "toolbox_memory.h"
#include <stdlib.h>

/* HEADER_ONLY_ASSEMBLER_START */

#ifdef __cplusplus
TOOLBOX_API const toolbox_memfuncs TOOLBOX_DEFAULT_MEMFUNCS = {
    malloc,   /* .malloc_fn */
    calloc,   /* .calloc_fn */
    free,     /* .free_fn */
    realloc   /* .realloc_fn */
};
#else
TOOLBOX_API const toolbox_memfuncs TOOLBOX_DEFAULT_MEMFUNCS = {
    .malloc_fn = malloc,
    .calloc_fn = calloc,
    .free_fn = free,
    .realloc_fn = realloc
};
#endif // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif

TOOLBOX_FUNC void* toolbox_custom_malloc(const toolbox_memfuncs* fun, unsigned long long size) {
    if (fun && fun->malloc_fn) return fun->malloc_fn(size);
    return malloc(size);
}

TOOLBOX_FUNC void* toolbox_custom_calloc(const toolbox_memfuncs* fun, unsigned long long num, unsigned long long size) {
    if (num && size > (SIZE_MAX / num)) return NULL;
    
    if (fun && fun->calloc_fn) return fun->calloc_fn(num, size);
    return calloc(num, size);
}

TOOLBOX_FUNC void toolbox_custom_free(const toolbox_memfuncs* fun, void* ptr) {
    if (fun && fun->free_fn) fun->free_fn(ptr);
    else free(ptr);
}

TOOLBOX_FUNC void* toolbox_custom_realloc(const toolbox_memfuncs* fun, void* ptr, unsigned long long newSize) {
    if (fun && fun->realloc_fn) return fun->realloc_fn(ptr, newSize);
    return realloc(ptr, newSize);
}

#ifdef __cplusplus
}
#endif

/* HEADER_ONLY_ASSEMBLER_STOP */