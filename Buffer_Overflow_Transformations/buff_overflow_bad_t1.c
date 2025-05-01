#include <string.h>

// Function to copy input string into a fixed-size buffer without bounds checking
void vulnerable_copy(char* token7) {
    // Fixed-size buffer of 12 characters
    char buffer[12];
    // Copy input to buffer, vulnerable to buffer overflow
    strcpy(buffer, token7);
}

// Main function to demonstrate the vulnerability
int main() {
    // Large input string exceeding buffer size
    char large_input[128] = "This string is definitely longer than 64 characters...";
    // Call the vulnerable function with large input
    vulnerable_copy(large_input);
    return 0;
}
