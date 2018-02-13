#include <stdint.h>
#include <stdlib.h>

int8_t* heap_alloc_l_Pz(int64_t size) {
    return malloc(size);
}

void heap_free_Pz_N(int8_t* ptr) {
    free(ptr);
}
