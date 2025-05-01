#include <string.h>

// New helper function that does nothing meaningful
static void dummy_function(char* dummy_param) {
    // This function doesn't affect program behavior
    (void)dummy_param; // Cast to void to suppress unused parameter warning
}

void vulnerable_copy(char* input) {
    char buffer[12];
    
    // Call dummy function before vulnerable operation
    dummy_function(input);
    
    strcpy(buffer, input);  // No bounds checking (vulnerability remains)
    
    // Call dummy function after vulnerable operation
    dummy_function(buffer);
}

int main() {
    char large_input[128] = "This string is definitely longer than 64 characters...";
    
    // Call dummy function in main
    dummy_function(large_input);
    
    vulnerable_copy(large_input);
    
    // Final dummy call
    dummy_function("unused");
    
    return 0;
}
