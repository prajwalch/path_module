/*
 * Author      : Prajwal Chapagain <prajjwal2058@gmail.com>
 * Date        : Tuesday Sep 14, 2021 14:43:19 NPT
 * License     : MIT
 */

#ifndef PATH_H

#define PATH_H

#include <stddef.h>

struct Path {
    char *dirname;
    char *basename;
    //char *filename;
    //char *file_ext;
    size_t dirname_len;
    size_t basename_len;
    //size_t filename_len;
    //size_t file_ext_len;
};

void path_dbgln(char *pathname, struct Path *path);
struct Path path_parse(char *pathname);

#endif /* PATH_H */

