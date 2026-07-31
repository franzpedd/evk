# Utilities

This directory contains utility tools and helper projects that facilitate the development, building, and packaging of the EVK library.

## List of Utilities

### headeronly_assembler.c

**Purpose**  
Generates a single-file header-only distribution of the EVK library by concatenating all public headers and implementation files into one cohesive header.

**Usage**
```bash
# from headeronly_assembler directory only
clang headeronly_assembler.c -o headeronly_assembler
./headeronly_assembler
```
---

### shader_dev.c

**Purpose**  
Compiles and generates a header containing the SPIR-V binary code of the shaders (located at ```shader_dev/shaders```). Must have either glslc or glslang on system's PATH

**Usage**
```bash
# from shader_dev directory only
clang shader_dev.c -o shader_dev
./shader_dev
```
---