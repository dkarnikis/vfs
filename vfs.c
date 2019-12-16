#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include "vfs.h"
#define MEM_ERROR "Failed to allocate memory\n"
#define DIR_ERROR "Directory does not exist\n"
#define NULL_ERROR "NULL value\n"
#define ZERO_ERROR "0 value\n"

VFS_API void
error(void *p, const char *f, int l, const char *fi) {
    fprintf(stderr, "Error occured at %s function at line %d at file %s with value %p\n", 
            f, l, fi, p);
    abort();
}
int debug = 1;
#define print_dashes printf("--------------------------------------------------\n")
#define err(a) error(a, __FUNCTION__, __LINE__, __FILE__)
#define debug_print(a, ...) \
    if (debug == 1) \
        printf(a, __VA_ARGS__)

#define check_err(a, b) \
    if (a == 0x0) { \
        err(b); \
    } 
struct vfs_directory *
vfs_create_directory(char *directory)
{
    struct vfs_directory *f;
    check_err(directory, NULL_ERROR);
    f = (struct vfs_directory *)calloc(1, sizeof(struct vfs_directory));
    check_err(f, MEM_ERROR);
    f->name = strdup(directory);
    check_err(f->name, MEM_ERROR);
    f->next = NULL;
    return f;
}

VFS_API struct vfs_instance *
vfs_setup(void)
{
    struct vfs_instance *i;
    i = (struct vfs_instance *)calloc(1, sizeof(struct vfs_instance));
    check_err(i, MEM_ERROR);
    return i;
}

VFS_API void
vfs_new_file(struct vfs_instance *i, char *file_name, char *directory_name, 
            void *data, int data_len)
{
    struct vfs_directory *f;
    struct vfs_entry *e, *tmp;
    check_err(i, NULL_ERROR);
    check_err(file_name, NULL_ERROR);
    check_err(directory_name, NULL_ERROR);
    f = vfs_find_directory(i, directory_name);
    if (!f) {
        debug_print("%s", "Directory does not exist\n");
        return;
    }
    debug_print("File entries = %d\n", f->file_entries);
    if (f->files == NULL) {
        f->files = (struct vfs_entry *)calloc(1, sizeof(struct vfs_entry));
        check_err(f->files, MEM_ERROR);
        f->files->file_name = strdup(file_name);
        check_err(f->files->file_name, MEM_ERROR);
        f->files->data = (void *)calloc(sizeof(char), data_len);
        check_err(f->files->data, MEM_ERROR);
        memcpy(f->files->data, data, data_len);
        f->files->directory_name = strdup(directory_name);
        f->files->data_len = data_len;
        f->file_entries++;
    } else {
        /* insert the new item at the start of the directory */
        tmp = (struct vfs_entry *)calloc(1, sizeof(struct vfs_entry));
        check_err(tmp, MEM_ERROR);
        tmp->file_name = strdup(file_name);
        check_err(tmp->file_name, MEM_ERROR);
        tmp->directory_name = strdup(directory_name);
        check_err(tmp->file_name, MEM_ERROR);
        tmp->data = (void *)calloc(sizeof(char), data_len);
        check_err(tmp->data, MEM_ERROR);
        memcpy(tmp->data, data, data_len);
        tmp->data_len = data_len;
        f->file_entries++;
        e = f->files;
        tmp->next = e;
        f->files = tmp;
    }
}

VFS_API struct vfs_entry *
vfs_read_file(struct vfs_instance *i, char *file_name, char *directory_name)
{
    int j;
    struct vfs_directory *f;
    struct vfs_entry *tmp;
    check_err(i, NULL_ERROR);
    check_err(file_name, NULL_ERROR);
    check_err(directory_name, NULL_ERROR);
    f = vfs_find_directory(i, directory_name);
    if (!f) {
        debug_print("%s", "Directory does not exist\n");
        return NULL;
    }
    tmp = f->files;
    debug_print("Entries = %d\n", f->file_entries);
    for (j = 0; j < f->file_entries; j++) {
        if (memcmp(file_name, tmp->file_name, strlen(file_name)) == 0) {
            vfs_print_entry_info(tmp);
            return tmp;
        }
        tmp = tmp->next;
    }
    return NULL;
}

VFS_API void
vfs_print_entry_info(struct vfs_entry *e)
{
    print_dashes;
    printf("file name: %s\n", e->file_name);
    printf("file data: %.*s\n", e->data_len, (char *)e->data);
    printf("data len : %d\n", e->data_len);
    printf("directory name: %s\n", e->directory_name);
    print_dashes;
}

VFS_API void
vfs_delete_file(struct vfs_instance *i, char *file_name, char *directory_name)
{
    int j;
    struct vfs_directory *f;
    struct vfs_entry *e, *last;
    check_err(i, NULL_ERROR);
    check_err(file_name, NULL_ERROR);
    check_err(directory_name, NULL_ERROR);
    f = vfs_find_directory(i, directory_name);
    if (!f) {
        debug_print("%s", "Directory does not exist\n");
        return ;
    }
    e = f->files;
    last = e;
    for (j = 0; j < f->file_entries; j++) {
        if (memcmp(file_name, e->file_name, strlen(file_name)) == 0) {
            if (f->files == e) {
                f->files = e->next;
            } else {
                last->next = e->next;
            }
            free(e->file_name);
            free(e->data);
            free(e->directory_name);
            free(e);
            f->file_entries--;
            break;
        }
        last = e;
        e = e->next;
    }
    debug_print("File deleted %s\n", file_name);
}

VFS_API void
vfs_print_directory_files(struct vfs_instance *i, char *directory_name)
{
    int j;
    struct vfs_directory *f;
    struct vfs_entry *e;
    check_err(i, NULL_ERROR);
    check_err(directory_name, NULL_ERROR);
    printf("Printing files for directory %s\n", directory_name);
    f = vfs_find_directory(i, directory_name);
    if (!f) {
        debug_print("%s", "Directory does not exist\n");
        return;
    }
    if (f->file_entries == 0) {
        printf("Directory empty\n");
        print_dashes;
        return ;
    }
    e = f->files;
    for (j = 0; j < f->file_entries; j++) {
        vfs_print_entry_info(e);
        e = e->next;
    }
    print_dashes;
}

VFS_API void
vfs_new_directory(struct vfs_instance *i, char *directory_name)
{
    struct vfs_directory *f, *tmp;
    int j;
    check_err(i, NULL_ERROR);
    check_err(directory_name, NULL_ERROR);
    tmp = i->dirs;
    i->dirs = vfs_create_directory(directory_name);
    i->dirs->next = tmp;
    i->directory_num++;
    f = i->dirs;
    for (j = 0; j < i->directory_num; j++) {
        printf("directory %s\n", f->name);
        f = f->next;
    }
}

VFS_API void
vfs_delete_directory(struct vfs_instance *i, char *directory_name)
{
    int j, f_entries;
    struct vfs_directory *f, *last_dir;
    struct vfs_entry *e, *tmp;
    check_err(i, NULL_ERROR);
    check_err(directory_name, NULL_ERROR);
    f = i->dirs;
    last_dir = i->dirs;
    for (j = 0; j < i->directory_num; j++) {
        if (memcmp(directory_name, f->name, strlen(f->name)) == 0) {
            e = f->files;
            f_entries = f->file_entries;
            for (j = 0; j < f_entries; j++) {
                tmp = e->next;
                debug_print("Deleting file %s\n", e->file_name);
                free(e->file_name);
                free(e->data);
                free(e->directory_name);
                free(e);
                f->file_entries--;
                e = tmp;
            }
            if (last_dir == i->dirs) {
                i->dirs = f->next;
            } else {
                last_dir->next = f->next;
            }   
            free(f->name);
            free(f);

            i->directory_num--;
            break;
        }

        last_dir = f;
        f = f->next;
    }
}

VFS_API void
vfs_delete_instance(struct vfs_instance *i)
{
    check_err(i, NULL_ERROR);
    int j;
    int dir_entries;
    struct vfs_directory *f, *next;
    dir_entries = i->directory_num;
    f = i->dirs;
    printf("Number of directories %d\n", dir_entries);
    for (j = 0; j < dir_entries; j++) {
        next = f->next;
        vfs_delete_directory(i, f->name);
        f = next;
    }
    free(i);
}

VFS_API struct vfs_directory *
vfs_find_directory(struct vfs_instance *i, char *directory_name)
{
    int j;
    struct vfs_directory *f;
    f = i->dirs;
    for (j = 0; j < i->directory_num; j++) {
        if (memcmp(directory_name, f->name, strlen(f->name)) == 0) {
            return f;
        }
        f = f->next;
    }
    return NULL;
}

VFS_API struct vfs_entry *
vfs_find_file(struct vfs_instance *i, char *file_name, char *directory_name)
{
    int j;
    struct vfs_directory *f;
    struct vfs_entry *e;
    f = vfs_find_directory(i, directory_name);
    if (!f)
        return NULL;
    e = f->files;
    for (j = 0; j < f->file_entries; j++) {
        if (memcmp(file_name, e->file_name, strlen(file_name)) == 0) {
            return e;
        }
        e = e->next;
    }
    return NULL;
}

VFS_API void
vfs_append_to_file(struct vfs_instance *i, char *file_name, char *directory_name,
            void *data, int len)
{
    struct vfs_entry *e;
    check_err(i, NULL_ERROR);
    check_err(file_name, NULL_ERROR);
    check_err(directory_name, NULL_ERROR);
    check_err(data, NULL_ERROR);
    check_err(len, ZERO_ERROR);
    e = vfs_find_file(i, file_name, directory_name);
    if (!e)
        return;
    e->data = realloc(e->data, len + e->data_len);
    check_err(e->data, NULL_ERROR);
    memcpy(&((char *)(e->data))[e->data_len], data, len);
    e->data_len += len;
}

VFS_API void vfs_write_at_file(struct vfs_instance *i, char *file_name, 
            char *directory_name, void *data, int len, int pos)
{
    struct vfs_entry *e;
    check_err(i, NULL_ERROR);
    check_err(file_name, NULL_ERROR);
    check_err(directory_name, NULL_ERROR);
    check_err(data, NULL_ERROR);
    check_err(len, ZERO_ERROR);
    e = vfs_find_file(i, file_name, directory_name);
    if (!e)
        return;
    memcpy(&((char *)(e->data))[pos], data, len);
}
