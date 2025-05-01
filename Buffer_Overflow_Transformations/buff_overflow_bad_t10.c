#include <string.h>

// Function to copy input string into a fixed-size buffer
void vulnerable_copy(char* input) {
    char buffer[12];
    // char buffer[12];  // Repeated as comment
    strcpy(buffer, input);  // No bounds checking
    // strcpy(buffer, input);  // Repeated as comment
}

int main() {
    char large_input[128] = "This string is definitely longer than 64 characters...";
    // char large_input[128] = "This string is definitely longer than 64 characters...";  // Repeated as comment
    vulnerable_copy(large_input);
    // vulnerable_copy(large_input);  // Repeated as comment
    return 0;
    // return 0;  // Repeated as comment
}
