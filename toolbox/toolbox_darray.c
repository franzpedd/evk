#include "toolbox_darray.h"

#include "toolbox_memory.h"
#include <stdlib.h>
#include <string.h>

/* HEADER_ONLY_ASSEMBLER_START */

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

/* HEADER_ONLY_ASSEMBLER_STOP */