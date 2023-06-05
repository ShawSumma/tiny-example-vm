#include <stdio.h>
#include <stdint.h>

#include "bc.h"

int main() {
    uint8_t bytecode[256] = {
        OP_CONST, 0, 40,
        OP_CONST, 1, 9,
        OP_ADD, 2, 1, 0,
        OP_PRINT, 2,
        OP_EXIT,
    };
    FILE *out = fopen("test.bin", "wb");
    fwrite(bytecode, 1, 256, out);
    fclose(out);
}