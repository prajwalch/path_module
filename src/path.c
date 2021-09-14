/*
 * Author      : Prajwal Chapagain <prajjwal2058@gmail.com>
 * Date        : Tuesday Sep 14, 2021 14:41:07 NPT
 * License     : MIT
 */

#include "path.h"

#include <stdlib.h>
#include <string.h>

#define DOT "."

void path_dbgln(const char *pathname, struct Path *path)
{
    printf("pathname : %s\n"
           "     len : %zu\n"
           "---------------------\n"
           "dirname  : %s\n"
           "    len  : %zu\n"
           "---------------------\n"
           "basename : %s\n"
           "     len : %zu\n"
           "---------------------\n"
           "filename : %s\n"
           "     len : %zu\n"
           "---------------------\n"
           "file ext : %s\n"
           "     len : %zu\n",
           pathname,
           strlen(pathname),
           path->dirname,
           path->dirname_len,
           path->basename,
           path->basename_len,
           path->filename,
           path->filename_len,
           path->file_ext,
           path->file_ext_len);
}

void path_free_mem(struct Path *path)
{
    if (path->dirname_len != 1 &&
        (strncmp(path->dirname, DOT, 1) != 0)) {
        free(path->dirname);
        path->dirname = NULL;
        path->dirname_len = 0;
    }

    path->basename = NULL;
    path->basename_len = 0;

    if (path->filename != NULL) {
        free(path->filename);
        path->filename = NULL;
        path->filename_len = 0;

        path->file_ext = NULL;
        path->file_ext_len = 0;
    }
}

static void path_get_filename(struct Path *path)
{
    if (path->basename != NULL) {
        const char *file_ext_dot = strchr(path->basename, '.');
        
        if (file_ext_dot != NULL) {
            path->filename_len = strnlen(path->basename, file_ext_dot - path->basename);
            path->filename = malloc(sizeof(char) * path->filename_len + 1);

            memset(path->filename, '\0', path->filename_len);
            memcpy(path->filename, path->basename, path->filename_len);

            path->file_ext_len = strlen(file_ext_dot);
            path->file_ext = file_ext_dot;
        }
    }
}

static void path_get_basename(const char *pathname, const char *last_slash, struct Path *path)
{
    if (last_slash == NULL) {
        path->basename_len = strlen(pathname);
        path->basename = pathname;
        return;
    }

    ++last_slash;
    path->basename_len = strlen(last_slash);
    path->basename = last_slash;
}

static void path_get_dirname(const char *pathname, const char *last_slash, struct Path *path)
{
    if (path->dirname == NULL) {
        size_t num_starting_slashes = strspn(pathname, DOT);
        const char *pathname_Wskip_slashes = pathname + num_starting_slashes;

        path->dirname_len = strnlen(pathname_Wskip_slashes, last_slash - pathname_Wskip_slashes);
        path->dirname = malloc(sizeof(char) * path->dirname_len + 1);

        memset(path->dirname, '\0', path->dirname_len + 1);
        memcpy(path->dirname, pathname_Wskip_slashes, path->dirname_len);
    }
}

static const char *path_check_remain_chars(const char *pathname, const char *last_slash)
{
    const char *ptr_idx = last_slash;
    for (; ptr_idx != pathname; --ptr_idx)
        if (ptr_idx[-1] != '/')
            return ptr_idx;
    return ptr_idx;
}

struct Path path_parse(const char *pathname)
{
    struct Path path = {
        .dirname = NULL,
        .basename = NULL,
        .filename = NULL,
        .file_ext = NULL,
        .dirname_len = 1, // its always one
        .basename_len = 0,
        .filename_len = 0,
        .file_ext_len = 0
    };

    const char *last_slash = strrchr(pathname, '/');
    const char *ptr_idx = NULL;

    if (last_slash != NULL && last_slash == pathname && last_slash[1] == '\0') {
        path.dirname = DOT;
        return path;
    }

    if (last_slash == NULL && pathname[0] == '.' && pathname[1] == '\0') {
        path.dirname = pathname;
        return path;
    }

    // ex: /root/base/
    // ignore the actual last slash and search another
    if (last_slash != NULL && last_slash != pathname && last_slash[1] == '\0') {
        ptr_idx = path_check_remain_chars(pathname, last_slash);

        if (ptr_idx != pathname)
            last_slash = memrchr(pathname, '/', ptr_idx - pathname);
    }

    // ex: root/
    // after first condition executed
    if (last_slash != NULL) {

        ptr_idx = path_check_remain_chars(pathname, last_slash);

        // we have multiple slashes at the begining of path
        // ex: //root/basename
        if (ptr_idx == pathname) {
            // ignore all of them and only return '/'
            path.dirname = DOT;
        }
        else {
            last_slash = ptr_idx;
        }
        path_get_dirname(pathname, last_slash, &path);
    } else {
        path.dirname = DOT;
    }

    path_get_basename(pathname, last_slash, &path);
    path_get_filename(&path);

    return path;
}
