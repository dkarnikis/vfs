#ifndef VFS_H
#define VFS_H
#define VFS_API extern

struct vfs_instance {
    int files;
    int directory_num;
    struct vfs_directory *dirs;
}__attribute__ ((packed));

struct vfs_directory {
    char *name;
    int file_entries;
    struct vfs_directory *next;
    struct vfs_entry *files;
}__attribute__ ((packed));

struct vfs_entry {
    char *file_name;
    void *file_data;
    int data_len;
    char *directory_name;
    struct vfs_entry *next;
}__attribute__ ((packed));

VFS_API struct vfs_instance *vfs_setup(void);
VFS_API struct vfs_directory *vfs_create_directory(char *directory_name);
VFS_API void vfs_new_directory(struct vfs_instance *i, char *directory_name); 
VFS_API void vfs_print_directory_files(struct vfs_instance *i, char *directory_name);
VFS_API void vfs_store_file_root(struct vfs_instance *i, char *file_name, void *data, int len);
VFS_API void vfs_store_file(struct vfs_instance *i, char *file_name, char *directory_name, void *data, int len);
VFS_API void vfs_delete_file(struct vfs_instance *i, char *file_name, char *directory_name);
VFS_API struct vfs_entry *vfs_read_file(struct vfs_instance *i, char *file_name, char *directory_name);
VFS_API void vfs_print_entry_info(struct vfs_entry *e);
VFS_API void vfs_delete_directory(struct vfs_instance *i, char *directory_name);
VFS_API void vfs_delete_instance(struct vfs_instance *i);
#endif
