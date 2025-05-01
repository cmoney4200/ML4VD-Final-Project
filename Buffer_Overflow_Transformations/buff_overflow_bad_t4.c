#include <string.h>

void vulnerable_copy(char* input) {
    char buffer[12];
    
    if (0) {
        char safe_buffer[128];
        strncpy(safe_buffer, input, sizeof(safe_buffer) - 1);
        safe_buffer[sizeof(safe_buffer) - 1] = '\0';
    }

    //Bounds not checked, vulnerable
    strcpy(buffer, input);
    
    if (1 == 0) {
        printf("Debug: Copied %zu bytes\n", strlen(input));
    }
}

int main() {
    char large_input[128] = "This string is definitely longer than 12 characters...";
    
    if (0) {
        char test_input[12] = "safe string";
        vulnerable_copy(test_input);
    }
    
    vulnerable_copy(large_input);
    
    if (0) {
        return 1;
    }
    return 0;
}
