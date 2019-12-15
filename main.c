#include <stdio.h>
#include "vfs.h"

int
main(int argc, char **argv)
{
    struct vfs_instance *vfs;
    vfs = vfs_setup();
    vfs_new_folder(vfs, "roo1");
    vfs_store_file(vfs, "lala", "roo1", "dedomena", 8);
    vfs_print_folder_files(vfs, "roo1");
    vfs_print_folder_files(vfs, "roo1");
}
