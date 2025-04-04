#include <viam/sdk/rpc/private/viam_rust_utils.h>

#include <cstdlib>

// TODO(RSDK-10366): Currently, rust_utils is not published for windows
// so we just implement the associated entry points as `abort`.
#ifdef _WIN32
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
#endif
