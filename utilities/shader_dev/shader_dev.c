/*
 * @file shader_dev.c
 * @brief Compiles GLSL shaders into bundled C header files
 * 
 * This program compiles GLSL shader stages into SPIR-V binaries and groups
 * stages with matching stems into single header files containing embedded
 * unsigned int arrays for header-only graphics pipeline distribution.
 */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdint.h>

/// @brief configuration
#define OPTIMIZE 1
#define OUTPUT_DIR "../../build/shader"
#define HEADER_DIR "../../evk/shader"
#define SHADER_INCLUDE_DIR "."  // search relative to execution path

#define MAX_STAGES_PER_BUNDLE 8
#define MAX_PATH_LEN 1024

/// @brief compiler enum
typedef enum CompilerType {
    COMPILER_NONE = 0,
    COMPILER_GLSLC,
    COMPILER_GLSLANG
} CompilerType;

/// @brief metadata for a single compiled shader stage
typedef struct CompiledStage {
    char stage_tag[32];   // e.g. "vert", "frag", "comp"
    char spv_path[MAX_PATH_LEN];
} CompiledStage;

/// @brief metadata for a bundle of shaders sharing the same stem
typedef struct ShaderBundle {
    char stem[256];
    CompiledStage stages[MAX_STAGES_PER_BUNDLE];
    size_t stage_count;
} ShaderBundle;

/// @brief extract file name from path
const char* path_get_filename(const char* path) {
    const char* last_slash = strrchr(path, '/');
    #ifdef _WIN32
    const char* last_backslash = strrchr(path, '\\');
    if (last_backslash && (!last_slash || last_backslash > last_slash)) {
        last_slash = last_backslash;
    }
    #endif
    return last_slash ? last_slash + 1 : path;
}

/// @brief extract stem (file name without extension)
void path_get_stem(const char* filename, char* stem, size_t max_len) {
    const char* dot = strrchr(filename, '.');
    size_t len = dot ? (size_t)(dot - filename) : strlen(filename);
    if (len >= max_len) len = max_len - 1;
    strncpy(stem, filename, len);
    stem[len] = '\0';
}

/// @brief extract extension including the dot
const char* path_get_ext(const char* filename) {
    const char* dot = strrchr(filename, '.');
    return dot ? dot : "";
}

/// @brief check if a compiler is available via system call
CompilerType find_compiler(void) {
    #ifdef _WIN32
    int res = system("glslc --version > nul 2>&1");
    #else
    int res = system("glslc --version > /dev/null 2>&1");
    #endif
    if (res == 0) {
        printf("Compiler: glslc\n");
        return COMPILER_GLSLC;
    }

    #ifdef _WIN32
    res = system("glslangValidator --version > nul 2>&1");
    #else
    res = system("glslangValidator --version > /dev/null 2>&1");
    #endif
    if (res == 0) {
        printf("Compiler: glslangValidator\n");
        return COMPILER_GLSLANG;
    }

    return COMPILER_NONE;
}

/// @brief map extension to glslang stage string
const char* get_glslang_stage(const char* ext) {
    if (strcmp(ext, ".vert") == 0) return "vertex";
    if (strcmp(ext, ".frag") == 0) return "fragment";
    if (strcmp(ext, ".comp") == 0) return "compute";
    if (strcmp(ext, ".geom") == 0) return "geometry";
    return NULL;
}

/// @brief compiles shader and populates out_spv_path and out_stage_tag on success
bool compile_shader(CompilerType compiler, const char* input_path, char* out_spv_path, size_t spv_path_len, char* out_stage_tag, size_t stage_tag_len) {
    const char* filename = path_get_filename(input_path);
    const char* ext = path_get_ext(filename);
    
    char stem[256];
    path_get_stem(filename, stem, sizeof(stem));

    const char* stage_tag = (ext[0] == '.') ? ext + 1 : ext;
    snprintf(out_stage_tag, stage_tag_len, "%s", stage_tag);
    snprintf(out_spv_path, spv_path_len, "%s/%s_%s.spv", OUTPUT_DIR, stem, stage_tag);

    char cmd[1024];

    if (compiler == COMPILER_GLSLANG) {
        const char* stage = get_glslang_stage(ext);
        snprintf(cmd, sizeof(cmd), "glslangValidator -V \"%s\" -o \"%s\"", input_path, out_spv_path);
        if (stage) {
            snprintf(cmd + strlen(cmd), sizeof(cmd) - strlen(cmd), " -S %s", stage);
        }

        if (OPTIMIZE) {
            snprintf(cmd + strlen(cmd), sizeof(cmd) - strlen(cmd), " -Os --strip-debug");
        }

        if (SHADER_INCLUDE_DIR && SHADER_INCLUDE_DIR[0] != '\0') {
            snprintf(cmd + strlen(cmd), sizeof(cmd) - strlen(cmd), " -I\"%s\"", SHADER_INCLUDE_DIR);
        }
    }

    else if (compiler == COMPILER_GLSLC) {
        snprintf(cmd, sizeof(cmd), "glslc \"%s\" -o \"%s\"", input_path, out_spv_path);
        if (OPTIMIZE) {
            snprintf(cmd + strlen(cmd), sizeof(cmd) - strlen(cmd), " -O");
        }

        if (SHADER_INCLUDE_DIR && SHADER_INCLUDE_DIR[0] != '\0') {
            snprintf(cmd + strlen(cmd), sizeof(cmd) - strlen(cmd), " -I\"%s\"", SHADER_INCLUDE_DIR);
        }
    }

    printf("  %s -> %s ", filename, path_get_filename(out_spv_path));

    int res = system(cmd);
    if (res != 0) {
        printf("[FAILURE]\n");
        return false;
    }

    FILE* f = fopen(out_spv_path, "rb");
    if (!f) {
        printf("[FAILURE] (Could not open output SPV)\n");
        return false;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fclose(f);

    printf("[SUCCESS] (%ld bytes)\n", size);
    return true;
}

/// @brief writes a single bundled header containing all compiled stages for a stem
bool generate_bundled_header(const ShaderBundle* bundle) {
    if (bundle->stage_count == 0) return false;

    char header_path[MAX_PATH_LEN];
    snprintf(header_path, sizeof(header_path), "%s/%s.h", HEADER_DIR, bundle->stem);

    FILE* out = fopen(header_path, "w");
    if (!out) {
        printf("    Header generation failed: Cannot write %s\n", header_path);
        return false;
    }

    // upper guard macro name (e.g. SPRITE_DEFAULT_H)
    char guard[300];
    snprintf(guard, sizeof(guard), "%s_H", bundle->stem);
    for (size_t i = 0; guard[i]; i++) {
        guard[i] = (char)toupper((unsigned char)guard[i]);
    }

    fprintf(out, "// Auto-generated bundled shader header for '%s'\n", bundle->stem);
    fprintf(out, "#ifndef %s\n", guard);
    fprintf(out, "#define %s\n\n", guard);
    fprintf(out, "/* HEADER_ONLY_ASSEMBLER_START */\n\n");
    fprintf(out, "#ifdef __cplusplus\nextern \"C\" {\n#endif\n\n");

    for (size_t s = 0; s < bundle->stage_count; s++) {
        const CompiledStage* stage = &bundle->stages[s];

        FILE* f = fopen(stage->spv_path, "rb");
        if (!f) {
            printf("    Warning: Skip missing SPV %s\n", stage->spv_path);
            continue;
        }

        fseek(f, 0, SEEK_END);
        long raw_size = ftell(f);
        fseek(f, 0, SEEK_SET);

        if (raw_size <= 0) {
            fclose(f);
            continue;
        }

        size_t padded_size = (raw_size + 3) & ~3;
        uint8_t* buffer = (uint8_t*)calloc(1, padded_size);
        if (!buffer) {
            fclose(f);
            continue;
        }

        fread(buffer, 1, raw_size, f);
        fclose(f);

        size_t uint32_count = padded_size / 4;
        uint32_t* uint32_data = (uint32_t*)buffer;

        // construct array variable identifier
        char var_name[280];
        snprintf(var_name, sizeof(var_name), "%s_%s_spv", bundle->stem, stage->stage_tag);

        fprintf(out, "// Stage: %s\n", stage->stage_tag);
        fprintf(out, "const unsigned int %s[] = {\n", var_name);

        for (size_t i = 0; i < uint32_count; i++) {
            if (i % 8 == 0) fprintf(out, "    ");
            
            uint32_t val = uint32_data[i];
            fprintf(out, "0x%08x", val);

            if (i < uint32_count - 1) fprintf(out, ", ");
            if ((i + 1) % 8 == 0 || i == uint32_count - 1) fprintf(out, "\n");
        }

        fprintf(out, "};\n");
        fprintf(out, "const unsigned int %s_size = %zu;\n\n", var_name, uint32_count);

        free(buffer);
    }

    fprintf(out, "#ifdef __cplusplus\n}\n#endif\n\n");
    fprintf(out, "/* HEADER_ONLY_ASSEMBLER_STOP */\n");
    fprintf(out, "#endif // %s\n", guard);

    fclose(out);
    printf("  Source: %s.h\n", bundle->stem);
    return true;
}

int main(void) {
    const char* shaders[] = {
        "shaders/billboard_default.vert",    "shaders/billboard_default.frag",
        "shaders/billboard_picking.vert",    "shaders/billboard_picking.frag",
        "shaders/grid.vert",                 "shaders/grid.frag",
        "shaders/sprite_default.vert",       "shaders/sprite_default.frag",
        "shaders/sprite_picking.vert",       "shaders/sprite_picking.frag",
    };
    size_t shader_count = sizeof(shaders) / sizeof(shaders[0]);

    if (shader_count == 0) {
        printf("No input shaders provided!\n");
        return 1;
    }

    printf("\n Processing %zu shader(s):\n", shader_count);
    for (size_t i = 0; i < shader_count; i++) {
        printf("  %s\n", shaders[i]);
    }

    CompilerType compiler = find_compiler();
    if (compiler == COMPILER_NONE) {
        printf("ERROR: No GLSL compiler found! Please install glslc or glslangValidator.\n");
        return 1;
    }

    // dynamic list of bundles
    ShaderBundle* bundles = NULL;
    size_t bundle_count = 0;

    printf("\nCompiling Shaders:\n");
    for (size_t i = 0; i < shader_count; i++) {
        char spv_path[MAX_PATH_LEN];
        char stage_tag[32];

        if (compile_shader(compiler, shaders[i], spv_path, sizeof(spv_path), stage_tag, sizeof(stage_tag))) {
            const char* filename = path_get_filename(shaders[i]);
            char stem[256];
            path_get_stem(filename, stem, sizeof(stem));

            // find existing bundle with matching stem or add a new one
            ShaderBundle* target_bundle = NULL;
            for (size_t b = 0; b < bundle_count; b++) {
                if (strcmp(bundles[b].stem, stem) == 0) {
                    target_bundle = &bundles[b];
                    break;
                }
            }

            if (!target_bundle) {
                bundles = (ShaderBundle*)realloc(bundles, (bundle_count + 1) * sizeof(ShaderBundle));
                target_bundle = &bundles[bundle_count++];
                strncpy(target_bundle->stem, stem, sizeof(target_bundle->stem) - 1);
                target_bundle->stem[sizeof(target_bundle->stem) - 1] = '\0';
                target_bundle->stage_count = 0;
            }

            if (target_bundle->stage_count < MAX_STAGES_PER_BUNDLE) {
                CompiledStage* stage = &target_bundle->stages[target_bundle->stage_count++];
                strncpy(stage->stage_tag, stage_tag, sizeof(stage->stage_tag) - 1);
                stage->stage_tag[sizeof(stage->stage_tag) - 1] = '\0';
                strncpy(stage->spv_path, spv_path, sizeof(stage->spv_path) - 1);
                stage->spv_path[sizeof(stage->spv_path) - 1] = '\0';
            }
        }
    }

    printf("\nGenerating Bundled Headers:\n");
    for (size_t b = 0; b < bundle_count; b++) {
        generate_bundled_header(&bundles[b]);
    }

    free(bundles);
    return 0;
}