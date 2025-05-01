#include <string.h>

void safe_copy(const char* input) {
    char buffer[64];
    strncpy(buffer, input, sizeof(buffer) - 1);  // Bounds-checked
    buffer[sizeof(buffer) - 1] = '\0';  // Ensure null-termination
}

int main() {
    char large_input[128] = "This string is longer but safely handled";
    safe_copy(large_input);
    return 0;
}
