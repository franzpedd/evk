#ifndef EVK_VULKAN_EXPERIMENTAL_INCLUDED
#define EVK_VULKAN_EXPERIMENTAL_INCLUDED

#include "evk_defines.h"
#include "evk_types.h"

/* HEADER_ONLY_ASSEMBLER_START */

#ifdef __cplusplus 
extern "C" {
#endif

/// @brief dumps the entire picking framebuffer into the terminal for analysis of ids being written
EVK_API void evkexp_dump_picking_framebuffer();

/// @brief debug the picking coordinates
EVK_API void evkexp_debug_pick_coordinates(float2 mousePos);

#ifdef __cplusplus 
}
#endif

/* HEADER_ONLY_ASSEMBLER_STOP */

#endif // EVK_VULKAN_EXPERIMENTAL_INCLUDED