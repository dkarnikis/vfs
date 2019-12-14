#include <stdio.h>
#include "vfs.h"

int
main(int argc, char **argv)
{
    struct vfs_instance *vfs;
    vfs = vfs_setup();
    vfs_store_file_root(vfs, "lala", "dedomena", 8);
    vfs_store_file_root(vfs, "lolz", "dedomxn2", 8);
    vfs_store_file_root(vfs, "lolo", "dedomen2", 8);
    vfs_store_file_root(vfs, "lolx", "dedomen2", 8);
    vfs_print_files(vfs, "root");
    vfs_read_file(vfs, "lala", "root");
    vfs_read_file(vfs, "lolz", "root");
    vfs_read_file(vfs, "lolo", "root");
    vfs_read_file(vfs, "lolx", "root");
}
