#include <string.h>

void safe_copy(const char* input) {
    char buffer[64];
    //Bounds safely checked
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
}

int main() {
    char large_input[128] = "This string is longer but safely handled";
    safe_copy(large_input);
    return 0;
}
