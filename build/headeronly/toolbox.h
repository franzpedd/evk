#ifndef TOOLBOX_INCLUDED
#define TOOLBOX_INCLUDED
#define TOOLBOX_HEADER_ONLY

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// defines the building configuration
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// build mode selection (only one should be defined)
// TOOLBOX_HEADER_ONLY  - Header-only (all functions static inline)
// TOOLBOX_SHARED       - Shared library (DLL/SO)
// TOOLBOX_STATIC       - Static library (default)

#if defined(TOOLBOX_SHARED)
    #if defined(_WIN32)
        #if defined(TOOLBOX_EXPORT) 
            #define TOOLBOX_API __declspec(dllexport)
        #else
            #define TOOLBOX_API __declspec(dllimport)
        #endif
    #elif defined(__GNUC__) || defined(__clang__)
        #if defined(TOOLBOX_EXPORT)
            #define TOOLBOX_API __attribute__((visibility("default")))
        #else
            #define TOOLBOX_API
        #endif
    #else
        #define TOOLBOX_API
    #endif
#else
    #define TOOLBOX_API
#endif

#if defined(TOOLBOX_HEADER_ONLY)
    #if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
        #define TOOLBOX_FUNC static inline
    #else
        #define TOOLBOX_FUNC static  // fallback for C89
    #endif
    #undef TOOLBOX_API
    #define TOOLBOX_API
    
#elif defined(TOOLBOX_SHARED)
    #define TOOLBOX_FUNC
#else
    #define TOOLBOX_FUNC
#endif // TOOLBOX_HEADER_ONLY

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// custom option macros
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief defines the size of the static hash table
#ifndef SHASHTABLE_SIZE
    #define SHASHTABLE_SIZE 128
#endif

/// @brief maximum of 16.7 million IDs (2MB memory usage)
#ifndef IDGEN_MAX_SAFE_IDS
    #define IDGEN_MAX_SAFE_IDS 16777216 
#endif

/// @brief maximum size, using 32 bits for backwards compatibility
#ifndef SHASHTABLE_SIZE_MAX
    #define SHASHTABLE_SIZE_MAX 4294967295
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
// enumerations
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief various types of erros that may happen when using the library
typedef enum toolbox_result
{
    TOOLBOX_SUCCESS = 0,
    TOOLBOX_ERROR_INVALID_PARAM,
    TOOLBOX_ERROR_MEMORY_ALLOC,
    TOOLBOX_ERROR_OUT_OF_BOUNDS,
    TOOLBOX_ERROR_EMPTY,
    TOOLBOX_ERROR_NOT_FOUND,
    TOOLBOX_ERROR_FULL
} toolbox_result;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// custom allocation functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief callback for custom allocation memories
typedef void* (*malloc_func)(size_t size);
typedef void* (*calloc_func)(size_t num, size_t size);
typedef void (*free_func)(void* ptr);
typedef void* (*realloc_func)(void* ptr, size_t newSize);

/// @brief bundle-up the memory functions into one structure
typedef struct toolbox_memfuncs
{
    malloc_func malloc_fn;
    calloc_func calloc_fn;
    free_func free_fn;
    realloc_func realloc_fn;
} toolbox_memfuncs;

/// @brief export memory functions
TOOLBOX_API extern const toolbox_memfuncs TOOLBOX_DEFAULT_MEMFUNCS;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// opaque objects
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief opaque dynamic array structure
typedef struct darray darray;

/// @brief opaque id generator structure
typedef struct idgen idgen;

/// @brief opaque structure for the hash entry
typedef struct shash shash;

/// @brief opaque structure for the hash table
typedef struct shashtable shashtable;


#ifdef __cplusplus
extern "C" {
#endif

/// @brief initializes the dynamic array
TOOLBOX_API darray* darray_init(unsigned long long elementSize, unsigned long long initialCapacity);

/// @brief initializes the dynamic array with custom memory allocation functions
TOOLBOX_API darray* darray_init_memfuncs(unsigned long long elementSize, unsigned long long initialCapacity, const toolbox_memfuncs* memfuncs);

/// @brief destroys the dynamic array, but not the content itself, be-warned
TOOLBOX_API void darray_destroy(darray* array);

/// @brief pushes an element into the array's back
TOOLBOX_API toolbox_result darray_push_back(darray* array, const void* element);

/// @brief removes an item from the array's back, not freeing it
TOOLBOX_API toolbox_result darray_pop_back(darray* array, void* elementOut);

/// @brief sees what is underneath a given index on the array
TOOLBOX_API const void* darray_const_peek(const darray* array, unsigned long long index);

/// @brief access the data underneath the array
TOOLBOX_API const void* darray_const_data(const darray* array);

/// @brief returns an item from the array
TOOLBOX_API toolbox_result darray_get(const darray* array, unsigned long long index, void* elementOut);

/// @brief sets an index of the array to be the element
TOOLBOX_API toolbox_result darray_set(darray* array, unsigned long long index, const void* element);

/// @brief inserts an item into a given position on the array
TOOLBOX_API toolbox_result darray_insert_at(darray* array, unsigned long long index, const void* element);

/// @brief removes an item from a given position on the array
TOOLBOX_API toolbox_result darray_remove_at(darray* array, unsigned long long index, void* elementOut);

/// @brief resizes the array to a new size (new elements are zero-initialized)
TOOLBOX_API toolbox_result darray_resize(darray* array, unsigned long long newSize);

/// @brief reserves a certain amount of items for the array
TOOLBOX_API toolbox_result darray_reserve(darray* array, unsigned long long newCapacity);

/// @brief reallocates the array into a smaller memory area
TOOLBOX_API toolbox_result darray_shrink_to_fit(darray* array);

/// @brief clears the array (size becomes 0, capacity unchanged)
TOOLBOX_API void darray_clear(darray* array);

/// @brief returns the array's current size
TOOLBOX_API unsigned long long darray_size(const darray* array);

/// @brief returns the array's current max capacity
TOOLBOX_API unsigned long long darray_capacity(const darray* array);

/// @brief returns if the array is currently empty
TOOLBOX_API bool darray_empty(const darray* array);

/// @brief returns the first element of the array (returns NULL if empty)
TOOLBOX_API const void* darray_front(const darray* array);

/// @brief returns the last element of the array (returns NULL if empty)
TOOLBOX_API const void* darray_back(const darray* array);

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C" {
#endif

/// @brief initializes generator, set to UINT32_MAX for full range
TOOLBOX_API idgen* idgen_create(unsigned int start_id);

/// @brief initializes generator with custom allocation functions
TOOLBOX_API idgen* idgen_create_memfuncs(unsigned int start_id, const toolbox_memfuncs* memfuncs);

/// @brief releases the resources of the id generator
TOOLBOX_API void idgen_destroy(idgen* gen);

/// @brief generate next ID, returns 0 if exhausted
TOOLBOX_API unsigned int idgen_next(idgen* gen);

/// @brief register an id
TOOLBOX_API bool idgen_register(idgen* gen, unsigned int id);

/// @brief unregister an id
TOOLBOX_API bool idgen_unregister(idgen* gen, unsigned int id);

/// @brief check if an ID is currently registered
TOOLBOX_API bool idgen_is_registered(idgen* gen, unsigned int id);

/// @brief get the number of currently registered IDs
TOOLBOX_API unsigned int idgen_count(idgen* gen);

/// @brief resets generator to initial state
TOOLBOX_API void idgen_reset(idgen* gen);

/// @brief get the maximum ID value
TOOLBOX_API unsigned int idgen_max_id(const idgen* gen);

/// @brief get the start ID value
TOOLBOX_API unsigned int idgen_start_id(const idgen* gen);

#ifdef __cplusplus
}
#endif


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


#ifdef TOOLBOX_IMPLEMENTATION
#undef TOOLBOX_IMPLEMENTATION

#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// internal
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct darray
{
    void* data;
    unsigned long long size;
    unsigned long long capacity;
    unsigned long long elementSize;
    toolbox_memfuncs memfuncs;
};

/// @brief helper to check for multiplication overflow
static TOOLBOX_FUNC bool is_multiply_overflow(unsigned long long a, unsigned long long b) {
    return (a > 0 && b > SIZE_MAX / a);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// external
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TOOLBOX_FUNC darray* darray_init(unsigned long long elementSize, unsigned long long initialCapacity) {
    return darray_init_memfuncs(elementSize, initialCapacity, &TOOLBOX_DEFAULT_MEMFUNCS);
}

TOOLBOX_FUNC darray* darray_init_memfuncs(unsigned long long elementSize, unsigned long long initialCapacity, const toolbox_memfuncs* memfuncs)
{
    if (elementSize == 0) return NULL;
    
    if (initialCapacity > 0 && is_multiply_overflow(initialCapacity, elementSize)) {
        return NULL;
    }

    const toolbox_memfuncs* funcs = memfuncs ? memfuncs : &TOOLBOX_DEFAULT_MEMFUNCS;
    
    darray* outArray = (darray*)toolbox_custom_malloc(funcs, sizeof(darray));
    if (!outArray) return NULL;

    memset(outArray, 0, sizeof(darray));
    outArray->size = 0;
    outArray->elementSize = elementSize;
    outArray->capacity = (initialCapacity > 0) ? initialCapacity : 1;
    outArray->memfuncs = *funcs;

    outArray->data = toolbox_custom_calloc(funcs, outArray->capacity, elementSize);
    if (!outArray->data) {
        toolbox_custom_free(funcs, outArray);
        return NULL;
    }

    return outArray;
}

TOOLBOX_FUNC void darray_destroy(darray* array) {
    if (!array) return;
    toolbox_custom_free(&array->memfuncs, array->data);
    toolbox_custom_free(&array->memfuncs, array);
}

TOOLBOX_FUNC toolbox_result darray_push_back(darray* array, const void* element) {
    if (!array) return TOOLBOX_ERROR_INVALID_PARAM;

    // ensure capacity
    if (array->size >= array->capacity) {
        if (array->capacity > SIZE_MAX / 2) {
            return TOOLBOX_ERROR_MEMORY_ALLOC;
        }
        toolbox_result res = darray_reserve(array, array->capacity * 2);
        if (res != TOOLBOX_SUCCESS) return res;
    }

    // add new element
    memcpy((char*)array->data + (array->size * array->elementSize), element, array->elementSize);
    array->size++;

    return TOOLBOX_SUCCESS;
}

TOOLBOX_FUNC toolbox_result darray_pop_back(darray* array, void* elementOut) {
    if (!array || array->size == 0) return TOOLBOX_ERROR_EMPTY;
    
    if (elementOut) {
        memcpy(elementOut, (char*)array->data + ((array->size - 1) * array->elementSize), array->elementSize);
    }
    
    memset((char*)array->data + ((array->size - 1) * array->elementSize), 0, array->elementSize);

    array->size--;
    return TOOLBOX_SUCCESS;
}

TOOLBOX_FUNC const void* darray_const_peek(const darray* array, unsigned long long index) {
    if (!array || index >= array->size) return NULL;
    return (char*)array->data + (index * array->elementSize);
}

TOOLBOX_FUNC const void* darray_const_data(const darray* array) {
    return array ? array->data : NULL;
}

TOOLBOX_FUNC toolbox_result darray_get(const darray* array, unsigned long long index, void* elementOut) {
    if (!array || !elementOut || index >= array->size) return TOOLBOX_ERROR_INVALID_PARAM;
    memcpy(elementOut, (char*)array->data + (index * array->elementSize), array->elementSize);
    return TOOLBOX_SUCCESS;
}

TOOLBOX_FUNC toolbox_result darray_set(darray* array, unsigned long long index, const void* element) {
    if (!array || !element || index >= array->size) return TOOLBOX_ERROR_INVALID_PARAM;
    memcpy((char*)array->data + (index * array->elementSize), element, array->elementSize);
    return TOOLBOX_SUCCESS;
}

TOOLBOX_FUNC toolbox_result darray_insert_at(darray* array, unsigned long long index, const void* element) {
    if (!array || !element || index > array->size) return TOOLBOX_ERROR_INVALID_PARAM;

    // ensure capacity
    if (array->size >= array->capacity) {
        if (array->capacity > SIZE_MAX / 2) {
            return TOOLBOX_ERROR_MEMORY_ALLOC;
        }
        toolbox_result result = darray_reserve(array, array->capacity * 2);
        if (result != TOOLBOX_SUCCESS) return result;
    }

    // shift elements to make space
    if (index < array->size) {
        char* data = (char*)array->data;
        memmove(data + ((index + 1) * array->elementSize), data + (index * array->elementSize), (array->size - index) * array->elementSize);
    }

    memcpy((char*)array->data + (index * array->elementSize), element, array->elementSize);
    array->size++;
    return TOOLBOX_SUCCESS;
}

TOOLBOX_FUNC toolbox_result darray_remove_at(darray* array, unsigned long long index, void* elementOut) {
    if (!array || index >= array->size) {
        return TOOLBOX_ERROR_INVALID_PARAM;
    }

    if (array->size == 0) {
        return TOOLBOX_ERROR_EMPTY;
    }

    // if the caller wants the removed element, copy it first
    if (elementOut) {
        memcpy(elementOut, (char*)array->data + (index * array->elementSize), array->elementSize);
    }

    // shift elements left to overwrite the removed element
    if (index < array->size - 1) {
        char* data = (char*)array->data;
        memmove(data + (index * array->elementSize), data + ((index + 1) * array->elementSize), (array->size - index - 1) * array->elementSize);
    }

    memset((char*)array->data + ((array->size - 1) * array->elementSize), 0, array->elementSize);
    
    array->size--;
    return TOOLBOX_SUCCESS;
}

TOOLBOX_FUNC toolbox_result darray_resize(darray* array, unsigned long long newSize) {
    if (!array) return TOOLBOX_ERROR_INVALID_PARAM;
    
    if (newSize > array->capacity) {
        toolbox_result res = darray_reserve(array, newSize);
        if (res != TOOLBOX_SUCCESS) return res;
    }
    
    if (newSize > array->size) {
        memset((char*)array->data + (array->size * array->elementSize), 0, (newSize - array->size) * array->elementSize);
    }
    
    array->size = newSize;
    return TOOLBOX_SUCCESS;
}

TOOLBOX_FUNC toolbox_result darray_reserve(darray* array, unsigned long long newCapacity) {
    if (!array) return TOOLBOX_ERROR_INVALID_PARAM;
    if (newCapacity <= array->capacity) return TOOLBOX_SUCCESS;

    if (is_multiply_overflow(newCapacity, array->elementSize)) {
        return TOOLBOX_ERROR_MEMORY_ALLOC;
    }

    unsigned long long newSizeBytes = newCapacity * array->elementSize;
    unsigned long long oldSizeBytes = array->size * array->elementSize;
    void* newData = NULL;

    // try realloc directly
    if (array->memfuncs.realloc_fn) {
        newData = toolbox_custom_realloc(&array->memfuncs, array->data, newSizeBytes);
        if (!newData) return TOOLBOX_ERROR_MEMORY_ALLOC;
    } 

    // fallback: allocate, copy and free
    else {
        newData = toolbox_custom_malloc(&array->memfuncs, newSizeBytes);
        if (!newData) return TOOLBOX_ERROR_MEMORY_ALLOC;

        if (array->data && oldSizeBytes > 0) {
            memcpy(newData, array->data, oldSizeBytes);
        }
        toolbox_custom_free(&array->memfuncs, array->data);
    }

    array->data = newData;
    array->capacity = newCapacity;

    return TOOLBOX_SUCCESS;
}

TOOLBOX_FUNC toolbox_result darray_shrink_to_fit(darray* array) {
    if (!array) return TOOLBOX_ERROR_INVALID_PARAM;
    if (array->size == array->capacity) return TOOLBOX_SUCCESS;

    if (array->size == 0) {
        toolbox_custom_free(&array->memfuncs, array->data);
        array->data = NULL;
        array->capacity = 0;
        return TOOLBOX_SUCCESS;
    }

    if (is_multiply_overflow(array->size, array->elementSize)) {
        return TOOLBOX_ERROR_MEMORY_ALLOC;
    }

    void* newData = toolbox_custom_malloc(&array->memfuncs, array->size * array->elementSize);
    if (!newData) return TOOLBOX_ERROR_MEMORY_ALLOC;

    memcpy(newData, array->data, array->size * array->elementSize);
    toolbox_custom_free(&array->memfuncs, array->data);
    array->data = newData;
    array->capacity = array->size;
    return TOOLBOX_SUCCESS;
}

TOOLBOX_FUNC void darray_clear(darray* array) {
    if (array) {
        array->size = 0;
        // note: memory is not freed, just reset size
    }
}

TOOLBOX_FUNC unsigned long long darray_size(const darray* array) {
    return array ? array->size : 0;
}

TOOLBOX_FUNC unsigned long long darray_capacity(const darray* array) {
    return array ? array->capacity : 0;
}

TOOLBOX_FUNC bool darray_empty(const darray* array) {
    return array ? (array->size == 0) : true;
}

TOOLBOX_FUNC const void* darray_front(const darray* array) {
    if (!array || array->size == 0) return NULL;
    return (char*)array->data;
}

TOOLBOX_FUNC const void* darray_back(const darray* array) {
    if (!array || array->size == 0) return NULL;
    return (char*)array->data + ((array->size - 1) * array->elementSize);
}

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C" {
#endif

struct idgen
{
    unsigned int current_id;
    unsigned int start_id;
    unsigned int max_id;
    unsigned int count;
    unsigned int bitset_size;       // number of unsigned int words
    unsigned int* used_bits;        // bitset representing used IDs
    toolbox_memfuncs memfuncs;
};

// macros for bit manipulation
#define BIT_INDEX(id, base)   ((id) - (base))
#define BIT_WORD(i)           ((i) >> 5)          // divide by 32
#define BIT_MASK(i)           (1u << ((i) & 31u))

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// internal
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static TOOLBOX_FUNC inline bool bit_test(const unsigned int* bits, unsigned int bitset_size, unsigned int idx) {
    unsigned int word = BIT_WORD(idx);
    if (word >= bitset_size) return false;
    return (bits[word] & BIT_MASK(idx)) != 0;
}

static TOOLBOX_FUNC inline void bit_set(unsigned int* bits, unsigned int bitset_size, unsigned int idx) {
    unsigned int word = BIT_WORD(idx);
    if (word >= bitset_size) return;
    bits[word] |= BIT_MASK(idx);
}

static TOOLBOX_FUNC inline void bit_clear(unsigned int* bits, unsigned int bitset_size, unsigned int idx) {
    unsigned int word = BIT_WORD(idx);
    if (word >= bitset_size) return;
    bits[word] &= ~BIT_MASK(idx);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// external
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TOOLBOX_API idgen* idgen_create(unsigned int start_id) {
    return idgen_create_memfuncs(start_id, &TOOLBOX_DEFAULT_MEMFUNCS);
}

TOOLBOX_API idgen* idgen_create_memfuncs(unsigned int start_id, const toolbox_memfuncs* memfuncs) {
    const toolbox_memfuncs* actual_memfuncs = memfuncs ? memfuncs : &TOOLBOX_DEFAULT_MEMFUNCS;
    
    if (start_id >= IDGEN_MAX_SAFE_IDS) return NULL;
    
    // check for overflow in bitset size calculation: (IDGEN_MAX_SAFE_IDS + 31) / 32
    if (IDGEN_MAX_SAFE_IDS > SIZE_MAX - 31u) return NULL;
    unsigned long long bitset_size_u64 = (unsigned long long)IDGEN_MAX_SAFE_IDS + 31u;
    if (bitset_size_u64 / 32u > SIZE_MAX) return NULL;
    
    unsigned int bitset_size = (unsigned int)(bitset_size_u64 / 32u);
    if (bitset_size == 0) bitset_size = 1;
    
    // check for overflow in allocation: bitset_size * sizeof(unsigned int)
    if (bitset_size > SIZE_MAX / sizeof(unsigned int)) return NULL;
    unsigned long long bitset_bytes = bitset_size * sizeof(unsigned int);
    
    // Allocate idgen structure
    idgen* gen = (idgen*)toolbox_custom_malloc(actual_memfuncs, sizeof(idgen));
    if (!gen) return NULL;
    
    memset(gen, 0, sizeof(*gen));
    gen->start_id = start_id;
    gen->current_id = start_id;
    gen->max_id = IDGEN_MAX_SAFE_IDS;
    gen->bitset_size = bitset_size;
    gen->memfuncs = *actual_memfuncs; // copy by value
    
    // allocate bitset
    gen->used_bits = (unsigned int*)toolbox_custom_malloc(actual_memfuncs, bitset_bytes);
    if (!gen->used_bits) {
        toolbox_custom_free(actual_memfuncs, gen);
        return NULL;
    }
    
    memset(gen->used_bits, 0, bitset_bytes);
    return gen;
}

TOOLBOX_API void idgen_destroy(idgen* gen) {
    if (!gen) return;
    if (gen->used_bits) {
        toolbox_custom_free(&gen->memfuncs, gen->used_bits);
    }
    toolbox_custom_free(&gen->memfuncs, gen);
}

TOOLBOX_API unsigned int idgen_next(idgen* gen) {
    if (!gen) return 0;
    
    // if all IDs are used, return 0
    if (gen->count >= (gen->max_id - gen->start_id)) {
        return 0;
    }
    
    unsigned int range = gen->max_id - gen->start_id;
    unsigned int max_attempts = range;
    
    for (unsigned int attempts = 0; attempts < max_attempts; attempts++) {
        unsigned int candidate = gen->current_id;
        
        // wrap around if needed
        if (candidate >= gen->max_id) {
            candidate = gen->start_id;
        }
        
        unsigned int idx = BIT_INDEX(candidate, gen->start_id);
        if (!bit_test(gen->used_bits, gen->bitset_size, idx)) {
            // found free ID
            bit_set(gen->used_bits, gen->bitset_size, idx);
            gen->count++;
            
            // advance current_id for next search
            gen->current_id = candidate + 1;
            if (gen->current_id >= gen->max_id) {
                gen->current_id = gen->start_id;
            }
            
            return candidate;
        }
        
        // move to next candidate
        gen->current_id++;
        if (gen->current_id >= gen->max_id) {
            gen->current_id = gen->start_id;
        }
    }
    
    return 0; // no free IDs found
}

TOOLBOX_API bool idgen_register(idgen* gen, unsigned int id) {
    if (!gen) return false;
    if (id < gen->start_id || id >= gen->max_id) return false;
    
    unsigned int idx = BIT_INDEX(id, gen->start_id);
    if (bit_test(gen->used_bits, gen->bitset_size, idx)) return false;
    
    bit_set(gen->used_bits, gen->bitset_size, idx);
    gen->count++;
    return true;
}

TOOLBOX_API bool idgen_unregister(idgen* gen, unsigned int id) {
    if (!gen) return false;
    if (id < gen->start_id || id >= gen->max_id) return false;
    
    unsigned int idx = BIT_INDEX(id, gen->start_id);
    if (!bit_test(gen->used_bits, gen->bitset_size, idx)) return false;
    
    bit_clear(gen->used_bits, gen->bitset_size, idx);
    gen->count--;
    
    // move current_id back for better reuse (optimization)
    if (id < gen->current_id) {
        gen->current_id = id;
    } else if (gen->current_id == gen->start_id && id == gen->max_id - 1) {
        // special case: wrap around
        gen->current_id = id;
    }
    
    return true;
}

TOOLBOX_API bool idgen_is_registered(idgen* gen, unsigned int id) {
    if (!gen) return false;
    if (id < gen->start_id || id >= gen->max_id) return false;
    
    unsigned int idx = BIT_INDEX(id, gen->start_id);
    return bit_test(gen->used_bits, gen->bitset_size, idx);
}

TOOLBOX_API unsigned int idgen_count(idgen* gen) {
    return gen ? gen->count : 0;
}

TOOLBOX_API void idgen_reset(idgen* gen) {
    if (!gen) return;
    if (gen->used_bits) {
        memset(gen->used_bits, 0, gen->bitset_size * sizeof(unsigned int));
    }
    gen->count = 0;
    gen->current_id = gen->start_id;
}

TOOLBOX_API unsigned int idgen_max_id(const idgen* gen) {
    return gen ? gen->max_id : 0;
}

TOOLBOX_API unsigned int idgen_start_id(const idgen* gen) {
    return gen ? gen->start_id : 0;
}

#ifdef __cplusplus
}
#endif


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


#ifdef __cplusplus
extern "C" {
#endif

/// @brief Internal helper to find the last path separator ('/' or '\')
static TOOLBOX_FUNC const char* find_last_separator(const char* path) {
    if (!path) return NULL;

    const char* last_slash = strrchr(path, '/');
    const char* last_backslash = strrchr(path, '\\');

    if (last_slash && last_backslash) {
        return (last_slash > last_backslash) ? last_slash : last_backslash;
    }
    return last_slash ? last_slash : last_backslash;
}

/// @brief Internal helper to safely copy up to (dest_size - 1) bytes with null termination
static TOOLBOX_FUNC bool safe_strcpy(char* dest, const char* src, size_t dest_size) {
    if (!dest || dest_size == 0) return false;
    if (!src) {
        dest[0] = '\0';
        return false;
    }

    size_t src_len = strlen(src);
    bool truncated = (src_len >= dest_size);
    size_t copy_len = truncated ? (dest_size - 1) : src_len;

    memcpy(dest, src, copy_len);
    dest[copy_len] = '\0';

    return !truncated;
}

TOOLBOX_FUNC bool filename_from_path(const char* path, char* outBuffer, size_t bufferSize) {
    if (!outBuffer || bufferSize == 0) return false;

    if (!path) {
        outBuffer[0] = '\0';
        return false;
    }

    const char* separator = find_last_separator(path);
    const char* filename = separator ? separator + 1 : path;

    if (*filename == '\0') {
        outBuffer[0] = '\0';
        return false;
    }

    return safe_strcpy(outBuffer, filename, bufferSize);
}

TOOLBOX_FUNC bool extension_from_path(const char* path, char* outBuffer, size_t bufferSize) {
    if (!outBuffer || bufferSize == 0) return false;

    if (!path) {
        outBuffer[0] = '\0';
        return false;
    }

    const char* separator = find_last_separator(path);
    const char* filename = separator ? separator + 1 : path;

    if (*filename == '\0') {
        outBuffer[0] = '\0';
        return false;
    }

    const char* dot = strrchr(filename, '.');

    // ignore leading dots (e.g., hidden files like ".gitignore")
    if (dot && dot != filename) {
        return safe_strcpy(outBuffer, dot, bufferSize);
    }

    outBuffer[0] = '\0';
    return false;
}

TOOLBOX_FUNC bool basename_from_path(const char* path, char* outBuffer, size_t bufferSize) {
    if (!outBuffer || bufferSize == 0) return false;

    if (!path) {
        outBuffer[0] = '\0';
        return false;
    }

    const char* separator = find_last_separator(path);
    const char* filename = separator ? separator + 1 : path;

    if (*filename == '\0') {
        outBuffer[0] = '\0';
        return false;
    }

    const char* dot = strrchr(filename, '.');

    if (dot && dot != filename) {
        size_t basename_len = (size_t)(dot - filename);
        bool truncated = (basename_len >= bufferSize);
        size_t copy_len = truncated ? (bufferSize - 1) : basename_len;

        memcpy(outBuffer, filename, copy_len);
        outBuffer[copy_len] = '\0';
        return !truncated;
    }

    return safe_strcpy(outBuffer, filename, bufferSize);
}

TOOLBOX_FUNC bool dirname_from_path(const char* path, char* outBuffer, size_t bufferSize) {
    if (!outBuffer || bufferSize == 0) return false;

    if (!path || *path == '\0') {
        outBuffer[0] = '\0';
        return false;
    }

    const char* separator = find_last_separator(path);

    if (!separator) {
        return safe_strcpy(outBuffer, ".", bufferSize);
    }

    size_t dir_len = (size_t)(separator - path);

    // handle consecutive separators (e.g., "path//file.txt")
    while (dir_len > 0 && (path[dir_len - 1] == '/' || path[dir_len - 1] == '\\')) {
        dir_len--;
    }

    if (dir_len == 0) {
        return safe_strcpy(outBuffer, "/", bufferSize);
    }

    bool truncated = (dir_len >= bufferSize);
    size_t copy_len = truncated ? (bufferSize - 1) : dir_len;

    memcpy(outBuffer, path, copy_len);
    outBuffer[copy_len] = '\0';
    return !truncated;
}

#ifdef __cplusplus
}
#endif

#endif // TOOLBOX_IMPLEMENTATION
#endif // TOOLBOX_INCLUDED
