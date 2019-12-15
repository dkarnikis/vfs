#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "vfs.h"

#define MEM_ERROR "Failed to allocate memory\n"
#define DIR_ERROR "Directory does not exist\n"
#define NULL_VALUE "NULL value\n"

VFS_API void
error(void *p, const char *f, int l) {
    fprintf(stderr, "Error occured at %s f at line %d with value %p\n", f, l, p);
    abort();
}

#define print_dashes printf("--------------------------------------------------\n")
#define err(a) error(a, __FUNCTION__,__LINE__)
#define debug_print printf
#define check_err(a, b) \
    if (a == NULL) { \
        err(b); \
    } 
struct vfs_directory *
vfs_create_directory(char *directory)
{
    struct vfs_directory *f;
    check_err(directory, NULL_VALUE);
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
    i->dirs = vfs_create_directory("root");
    i->directory_num++;
    return i;
}

VFS_API void
vfs_store_file_root(struct vfs_instance *i, char *file_name, void *data, int len)
{
    vfs_store_file(i, file_name, "root", data, len);
}

VFS_API void
vfs_store_file(struct vfs_instance *i, char *file_name, char *directory_name, void *data, int data_len)
{
    int j;
    struct vfs_directory *f;
    struct vfs_entry *e, *tmp;
    f = NULL;
    check_err(i, NULL_VALUE);
    check_err(file_name, NULL_VALUE);
    check_err(directory_name, NULL_VALUE);
    for (j = 0; j < i->directory_num; j++) {
        f = i->dirs;
        if (memcmp(directory_name, f->name, strlen(directory_name)) == 0) {
            debug_print("Found the requested directory to store the file\n");
            break;
        }
        f = f->next;
    }
    check_err(f, DIR_ERROR);
    debug_print("File entries = %d\n", f->file_entries);
    if (f->files == NULL) {
        f->files = (struct vfs_entry *)calloc(1, sizeof(struct vfs_entry));
        f->files->file_name = strdup(file_name);
        check_err(f->files->file_name, MEM_ERROR);
        f->files->file_data = (void *)calloc(sizeof(char), data_len);
        check_err(f->files->file_data, MEM_ERROR);
        memcpy(f->files->file_data, data, data_len);
        f->files->directory_name = strdup(directory_name);
        f->files->data_len = data_len;
        f->file_entries++;
    } else {
        /* insert the new item at the start of the directory */
        tmp = (struct vfs_entry *)calloc(1, sizeof(struct vfs_entry));
        tmp->file_name = strdup(file_name);
        tmp->directory_name = strdup(directory_name);
        check_err(tmp->file_name, MEM_ERROR);
        tmp->file_data = (void *)calloc(sizeof(char), data_len);
        check_err(tmp->file_data, MEM_ERROR);
        memcpy(tmp->file_data, data, data_len);
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
    check_err(i, NULL_VALUE);
    check_err(file_name, NULL_VALUE);
    check_err(directory_name, NULL_VALUE);
    f = NULL;
    for (j = 0; j < i->directory_num; j++) {
        f = i->dirs;
        if (memcmp(directory_name, f->name, strlen(directory_name)) == 0) {
            debug_print("Found the requested directory to load the file\n");
            debug_print("%s  %s\n", f->name, directory_name);
            break;
        }
        f = f->next;
    }
    tmp = f->files;
    printf("Entries = %d\n", f->file_entries);
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
    printf("file data: %.*s\n", e->data_len, (char *)e->file_data);
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
    check_err(i, NULL_VALUE);
    check_err(file_name, NULL_VALUE);
    check_err(directory_name, NULL_VALUE);
    f = NULL;
    for (j = 0; j < i->directory_num; j++) {
        f = i->dirs;
        if (memcmp(directory_name, f->name, strlen(f->name)) == 0) {
            break;
        }
        f = f->next;
    }
    check_err(f, DIR_ERROR);
    e = f->files;
    last = e;
    for (j = 0; j < f->file_entries; j++) {
        printf("Comparing %s with %s\n", file_name, e->file_name);
        if (memcmp(file_name, e->file_name, strlen(file_name)) == 0) {
            printf("Found entry\n");
            if (f->files == e) {
                printf("Our entry is at the head\n");
                f->files = e->next;
            } else {
                last->next = e->next;
            }
            free(e->file_name);
            free(e->file_data);
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
    check_err(i, NULL_VALUE);
    check_err(directory_name, NULL_VALUE);
    printf("Printing files for directory %s\n", directory_name);
    f = i->dirs;
    for (j = 0; j < i->directory_num; j++) {
        if (memcmp(directory_name, f->name, strlen(f->name)) == 0) {
            e = f->files;
            for (j = 0; j < f->file_entries; j++) {
                vfs_print_entry_info(e);
                e = e->next;
            }
            break;
        }
        f = f->next;
    }
    print_dashes;
}

VFS_API void
vfs_new_directory(struct vfs_instance *i, char *directory_name)
{
    struct vfs_directory *f, *tmp;
    int j;
    check_err(i, NULL_VALUE);
    check_err(directory_name, NULL_VALUE);
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
    check_err(i, NULL_VALUE);
    check_err(directory_name, NULL_VALUE);
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
                free(e->file_data);
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
    check_err(i, NULL_VALUE);
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
