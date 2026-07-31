#ifndef MEMM_CORE_INCLUDED
#define MEMM_CORE_INCLUDED

#include "memm_defines.h"

/* HEADER_ONLY_ASSEMBLER_START */

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

/// @brief initializes the memory manager
MEMM_API bool memm_init();

/// @brief shuts down the memory manager
MEMM_API void memm_shutdown();

/// @brief allocates memory
MEMM_API void* memm_malloc(unsigned long long size, const char* file, int line);

/// @brief zeroed-allocates memory
MEMM_API void* memm_calloc(unsigned long long num, unsigned long long size, const char* file, int line);

/// @brief reallocates/changes size of a previously allocated memory block
MEMM_API void* memm_realloc(void* ptr, unsigned long long size, const char* file, int line);

/// @brief deallocates memory
MEMM_API void memm_free(void* ptr, const char* file, int line);

/// @brief returns how much memory is currently being used
MEMM_API unsigned long long memm_get_current_usage(void);

/// @brief returns the peak usage (max bytes simultaneously allocated)
MEMM_API unsigned long long memm_get_peak_usage(void);

/// @brief returns how many allocation calls were issued
MEMM_API unsigned long long memm_get_allocation_count(void);

/// @brief returns how many free calls were issued
MEMM_API unsigned long long memm_get_free_count(void);

/// @brief returns how many tracked allocations are still active
MEMM_API unsigned long long memm_get_active_allocation_count(void);

/// @brief fills out a buffer with statistics about the memory manager
MEMM_API unsigned long long memm_get_stats_string(char* buffer, unsigned long long buffer_size);

/// @brief fills out a buffer with information about current tracked allocations
MEMM_API unsigned long long memm_get_allocations_string(char* buffer, unsigned long long buffer_size);

/// @brief fills out a buffer with information about potential memory leaks
MEMM_API unsigned long long memm_get_leaks_string(char* buffer, unsigned long long buffer_size);

/// @brief prints general status, must have defined MEMM_ENABLE_LOGGING
MEMM_API void memm_print_stats();

/// @brief prints status about allocations made, must have defined MEMM_ENABLE_LOGGING
MEMM_API void memm_print_allocations();

/// @brief prints leaks information, must have defined MEMM_ENABLE_LOGGING
MEMM_API void memm_print_leaks();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// macros for memory allocation std-alike
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief macro functions to access like conventional allocation funcs
#define m_malloc(size) memm_malloc(size, __FILE__, __LINE__)
#define m_calloc(num, size) memm_calloc(num, size, __FILE__, __LINE__)
#define m_realloc(ptr, size) memm_realloc(ptr, size, __FILE__, __LINE__)
#define m_free(ptr) memm_free(ptr, __FILE__, __LINE__)

#ifdef __cplusplus
}
#endif

/* HEADER_ONLY_ASSEMBLER_STOP */

#endif // MEMM_CORE_INCLUDED