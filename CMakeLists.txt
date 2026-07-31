# cmake version
cmake_minimum_required(VERSION 3.10)

# project declaration
project(evk VERSION 1.0.0 LANGUAGES C)

# cmake configuration
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# enable IDE folder support for Visual Studio
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# platform and architecture
if(WIN32)
    set(PLATFORM_NAME "win32")
elseif(UNIX AND NOT APPLE)
    set(PLATFORM_NAME "linux")
elseif(APPLE)
    set(PLATFORM_NAME "macos")
else()
    set(PLATFORM_NAME "other")
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCH_NAME "x64")
else()
    set(ARCH_NAME "x86")
endif()

# output folder hierarchy: build/<platform>-<arch>/(dynamic|static)
set(PLATFORM_DIR "${PLATFORM_NAME}-${ARCH_NAME}")
set(DYNAMIC_OUTPUT_DIR "${CMAKE_BINARY_DIR}/${PLATFORM_DIR}/dynamic")
set(STATIC_OUTPUT_DIR  "${CMAKE_BINARY_DIR}/${PLATFORM_DIR}/static")

# project macro options
option(EVK_ENABLE_VALIDATIONS "Enable API and Vulkan log information about info, warnings and errors" ON)

# project .c files
set(SOURCES
    memm/memm_core.c
    #
    toolbox/toolbox_darray.c
    toolbox/toolbox_idgen.c
    toolbox/toolbox_memory.c
    toolbox/toolbox_shashtable.c
    toolbox/toolbox_util.c
    #
    vecmath/vecmath_basic_op.c
    vecmath/vecmath_mat_op.c
    vecmath/vecmath_quat_op.c
    vecmath/vecmath_ray_op.c
    vecmath/vecmath_util.c
    vecmath/vecmath_vec_op.c
    #
    evk/evk_core.c
    evk/evk_vulkan_core.c
    evk/evk_vulkan_drawable_2d.c
    evk/evk_vulkan_experimental.c
    evk/evk_vulkan_renderphase.c
)

# project .h files
set(HEADERS
    memm/memm_core.h
    memm/memm_defines.h
    memm/memm.h
    #
    toolbox/toolbox_darray.h
    toolbox/toolbox_defines.h
    toolbox/toolbox_idgen.h
    toolbox/toolbox_memory.h
    toolbox/toolbox_shashtable.h
    toolbox/toolbox_types.h
    toolbox/toolbox_util.h
    toolbox/toolbox.h
    #
    vecmath/vecmath_basic_op.h
    vecmath/vecmath_defines.h
    vecmath/vecmath_mat_op.h
    vecmath/vecmath_quat_op.h
    vecmath/vecmath_ray_op.h
    vecmath/vecmath_types.h
    vecmath/vecmath_util.h
    vecmath/vecmath_vec_op.h
    vecmath/vecmath.h
    #
    evk/shader/billboard_default.h
    evk/shader/billboard_picking.h
    evk/shader/grid.h
    evk/shader/sprite_default.h
    evk/shader/sprite_picking.h
    #
    evk/evk_core.h
    evk/evk_defines.h
    evk/evk_types.h
    evk/evk_vulkan_core.h
    evk/evk_vulkan_drawable_2d.h
    evk/evk_vulkan_experimental.h
    evk/evk_vulkan_renderphase.h
    evk/evk.h
)

# preserve disk directory structure in visual studio filters
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

# ------------------------------------------------------------------------------
# Dynamic Library (EVK_D)
# ------------------------------------------------------------------------------
add_library(EVK_D SHARED ${SOURCES} ${HEADERS})
target_compile_definitions(EVK_D PRIVATE EVK_SHARED EVK_EXPORT)
target_compile_definitions(EVK_D PUBLIC EVK_SHARED)

set_target_properties(EVK_D PROPERTIES 
    OUTPUT_NAME evk 
    VERSION ${PROJECT_VERSION} 
    SOVERSION ${PROJECT_VERSION_MAJOR}
    # Binaries (.dll / .so)
    RUNTIME_OUTPUT_DIRECTORY "${DYNAMIC_OUTPUT_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${DYNAMIC_OUTPUT_DIR}"
    # Windows DLL import library (.lib)
    ARCHIVE_OUTPUT_DIRECTORY "${DYNAMIC_OUTPUT_DIR}"
    # Windows DLL debug symbols (.pdb)
    PDB_OUTPUT_DIRECTORY     "${DYNAMIC_OUTPUT_DIR}"
)

target_include_directories(EVK_D PRIVATE evk memm thirdparty toolbox vecmath)

if(EVK_ENABLE_VALIDATIONS)
    target_compile_definitions(EVK_D PRIVATE EVK_ENABLE_VALIDATIONS=1)
endif()

if(MSVC)
    set_target_properties(EVK_D PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:EVK_D>")
endif()

# ------------------------------------------------------------------------------
# Static Library (EVK_S)
# ------------------------------------------------------------------------------
add_library(EVK_S STATIC ${SOURCES} ${HEADERS})
target_compile_definitions(EVK_S PUBLIC EVK_STATIC)

set_target_properties(EVK_S PROPERTIES 
    OUTPUT_NAME evk 
    CLEAN_DIRECT_OUTPUT 1
    # Static libraries (.a on Linux, .lib on Windows)
    ARCHIVE_OUTPUT_DIRECTORY     "${STATIC_OUTPUT_DIR}"
    # Compiler-generated PDB symbols for static build
    COMPILE_PDB_OUTPUT_DIRECTORY "${STATIC_OUTPUT_DIR}"
)

target_include_directories(EVK_S PUBLIC evk memm thirdparty toolbox vecmath)

if(EVK_ENABLE_VALIDATIONS)
    target_compile_definitions(EVK_S PRIVATE EVK_ENABLE_VALIDATIONS=1)
endif()

if(MSVC)
    set_target_properties(EVK_S PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:EVK_S>")
endif()