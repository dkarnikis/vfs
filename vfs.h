#ifndef VFS_H
#define VFS_H
#define VFS_API extern

struct vfs_instance {
    int files;
    int folder_num;
    char *name;
    struct vfs_folder *folders;
    int id_counter;
};

struct vfs_folder {
    char *name;
    int file_entries;
    struct vfs_folder *next;
    struct vfs_entry *files;
};

struct vfs_entry {
    char *file_name;
    void *file_data;
    char *folder_name;
    int data_len;
    int id;
    struct vfs_entry *next;
};
VFS_API struct vfs_instance *vfs_setup(void);
VFS_API struct vfs_folder *vfs_create_folder(char *folder_name);
VFS_API void vfs_ls_folder(struct vfs_instance *i, char *folder_name);
VFS_API void vfs_store_file_root(struct vfs_instance *i, char *file_name, void *data, int len);
VFS_API void vfs_store_file(struct vfs_instance *i, char *file_name, char *folder_name, void *data, int len);
VFS_API void vfs_delete_file(struct vfs_instance *i, char *file_name, char *folder_name);
VFS_API void vfs_read_file(struct vfs_instance *i, char *file_name, char *folder_name);
VFS_API void vfs_print_files(struct vfs_instance *i, char *folder_name);
VFS_API void vfs_print_vfs_entry_info(struct vfs_entry *e);

#endif
