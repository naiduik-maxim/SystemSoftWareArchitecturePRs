#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/mman.h>
#include "elfloader.h"

void dump_table(elf64_section_header_t *elf_section_headers, size_t section_count, char *section_strtab){
    printf("ND  |                 MAME |     TYPE | FLAG | OFFSET |   SIZE | ADDRESS          |\n");
    printf("----+----------------------+----------+------+--------+--------+------------------+\n");
    for(size_t i = 0; i < section_count; i++){
        elf64_section_header_t *section = (elf_section_headers + i);
        char *name = section_strtab + section->name_index;

        printf("%3zu | ", i);
        printf("%20s | ", name);
        printf("%8s | ", ELF_SECTION_TYPE[section->type]);
        printf("%c%c%c%c | ",
            (section->flags & ELF_FLAG_WRITE) ? 'W' : ' ',
            (section->flags & ELF_FLAG_ALLOC) ? 'A' : ' ',
            (section->flags & ELF_FLAG_EXEC) ? 'X' : ' ',
            (section->flags & ~0x7) ? '?' : ' '
        );
        printf("%6lx | ", section->offset);
        printf("%6lx | ", section->size);
        printf("%.16lx | ", section->address);
        printf("\n");
    
    }
}

int main(int argc, char *argv[]){
    if(argc != 2){
        printf("Input elf file name!");
        return 1;
    }

    char *elf_name = argv[1];
    FILE *elf = fopen(elf_name, "rb");
    if(elf == NULL){
        fprintf(stderr, "Unable to open file\n");
        return 1;
    }

    elf64_header_t *elf_header = malloc(sizeof(elf64_header_t));
    fread(elf_header, sizeof(elf64_header_t), 1, elf);

    size_t section_count = elf_header->section_header_count;
    size_t section_header_size = sizeof(elf64_section_header_t);
    elf64_section_header_t *elf_headers = malloc(section_header_size * section_count);

    fseek(elf, elf_header->section_header_offset, SEEK_SET);
    fread(elf_headers, section_header_size, section_count, elf);

    printf(">> Allocating sections memory and loading string and symbol tables\n");
    for(size_t i = 0; i < section_count; i++){
        elf64_section_header_t *section = (elf_headers + i);

        if((section->flags & ELF_FLAG_ALLOC) && section->size > 0){
            size_t size = section->size;
            size_t alignment = section->alignment;

            if (alignment == 0)
                alignment = 1;

            size_t alloc_size = size + alignment;

            char *raw = mmap(NULL, section->size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
            
            if (raw == MAP_FAILED){
                fprintf(stderr, "Unable to mmap\n");
                return 2;
            }

            uintptr_t addr = (uintptr_t)raw;
            uintptr_t aligned =
                (addr + alignment - 1) & ~(alignment - 1);

            char *mem = (char *)aligned;

            if(section->type == ELF_SECTION_PROGBITS){
                fseek(elf, section->offset, SEEK_SET);
                fread(mem, section->size, 1, elf);
            } else if(section->type == ELF_SECTION_NOBITS){
                memset(mem, '\0', section->size);
            }

            section->address = (uint64_t)(size_t) mem;        
        }

        if (section->type == ELF_SECTION_SYMTAB || section->type == ELF_SECTION_STRTAB){
            elf64_symbol_t *table = malloc(section->size);

            fseek(elf, section->offset, SEEK_SET);
            fread(table, section->size, 1, elf);
            section->address = (uint64_t)(size_t) table;
        }
    }

    for(size_t i = 0; i < section_count; i++) {
        elf64_section_header_t *section = (elf_headers + i);

        if(section->type == ELF_SECTION_RELA) {
            size_t entry_count = section->size / section->entry_size;
            elf64_rela_entry_t *entries = malloc(section->size);
            section->address = (uint64_t)(size_t)entries;
            fseek(elf, section->offset, SEEK_SET);
            fread(entries, section->size, 1, elf);

            elf64_section_header_t *relocation_sec = (elf_headers + section->info);
            char *relocation_sec_data = (char *) relocation_sec->address;

            elf64_section_header_t *section_table = (elf_headers + section->link);
            elf64_symbol_t *symbol_table = (elf64_symbol_t *) (section_table->address);

            elf64_section_header_t *section_string_table = (elf_headers + section_table->link);
            char *string_table = (char *)(section_string_table->address);

            for(size_t entry_i = 0; entry_i < entry_count; entry_i++) {
                elf64_rela_entry_t *entry = (entries + entry_i);
                elf64_symbol_t *symbol = (symbol_table + entry->symbol);
                char *symbol_name = (string_table + symbol->name_index);

                uint64_t symbol_value = 0;

                if(symbol->section > 0) {
                    elf64_section_header_t *symbol_section = (elf_headers + symbol->section);
                    symbol_value = symbol_section->address + symbol->value + entry->addend;
                    
                    if(symbol->name_index) {
                         printf("Relocating: %s\n", symbol_name);
                    }
                } else {
                    if (strcmp("printf", symbol_name) == 0) {
                        symbol_value = (uint64_t)(&printf) + entry->addend;
                    } else if (strcmp("puts", symbol_name) == 0) {
                        symbol_value = (uint64_t)(&puts);
                    } else {
                        fprintf(stderr, "Unknown symbol: %s\n", symbol_name);
                        return 1;
                    }

                    symbol_value += entry->addend;
                }
                if(entry->type == ELF_REL_TYPE_64) {
                    uint64_t *location = (uint64_t *)(relocation_sec_data + entry->offset);
                    *location = symbol_value;
                } 
                else if(entry->type == ELF_REL_TYPE_PC32 || entry->type == ELF_REL_TYPE_PLT32) {

                    uint32_t *location = (uint32_t *)(relocation_sec_data + entry->offset);

                    uint64_t P = (uint64_t)location;

                    int64_t result = (int64_t)(symbol_value - P);

                    *location = (uint32_t)result;
                }
                
                else if(entry->type == ELF_REL_TYPE_32) {

                    uint32_t *location = (uint32_t *)(relocation_sec_data + entry->offset);

                    *location = (uint32_t)symbol_value;
                }
                else {
                    fprintf(stderr, "Unknown relocation type: %d\n", entry->type);
                    return 1;
                }
            }
        }
    }

    for(size_t i = 0; i < section_count; i++){
    elf64_section_header_t *section = (elf_headers + i);

        if((section->flags & ELF_FLAG_ALLOC) && section->size > 0){

            int prot = PROT_READ;
            if(section->flags & ELF_FLAG_WRITE) prot |= PROT_WRITE;
            if(section->flags & ELF_FLAG_EXEC) prot |= PROT_EXEC;

            mprotect((void *) section->address, section->size, prot);
        }
    }

    elf64_section_header_t *shstrtab = (elf_headers + elf_header->string_table_index);
    dump_table(elf_headers, section_count, (char *) shstrtab->address);

    printf(">> Finding run() function\n");
    void_func_t run_func;
    for(size_t i = 0; i < section_count; i++){
        elf64_section_header_t *section = (elf_headers + i);

        if(section->type == ELF_SECTION_SYMTAB){
            elf64_symbol_t *symbol_table = (elf64_symbol_t *) section->address;
        
            elf64_section_header_t *section_string_table = (elf_headers + section->link);
            char *string_table = (char *)(section_string_table->address);

            size_t symbol_count = section->size / section->entry_size;
            for(size_t j = 0; j < symbol_count; j++){
                elf64_symbol_t *symbol = (symbol_table + j);
                char *symbol_name = (string_table + symbol->name_index);
                elf64_section_header_t *run_section = (elf_headers + symbol->section);

                if (strcmp("run", symbol_name) == 0 && ((symbol->info >> 4) & 0xf) == 1 && (run_section->flags & ELF_FLAG_EXEC)){
                    run_func = (void_func_t)(run_section->address + symbol->value);
                    break;
                }
            }
        }

        if(run_func) break;

    }

    if(run_func != NULL){
        printf(">> Running run() function\n\n");
        run_func();
    } else {
        fprintf(stderr, "Unable to locate run()\n");
        return 1;
    }

    return 0;
}