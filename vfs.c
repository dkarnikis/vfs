#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "vfs.h"

VFS_API void
error(const char *f, int l) {
    fprintf(stderr, "Error occured at %s f at line %d\n", f, l);
    abort();
}

#define err error(__FUNCTION__,__LINE__)
#define debug_print printf
struct vfs_folder *
vfs_create_folder(char *folder)
{
    struct vfs_folder *f;
    f = (struct vfs_folder *)calloc(1, sizeof(struct vfs_folder));
    if (f == NULL) {
        err;
    }
    f->name = strdup(folder);
    if (f->name == NULL) {
        err;
    }
    f->next = NULL;
    return f;
}

VFS_API struct vfs_instance *
vfs_setup(void)
{
    struct vfs_instance *i;
    i = (struct vfs_instance *)calloc(1, sizeof(struct vfs_instance));
    if (i == NULL) {
        err;
    }
    i->folders = vfs_create_folder("root");
    i->folder_num++;
    return i;
}

VFS_API void
vfs_store_file_root(struct vfs_instance *i, char *file_name, void *data, int len)
{
    vfs_store_file(i, file_name, "root", data, len);
}

VFS_API void
vfs_store_file(struct vfs_instance *i, char *file_name, char *folder_name, void *data, int data_len)
{
    assert(i);
    assert(file_name);
    assert(folder_name);
    int j;
    struct vfs_folder *f;
    struct vfs_entry *e, *tmp;
    for (j = 0; j < i->folder_num; j++) {
        f = i->folders;
        if (memcmp(folder_name, f->name, strlen(folder_name)) == 0) {
            debug_print("Found the requested folder to store the file\n");
            break;
        }
        f = f->next;
    }
    if (f == NULL) {
        debug_print("Folder does not exist\n");
        abort();
    }
    debug_print("File entries = %d\n", f->file_entries);
    if (f->files == NULL) {
        f->files = (struct vfs_entry *)calloc(1, sizeof(struct vfs_entry));
        f->files->file_name = strdup(file_name);
        if (f->files->file_name == NULL)
            err;
        f->files->file_data = (void *)calloc(sizeof(char), data_len);
        if (f->files->file_data == NULL)
            err;
        memcpy(f->files->file_data, data, data_len);
        f->files->folder_name = strdup(folder_name);
        f->files->data_len = data_len;
        f->file_entries++;
    } else {
        /* insert the new item at the start of the folder */
        tmp = (struct vfs_entry *)calloc(1, sizeof(struct vfs_entry));
        tmp->file_name = strdup(file_name);
        tmp->folder_name = strdup(folder_name);
        if (tmp->file_name == NULL)
            err;
        tmp->file_data = (void *)calloc(sizeof(char), data_len);
        if (tmp->file_data == NULL)
            err;
        memcpy(tmp->file_data, data, data_len);
        tmp->data_len = data_len;
        f->file_entries++;
        e = f->files;
        tmp->next = e;
        f->files = tmp;
    }
}

VFS_API void
vfs_read_file(struct vfs_instance *i, char *file_name, char *folder_name)
{
    assert(i);
    assert(file_name);
    assert(folder_name);
    int j;
    struct vfs_folder *f;
    struct vfs_entry *tmp;
    for (j = 0; j < i->folder_num; j++) {
        f = i->folders;
        if (memcmp(folder_name, f->name, strlen(folder_name)) == 0) {
            debug_print("Found the requested folder to load the file\n");
            debug_print("%s  %s\n", f->name, folder_name);
            break;
        }
        f = f->next;
    }
    tmp = f->files;
    printf("Entries = %d\n", f->file_entries);
    for (j = 0; j < f->file_entries; j++) {
        if (memcmp(file_name, tmp->file_name, strlen(file_name)) == 0) {
            vfs_print_entry_info(tmp);
            break;
        }
        tmp = tmp->next;
    }
}


VFS_API void
vfs_print_entry_info(struct vfs_entry *e)
{
    printf("--------------------------------------\n");
    printf("file name: %s\n", e->file_name);
    printf("file data: %.*s\n", e->data_len, (char *)e->file_data);
    printf("data len : %d\n", e->data_len);
    printf("folder name: %s\n", e->folder_name);
    printf("--------------------------------------\n");
}

VFS_API void
vfs_delete_file(struct vfs_instance *i, char *file_name, char *folder_name)
{
    assert(i);
    assert(file_name);
    assert(folder_name);
    int j;
    struct vfs_folder *f;
    struct vfs_entry *e, *last;
    f = NULL;
    for (j = 0; j < i->folder_num; j++) {
        f = i->folders;
        if (memcmp(folder_name, f->name, strlen(f->name)) == 0) {
            break;
        }

    }
    if (f == NULL) {
        err;
        abort();
    }
    e = f->files;
    last = e;
    for (j = 0; j < f->file_entries; j++) {
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
            free(e->folder_name);
            free(e);
            f->file_entries--;
            break;
        }
        last = e;
        e = e->next;
    }
    printf("f entries = %d\n", f->file_entries);
}


VFS_API void
vfs_print_folder_files(struct vfs_instance *i, char *folder_name)
{
    assert(i);
    assert(folder_name);
    int j;
    struct vfs_folder *f;
    struct vfs_entry *e;
    printf("Printing %s files\n", folder_name);
    f = i->folders;
    for (j = 0; j < i->folder_num; j++) {
        if (memcmp(folder_name, f->name, strlen(f->name)) == 0) {
            e = f->files;
            for (j = 0; j < f->file_entries; j++) {
                vfs_print_entry_info(e);
                e = e->next;

            }
            break;
        }
        f = f->next;

    }
    printf("--------------------------------\n");
}

VFS_API void
vfs_new_folder(struct vfs_instance *i, char *folder_name)
{
    assert(i);
    assert(folder_name);
    struct vfs_folder *f, *tmp;
    int j;
    tmp = i->folders;
    i->folders = vfs_create_folder(folder_name);
    i->folders->next = tmp;
    i->folder_num++;
    f = i->folders;
    for (j = 0; j < i->folder_num; j++) {
        printf("Fodler %s\n", f->name);
        f = f->next;

    }

}
