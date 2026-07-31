#include "toolbox_shashtable.h"
#include "toolbox_memory.h"
#include <string.h>

/* HEADER_ONLY_ASSEMBLER_START */

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// internal
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct shash 
{
    char* key;
    void* value;
    struct shash* next;
};

struct shashtable
{
    shash* buckets[SHASHTABLE_SIZE];
    toolbox_memfuncs memfuncs;
    unsigned long long count;
};

/// @brief internal string duplication with proper error handling
static toolbox_result internal_strdup(const toolbox_memfuncs* fun, const char* src, char** output) {
    if (!fun || !src || !output) return TOOLBOX_ERROR_INVALID_PARAM;
    
    unsigned long long len = strlen(src);
    
    // check for overflow
    if (len == SHASHTABLE_SIZE_MAX) return TOOLBOX_ERROR_MEMORY_ALLOC;
    
    char* str = (char*)toolbox_custom_malloc(fun, len + 1);
    if (!str) return TOOLBOX_ERROR_MEMORY_ALLOC;
    
    memcpy(str, src, len + 1);
    *output = str;
    
    return TOOLBOX_SUCCESS;
}

/// @brief improved hash function with overflow protection
static TOOLBOX_FUNC unsigned long shash_djb2_hash(const char *str) {
    if (!str) return 0;
    if (SHASHTABLE_SIZE == 0) return 0;
    
    unsigned long hash = 5381;
    int c;
    
    while ((c = *str++)) {
        // use unsigned overflow (defined behavior in C)
        hash = ((hash << 5) + hash) + (unsigned long)c;  // hash * 33 + c
        // mix to reduce collisions, without UB
        hash ^= (hash >> 16);
    }
    
    return hash % SHASHTABLE_SIZE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// external
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TOOLBOX_FUNC shashtable* shashtable_init(void) {
    return shashtable_init_memfuncs(&TOOLBOX_DEFAULT_MEMFUNCS);
}

TOOLBOX_FUNC shashtable* shashtable_init_memfuncs(const toolbox_memfuncs* memfuncs)
{
    const toolbox_memfuncs* actual_memfuncs = memfuncs ? memfuncs : &TOOLBOX_DEFAULT_MEMFUNCS;
    
    shashtable* outHashtable = (shashtable*)toolbox_custom_malloc(actual_memfuncs, sizeof(shashtable));
    if (!outHashtable) return NULL;
    
    for (unsigned long long i = 0; i < SHASHTABLE_SIZE; i++) {
        outHashtable->buckets[i] = NULL;
    }
    
    outHashtable->memfuncs = *actual_memfuncs;
    outHashtable->count = 0;
    
    return outHashtable;
}

TOOLBOX_FUNC void shashtable_destroy(shashtable* table) {
    if (!table) return;
    
    for (unsigned long long i = 0; i < SHASHTABLE_SIZE; i++) {
        shash* entry = table->buckets[i];
        while (entry) {
            shash* next = entry->next;
            if (entry->key) {
                toolbox_custom_free(&table->memfuncs, entry->key);
            }
            toolbox_custom_free(&table->memfuncs, entry);
            entry = next;
        }
    }
    toolbox_custom_free(&table->memfuncs, table);
}

TOOLBOX_FUNC toolbox_result shashtable_insert(shashtable* table, const char* key, void* value) {
    if (!table || !key) return TOOLBOX_ERROR_INVALID_PARAM;
    
    if (table->count == SHASHTABLE_SIZE_MAX) return TOOLBOX_ERROR_FULL;
    
    unsigned long index = shash_djb2_hash(key);
    if (index >= SHASHTABLE_SIZE) return TOOLBOX_ERROR_INVALID_PARAM;
    
    // check if key already exists
    shash* current = table->buckets[index];
    while (current) {
        if (current->key && strcmp(current->key, key) == 0) {
            current->value = value; // Update existing
            return TOOLBOX_SUCCESS;
        }
        current = current->next;
    }
    
    shash* new_entry = (shash*)toolbox_custom_malloc(&table->memfuncs, sizeof(shash));
    if (!new_entry) return TOOLBOX_ERROR_MEMORY_ALLOC;
    
    new_entry->key = NULL;
    new_entry->value = value;
    new_entry->next = NULL;
    
    // duplicate key
    toolbox_result result = internal_strdup(&table->memfuncs, key, &new_entry->key);
    if (result != TOOLBOX_SUCCESS) {
        toolbox_custom_free(&table->memfuncs, new_entry);
        return result;
    }
    
    // insert at beginning of chain
    new_entry->next = table->buckets[index];
    table->buckets[index] = new_entry;
    table->count++;
    
    return TOOLBOX_SUCCESS;
}

TOOLBOX_FUNC toolbox_result shashtable_delete(shashtable* table, const char* key) {
    if (!table || !key) return TOOLBOX_ERROR_INVALID_PARAM;
    
    unsigned long index = shash_djb2_hash(key);
    if (index >= SHASHTABLE_SIZE) return TOOLBOX_ERROR_INVALID_PARAM;
    
    shash* current = table->buckets[index];
    shash* prev = NULL;
    
    while (current) {
        if (current->key && strcmp(current->key, key) == 0) {
            if (prev) {
                prev->next = current->next;
            } else {
                table->buckets[index] = current->next;
            }
            
            if (current->key) {
                toolbox_custom_free(&table->memfuncs, current->key);
            }
            toolbox_custom_free(&table->memfuncs, current);
            table->count--;
            
            return TOOLBOX_SUCCESS;
        }
        prev = current;
        current = current->next;
    }
    
    return TOOLBOX_ERROR_NOT_FOUND;
}

TOOLBOX_FUNC void* shashtable_lookup(shashtable* table, const char* key)
{
    if (!table || !key) return NULL;
    
    unsigned long index = shash_djb2_hash(key);
    if (index >= SHASHTABLE_SIZE) return NULL;
    
    const shash* current = table->buckets[index];
    
    while (current) {
        if (current->key && strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    
    return NULL;
}

TOOLBOX_FUNC bool shashtable_contains(shashtable* table, const char* key) {
    return shashtable_lookup(table, key) != NULL;
}

TOOLBOX_FUNC unsigned long long shashtable_count(shashtable* table) {
    return table ? table->count : 0;
}

TOOLBOX_FUNC void shashtable_clear(shashtable* table) {
    if (!table) return;
    
    for (unsigned long long i = 0; i < SHASHTABLE_SIZE; i++) {
        shash* entry = table->buckets[i];
        while (entry) {
            shash* next = entry->next;
            if (entry->key) {
                toolbox_custom_free(&table->memfuncs, entry->key);
            }
            toolbox_custom_free(&table->memfuncs, entry);
            entry = next;
        }
        table->buckets[i] = NULL;
    }
    table->count = 0;
}

TOOLBOX_FUNC void shashtable_foreach(shashtable* table, bool (*callback)(const char* key, void* value, void* userdata), void* userdata) {
    if (!table || !callback) return;
    
    for (unsigned long long i = 0; i < SHASHTABLE_SIZE; i++) {
        shash* entry = table->buckets[i];
        while (entry) {
            if (entry->key) {
                if (!callback(entry->key, entry->value, userdata)) {
                    return; // stop iteration if callback returns false
                }
            }
            entry = entry->next;
        }
    }
}

#ifdef __cplusplus
}
#endif

/* HEADER_ONLY_ASSEMBLER_STOP */