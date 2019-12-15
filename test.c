#include <stdio.h>
#include "vfs.h"

int
main(int argc, char **argv)
{
    (void)(argc);
    (void)(argv);
    struct vfs_instance *vfs;
    struct vfs_entry *e;
    vfs = vfs_setup();
    vfs_new_directory(vfs, "roo1");
    vfs_new_directory(vfs, "roo1dsd");
    vfs_new_directory(vfs, "roo22");
    vfs_store_file(vfs, "hahahaha", "root", "ahssahdahsdahsdahdsasdhasdaha", 10);
    vfs_store_file(vfs, "hahahaha1", "root", "ahssahdahsdahsdahdsasdhasdaha", 15);
    e = vfs_read_file(vfs, "hahahaha1", "root");
    vfs_delete_file(vfs, "hahahaha1", "root");
    vfs_print_directory_files(vfs, "root");
    vfs_print_directory_files(vfs, "roo1");
    vfs_delete_directory(vfs, "roo1");
    vfs_print_directory_files(vfs, "roo1");
    vfs_delete_instance(vfs);
}
