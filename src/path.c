/*
 * Author      : Prajwal Chapagain <prajjwal2058@gmail.com>
 * Date        : Tuesday Sep 14, 2021 14:41:07 NPT
 * License     : MIT
 */

#include "path.h"

#include <stdlib.h>
#include <string.h>

void path_dbgln(char *pathname, struct Path *path)
{
    printf("pathname : %s\n"
           "---------------------\n"
           "dirname  : %.*s\n"
           "---------------------\n"
           "basename : %s\n"
           "---------------------\n"
           "filename : %.*s\n"
           "---------------------\n"
           "file ext : %s\n",
           pathname,
           (int)path->dirname_len,
           path->dirname,
           path->basename,
           (int)path->filename_len,
           path->filename,
           path->file_ext);
}

static char *skip_slash(char *path_ptr)
{
    size_t num_starting_slashes = strspn(path_ptr, "/");
    return (path_ptr + num_starting_slashes);
}

static void path_get_filename(struct Path *path)
{
    if (path->basename != NULL) {
        char *file_ext_dot = strchr(path->basename, '.');

        if (file_ext_dot != NULL) {
            path->filename_len = strnlen(path->basename, file_ext_dot - path->basename);
            path->filename = path->basename;

            path->file_ext_len = strlen(file_ext_dot);
            path->file_ext = file_ext_dot;
        }
    }
}

static void path_get_basename(char *pathname, char *last_slash, struct Path *path)
{
    if (last_slash == NULL) {
        path->basename_len = strlen(pathname);
        path->basename = pathname;
        return;
    }

    char *basensme_Wskip_slashes = skip_slash(last_slash);

    path->basename_len = strlen(basensme_Wskip_slashes);
    path->basename = basensme_Wskip_slashes;
}

static void path_get_dirname(char *pathname, char *last_slash, struct Path *path)
{
    if (path->dirname == NULL) {
        char *pathname_Wskip_slashes = skip_slash(pathname);

        path->dirname_len = strnlen(pathname_Wskip_slashes, last_slash - pathname_Wskip_slashes);
        path->dirname = pathname_Wskip_slashes;
    }
}

static char *path_check_remain_chars(char *pathname, char *last_slash)
{
    char *ptr_idx = last_slash;

    for (; ptr_idx != pathname; --ptr_idx)
        if (ptr_idx[-1] != '/')
            return ptr_idx;

    return ptr_idx;
}

struct Path path_parse(char *pathname)
{
    const static char DOT[] = ".";

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

    char *last_slash = strrchr(pathname, '/');
    char *ptr_idx = NULL;

    if (last_slash != NULL && last_slash == pathname && last_slash[1] == '\0') {
        path.dirname = (char *)DOT;
        return path;
    }

    if (last_slash == NULL && pathname[0] == '.' && pathname[1] == '\0') {
        path.dirname = pathname;
        return path;
    }

    // ignore the actual last useless slash and search another
    // ex: /root/base/
    if (last_slash != NULL && last_slash != pathname && last_slash[1] == '\0') {
        ptr_idx = path_check_remain_chars(pathname, last_slash);

        if (ptr_idx != pathname) {
            // remove the last useless slash/es on orginal pathname
            ptr_idx[0] = '\0';
            last_slash = memrchr(pathname, '/', ptr_idx - pathname);
        }
    }

    if (last_slash != NULL) {
        ptr_idx = path_check_remain_chars(pathname, last_slash);

        // there are multiple slashes at the begining of path
        // ex: //root/basename
        if (ptr_idx == pathname) {
            // ignore all of them and only return '.'
            path.dirname = (char *)DOT;
        } else {
            last_slash = ptr_idx;
        }
        
        path_get_dirname(pathname, last_slash, &path);
    } else {
        path.dirname = (char *)DOT;
    }

    path_get_basename(pathname, last_slash, &path);
    path_get_filename(&path);

    return path;
}
