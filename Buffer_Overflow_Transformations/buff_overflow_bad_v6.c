#include <string.h>

// New helper function containing the vulnerable operation
void perform_unsafe_copy(char* dest, char* src) {
    strcpy(dest, src);  // Still no bounds checking
}

void vulnerable_copy(char* input) {
    char buffer[12];
    // Call to helper function with vulnerable operation
    perform_unsafe_copy(buffer, input);  // Buffer overflow remains
}

int main() {
    char large_input[128] = "This string is definitely longer than 64 characters...";
    vulnerable_copy(large_input);  // Same dangerous call
    return 0;
}
