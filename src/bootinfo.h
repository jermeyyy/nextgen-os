#ifndef BOOTINFO_H
#define BOOTINFO_H

typedef struct aout_symbol_table {
    unsigned long tabsize;
    unsigned long strsize;
    unsigned long addr;
    unsigned long reserved;
} aout_symbol_table_t;

typedef struct elf_section_header_table {
    unsigned long num;
    unsigned long size;
    unsigned long addr;
    unsigned long shndx;
} elf_section_header_table_t;

typedef struct multiboot_info {
    unsigned long int m_flags;
    unsigned long int m_memoryLo;
    unsigned long int m_memoryHi;
    unsigned long int m_bootDevice;
    unsigned long int m_cmdLine;
    unsigned long int m_modsCount;
    unsigned long int m_modsAddr;
    union {
        aout_symbol_table_t aout_sym;
        elf_section_header_table_t elf_sec;
    } u;
    unsigned long int m_mmap_length;
    unsigned long int m_mmap_addr;
} mboot;

typedef struct memory_map {
    unsigned long size;
    unsigned long base_addr_low;
    unsigned long base_addr_high;
    unsigned long length_low;
    unsigned long length_high;
    unsigned long type;
} memory_map_t;

#endif
