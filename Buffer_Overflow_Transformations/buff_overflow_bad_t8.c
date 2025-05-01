#include <string.h>

//Our own buffer overflow vulnerability code
static void dummy_function(char* dummy_param) {
    (void)dummy_param;
}

void vulnerable_copy(char* input) {
    char buffer[12];
    dummy_function(input);

    //Bounds not checked, vulnerable
    strcpy(buffer, input);
    
    dummy_function(buffer);
}

int main() {
    char large_input[128] = "This string is definitely longer than 12 characters...";
    
    dummy_function(large_input);
    
    vulnerable_copy(large_input);
    
    dummy_function("unused");
    
    return 0;
}
