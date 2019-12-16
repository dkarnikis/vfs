#include <stdio.h>
#include "vfs.h"

int
main(int argc, char **argv)
{
    (void)(argc);
    (void)(argv);
    struct vfs_instance *vfs;
    vfs = vfs_setup();
    vfs_new_directory(vfs, "root");
    vfs_new_directory(vfs, "root2");
    vfs_new_file(vfs, "dimitris", "root", "hahaxd", 6);
    vfs_read_file(vfs, "dimitris", "root");
    vfs_append_to_file(vfs, "dimitris", "root", "dimitris", 8);
    vfs_append_to_file(vfs, "dimitris", "root", "dimitris", 8);
    vfs_read_file(vfs, "dimitris", "root");
    vfs_write_at_file(vfs, "dimitris", "root", "xd", 2, 10);
    vfs_read_file(vfs, "dimitris", "root");
    vfs_print_directory_files(vfs, "root2");
}
