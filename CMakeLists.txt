# cmake version
cmake_minimum_required(VERSION 3.10)

# cmake build options
option(EVK_ENABLE_VALIDATIONS "Enable vulkan log messages" ON)

# cmake project
project(EVK LANGUAGES C)

# config
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

# sources
add_executable(${PROJECT_NAME} 
evk/include/evk.h 
evk/include/evk_types.h 
evk/include/evk_vulkan_core.h
evk/include/evk_vulkan_renderphase.h
examples/example_win32.c
)

# include dirs
target_include_directories(${PROJECT_NAME} PRIVATE evk/include evk/thirdparty)

# defines
if(EVK_ENABLE_VALIDATIONS)
target_compile_definitions(${PROJECT_NAME} PRIVATE EVK_ENABLE_VALIDATIONS=1)
endif()