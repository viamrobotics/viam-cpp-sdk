#if defined(__cplusplus)
extern "C" {
#endif

// Prototypes for the entrypoints from viam_rust_utils
// that we use in the SDK.

void* init_rust_runtime();
int free_rust_runtime(void* ptr);
void free_string(const char* s);
char* dial(const char* uri,
           const char* entity,
           const char* type,
           const char* payload,
           bool allow_insecure,
           float timeout,
           void* ptr);

#if defined(__cplusplus)
}  // extern "C"
#endif
