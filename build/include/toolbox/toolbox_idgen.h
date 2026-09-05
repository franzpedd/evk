#ifndef TOOLBOX_IDGEN_INCLUDED
#define TOOLBOX_IDGEN_INCLUDED

#include "toolbox_defines.h"
#include "toolbox_types.h"

/* HEADER_ONLY_ASSEMBLER_START */

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

/* HEADER_ONLY_ASSEMBLER_STOP */

#endif // TOOLBOX_IDGEN_INCLUDED