#ifndef MEMM_INCLUDED
#define MEMM_INCLUDED
#define MEMM_HEADER_ONLY

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// defines the building configuration
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// build mode selection (only one should be defined)
// MEMM_HEADER_ONLY  - Header-only (all functions static inline)
// MEMM_SHARED       - Shared library (DLL/SO)
// MEMM_STATIC       - Static library (default)

#if defined(MEMM_SHARED)
    #if defined(_WIN32)
        #if defined(MEMM_EXPORT) 
            #define MEMM_API __declspec(dllexport)
        #else
            #define MEMM_API __declspec(dllimport)
        #endif
    #elif defined(__GNUC__) || defined(__clang__)
        #if defined(MEMM_EXPORT)
            #define MEMM_API __attribute__((visibility("default")))
        #else
            #define MEMM_API
        #endif
    #else
        #define MEMM_API
    #endif
#else
    #define MEMM_API
#endif

#if defined(MEMM_HEADER_ONLY)
    #if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
        #define MEMM_FUNC static inline
    #else
        #define MEMM_FUNC static  // fallback for C89
    #endif
    #undef MEMM_API
    #define MEMM_API
    
#elif defined(MEMM_SHARED)
    #define MEMM_FUNC
#else
    #define MEMM_FUNC
#endif // MEMM_HEADER_ONLY

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// costumizable constants and validation
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief sets how many allocations the system can hold information about
#ifndef MEMM_HASH_TABLE_SIZE
    #define MEMM_HASH_TABLE_SIZE 2048
#endif

/// @brief sets how many bytes the helper-macros string have at max
#ifndef MEMM_MAX_STRING_LENGTH
    #define MEMM_MAX_STRING_LENGTH 2048
#endif

/// @brief compile-time validation that size is power of 2
#if (MEMM_HASH_TABLE_SIZE & (MEMM_HASH_TABLE_SIZE - 1)) != 0
    #error "MEMM_HASH_TABLE_SIZE must be a power of 2 for hashing efficiency"
#endif

/// @brief enable/disable logging for memory error messages
#ifndef MEMM_DISABLE_LOGGING
    #define MEMM_ENABLE_LOGGING 1
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// boolean implementation
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __bool_true_false_are_defined
#ifndef __cplusplus
    #if defined(__GNUC__) || defined(__clang__) || defined(_MSC_VER)
        typedef _Bool bool;
    #else
        typedef unsigned char bool;
    #endif
    
    #define true  1
    #define false 0
#endif // __cplusplus

#define __bool_true_false_are_defined 1
#endif // __bool_true_false_are_defined


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


#ifdef MEMM_IMPLEMENTATION
#undef MEMM_IMPLEMENTATION

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// internal implementation
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief holds an allocation information
typedef struct memm_allocation
{
    void* ptr;
    unsigned long long size;
    const char* file;
    int line;
    time_t timestamp;
    struct memm_allocation* next;
} memm_allocation_t;

/// @brief holds the memm state
typedef struct memm
{
    memm_allocation_t* hash_table[MEMM_HASH_TABLE_SIZE];
    unsigned long long total_allocated;
    unsigned long long total_freed;
    unsigned long long peak_memory;
    unsigned long long allocation_count;
    unsigned long long free_count;
    bool initialized;
} memm_t;

/// @brief global state
static memm_t g_memm = { 0 };

/// @brief internal allocation tracking (uses real malloc/free)
static MEMM_FUNC void* internal_malloc(unsigned long long size) {
    return malloc(size);
}

/// @brief internal memory de-allocation
static MEMM_FUNC void internal_free(void* ptr) {
    free(ptr);
}

/// @brief hashes pointer for hash table
static MEMM_FUNC unsigned long long memm_hash_ptr(void* ptr) {
    return ((unsigned long long)ptr) & (MEMM_HASH_TABLE_SIZE - 1);
}

/// @brief register an allocation
static MEMM_FUNC bool memm_register_allocation(void* ptr, unsigned long long size, const char* file, int line) {
    if (!ptr || !g_memm.initialized) return false;
    if (size == 0) return true;
    
    unsigned long long hash = memm_hash_ptr(ptr);
    memm_allocation_t* alloc = (memm_allocation_t*)internal_malloc(sizeof(memm_allocation_t));
    
    if (!alloc) {
        #if MEMM_ENABLE_LOGGING
        fprintf(stderr, "MEMM-ERROR: Failed to register allocation for %p\n", ptr);
        #endif
        return false;
    }
    
    alloc->ptr = ptr;
    alloc->size = size;
    alloc->file = file;
    alloc->line = line;
    alloc->timestamp = time(NULL);
    alloc->next = g_memm.hash_table[hash];
    g_memm.hash_table[hash] = alloc;
    
    g_memm.total_allocated += size;
    g_memm.allocation_count++;
    
    unsigned long long current_usage = g_memm.total_allocated - g_memm.total_freed;
    if (current_usage > g_memm.peak_memory) {
        g_memm.peak_memory = current_usage;
    }
    
    return true;
}

/// @brief unregister an allocation
static MEMM_FUNC bool memm_unregister_allocation(void* ptr, const char* file, int line) {
    if (!ptr || !g_memm.initialized) return false;
    
    unsigned long long hash = memm_hash_ptr(ptr);
    memm_allocation_t** current = &g_memm.hash_table[hash];
    
    while (*current) {
        if ((*current)->ptr == ptr) {
            memm_allocation_t* to_free = *current;
            *current = to_free->next;
            
            g_memm.total_freed += to_free->size;
            g_memm.free_count++;
            
            internal_free(to_free);
            return true;
        }
        current = &(*current)->next;
    }
    
    #if MEMM_ENABLE_LOGGING
    fprintf(stderr, "MEMM-ERROR: Attempt to free unknown pointer %p (%s:%d)\n", ptr, file, line);
    #endif
    return false;
}

/// @brief safe multiplication with overflow check
static MEMM_FUNC bool safe_multiply(unsigned long long a, unsigned long long b, unsigned long long* result) {
    if (a > 0 && b > SIZE_MAX / a) return false;
    *result = a * b;
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MEMM_FUNC bool memm_init(void) {
    if (g_memm.initialized) return true;
    memset(&g_memm, 0, sizeof(g_memm));
    g_memm.initialized = 1;
    
    #if MEMM_ENABLE_LOGGING
    printf("Memory manager initialized with %d buckets\n", MEMM_HASH_TABLE_SIZE);
    #endif
    
    return true;
}

MEMM_FUNC void memm_shutdown(void) {
    if (!g_memm.initialized) return;
    
    // Note: We don't free user allocations here - that would cause double frees
    // Just clean up our tracking structures
    for (unsigned long long i = 0; i < MEMM_HASH_TABLE_SIZE; i++) {
        memm_allocation_t* current = g_memm.hash_table[i];
        while (current) {
            memm_allocation_t* next = current->next;
            internal_free(current);
            current = next;
        }
        g_memm.hash_table[i] = NULL;
    }
    
    #if MEMM_ENABLE_LOGGING
    printf("Memory manager shutdown complete\n");
    #endif
    g_memm.initialized = 0;
}

MEMM_FUNC void* memm_malloc(unsigned long long size, const char* file, int line) {
    if (!g_memm.initialized) return NULL;
    if (size == 0) return NULL;
    
    void* ptr = malloc(size);
    if (ptr) {
        memm_register_allocation(ptr, size, file, line);
    } else {
        #if MEMM_ENABLE_LOGGING
        fprintf(stderr, "MEMM-ERROR: malloc failed for %zu bytes (%s:%d)\n", size, file, line);
        #endif
    }
    return ptr;
}

MEMM_FUNC void* memm_calloc(unsigned long long num, unsigned long long size, const char* file, int line) {
    if (!g_memm.initialized) return NULL;
    if (num == 0 || size == 0) return NULL;
    
    unsigned long long total_size;
    if (!safe_multiply(num, size, &total_size)) {
        #if MEMM_ENABLE_LOGGING
        fprintf(stderr, "MEMM-ERROR: calloc overflow for %zu * %zu (%s:%d)\n", num, size, file, line);
        #endif
        return NULL;
    }
    
    void* ptr = calloc(num, size);
    if (ptr) {
        memm_register_allocation(ptr, total_size, file, line);
    } else {
        #if MEMM_ENABLE_LOGGING
        fprintf(stderr, "MEMM-ERROR: calloc failed for %zu elements of %zu bytes (%s:%d)\n", num, size, file, line);
        #endif
    }
    return ptr;
}

MEMM_FUNC void* memm_realloc(void* ptr, unsigned long long size, const char* file, int line) {
    if (!g_memm.initialized) return NULL;
    
    if (ptr == NULL) {
        return memm_malloc(size, file, line);
    }
    
    if (size == 0) {
        memm_free(ptr, file, line);
        return NULL;
    }
    
    // get old size before unregistering
    unsigned long long hash = memm_hash_ptr(ptr);
    memm_allocation_t* current = g_memm.hash_table[hash];
    unsigned long long old_size = 0;
    
    while (current) {
        if (current->ptr == ptr) {
            old_size = current->size;
            break;
        }
        current = current->next;
    }
    
    memm_unregister_allocation(ptr, file, line);
    
    // attempt realloc
    void* new_ptr = realloc(ptr, size);
    if (new_ptr) {
        memm_register_allocation(new_ptr, size, file, line);
    } 

    else {
        // realloc failed, re-register the original pointer
        if (old_size > 0) {
            memm_register_allocation(ptr, old_size, file, line);
        }
        #if MEMM_ENABLE_LOGGING
        fprintf(stderr, "MEMM-ERROR: realloc failed for %zu bytes (%s:%d)\n", size, file, line);
        #endif
    }
    
    return new_ptr;
}

MEMM_FUNC void memm_free(void* ptr, const char* file, int line) {
    if (!g_memm.initialized || !ptr) return;
    
    bool tracked = memm_unregister_allocation(ptr, file, line);
    free(ptr);
    
    if (!tracked) {
        #if MEMM_ENABLE_LOGGING
        fprintf(stderr, "MEMM-WARN: freed untracked pointer %p (%s:%d)\n", ptr, file, line);
        #endif
    }
}

MEMM_FUNC unsigned long long memm_get_current_usage() {
    if (!g_memm.initialized) return 0;
    return g_memm.total_allocated - g_memm.total_freed;
}

MEMM_FUNC unsigned long long memm_get_peak_usage() {
    if (!g_memm.initialized) return 0;
    return g_memm.peak_memory;
}

MEMM_FUNC unsigned long long memm_get_allocation_count() {
    if (!g_memm.initialized) return 0;
    return g_memm.allocation_count;
}

MEMM_FUNC unsigned long long memm_get_free_count() {
    if (!g_memm.initialized) return 0;
    return g_memm.free_count;
}

MEMM_FUNC unsigned long long memm_get_active_allocation_count() {
    if (!g_memm.initialized) return 0;
    return g_memm.allocation_count - g_memm.free_count;
}

MEMM_FUNC unsigned long long memm_get_stats_string(char* buffer, unsigned long long buffer_size) {
    if (!buffer || buffer_size == 0 || !g_memm.initialized) {
        return 0;
    }
    
    unsigned long long active = memm_get_active_allocation_count();
    unsigned long long current = memm_get_current_usage();
    unsigned long long peak = memm_get_peak_usage();
    
    int written = snprintf(buffer, buffer_size,
        "=== MEMORY STATISTICS ===\n"
        "Total allocated:      %zu bytes\n"
        "Total freed:          %zu bytes\n"
        "Current usage:        %zu bytes\n"
        "Peak memory usage:    %zu bytes\n"
        "Allocation calls:     %zu\n"
        "Free calls:           %zu\n"
        "Active allocations:   %zu\n"
        "Hash table size:      %d buckets\n",
        g_memm.total_allocated,
        g_memm.total_freed,
        current,
        peak,
        g_memm.allocation_count,
        g_memm.free_count,
        active,
        MEMM_HASH_TABLE_SIZE
    );
    
    if (written < 0 || (unsigned long long)written >= buffer_size) {
        buffer[buffer_size - 1] = '\0';
        return buffer_size - 1;
    }
    
    return (unsigned long long)written;
}

MEMM_FUNC unsigned long long memm_get_allocations_string(char* buffer, unsigned long long buffer_size) {
    if (!buffer || buffer_size == 0 || !g_memm.initialized) {
        return 0;
    }
    
    char* cursor = buffer;
    unsigned long long remaining = buffer_size;
    unsigned long long total_written = 0;
    int written = 0;
    
    written = snprintf(cursor, remaining, "=== CURRENT ALLOCATIONS ===\n");
    if (written < 0) {
        buffer[0] = '\0';
        return 0;
    }
    
    if ((unsigned long long)written >= remaining) {
        buffer[buffer_size - 1] = '\0';
        return buffer_size - 1;
    }
    
    cursor += written;
    remaining -= written;
    total_written += (unsigned long long)written;
    
    unsigned long long total_count = 0;
    unsigned long long total_bytes = 0;
    
    for (unsigned long long i = 0; i < MEMM_HASH_TABLE_SIZE && remaining > 1; i++) {
        memm_allocation_t* current = g_memm.hash_table[i];
        while (current && remaining > 1) {
            written = snprintf(cursor, remaining, "  %p: %6zu bytes @ %s:%d\n", 
                               current->ptr, current->size, current->file, current->line);
            
            if (written < 0) break;
            
            unsigned long long written_size = (unsigned long long)written;
            if (written_size >= remaining) {
                written_size = remaining - 1;
            }
            
            cursor += written_size;
            remaining -= written_size;
            total_written += written_size;
            total_count++;
            total_bytes += current->size;
            current = current->next;
        }
    }
    
    if (total_count == 0 && remaining > 1) {
        written = snprintf(cursor, remaining, "  No active allocations\n");
        if (written > 0) {
            unsigned long long written_size = (unsigned long long)written;
            if (written_size >= remaining) {
                written_size = remaining - 1;
            }
            total_written += written_size;
        }
    } else if (remaining > 1) {
        written = snprintf(cursor, remaining, "  Total: %zu allocations, %zu bytes\n", total_count, total_bytes);
        if (written > 0) {
            unsigned long long written_size = (unsigned long long)written;
            if (written_size >= remaining) {
                written_size = remaining - 1;
            }
            total_written += written_size;
        }
    }
    
    if (remaining == 0 && buffer_size > 0) {
        buffer[buffer_size - 1] = '\0';
    }
    
    return total_written;
}

MEMM_FUNC unsigned long long memm_get_leaks_string(char* buffer, unsigned long long buffer_size) {
    if (!buffer || buffer_size == 0 || !g_memm.initialized) return 0;
    
    char* cursor = buffer;
    unsigned long long remaining = buffer_size;
    unsigned long long total_written = 0;
    int written = 0;
    
    written = snprintf(cursor, remaining, "=== MEMORY LEAK REPORT ===\n");
    if (written < 0) {
        buffer[0] = '\0';
        return 0;
    }
    
    if ((unsigned long long)written >= remaining) {
        buffer[buffer_size - 1] = '\0';
        return buffer_size - 1;
    }
    
    cursor += written;
    remaining -= written;
    total_written += (unsigned long long)written;
    
    unsigned long long leak_count = 0;
    unsigned long long leak_bytes = 0;
    
    for (unsigned long long i = 0; i < MEMM_HASH_TABLE_SIZE && remaining > 1; i++) {
        memm_allocation_t* current = g_memm.hash_table[i];
        while (current && remaining > 1) {
            written = snprintf(cursor, remaining, "  LEAK: %6zu bytes at %p (%s:%d)\n", 
                               current->size, current->ptr, current->file, current->line);
            
            if (written < 0) break;
            
            unsigned long long written_size = (unsigned long long)written;
            if (written_size >= remaining) {
                written_size = remaining - 1;
            }
            
            cursor += written_size;
            remaining -= written_size;
            total_written += written_size;
            leak_count++;
            leak_bytes += current->size;
            current = current->next;
        }
    }
    
    if (leak_count == 0 && remaining > 1) {
        written = snprintf(cursor, remaining, "  No memory leaks detected!\n");
        if (written > 0) {
            unsigned long long written_size = (unsigned long long)written;
            if (written_size >= remaining) {
                written_size = remaining - 1;
            }
            total_written += written_size;
        }
    }
    
    else if (remaining > 1) {
        written = snprintf(cursor, remaining, "  TOTAL LEAKS: %zu allocations, %zu bytes\n", leak_count, leak_bytes);
        if (written > 0) {
            unsigned long long written_size = (unsigned long long)written;
            if (written_size >= remaining) {
                written_size = remaining - 1;
            }
            total_written += written_size;
        }
    }
    
    if (remaining == 0 && buffer_size > 0) buffer[buffer_size - 1] = '\0';
    
    return total_written;
}

MEMM_FUNC void memm_print_stats()
{
    #ifdef MEMM_ENABLE_LOGGING
    char buf[MEMM_MAX_STRING_LENGTH];
        if (memm_get_stats_string(buf, sizeof(buf)) > 0) {
        printf("%s", buf);
    }
    #endif
}

MEMM_FUNC void memm_print_allocations()
{
    #ifdef MEMM_ENABLE_LOGGING
    char buf[MEMM_MAX_STRING_LENGTH];
    if (memm_get_allocations_string(buf, sizeof(buf)) > 0) {
        printf("%s", buf);
    }
    #endif
}

MEMM_API void memm_print_leaks()
{
    #ifdef MEMM_ENABLE_LOGGING
    char buf[MEMM_MAX_STRING_LENGTH];
    if (memm_get_leaks_string(buf, sizeof(buf)) > 0) {
        printf("%s", buf);
    }
    #endif
}

#ifdef __cplusplus
}
#endif

#endif // MEMM_IMPLEMENTATION
#endif // MEMM_INCLUDED
