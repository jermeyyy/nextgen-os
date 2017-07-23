#ifndef INITRD_H
#define INITRD_H

#include "system.h"

typedef struct {
    unsigned int nfiles; // The number of files in the ramdisk.
} initrd_header_t;

typedef struct {
    unsigned char magic;     // Magic number, for error checking.
    char name[64];  // Filename.
    unsigned int offset;   // Offset in the initrd that the file starts.
    unsigned int length;   // Length of the file.
} initrd_file_header_t;

// Initialises the initial ramdisk. It gets passed the address of the multiboot module,
// and returns a completed filesystem node.

fs_node_t *initialise_initrd(unsigned int location);

void initrd_list();

void cat(char *arg);

#endif
