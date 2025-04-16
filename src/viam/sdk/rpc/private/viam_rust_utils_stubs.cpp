#include <viam/sdk/rpc/private/viam_rust_utils.h>

#include <cstdlib>

void* init_rust_runtime() {
    abort();
}

int free_rust_runtime(void*) {
    abort();
}

void free_string(const char*) {
    abort();
}

char* dial(const char*, const char*, const char*, const char*, bool, float, void*) {
    abort();
}
