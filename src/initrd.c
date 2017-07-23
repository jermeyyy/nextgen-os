#include "system.h"

initrd_header_t *initrd_header;     //  header.
initrd_file_header_t *file_headers; // list of file headers.
fs_node_t *initrd_root;             //  root directory node.
fs_node_t *initrd_dev;              // directory node for /dev, so we can mount devfs later on.
fs_node_t *root_nodes;              // list of file nodes.
int nroot_nodes;                    // number of file nodes.

struct dirent dirent;

static unsigned int initrd_read(fs_node_t *node, unsigned int offset, unsigned int size, unsigned char *buffer) {
    initrd_file_header_t header = file_headers[node->inode];
    if (offset > header.length)
        return 0;
    if (offset + size > header.length)
        size = header.length - offset;
    memcpy(buffer, (unsigned char *) (header.offset + offset), size);
    return size;
}

static struct dirent *initrd_readdir(fs_node_t *node, unsigned int index) {
    if (node == initrd_root && index == 0) {
        strcpy(dirent.name, "dev");
        dirent.name[3] = 0;
        dirent.ino = 0;
        return &dirent;
    }

    if (index - 1 >= nroot_nodes)
        return 0;
    strcpy(dirent.name, root_nodes[index - 1].name);
    dirent.name[strlen(root_nodes[index - 1].name)] = 0;
    dirent.ino = root_nodes[index - 1].inode;
    return &dirent;
}

static fs_node_t *initrd_finddir(fs_node_t *node, char *name) {
    if (node == initrd_root &&
        !strcmp(name, "dev"))
        return initrd_dev;

    int i;
    for (i = 0; i < nroot_nodes; i++)
        if (!strcmp(name, root_nodes[i].name))
            return &root_nodes[i];
    return 0;
}

fs_node_t *initialise_initrd(unsigned int location) {
    // Initialise the main and file header pointers and populate the root directory.
    initrd_header = (initrd_header_t *) location;
    file_headers = (initrd_file_header_t *) (location + sizeof(initrd_header_t));

    // Initialise the root directory.
    initrd_root = (fs_node_t *) kmalloc(sizeof(fs_node_t));
    strcpy(initrd_root->name, "initrd");
    initrd_root->mask = initrd_root->uid = initrd_root->gid = initrd_root->inode = initrd_root->length = 0;
    initrd_root->flags = FS_DIRECTORY;
    initrd_root->read = 0;
    initrd_root->write = 0;
    initrd_root->open = 0;
    initrd_root->close = 0;
    initrd_root->readdir = &initrd_readdir;
    initrd_root->finddir = &initrd_finddir;
    initrd_root->ptr = 0;
    initrd_root->impl = 0;

    // Initialise the /dev directory (required!)
    initrd_dev = (fs_node_t *) kmalloc(sizeof(fs_node_t));
    strcpy(initrd_dev->name, "dev");
    initrd_dev->mask = initrd_dev->uid = initrd_dev->gid = initrd_dev->inode = initrd_dev->length = 0;
    initrd_dev->flags = FS_DIRECTORY;
    initrd_dev->read = 0;
    initrd_dev->write = 0;
    initrd_dev->open = 0;
    initrd_dev->close = 0;
    initrd_dev->readdir = &initrd_readdir;
    initrd_dev->finddir = &initrd_finddir;
    initrd_dev->ptr = 0;
    initrd_dev->impl = 0;

    root_nodes = (fs_node_t *) kmalloc(sizeof(fs_node_t) * initrd_header->nfiles);
    nroot_nodes = initrd_header->nfiles;

    // For every file...
    int i;
    for (i = 0; i < initrd_header->nfiles; i++) {
        // Edit the file's header - currently it holds the file offset
        // relative to the start of the ramdisk. We want it relative to the start
        // of memory.
        file_headers[i].offset += location;
        // Create a new file node.
        strcpy(root_nodes[i].name, &file_headers[i].name);
        root_nodes[i].mask = root_nodes[i].uid = root_nodes[i].gid = 0;
        root_nodes[i].length = file_headers[i].length;
        root_nodes[i].inode = i;
        root_nodes[i].flags = FS_FILE;
        root_nodes[i].read = &initrd_read;
        root_nodes[i].write = 0;
        root_nodes[i].readdir = 0;
        root_nodes[i].finddir = 0;
        root_nodes[i].open = 0;
        root_nodes[i].close = 0;
        root_nodes[i].impl = 0;
    }
    settextcolor(GREEN, BLACK);
    printf("[OK]  ");
    settextcolor(WHITE, BLACK);
    printf("Initrd FS ");
    settextcolor(BLUE, BLACK);
    printf("@ 0x%x\n", location);
    settextcolor(WHITE, BLACK);
    return initrd_root;
}

void initrd_list() {
    /*int i = 0;
    while ( (node = readdir_fs(fs_root, i)) != 0)
    {
        fs_node_t *fsnode = finddir_fs(fs_root, node->name);
        i++;
    }*/
}

void ls() {
    int i = 0;
    printf("\n");
    struct dirent *node = 0;
    while ((node = readdir_fs(fs_root, i)) != 0) {
        settextcolor(WHITE, BLACK);
        fs_node_t *fsnode = finddir_fs(fs_root, node->name);

        if ((fsnode->flags & 0x7) == FS_DIRECTORY) {
            printf("\t/");
            //puts((char*)node->name);
            printf("\n");
        } else {
            printf("\t  |-- ");
            puts((char *) node->name);
            printf(" \n");
        }
        i++;
    }
    printf("\n");
}


void cat(char *filename) {
    putch('\n');
    int i = 0;
    while ((node = readdir_fs(fs_root, i)) != 0) {
        fs_node_t *fsnode = finddir_fs(fs_root, node->name);
        if (strcmp(node->name, filename) == 0) {
            fs_node_t *fsnode = finddir_fs(fs_root, filename);
            settextcolor(LIGHT_BLUE, BLACK);
            char buf[256];
            unsigned int sz = read_fs(fsnode, 0, 256, buf);
            for (int j = 0; j < sz; j++)
                printf("%c", buf[j]);
            return;
        }
        i++;
    }
    settextcolor(RED, BLACK);
    printf("No such file: ");
    puts(filename);
    putch('\n');
    settextcolor(WHITE, BLACK);
}
