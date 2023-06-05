
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>

#include "bc.h"

#define read(ptr)                                                           \
  ({                                                                           \
    uint8_t ret = *(ptr);                                                        \
    (ptr) += 1;                                                                  \
    ret;                                                                       \
  })

void vm_run(uint8_t *ptr) {
    static void *jump_table[] = {
        [OP_EXIT] = &&do_exit,
        [OP_CONST] = &&do_const,
        [OP_ADD] = &&do_add,
        [OP_PRINT] = &&do_print,
    };
    uint8_t regs[256] = {0};
    goto *jump_table[*ptr++];
do_const: {
    uint8_t out_reg = read(ptr);
    uint8_t value = read(ptr);
    regs[out_reg] = value;
    goto *jump_table[*ptr++];
}
do_add: {
    uint8_t out_reg = read(ptr);
    uint8_t lhs_reg = read(ptr);
    uint8_t rhs_reg = read(ptr);
    regs[out_reg] = regs[lhs_reg] + regs[rhs_reg];
    goto *jump_table[*ptr++];
}
do_print: {
    uint8_t reg = read(ptr);
    printf("debug %"PRIu8"\n", regs[reg]);
    goto *jump_table[*ptr++];
}
do_exit: {
    return;
}
}

int main(int argc, char *argv[static argc]) {
    if (argc < 2) {
        fprintf(stderr, "error: no file given");
        return 1;
    }
    FILE *file = fopen(argv[1], "r");
    size_t alloc = 256;
    uint8_t *mem = malloc(alloc);
    uint8_t *head = mem;
    while (!feof(file)) {
        ptrdiff_t diff = head - mem;
        if (diff + 1 >= alloc) {
            alloc *= 2;
            mem = realloc(mem, alloc);
            head = mem + diff;
        }
        *head++ = fgetc(file);
    }
    fclose(file);
    vm_run(mem);
}
