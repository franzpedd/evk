#ifndef TOOLBOX_DARRAY_INCLUDED
#define TOOLBOX_DARRAY_INCLUDED

#include "toolbox_defines.h"
#include "toolbox_types.h"

/* HEADER_ONLY_ASSEMBLER_START */

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

/* HEADER_ONLY_ASSEMBLER_STOP */

#endif // TOOLBOX_DARRAY_INCLUDED