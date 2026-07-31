#include "toolbox_util.h"
#include <string.h>
#include <stddef.h>

/* HEADER_ONLY_ASSEMBLER_START */

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Internal helper to find the last path separator ('/' or '\')
static TOOLBOX_FUNC const char* find_last_separator(const char* path) {
    if (!path) return NULL;

    const char* last_slash = strrchr(path, '/');
    const char* last_backslash = strrchr(path, '\\');

    if (last_slash && last_backslash) {
        return (last_slash > last_backslash) ? last_slash : last_backslash;
    }
    return last_slash ? last_slash : last_backslash;
}

/// @brief Internal helper to safely copy up to (dest_size - 1) bytes with null termination
static TOOLBOX_FUNC bool safe_strcpy(char* dest, const char* src, size_t dest_size) {
    if (!dest || dest_size == 0) return false;
    if (!src) {
        dest[0] = '\0';
        return false;
    }

    size_t src_len = strlen(src);
    bool truncated = (src_len >= dest_size);
    size_t copy_len = truncated ? (dest_size - 1) : src_len;

    memcpy(dest, src, copy_len);
    dest[copy_len] = '\0';

    return !truncated;
}

TOOLBOX_FUNC bool filename_from_path(const char* path, char* outBuffer, size_t bufferSize) {
    if (!outBuffer || bufferSize == 0) return false;

    if (!path) {
        outBuffer[0] = '\0';
        return false;
    }

    const char* separator = find_last_separator(path);
    const char* filename = separator ? separator + 1 : path;

    if (*filename == '\0') {
        outBuffer[0] = '\0';
        return false;
    }

    return safe_strcpy(outBuffer, filename, bufferSize);
}

TOOLBOX_FUNC bool extension_from_path(const char* path, char* outBuffer, size_t bufferSize) {
    if (!outBuffer || bufferSize == 0) return false;

    if (!path) {
        outBuffer[0] = '\0';
        return false;
    }

    const char* separator = find_last_separator(path);
    const char* filename = separator ? separator + 1 : path;

    if (*filename == '\0') {
        outBuffer[0] = '\0';
        return false;
    }

    const char* dot = strrchr(filename, '.');

    // ignore leading dots (e.g., hidden files like ".gitignore")
    if (dot && dot != filename) {
        return safe_strcpy(outBuffer, dot, bufferSize);
    }

    outBuffer[0] = '\0';
    return false;
}

TOOLBOX_FUNC bool basename_from_path(const char* path, char* outBuffer, size_t bufferSize) {
    if (!outBuffer || bufferSize == 0) return false;

    if (!path) {
        outBuffer[0] = '\0';
        return false;
    }

    const char* separator = find_last_separator(path);
    const char* filename = separator ? separator + 1 : path;

    if (*filename == '\0') {
        outBuffer[0] = '\0';
        return false;
    }

    const char* dot = strrchr(filename, '.');

    if (dot && dot != filename) {
        size_t basename_len = (size_t)(dot - filename);
        bool truncated = (basename_len >= bufferSize);
        size_t copy_len = truncated ? (bufferSize - 1) : basename_len;

        memcpy(outBuffer, filename, copy_len);
        outBuffer[copy_len] = '\0';
        return !truncated;
    }

    return safe_strcpy(outBuffer, filename, bufferSize);
}

TOOLBOX_FUNC bool dirname_from_path(const char* path, char* outBuffer, size_t bufferSize) {
    if (!outBuffer || bufferSize == 0) return false;

    if (!path || *path == '\0') {
        outBuffer[0] = '\0';
        return false;
    }

    const char* separator = find_last_separator(path);

    if (!separator) {
        return safe_strcpy(outBuffer, ".", bufferSize);
    }

    size_t dir_len = (size_t)(separator - path);

    // handle consecutive separators (e.g., "path//file.txt")
    while (dir_len > 0 && (path[dir_len - 1] == '/' || path[dir_len - 1] == '\\')) {
        dir_len--;
    }

    if (dir_len == 0) {
        return safe_strcpy(outBuffer, "/", bufferSize);
    }

    bool truncated = (dir_len >= bufferSize);
    size_t copy_len = truncated ? (bufferSize - 1) : dir_len;

    memcpy(outBuffer, path, copy_len);
    outBuffer[copy_len] = '\0';
    return !truncated;
}

#ifdef __cplusplus
}
#endif

/* HEADER_ONLY_ASSEMBLER_STOP */