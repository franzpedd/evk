/*
 * @file headeronly_assembler.c
 * @brief Assembles header-only version of projects
 * 
 * This program concatenates multiple source files into a single header file
 * for header-only distribution. It respects special markers to include only
 * the relevant sections of each file.
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// @brief start marker for header-only sections in source files
#define HEADERGUARD_START "/* HEADER_ONLY_ASSEMBLER_START */"

/// @brief end marker for header-only sections in source files
#define HEADERGUARD_END "/* HEADER_ONLY_ASSEMBLER_STOP */"

/// @brief creates a visual separator for the output file
#define SEPARATOR(msg) \
    "\n//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n" \
    "// " msg "\n" \
    "//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n"   

/// @brief system call for copying directory
#ifdef _WIN32
    #define COPY_CMD "xcopy /E /I"
#else
    #define COPY_CMD "cp -r"
#endif

/// @brief writes the content of a source file to the output
static void fprintf_content_node(const char* filePath, FILE* output, int bypassMarkers) {
    FILE* file = fopen(filePath, "r");
    if (!file) {
        fprintf(stderr, "Error: Failed to open file: %s\n", filePath);
        return;
    }

    char buffer[1024];
    int insideMarker = 0;
    int foundStart = 0;
    int foundEnd = 0;

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (bypassMarkers) {
            fprintf(output, "%s", buffer);
            continue;
        }
        
        if (strstr(buffer, HEADERGUARD_START) != NULL) {
            insideMarker = 1;
            foundStart = 1;
            continue;
        }

        if (insideMarker && strstr(buffer, HEADERGUARD_END) != NULL) {
            insideMarker = 0;
            foundEnd = 1;
            continue;
        }

        if (insideMarker) {
            fprintf(output, "%s", buffer);
        }
    }

    fclose(file);

    if (!bypassMarkers) {
        if (!foundStart) {
            fprintf(stderr, "Warning: Start marker not found in %s\n", filePath);
        }
        if (!foundEnd) {
            fprintf(stderr, "Warning: End marker not found in %s\n", filePath);
        }
    }
}

/// @brief writes file content to output with optional C-style commenting
static void fprintf_constructor_general(FILE* outputFile, const char* filepath, int commented)  {
    if (!outputFile || !filepath) {
        fprintf(stderr, "Error: Invalid arguments to fprintf_constructor_general\n");
        return;
    }

    if (commented == 1) {
        fprintf(outputFile, "/*\n");
    }

    fprintf_content_node(filepath, outputFile, commented);

    if (commented == 1) {
        fprintf(outputFile, "\n*/");
    }
}

/// @brief copies the directory of thirdparty into header-only dir
static int copy_thirdparty(const char* src, const char* dst) {
    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "%s \"%s\" \"%s\"", COPY_CMD, src, dst);
    return system(cmd);
}

/// @brief assembles the header-only vecmath API
static void fwrite_vecmath(const char* output)  {
    FILE* outputFile = fopen(output, "w");
    if (!outputFile) {
        fprintf(stderr, "Error: Failed to create output file: %s\n", output);
        return;
    }

    fprintf(outputFile, 
        "#ifndef VECMATH_INCLUDED\n"
        "#define VECMATH_INCLUDED\n"
        "#define VECMATH_HEADER_ONLY\n"
    );

    //fprintf_constructor_general(outputFile, "../../vecmath/vecmath.h", 0); // not used on header-only
    fprintf_constructor_general(outputFile, "../../vecmath/vecmath_defines.h", 0);
    fprintf_constructor_general(outputFile, "../../vecmath/vecmath_types.h", 0);
    fprintf_constructor_general(outputFile, "../../vecmath/vecmath_basic_op.h", 0);
    fprintf_constructor_general(outputFile, "../../vecmath/vecmath_vec_op.h", 0);
    fprintf_constructor_general(outputFile, "../../vecmath/vecmath_mat_op.h", 0);
    fprintf_constructor_general(outputFile, "../../vecmath/vecmath_quat_op.h", 0);
    fprintf_constructor_general(outputFile, "../../vecmath/vecmath_ray_op.h", 0);
    fprintf_constructor_general(outputFile, "../../vecmath/vecmath_util.h", 0);
    
    fprintf(outputFile, 
        "\n#ifdef VECMATH_IMPLEMENTATION\n"
        "#undef VECMATH_IMPLEMENTATION\n\n"
        "#include <math.h>\n"
        "#include <string.h>\n"
    );
    
    fprintf_constructor_general(outputFile, "../../vecmath/vecmath_basic_op.c", 0);
    fprintf_constructor_general(outputFile, "../../vecmath/vecmath_vec_op.c", 0);
    fprintf_constructor_general(outputFile, "../../vecmath/vecmath_mat_op.c", 0);
    fprintf_constructor_general(outputFile, "../../vecmath/vecmath_quat_op.c", 0);
    fprintf_constructor_general(outputFile, "../../vecmath/vecmath_ray_op.c", 0);
    fprintf_constructor_general(outputFile, "../../vecmath/vecmath_util.c", 0);
    
    fprintf(outputFile, 
        "#endif // VECMATH_IMPLEMENTATION\n"
        "#endif // VECMATH_INCLUDED\n"
    );
}

/// @brief assembles the header-only memm API
static void fwrite_memm(const char* output) {
    FILE* outputFile = fopen(output, "w");
    if (!outputFile) {
        fprintf(stderr, "Error: Failed to create output file: %s\n", output);
        return;
    }

    fprintf(outputFile, 
        "#ifndef MEMM_INCLUDED\n"
        "#define MEMM_INCLUDED\n"
        "#define MEMM_HEADER_ONLY\n"
    );

    //fprintf_constructor_general(outputFile, "../../memm/memm.h", 0); // not used on header-only
    fprintf_constructor_general(outputFile, "../../memm/memm_defines.h", 0);
    fprintf_constructor_general(outputFile, "../../memm/memm_core.h", 0);

    fprintf(outputFile, 
        "\n#ifdef MEMM_IMPLEMENTATION\n"
        "#undef MEMM_IMPLEMENTATION\n\n"
        "#include <stdlib.h>\n"
        "#include <stdio.h>\n"
        "#include <string.h>\n"
        "#include <time.h>\n"
    );

    fprintf_constructor_general(outputFile, "../../memm/memm_core.c", 0);

    fprintf(outputFile, 
        "#endif // MEMM_IMPLEMENTATION\n"
        "#endif // MEMM_INCLUDED\n"
    );
}

/// @brief assembles the header-only toolbox API
static void fwrite_toolbox(const char* output) {
    FILE* outputFile = fopen(output, "w");
    if (!outputFile) {
        fprintf(stderr, "Error: Failed to create output file: %s\n", output);
        return;
    }

    fprintf(outputFile, 
        "#ifndef TOOLBOX_INCLUDED\n"
        "#define TOOLBOX_INCLUDED\n"
        "#define TOOLBOX_HEADER_ONLY\n"
    );

    //fprintf_constructor_general(outputFile, "../../toolbox/toolbox.h", 0); // not used on header-only
    fprintf_constructor_general(outputFile, "../../toolbox/toolbox_defines.h", 0);
    fprintf_constructor_general(outputFile, "../../toolbox/toolbox_types.h", 0);
    fprintf_constructor_general(outputFile, "../../toolbox/toolbox_darray.h", 0);
    fprintf_constructor_general(outputFile, "../../toolbox/toolbox_idgen.h", 0);
    fprintf_constructor_general(outputFile, "../../toolbox/toolbox_memory.h", 0);
    fprintf_constructor_general(outputFile, "../../toolbox/toolbox_shashtable.h", 0);
    fprintf_constructor_general(outputFile, "../../toolbox/toolbox_util.h", 0);

    fprintf(outputFile, 
        "\n#ifdef TOOLBOX_IMPLEMENTATION\n"
        "#undef TOOLBOX_IMPLEMENTATION\n\n"
        "#include <stdlib.h>\n"
        "#include <string.h>\n"
    );

    fprintf_constructor_general(outputFile, "../../toolbox/toolbox_darray.c", 0);
    fprintf_constructor_general(outputFile, "../../toolbox/toolbox_idgen.c", 0);
    fprintf_constructor_general(outputFile, "../../toolbox/toolbox_memory.c", 0);
    fprintf_constructor_general(outputFile, "../../toolbox/toolbox_shashtable.c", 0);
    fprintf_constructor_general(outputFile, "../../toolbox/toolbox_util.c", 0);

    fprintf(outputFile, 
        "#endif // TOOLBOX_IMPLEMENTATION\n"
        "#endif // TOOLBOX_INCLUDED\n"
    );
}

static void fwrite_evk(const char* output) {
    FILE* outputFile = fopen(output, "w");
    if (!outputFile) {
        fprintf(stderr, "Error: Failed to create output file: %s\n", output);
        return;
    }

    fprintf(outputFile, 
        "#ifndef EVK_INCLUDED\n"
        "#define EVK_INCLUDED\n"
        "#define EVK_HEADER_ONLY\n\n"
        "#include \"memm.h\"\n"
        "#include \"toolbox.h\"\n"
        "#include \"vecmath.h\"\n"
    );

    //fprintf_constructor_general(outputFile, "../../evk/evk.h", 0); // not used on header-only
    fprintf(outputFile, SEPARATOR("Project: EVK-definition"));
    fprintf_constructor_general(outputFile, "../../evk/evk_defines.h", 0);
    fprintf_constructor_general(outputFile, "../../evk/evk_types.h", 0);
    fprintf_constructor_general(outputFile, "../../evk/evk_core.h", 0);

    fprintf(outputFile, 
        "\n#ifdef EVK_IMPLEMENTATION\n"
        "#undef EVK_IMPLEMENTATION\n\n"
        "#define MEMM_IMPLEMENTATION\n"
        "#include \"memm.h\"\n"
        "#define TOOLBOX_IMPLEMENTATION\n"
        "#include \"toolbox.h\"\n"
        "#define VECMATH_IMPLEMENTATION\n"
        "#include \"vecmath.h\"\n"
        "\n"
        "#define STB_IMAGE_IMPLEMENTATION\n"
        "#include <stb/stb_image.h>\n"
        "#define VOLK_IMPLEMENTATION\n"
        "#include <volk/volk.h>\n"
        "\n"
        "#include <stdio.h>\n"
        "#include <stdlib.h>\n"
        "#include <stdarg.h>\n"
        "#include <string.h>\n"
        "#include <time.h>\n"
    );

    fprintf(outputFile, SEPARATOR("Project: EVK-implementation"));
    fprintf_constructor_general(outputFile, "../../evk/shader/billboard_default.h", 0);
    fprintf_constructor_general(outputFile, "../../evk/shader/billboard_picking.h", 0);
    fprintf_constructor_general(outputFile, "../../evk/shader/grid.h", 0);
    fprintf_constructor_general(outputFile, "../../evk/shader/sprite_default.h", 0);
    fprintf_constructor_general(outputFile, "../../evk/shader/sprite_picking.h", 0);
    //
    fprintf_constructor_general(outputFile, "../../evk/evk_core.c", 0);
    fprintf_constructor_general(outputFile, "../../evk/evk_vulkan_core.c", 0);
    fprintf_constructor_general(outputFile, "../../evk/evk_vulkan_drawable_2d.c", 0);
    fprintf_constructor_general(outputFile, "../../evk/evk_vulkan_experimental.c", 0);
    fprintf_constructor_general(outputFile, "../../evk/evk_vulkan_renderphase.c", 0);

    fprintf(outputFile, 
        "#endif // EVK_IMPLEMENTATION\n"
        "#endif // EVK_INCLUDED\n"
    );
}

/// @brief entrypoint
int main()
{
    // copy thirdparty structure
    copy_thirdparty("../../thirdparty", "../../build/headeronly/thirdparty");

    // generate project
    fwrite_vecmath("../../build/headeronly/vecmath.h");
    fwrite_memm("../../build/headeronly/memm.h");
    fwrite_toolbox("../../build/headeronly/toolbox.h");
    fwrite_evk("../../build/headeronly/evk.h");

    return 0;
}
