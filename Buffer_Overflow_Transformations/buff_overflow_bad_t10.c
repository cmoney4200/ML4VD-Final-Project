#include <string.h>

void vulnerable_copy(char* input) {
    char buffer[12];
    //char buffer[12];
    //Bounds not checked, vulnerable
    strcpy(buffer, input);
    //strcpy(buffer, input);
}

int main() {
    char large_input[128] = "This string is definitely longer than 12 characters...";
    //char large_input[128] = "This string is definitely longer than 12 characters...";
    vulnerable_copy(large_input);
    //vulnerable_copy(large_input);
    return 0;
    //return 0;
}
