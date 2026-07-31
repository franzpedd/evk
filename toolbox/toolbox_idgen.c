#include "toolbox_idgen.h"
#include "toolbox_memory.h"

#include <stdlib.h>
#include <string.h>

/* HEADER_ONLY_ASSEMBLER_START */

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

/* HEADER_ONLY_ASSEMBLER_STOP */