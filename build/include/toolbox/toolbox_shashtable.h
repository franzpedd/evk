#ifndef TOOLBOX_SHASHTABLE_INCLUDED
#define TOOLBOX_SHASHTABLE_INCLUDED

#include "toolbox_defines.h"
#include "toolbox_types.h"

/* HEADER_ONLY_ASSEMBLER_START */

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// functions
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief creates the hashtable
TOOLBOX_API shashtable* shashtable_init(void);

/// @brief creates the hashtable with custom memory allocation functions
TOOLBOX_API shashtable* shashtable_init_memfuncs(const toolbox_memfuncs* memfuncs);

/// @brief destroys the hashtable
TOOLBOX_API void shashtable_destroy(shashtable* table);

/// @brief inserts an item into the hashtable
TOOLBOX_API toolbox_result shashtable_insert(shashtable* table, const char* key, void* value);

/// @brief deletes an item from the hashtable
TOOLBOX_API toolbox_result shashtable_delete(shashtable* table, const char* key);

/// @brief returns a peek at the given hash entry associated with given key
TOOLBOX_API void* shashtable_lookup(shashtable* table, const char* key);

/// @brief checks if a given key exists in the hashtable
TOOLBOX_API bool shashtable_contains(shashtable* table, const char* key);

/// @brief returns how many entries exists in the hashtable
TOOLBOX_API unsigned long long shashtable_count(shashtable* table);

/// @brief clears all entries from the hashtable
TOOLBOX_API void shashtable_clear(shashtable* table);

/// @brief iterates over all entries (callback returns false to stop)
TOOLBOX_API void shashtable_foreach(shashtable* table, bool (*callback)(const char* key, void* value, void* userdata), void* userdata);

#ifdef __cplusplus
}
#endif

/* HEADER_ONLY_ASSEMBLER_STOP */

#endif // TOOLBOX_SHASHTABLE_INCLUDED