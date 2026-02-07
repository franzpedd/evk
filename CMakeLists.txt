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
evk/include/evk.h evk/include/evk_impl.h 
evk/include/evk_types.h 
evk/include/evk_vulkan_core.h evk/include/evk_vulkan_core_impl.h
evk/include/evk_vulkan_drawable.h evk/include/evk_vulkan_drawable_impl.h
evk/include/evk_vulkan_renderphase.h evk/include/evk_vulkan_renderphase_impl.h
examples/example_win32.c
)

# include dirs
target_include_directories(${PROJECT_NAME} PRIVATE evk/include evk/thirdparty)

# defines
if(EVK_ENABLE_VALIDATIONS)
target_compile_definitions(${PROJECT_NAME} PRIVATE EVK_ENABLE_VALIDATIONS=1)
endif()

# copy assets to build directory
if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND if exist "${CMAKE_CURRENT_SOURCE_DIR}/assets" (
            if not exist "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets" mkdir "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
            xcopy /E /Y /I "${CMAKE_CURRENT_SOURCE_DIR}/assets" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
        )
        COMMENT "Copying assets to build directory"
        VERBATIM
    )
else()
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
        COMMENT "Copying assets to build directory"
        VERBATIM
    )
endif()

# set visual studio debug working directory
if(MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
endif()